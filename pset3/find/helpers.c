/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <math.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if ( n < 0){
        return false;
    }
    int min = 0;
    int max = n-1;
    int mid;
    while (min <= max){
        mid = floor((max+min)/2);
       // eprintf("%i %i %i\n", min, mid, max);
        if (values[mid]== value){
            return true;
        }
        if (value > values[mid]){
            min = mid + 1;
        }
        if (value < values[mid]){
            max = mid -1;
        }
        //for (int k=0;k<size;k++){
        //    eprintf("%i",values[k]);
       // }
        //eprintf("\n");
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    for (int i = 0; i<n-1; i++){
        int minplace = i;
        for (int j = i+1; j<n;j++){
            if (values[j] < values[minplace]){
                minplace = j;
            }
        }
        int temp = values[i];
        values[i]=values[minplace];
        values[minplace]=temp;
    }
    return;
}
