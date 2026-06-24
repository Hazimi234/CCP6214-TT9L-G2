// *********************************************************
// Program: radix_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC3L
// Tutorial Class: TT9L
// Trimester: 2610
// Member_1: 242UC244L7 | Alif Akmal Bin Abdul Halim | ALIF.AKMAL.ABDUL@student.mmu.edu.my | 018-355 5944
// Member_2: 242UC244RD | Brian Ng Zheng Yang | BRIAN.NG.ZHENG@student.mmu.edu.my | 017-779 3199
// Member_3: 242UC244PU | Meor Hazimi Bin Meor Mohammad Fared | MEOR.HAZIMI.MEOR@student.mmu.edu.my | 019-752 1755
// Member_4: 253UC256QK | Sarah Yasmin Binti Shaharrin | SARAH.YASMIN.SHAHARRIN@student.mmu.edu.my | 017-449 5246
// *********************************************************
// Task Distribution
// Member_1: Heap Sort
// Member_2: Radix Sort
// Member_3: Dataset Generator
// Member_4: Hash Table Search
// *********************************************************
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

struct Record {
    long long id;
    string str;
};

// Counting Sort subroutine
void countingSort(vector<Record>& arr, long long exp) {
    int n = arr.size();
    vector<Record> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; i++) {
        // isolate 0-9 digit
        count[(arr[i].id / exp) % 10]++;
    }
    for (int i = 1; i < 10; i++) {
        // cumulative count to determine position in output
        count[i] += count[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
        // place the record in the correct position in output
        output[count[(arr[i].id / exp) % 10] - 1] = arr[i];
        count[(arr[i].id / exp) % 10]--;
    }
    for (int i = 0; i < n; i++) {
        // copy back to arr
        arr[i] = output[i];
    }
}

int main() {
    string datasetSize;
    cout << "Enter the dataset size you want to sort (e.g., 1000): ";
    cin >> datasetSize;

    string inputFilename = "dataset_" + datasetSize + ".csv";
    
    // I/O Read (Not Timed)
    ifstream inFile(inputFilename);
    if (!inFile.is_open()) {
        cout << "Error: Could not open " << inputFilename << " - Ensure you generated it first!\n";
        return 1;
    }

    vector<Record> arr;
    string line;
    cout << "Reading data into memory...\n";
    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, textStr;
        getline(ss, idStr, ',');
        getline(ss, textStr, ',');
        arr.push_back({stoll(idStr), textStr});
    }
    inFile.close();

    // Sorting (Timed)
    cout << "Starting Radix Sort...\n";
    auto start_time = chrono::high_resolution_clock::now();

    long long exp = 1;
    for (int d = 10; d >= 1; d--) {
        countingSort(arr, exp);
        exp *= 10;
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;

    // I/O Write (Not Timed)
    string outFilename = "radix_sorted_dataset_" + datasetSize + ".csv";
    ofstream outFile(outFilename);
    
    // Writing output in the format specified by the sample (Integer/String)
    for (const auto& rec : arr) {
        outFile << rec.id << "/" << rec.str << "\n";
    }
    
    // Also write the required running time into the output file
    outFile << "\nRunning time: " << duration.count() << " seconds\n";
    outFile.close();

    cout << "----------------------------------------\n";
    cout << "SORT SUCCESSFUL!\n";
    cout << "Running time: " << duration.count() << " seconds\n";
    cout << "Output saved to: " << outFilename << "\n";
    cout << "----------------------------------------\n";

    return 0;
}