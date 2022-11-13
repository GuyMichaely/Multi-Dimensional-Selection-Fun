# Multi-Dimensional-Selection-Fun
My solution to a interview question that bugged me

## The problem:
Given a 2-d array *arr* of size *n x m*, a selection is defined as an array of integers such that it contains at least ⌈*m / 2*⌉ integers from each row of *arr*. The cost of the selection is defined as the maximum difference between any two integers of the selection. 

Suppose *k* is the minimum cost of all the possible selections for the given 2-d array. Find the maximum value of the product of *k* \* the number of integers considered in the selection with the minimum cost.

**Example**

Suppose *n* = 3, *m* = 2, and *arr* = [[1, 2], [3, 4], [8, 9]]

Some of the possible selections are [2, 3, 8], [1, 2, 3, 9], [1, 3, 4, 8, 9] etc. The cost of these selections are 8 - 2 = 6, 9 - 1 = 8, and 8 respectively.

Here the minimum cost of all the possible selections is 6. The possible selections with the cost 6 are [2, 4, 8] and [2, 3, 4, 8]. The maximum value of the required product is obtained using the latter selection i.e. 6 * 4 = 24. Hence the answer is 24.

<u>**Function description**</u>

getMaxProduct has the following parameter:

* *int arr[n][m]*:  the given 2-d array
    
Returns:
    
* *int*: the maximum possible product

**Constraints**

1 ≤ *n* ≤ 10^3

2 ≤ *m* ≤ 10^3

1 ≤ *arr[i][j]* ≤ 10^9

**Example cases**

Input = [[2, 3], [1, 2], [4, 3]], output = 4. The minimum cost across all the selections is 1. The optimal selection with cost 1 is [2, 3, 2, 3] with the product 1 * 4 = 4.

Input = [[1, 4, 3], [3, 5, 6]], output = 8. The minimum cost across all the selections is 2. The optimal selection with cost 2 is [4, 3, 3, 5] with the product 2 * 4 = 8.

## My solution:

First, realize that if you take elements *x<sub>1</sub>* and *x<sub>2</sub>* from *arr[i]*, there is no score penalty to taking all elements *e* ∈ [*x<sub>1</sub>*, *x<sub>2</sub>*]. Doing so also allows you to maximise your selection length. For this reason, I first sort each *arr[i]* and only consider contiguous subarrays within each *arr[i]*.

Next, realize that, given a global minimum element in a selection, the optimal maxmimum element for that selection can be found iteratively. Given that the rows of *arr* are sorted, an initial selection from *arr* that aims to minimize selection score should take only ⌈*m / 2*⌉ elements from each *arr[i]*. In order to minimize score, we should want each selection within an array to be as far to the *left* as possible so as to avoid larger values on the *right*.

Hence, each selection within an array will start with the smallest element ≥ the given global minimum and will take only as many elements to the right as neccesary (⌈*m / 2*⌉). From this selection we can identify the optimal global maximum for the given global minimum.

Finally, having identified a global maximum, we can extend each selection from *arr\[i]* to the right up to the last element ≤ the global maximum. The resulting selection has minimum score and maximum length for the given global minimum.

Knowing this, we can simply take each element in the matrix as a global minimum and return the solution with the best *k* \* *length* score.
