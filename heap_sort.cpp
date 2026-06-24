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


// To heapify a subtree rooted with node i, which is an index in arr[].
// n is the size of the heap.
void heapify(vector<Record> &arr, int n, int i)
{
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // left child = 2*i + 1
    int right = 2 * i + 2; // right child = 2*i + 2

    // If left child is larger than root
    if (left < n && arr[left].id > arr[largest].id)
        largest = left;

    // If right child is larger than largest so far
    if (right < n && arr[right].id > arr[largest].id)
        largest = right;

    // If largest is not root
    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

int main()
{
    string datasetSize;
    cout << "Enter the dataset size you want to sort (e.g., 1000): ";
    cin >> datasetSize;

    string inputFilename = "dataset_" + datasetSize + ".csv"; // Input file name based on user input

    // 1. I/O Read Phase (Not Timed)
    ifstream inFile(inputFilename);
    if (!inFile.is_open())
    {
        cout << "Error: Could not open " << inputFilename << " - Ensure you generated it first!\n";
        return 1;
    }

    vector<Record> arr; // Vector to hold the records
    string line;
    cout << "Reading data into memory...\n";
    while (getline(inFile, line))
    {
        stringstream ss(line); // Use stringstream to parse the line
        string idStr, textStr;
        getline(ss, idStr, ',');
        getline(ss, textStr, ',');
        arr.push_back({stoll(idStr), textStr}); // Convert string to long long and store in vector
    }
    inFile.close();

    int n = arr.size(); // Get the size of the array

    // 2. Sorting Phase (TIMED)
    cout << "Starting Heap Sort...\n";
    auto start_time = chrono::high_resolution_clock::now();

    // Build Max Heap
    for (int i = n / 2 - 1; i >= 0; i--) // Start from the last non-leaf node and heapify each node
    {
        heapify(arr, n, i); // Call heapify on each node
    }

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--) // Move current root to end and reduce the heap size
    {
        swap(arr[0], arr[i]); // Move current root to end
        heapify(arr, i, 0);   // Call heapify on the reduced heap
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

    return 0;
}