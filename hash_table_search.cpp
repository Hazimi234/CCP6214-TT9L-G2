// *********************************************************
// Program: hash_table_search.cpp
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
#include <chrono>

using namespace std;

// Structure to represent a single slot in the hash table
struct HashNode
{
    long long id = -1;
    string str = "";
    bool isOccupied = false;
};

// Array-based Hash Table using Linear Probing
class HashTable
{
public:
    HashNode *table;
    int capacity;

    // Initialize table size to double the dataset size to maintain a load factor of 0.5
    HashTable(int size)
    {
        capacity = size * 2;
        table = new HashNode[capacity];
    }
    ~HashTable() { delete[] table; }

    // Basic modulo hash function
    int hashFunction(long long key) { return key % capacity; }

    // Insert function handling collisions via linear probing
    void insert(long long key, string value)
    {
        int index = hashFunction(key);
        while (table[index].isOccupied)
        {
            index = (index + 1) % capacity;
        }
        table[index].id = key;
        table[index].str = value;
        table[index].isOccupied = true;
    }

    // Search function returns true if the key is found, false otherwise
    bool search(long long key)
    {
        int index = hashFunction(key);
        int startIndex = index;
        while (table[index].isOccupied)
        {
            if (table[index].id == key)
                return true;
            index = (index + 1) % capacity;
            if (index == startIndex)
                break;
        }
        return false;
    }

    // Function to construct a target for the worst-case search scenario
    long long generateWorstCaseKey()
    {
        int maxLen = 0, bestStart = 0;

        // Find the largest contiguous block of occupied slots (largest cluster)
        for (int i = 0; i < capacity; i++)
        {
            if (table[i].isOccupied)
            {
                int len = 0, curr = i;
                while (table[curr].isOccupied && len < capacity)
                {
                    len++;
                    curr = (curr + 1) % capacity;
                }
                if (len > maxLen)
                {
                    maxLen = len;
                    bestStart = i;
                }
            }
        }

        /* Generate a 12-digit key that hashes to the start of the largest cluster
        since the datasets strictly contain 10-digit IDs, a 12-digit ID ensures
        the item will definitely not be found, forcing a full cluster scan (O(N) behavior).*/
        long long multiplier = 100000000000LL / capacity + 1;
        long long fakeKey = bestStart + (multiplier * capacity);

        return fakeKey;
    }
};

int main()
{
    string datasetSize;
    cout << "Enter the dataset size you want to search (e.g., 1000): ";
    cin >> datasetSize;

    string inputFilename = "dataset_" + datasetSize + ".csv";

    ifstream inFile(inputFilename);
    if (!inFile.is_open())
    {
        cout << "Error: Could not open " << inputFilename << "\n";
        return 1;
    }

    vector<long long> keys;
    string line;
    cout << "Reading data into memory...\n";
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string idStr, textStr;
        getline(ss, idStr, ',');
        getline(ss, textStr, ',');
        keys.push_back(stoll(idStr));
    }
    inFile.close();

    long long N = keys.size();
    if (N == 0)
        return 1;

    cout << "Building Hash Table...\n";
    HashTable ht(N);
    for (long long key : keys)
    {
        ht.insert(key, "dummy");
    }

    long long bestCaseKey = keys[0];
    long long worstCaseKey = ht.generateWorstCaseKey();

    /* Counter variables used to verify search logic and prevent the C++ compiler
    from optimizing away the empty loops during timing measurements.*/
    long long foundBest = 0, foundAvg = 0, foundWorst = 0;

    // --- 1. BEST CASE ---
    cout << "Running Best Case tests...\n";
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
    {
        if (ht.search(bestCaseKey))
            foundBest++;
    }
    auto end = chrono::high_resolution_clock::now();
    double bestTime = chrono::duration<double>(end - start).count();

    // --- 2. AVERAGE CASE ---
    cout << "Running Average Case tests...\n";
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
    {
        if (ht.search(keys[i]))
            foundAvg++;
    }
    end = chrono::high_resolution_clock::now();
    double avgTime = chrono::duration<double>(end - start).count();

    // --- 3. WORST CASE ---
    cout << "Running Worst Case tests...\n";
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
    {
        if (ht.search(worstCaseKey))
            foundWorst++;
    }
    end = chrono::high_resolution_clock::now();
    double worstTime = chrono::duration<double>(end - start).count();

    // Output results to file
    string outFilename = "hash_table_search_dataset_" + datasetSize + ".txt";
    ofstream outFile(outFilename);
    outFile << "Best case time: " << bestTime << " seconds\n";
    outFile << "Average case time: " << avgTime << " seconds\n";
    outFile << "Worst case time: " << worstTime << " seconds\n";
    outFile.close();

    // Print summary to terminal
    cout << "\n----------------------------------------\n";
    cout << "SEARCH BENCHMARK SUCCESSFUL!\n";
    cout << "Best case time: " << bestTime << " seconds\n";
    cout << "Average case time: " << avgTime << " seconds\n";
    cout << "Worst case time: " << worstTime << " seconds\n";
    cout << "Output saved to: " << outFilename << "\n";

    // Printing the verification values ensures the compiler executes the search operations
    cout << "[Verification] Best found: " << foundBest << " | Avg found: " << foundAvg << " | Worst found: " << foundWorst << "\n";
    cout << "----------------------------------------\n";

    return 0;
}