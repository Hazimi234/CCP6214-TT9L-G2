// *********************************************************
// Program: heap_sort_step.cpp
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
// Member_3: Hash Table Search
// Member_4: Dataset Generator
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Record
{
    long long id;
    string str;
};

// Helper function to print the array to BOTH the screen and the file
void printArray(const vector<Record> &arr, string label, ofstream &outFile)
{
    cout << "[";
    outFile << "[";
    for (size_t i = 0; i < arr.size(); ++i)
    {
        cout << arr[i].id << "/" << arr[i].str;
        outFile << arr[i].id << "/" << arr[i].str;
        if (i < arr.size() - 1)
        {
            cout << ", ";
            outFile << ", ";
        }
    }
    cout << "] " << label << "\n";
    outFile << "] " << label << "\n";
}

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
    // ---- TUTOR SPECIFIED VARIABLES ----
    string inputFilename = "dataset_1000.csv";
    int startRow = 1;
    int endRow = 7;
    // -----------------------------------

    ifstream inFile(inputFilename);
    if (!inFile.is_open())
    {
        cout << "Error: Could not open " << inputFilename << "\n";
        return 1;
    }

    vector<Record> arr;
    string line;
    int currentRow = 1;

    // Parse the CSV
    while (getline(inFile, line))
    {
        if (currentRow >= startRow && currentRow <= endRow)
        {
            stringstream ss(line);
            string idStr, textStr;
            getline(ss, idStr, ',');
            getline(ss, textStr, ',');
            arr.push_back({stoll(idStr), textStr});
        }
        if (currentRow > endRow)
            break;
        currentRow++;
    }
    inFile.close();

    string outFilename = inputFilename.substr(0, inputFilename.find(".csv")) +
                         "_heap_sorted_step_" + to_string(startRow) + "_" + to_string(endRow) + ".txt";
    ofstream outFile(outFilename);

    cout << "(using maxheap)\n";
    outFile << "(using maxheap)\n\n";

    int n = arr.size();

    // Phase 1: Build heap (rearrange array into a Max Heap)
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    // Print the initial built Max Heap
    printArray(arr, "initial", outFile);

    // Phase 2: One by one extract an element from the heap
    for (int i = n - 1; i > 0; i--)
    {
        // Move current root (the biggest number) to the end
        swap(arr[0], arr[i]);

        // Call max heapify on the reduced heap
        heapify(arr, i, 0);

        // Print step tracking countdown (i=6, i=5, ... i=1)
        printArray(arr, "$i=" + to_string(i) + "$", outFile);
    }

    cout << "\nStep tracker saved to: " << outFilename << "\n";
    return 0;
}