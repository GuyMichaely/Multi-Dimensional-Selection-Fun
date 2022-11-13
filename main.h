#include <math.h>
#include <stdbool.h>
#include <stdio.h>

/* constraints: 1 <= arr[i][j] <= 10^9
log2(10^9) = 29.9 -> need 30 bits to radix sort
*/
const int MAX = 1e9;
const int MAX_BITS = 30;

void radixSort(const int length, int arr[length]) {
	const int j = log2(1e9) + 1;
	int buckets[2][length];
	int bucketIndices[2];

	for (int i = 0; i < MAX_BITS; i++) {
		// sort in to buckets
		bucketIndices[0] = bucketIndices[1] = 0;
		for (int j = 0; j < length; j++) {
			const int bit = (arr[j] >> i) & 1;
			buckets[bit][bucketIndices[bit]++] = arr[j];
		}

		// merge buckets
		for (int j = 0; j < bucketIndices[0]; j++) {
			arr[j] = buckets[0][j];
		}
		for (int j = 0; j < bucketIndices[1]; j++) {
			arr[bucketIndices[0] + j] = buckets[1][j];
		}
	}
}

int min(int a, int b) {
	return a < b ? a : b;
}
int max(int a, int b) {
	return a > b ? a : b;
}

// iterate through the n bounds
// set bounds to 0-end
void intializeBounds(int n, int bounds[n][2], int end) {
	for (int i = 0; i < n; i++) {
		bounds[i][0] = 0;
		bounds[i][1] = end;
	}
}

// return min of left bound of each row
int minInMatrix(int n, int m, int **arr, int boundIndices[n][2]) {
	int smallest = MAX;
	for (int i = 0; i < n; i++) {
		smallest = min(smallest, arr[i][boundIndices[i][0]]);
	}
	return smallest;
}

// return max of right bound of each row
int maxInMatrix(int n, int m, int **arr, int boundIndices[n][2]) {
	int largest = 1;
	for (int i = 0; i < n; i++) {
		largest = max(largest, arr[i][boundIndices[i][1]-1]);
	}
	return largest;
}

// binary search to find the rightmost element <= target
int lastIndexLEQ(int length, int arr[length], int target) {
	int low = 0;
	int high = length - 1;
	while (low <= high) {
		const int mid = (low + high) / 2;
		if (arr[mid] <= target) {
			low = mid + 1;
		} else {
			high = mid-1;
		}
	}
	return low-1;
}
int firstIndexGT(int length, int arr[length], int target) {
	return lastIndexLEQ(length, arr, target) + 1;
}

// find all rows that indicate their min as the global min
// update them to the next smallest element larger than that min
// if after updating it is still possible to take at least m/2 elements from the row,
// return true
// else return false
bool updateGlobalMin(int n, int m, int **arr, int boundIndices[n][2], int *globalMin) {
	for (int i = 0; i < n; i++) {
		if (arr[i][boundIndices[i][0]] == *globalMin) {
			boundIndices[i][0] += firstIndexGT(m - boundIndices[i][0], arr[i] + boundIndices[i][0], *globalMin);
			boundIndices[i][1] = max(boundIndices[i][0] + (m+1)/2, boundIndices[i][1]);
			if (boundIndices[i][1] > m) {
				return false;
			}
		}
	}

	*globalMin = minInMatrix(n, m, arr, boundIndices);
	return true;
}

// extend the right bound of each row to take as many elements to the right
// without taking an element greather than the global max
void extendRight(int n, int m, int **arr, int boundIndices[n][2], int globalMax) {
	for (int i = 0; i < n; i++) {
		const int lastElementIndex = boundIndices[i][1] - 1;
		boundIndices[i][1] += lastIndexLEQ(m - lastElementIndex, arr[i] + lastElementIndex, globalMax);
	}
}

// calculate the length of the selections from the bounds
long selectionLength(int n, int boundIndices[n][2]) {
	long sum = 0;
	for (int i = 0; i < n; i++) {
		sum += boundIndices[i][1] - boundIndices[i][0];
	}
	return sum;
}

int getMaxProduct(int n, int m, int **arr) {
	// sort each row
	// so we can make our choices from each row contiguous subarrays
	for (int i = 0; i < n; i++) {
		radixSort(m, arr[i]);
	}

	// will keep track of the bounds of the selection in each row
	int boundIndices[n][2];
	intializeBounds(n, boundIndices, (m+1)/2);

	// trying to minimize the score and maximize the length
	int bestScore = MAX;
	long bestLength = 0;

	int globalMin = minInMatrix(n, m, arr, boundIndices);
	do {
		const int globalMax = maxInMatrix(n, m, arr, boundIndices);
		int score = globalMax - globalMin;
		if (score > bestScore) {
			continue;
		}

		extendRight(n, m, arr, boundIndices, globalMax);
		const long length = selectionLength(n, boundIndices);
		if (score < bestScore || length > bestLength) {
			bestScore = score;
			bestLength = length;
		}
	} while (updateGlobalMin(n, m, arr, boundIndices, &globalMin));

	return bestScore * bestLength;
}