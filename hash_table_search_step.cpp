// *********************************************************
// Program: hash_table_search_step.cpp
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

// Custom Node for our Array-Based Hash Table
struct HashNode
{
    long long id = -1;
    string str = "";
    bool isOccupied = false;
};

// Custom Hash Table Class using Linear Probing for Collisions
class HashTable
{
private:
    HashNode *table;
    int capacity;

public:
    // Initialize with double the dataset size to keep load factor around 0.5
    HashTable(int size)
    {
        capacity = size * 2;
        table = new HashNode[capacity];
    }

    ~HashTable()
    {
        delete[] table;
    }

    // The "Magic Formula" - Modulo hashing
    int hashFunction(long long key)
    {
        return key % capacity;
    }

    // Insert using Linear Probing
    void insert(long long key, string value)
    {
        int index = hashFunction(key);
        // If collision occurs, move to the next available slot
        while (table[index].isOccupied)
        {
            index = (index + 1) % capacity;
        }
        table[index].id = key;
        table[index].str = value;
        table[index].isOccupied = true;
    }

    // Search function that returns the formatted string for the step output
    string searchTarget(long long key)
    {
        int index = hashFunction(key);
        int startIndex = index;

        while (table[index].isOccupied)
        {
            if (table[index].id == key)
            {
                // Target Found
                return to_string(key) + " = " + to_string(key) + "/" + table[index].str;
            }
            index = (index + 1) % capacity;
            if (index == startIndex)
                break; // We looped the whole table
        }
        // Target Not Found
        return "-1 != " + to_string(key);
    }
};

int main()
{
    // ---- TUTOR SPECIFIED VARIABLES ----
    string inputFilename = "dataset_1000.csv";
    // You will need to change 'foundTarget' to a real ID that exists in your specific dataset_1000.csv
    long long foundTarget = 7666333391LL;
    long long notFoundTarget = 123456789LL;
    // -----------------------------------

    ifstream inFile(inputFilename);
    if (!inFile.is_open())
    {
        cout << "Error: Could not open " << inputFilename << "\n";
        return 1;
    }

    // 1. Read dataset and count elements
    vector<pair<long long, string>> records;
    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string idStr, textStr;
        getline(ss, idStr, ',');
        getline(ss, textStr, ',');
        records.push_back({stoll(idStr), textStr});
    }
    inFile.close();

    // 2. Build the custom Hash Table
    HashTable ht(records.size());
    for (const auto &rec : records)
    {
        ht.insert(rec.first, rec.second);
    }

    // 3. Perform Searches and Write Outputs

    // Process Found Target
    string outFound = "dataset_1000_hash_table_search_step_" + to_string(foundTarget) + ".txt";
    ofstream fileFound(outFound);
    string resultFound = ht.searchTarget(foundTarget);
    fileFound << resultFound << "\n";
    fileFound.close();
    cout << "Generated: " << outFound << " -> " << resultFound << "\n";

    // Process Not Found Target
    string outNotFound = "dataset_1000_hash_table_search_step_" + to_string(notFoundTarget) + ".txt";
    ofstream fileNotFound(outNotFound);
    string resultNotFound = ht.searchTarget(notFoundTarget);
    fileNotFound << resultNotFound << "\n";
    fileNotFound.close();
    cout << "Generated: " << outNotFound << " -> " << resultNotFound << "\n";

    return 0;
}