// *********************************************************
// Program: radix_sort_step.cpp
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

using namespace std;

// Structure to hold the two fields of each record
struct Record
{
    long long id;
    string str;
};

// Print the array to the screen and the file
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

// Counting Sort subroutine specifically for base 10 digits
void countingSort(vector<Record> &arr, long long exp)
{
    int n = arr.size();
    vector<Record> output(n);
    int count[10] = {0};

    // Store count of occurrences in count[] array
    for (int i = 0; i < n; i++)
    {
        count[(arr[i].id / exp) % 10]++;
    }

    // Change count[i] so that count[i] now contains actual
    // position of this digit in output[]
    for (int i = 1; i < 10; i++)
    {
        count[i] += count[i - 1];
    }

    // Build the output array (working backwards for stability)
    for (int i = n - 1; i >= 0; i--)
    {
        output[count[(arr[i].id / exp) % 10] - 1] = arr[i];
        count[(arr[i].id / exp) % 10]--;
    }

    // Copy the output array to arr[]
    for (int i = 0; i < n; i++)
    {
        arr[i] = output[i];
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

    // Parse the CSV to extract exactly the rows we want
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

    // Setup output file
    string outFilename = inputFilename.substr(0, inputFilename.find(".csv")) +
    "_radix_sorted_step_" + to_string(startRow) + "_" + to_string(endRow) + ".txt";
    ofstream outFile(outFilename);

    // Redirect standard output to both screen and file for convenience
    cout << "(processing from the rightmost character)\n";
    outFile << "(processing from the rightmost character)\n";

    // Print the original array before sorting
    printArray(arr, "original", outFile);

    // Process from rightmost digit (d=10) to leftmost digit (d=1)
    long long exp = 1;
    for (int d = 10; d >= 1; d--)
    {
        countingSort(arr, exp);
        // Print the array after each digit processing
        printArray(arr, "$d=" + to_string(d) + "$", outFile);
        exp *= 10;
    }

    cout << "\nStep tracker saved to: " << outFilename << "\n";
    return 0;
}