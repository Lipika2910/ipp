#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

using namespace std;

int main() {
    int n, maxValue, rangeSize;

    // Input from user
    cout << "Enter number of elements: ";
    cin >> n;

    cout << "Enter maximum possible value: ";
    cin >> maxValue;

    cout << "Enter range size for buckets: ";
    cin >> rangeSize;

    vector<int> arr(n);
    cout << "Enter the elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int bucketCount = (maxValue / rangeSize) + 1;
    vector<vector<int> > buckets(bucketCount); // Note: space between > >
 
    // Step 1: Distribute elements into buckets
    for (int i = 0; i < n; i++) {
        int bucketIndex = arr[i] / rangeSize;
        buckets[bucketIndex].push_back(arr[i]);
    }

    vector<int> sortedArr;

    #pragma omp parallel
    {
        vector<int> localSorted;

        #pragma omp for nowait
        for (int i = 0; i < bucketCount; i++) {
            if (!buckets[i].empty()) {
                int localMax = *max_element(buckets[i].begin(), buckets[i].end());
                vector<int> hist(localMax + 1, 0);

                // Count frequencies
                for (int j = 0; j < buckets[i].size(); j++) {
                    hist[buckets[i][j]]++;
                }

                // Reconstruct sorted values from histogram
                for (int val = 0; val <= localMax; val++) {
                    for (int count = 0; count < hist[val]; count++) {
                        localSorted.push_back(val);
                    }
                }
            }
        }

        // Merge results
        #pragma omp critical
        {
            sortedArr.insert(sortedArr.end(), localSorted.begin(), localSorted.end());
        }
    }

    // Output
    cout << "\nSorted array:\n";
    for (int i = 0; i < sortedArr.size(); i++) {
        cout << sortedArr[i] << " ";
    }
    cout << "\n";

    return 0;
}

