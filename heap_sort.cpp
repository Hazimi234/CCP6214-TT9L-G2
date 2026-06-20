// *********************************************************
// Program: heap_sort.cpp
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

struct Record
{
    long long id;
    string str;
};

// Standard Max Heapify subroutine
void heapify(vector<Record> &arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].id > arr[largest].id)
        largest = left;

    if (right < n && arr[right].id > arr[largest].id)
        largest = right;

    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

int main()
{
    string datasetSize;
    cout << "Enter the dataset size you want to sort (e.g., 1000): ";
    cin >> datasetSize;

    string inputFilename = "dataset_" + datasetSize + ".csv";

    // 1. I/O Read Phase (Not Timed)
    ifstream inFile(inputFilename);
    if (!inFile.is_open())
    {
        cout << "Error: Could not open " << inputFilename << " - Ensure you generated it first!\n";
        return 1;
    }

    vector<Record> arr;
    string line;
    cout << "Reading data into memory...\n";
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string idStr, textStr;
        getline(ss, idStr, ',');
        getline(ss, textStr, ',');
        arr.push_back({stoll(idStr), textStr});
    }
    inFile.close();

    int n = arr.size();

    // 2. Sorting Phase (TIMED)
    cout << "Starting Heap Sort...\n";
    auto start_time = chrono::high_resolution_clock::now();

    // Build Max Heap
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--)
    {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;

    // 3. I/O Write Phase (Not Timed)
    string outFilename = "heap_sorted_dataset_" + datasetSize + ".csv";
    ofstream outFile(outFilename);

    for (const auto &rec : arr)
    {
        outFile << rec.id << "/" << rec.str << "\n";
    }

    // Write running time into the output file
    outFile << "\nRunning time: " << duration.count() << " seconds\n";
    outFile.close();

    cout << "----------------------------------------\n";
    cout << "SORT SUCCESSFUL!\n";
    cout << "Running time: " << duration.count() << " seconds\n";
    cout << "Output saved to: " << outFilename << "\n";
    cout << "----------------------------------------\n";
    cout << "Make sure to take a screenshot of this command prompt window!\n";

    return 0;
}