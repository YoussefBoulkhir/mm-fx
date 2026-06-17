#include <iostream>
#include <vector>
#include <limits>

using namespace std;

vector<int> topKVirtual(const vector<int>& nums, int K) {
    int head = 0; // décalage virtuel

    int n = nums.size();
    vector<int> result;

    // copie locale pour marquer les éléments déjà pris
    vector<bool> used(n, false);

    for (int t = 0; t < K; t++) {
        int bestVal = numeric_limits<int>::min();
        int bestIdx = -1;

        // scan virtuel uniquement
        for (int i = head; i < n; i++) {
            if (used[i]) continue;

            if (nums[i] > bestVal) {
                bestVal = nums[i];
                bestIdx = i;
            }
        }

        if (bestIdx == -1) break;

        result.push_back(bestVal);
        used[bestIdx] = true;

        // décalage virtuel (optionnel mais cohérent avec ton idée)
        while (head < n && used[head]) {
            head++;
        }
    }

    return result;
}

int main() {
    vector<int> nums = {10, 2, 50, 4, 30, 1, 7};
    int K = 3;

    vector<int> res = topKVirtual(nums, K);

    cout << "Top K: ";
    for (int x : res) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}