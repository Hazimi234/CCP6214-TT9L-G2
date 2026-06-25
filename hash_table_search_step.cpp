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

// Structure to represent a single slot in the hash table
struct HashNode
{
    long long id = -1;
    string str = "";
    bool isOccupied = false;
};

// Array-based Hash Table using Linear Probing for collision resolution
class HashTable
{
private:
    HashNode *table;
    int capacity;

public:
    // Initialize table size to double the dataset size to maintain a load factor of 0.5
    HashTable(int size)
    {
        capacity = size * 2;
        table = new HashNode[capacity];
    }

    ~HashTable()
    {
        delete[] table;
    }

    // Basic modulo hash function
    int hashFunction(long long key)
    {
        return key % capacity;
    }

    // Insert function handling collisions via linear probing
    void insert(long long key, string value)
    {
        int index = hashFunction(key);
        // Move to the next available slot if current index is occupied
        while (table[index].isOccupied)
        {
            index = (index + 1) % capacity;
        }
        table[index].id = key;
        table[index].str = value;
        table[index].isOccupied = true;
    }

    // Search function tailored to return the specific string format required for step tracking
    string searchTarget(long long key)
    {
        int index = hashFunction(key);
        int startIndex = index;

        while (table[index].isOccupied)
        {
            if (table[index].id == key)
            {
                // Target successfully found
                return to_string(key) + " = " + to_string(key) + "/" + table[index].str;
            }
            index = (index + 1) % capacity;
            if (index == startIndex)
                break; // Terminate if the entire table has been traversed
        }
        // Target does not exist in the table
        return "-1 != " + to_string(key);
    }
};

int main()
{
    // Variables specified for step tracking output
    string inputFilename = "dataset_1000.csv";

    // Testing targets (foundTarget must be updated to a real ID from the dataset)
    long long foundTarget = 7666333391LL;
    long long notFoundTarget = 123456789LL;

    ifstream inFile(inputFilename);
    if (!inFile.is_open())
    {
        cout << "Error: Could not open " << inputFilename << "\n";
        return 1;
    }

    // 1. Read dataset and populate the records vector
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

    // 2. Build and populate the custom Hash Table
    HashTable ht(records.size());
    for (const auto &rec : records)
    {
        ht.insert(rec.first, rec.second);
    }

    // 3. Execute searches and generate output files

    // Process the existing target
    string outFound = "dataset_1000_hash_table_search_step_" + to_string(foundTarget) + ".txt";
    ofstream fileFound(outFound);
    string resultFound = ht.searchTarget(foundTarget);
    fileFound << resultFound << "\n";
    fileFound.close();
    cout << "Generated: " << outFound << " -> " << resultFound << "\n";

    // Process the non-existent target
    string outNotFound = "dataset_1000_hash_table_search_step_" + to_string(notFoundTarget) + ".txt";
    ofstream fileNotFound(outNotFound);
    string resultNotFound = ht.searchTarget(notFoundTarget);
    fileNotFound << resultNotFound << "\n";
    fileNotFound.close();
    cout << "Generated: " << outNotFound << " -> " << resultNotFound << "\n";

    return 0;
}