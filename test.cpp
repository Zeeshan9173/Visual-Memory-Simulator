#include <bits/stdc++.h>
using namespace std;

class RandomAccessArray {
private:
    vector<int> arr;
    vector<int> idx;   // stores shuffled indices

public:
    RandomAccessArray(vector<int> input) {
        arr = input;
        int n = arr.size();
        idx.resize(n);

        // initialize indices
        for (int i = 0; i < n; i++) {
            idx[i] = i;
        }

        shuffleIndices();
    }

    // Fisher-Yates Shuffle
    void shuffleIndices() {
        int n = idx.size();
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(idx[i], idx[j]);
        }
    }

    // Access element in random order
    int get(int i) {
        cout << "Accessing index " << idx[i] << " value = " << arr[idx[i]] << "\n";
        return arr[idx[i]];
    }

    // Update value using random order index
    void update(int i, int value) {
        cout << "Updating index " << idx[i] << " from " << arr[idx[i]] 
             << " to " << value << "\n";
        arr[idx[i]] = value;
    }

    // Print trace of current state
    void printState() {
        cout << "Current array:\n";
        for (int x : arr) cout << x << " ";
        cout << "\n";

        cout << "Index mapping:\n";
        for (int x : idx) cout << x << " ";
        cout << "\n";
    }
};

int main() {
    srand(time(0));  // seed

    vector<int> input = {10, 20, 30, 40, 50};
    RandomAccessArray obj(input);

    obj.printState();

    obj.get(0);
    obj.get(1);

    obj.update(0, 999);

    obj.printState();

    obj.shuffleIndices(); // reshuffle
    cout << "\nAfter reshuffle:\n";
    obj.printState();

    return 0;
}