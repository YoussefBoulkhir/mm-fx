#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int binarySearchIterative(const vector<int>& v, int target) {
    int left = 0, right = (int)v.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (v[mid] == target) return mid;

        if (v[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

int binarySearchRecursive(const vector<int>& v, int left, int right, int target) {
    if (left > right) return -1;

    int mid = left + (right - left) / 2;

    if (v[mid] == target) return mid;

    if (v[mid] < target)
        return binarySearchRecursive(v, mid + 1, right, target);

    return binarySearchRecursive(v, left, mid - 1, target);
}

int main() {
    vector<int> v = {1, 3, 5, 7, 9, 11, 13, 15};

    int target = 7;

    cout << "Array: ";
    for (int x : v) cout << x << " ";
    cout << "\nTarget: " << target << "\n\n";

    // =========================
    // 1. Binary Search Iterative
    // =========================
    int idx1 = binarySearchIterative(v, target);
    cout << "[Iterative BS] Index = " << idx1 << endl;

    // =========================
    // 2. Binary Search Recursive
    // =========================
    int idx2 = binarySearchRecursive(v, 0, v.size() - 1, target);
    cout << "[Recursive BS] Index = " << idx2 << endl;

    // =========================
    // 3. std::lower_bound
    // =========================
    auto lb = lower_bound(v.begin(), v.end(), target);
    cout << "[lower_bound] Index = " << (lb - v.begin()) << endl;

    // =========================
    // 4. std::upper_bound
    // =========================
    auto ub = upper_bound(v.begin(), v.end(), target);
    cout << "[upper_bound] Index = " << (ub - v.begin()) << endl;

    cout << "\n--- Semantique ---\n";
    cout << "lower_bound : premier element >= target\n";
    cout << "upper_bound : premier element > target\n";

    return 0;
}