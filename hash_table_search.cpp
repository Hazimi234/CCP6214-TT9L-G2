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

struct HashNode
{
    long long id = -1;
    string str = "";
    bool isOccupied = false;
};

class HashTable
{
public: // Made public purely for the benchmark to find the worst-case cluster
    HashNode *table;
    int capacity;

    HashTable(int size)
    {
        capacity = size * 2;
        table = new HashNode[capacity];
    }
    ~HashTable() { delete[] table; }

    int hashFunction(long long key) { return key % capacity; }

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

    // Helper to find a key that will trigger the absolute worst-case probing sequence
    long long generateWorstCaseKey()
    {
        int maxLen = 0, bestStart = 0;
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
        // Generate a non-existent 10-digit key that hashes to 'bestStart'
        long long fakeKey = bestStart;
        while (fakeKey < 1000000000LL)
            fakeKey += capacity;
        fakeKey += (capacity * 5LL); // Ensure it's not in the dataset
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

    // Build the Hash Table
    cout << "Building Hash Table...\n";
    HashTable ht(N);
    for (long long key : keys)
    {
        ht.insert(key, "dummy"); // We only need the ID for search timing
    }

    // --- SETUP TEST CASES ---
    long long bestCaseKey = keys[0];                    // First item inserted usually has 0 collisions
    long long worstCaseKey = ht.generateWorstCaseKey(); // Forces massive probing

    // --- 1. BEST CASE ---
    cout << "Running Best Case tests...\n";
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
        ht.search(bestCaseKey);
    auto end = chrono::high_resolution_clock::now();
    double bestTime = chrono::duration<double>(end - start).count();

    // --- 2. AVERAGE CASE ---
    cout << "Running Average Case tests...\n";
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
        ht.search(keys[i]);
    end = chrono::high_resolution_clock::now();
    double avgTime = chrono::duration<double>(end - start).count();

    // --- 3. WORST CASE ---
    cout << "Running Worst Case tests...\n";
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
        ht.search(worstCaseKey);
    end = chrono::high_resolution_clock::now();
    double worstTime = chrono::duration<double>(end - start).count();

    // --- OUTPUT ---
    string outFilename = "hash_table_search_dataset_" + datasetSize + ".txt";
    ofstream outFile(outFilename);
    outFile << "Best case time: " << bestTime << " seconds\n";
    outFile << "Average case time: " << avgTime << " seconds\n";
    outFile << "Worst case time: " << worstTime << " seconds\n";
    outFile.close();

    cout << "\n----------------------------------------\n";
    cout << "SEARCH BENCHMARK SUCCESSFUL!\n";
    cout << "Best case time: " << bestTime << " seconds\n";
    cout << "Average case time: " << avgTime << " seconds\n";
    cout << "Worst case time: " << worstTime << " seconds\n";
    cout << "Output saved to: " << outFilename << "\n";
    cout << "----------------------------------------\n";
    cout << "Take a screenshot of this command prompt window!\n";

    return 0;
}