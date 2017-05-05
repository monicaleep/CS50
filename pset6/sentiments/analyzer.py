import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = []
        self.negatives = []
        with open(positives) as lines:
            for line in lines:
                if not line.startswith(";") and line.strip():
                    self.positives.append(line.strip())
        with open(negatives) as lines:
            for line in lines:
                if not line.startswith(";") and line.strip():
                    self.negatives.append(line.strip())

        # TODO

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        for word in tokens:
            word1 = word.lower()
            if word1 in self.positives:
                score=score + 1
            if word1 in self.negatives:
                score=score - 1
        return score
