from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    result = db.execute("SELECT * FROM users WHERE id=:id",id=session["user_id"])
    cash = result[0]['cash']
    stocks = db.execute("SELECT * FROM portfolio WHERE userid=:id",id=session["user_id"])
    worths = {}
    total = 0
    for stock in stocks:
        worths[stock['stock_name']] = lookup(stock['stock_sym'])['price']
        total = total + lookup(stock['stock_sym'])['price']*stock['quantity']
    grand = cash + total    
    return render_template("index.html",cash=cash,stocks=stocks,worths=worths, total=total, grand=grand)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    #render the buying template if via get
    if request.method == "GET":
        return render_template("buy.html")
    else:
        #ensure proper form usage
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Please enter stock symbol and number of shares")
        
        #initialize variables
        quote, number = lookup(request.form.get("symbol")), request.form.get("shares")
        
        #validate inputs
        if not quote:
            return apology("Invalid stock symbol")
        if not number.isdigit():
            return apology("Invalid shares, only type integers please")
        if int(number) < 1:
            return apology("Invalid shares, must be a positive integer")
        cost = quote['price'] * int(number)
        result = db.execute("SELECT cash FROM users WHERE id=:id",id=session["user_id"])
        
        #if cost is more than cash reserves
        if cost > result[0]['cash']:
            return apology("You don't have enough funds")
        #check if already owning some stocks
        rows = db.execute("SELECT * FROM portfolio WHERE userid=:id AND stock_sym=:sym",id=session["user_id"],sym=request.form.get("symbol"))
        if len(rows) != 1:
            #add share to portfolio
            result = db.execute("INSERT INTO portfolio (stock_name,quantity,purchase_price,userid,stock_sym) VALUES (:stock_name,:quantity,:purchase_price,:userid,:stock_sym)",stock_name=quote['name'],quantity=int(number),purchase_price=quote['price'],userid=session["user_id"],stock_sym=quote['symbol'])
        else:
            result = db.execute("UPDATE portfolio SET quantity = quantity + :quantity WHERE userid=:id AND stock_sym=:sym",quantity=int(number),id=session["user_id"],sym=quote['symbol'])
        db.execute("INSERT INTO history (quantity,price,id,symbol) VALUES (:quantity,:purchase_price,:userid,:stock_sym)",quantity=int(number),purchase_price=quote['price'],userid=session["user_id"],stock_sym=quote['symbol'])
        #deduct from cash reserves
        db.execute("UPDATE users SET cash = cash - :cost WHERE id=:id",cost=cost,id=session["user_id"])
        #make sure nothing went wrong when inserting
        if not result:
            return apology("Database Error")
        #if success, redirect to index page
        if result:
            return redirect(url_for("index"))
        return apology("TODO")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    rows = db.execute("SELECT * FROM history WHERE id=:id",id=session["user_id"])
    return render_template("history.html",rows=rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")
        
@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    if request.method == "GET":
        return render_template("change.html")
    else:
        if not request.form.get("oldpassword"):
            return apology("Please enter old password")
        rows = db.execute("SELECT * FROM users WHERE id=:id",id=session["user_id"])
        if not pwd_context.verify(request.form.get("oldpassword"), rows[0]["hash"]):
            return apology("Old Password Not Correct")
        elif not request.form.get("password") or not request.form.get("password1"):
            return apology("Enter new password twice")
        #ensure two passwords match
        elif request.form.get("password") != request.form.get("password1"):
            return apology("New passwords do not match")
        #update DB with new PW
        db.execute("UPDATE users SET hash=:hash WHERE id=:id",hash=pwd_context.encrypt(request.form.get("password")),id=session["user_id"])
        return redirect(url_for("index"))
        
    

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method =="GET":
        return render_template("quote.html")
    else:
        if not request.form.get("stock"):
            return apology("Please enter a stock symbol")
        quote = lookup(request.form.get("stock"))
        if not quote:
            return apology("Invalid Stock")
        return render_template("quoted.html",quote=quote)
    

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":
     
        # ensure user typed a username
        if not request.form.get("username"):
            return apology("Must provide username")
      
        #ensure user typed a password
        elif not request.form.get("password"):
            return apology("Must provide password")
     
        #ensure user repeats password
        elif not request.form.get("password1"):
            return apology("Please re-enter your password")

        #ensure two passwords match
        elif request.form.get("password") != request.form.get("password1"):
            return apology("Passwords do not match")
            
        #add user to db
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username,:hash)",username=request.form.get("username"),hash=pwd_context.encrypt(request.form.get("password")))
        
        if not result:
            return apology("Username aleady taken")
        # query database for username
           
        # remember which user has logged in
        session["user_id"] = result

        # redirect user to home page
        return redirect(url_for("index"))
        
    # else the user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        return render_template("sell.html")
    else:
        #ensure proper form usage
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Please enter stock symbol and number of shares")
        
        #initialize variables
        quote, number = lookup(request.form.get("symbol")), request.form.get("shares")
        
        #validate inputs
        if not quote:
            return apology("Invalid stock symbol")
        if not number.isdigit():
            return apology("Invalid shares, only type integers please")
        if int(number) < 1:
            return apology("Invalid shares, must be a positive integer")
        rows = db.execute("SELECT * FROM portfolio WHERE userid=:id AND stock_sym=:sym",id=session["user_id"],sym=request.form.get("symbol"))
        if len(rows) != 1:
            return apology("You don't own this stock")
        if int(number) > rows[0]['quantity']:
            return apology("You can't sell more than you own")
        value = int(number)*quote['price']
        db.execute("UPDATE users SET cash = cash + :value WHERE id=:id",value=value,id=session["user_id"])
        db.execute("UPDATE portfolio SET quantity = quantity - :num WHERE userid=:id AND stock_sym=:sym",num=int(number),id=session["user_id"],sym=quote['symbol'])
        db.execute("INSERT INTO history (quantity,price,id,symbol) VALUES (:quantity,:purchase_price,:userid,:stock_sym)",quantity=-int(number),purchase_price=quote['price'],userid=session["user_id"],stock_sym=quote['symbol'])
        rows = db.execute("SELECT * FROM portfolio WHERE userid=:id AND stock_sym=:sym",id=session["user_id"],sym=request.form.get("symbol"))
        if rows[0]['quantity'] == 0:
            db.execute("DELETE FROM portfolio WHERE userid=:id AND stock_sym=:sym",id=session["user_id"],sym=request.form.get("symbol"))
        return redirect(url_for("index"))
