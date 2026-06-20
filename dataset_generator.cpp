// *********************************************************
// Program: dataset_generator.cpp
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
#include <string>
#include <random>
#include <vector>
#include <chrono>

using namespace std;

int main()
{
    // 1. Initialize Random Engine with your specific Student ID seed
    mt19937_64 rng(2421324461U);

    // 2. Define ranges for the 10-digit integer and 5 lowercase letters
    uniform_int_distribution<long long> intDist(1000000000LL, 9999999999LL);
    uniform_int_distribution<int> charDist('a', 'z');

    long long n;
    cout << "Enter the desired dataset size (e.g., 1000000): ";
    if (!(cin >> n) || n <= 0)
    {
        cout << "Invalid input. Please enter a positive number." << endl;
        return 1;
    }

    string filename = "dataset_" + to_string(n) + ".csv";
    ofstream outFile(filename);

    if (!outFile.is_open())
    {
        cout << "Error: Could not create the file " << filename << "\n";
        return 1;
    }

    // SENIOR DEV OPTIMIZATION:
    // Instead of an unordered_set which takes ~24 bytes per entry and will crash your RAM,
    // we use a vector<bool> (Bit Array) that maps every single possible 10-digit number to a single bit.
    // 9,000,000,000 bits = exactly 1.125 GB of RAM used, perfectly safe for your 16GB laptop!
    cout << "Allocating memory for uniqueness tracking" << endl;
    vector<bool> usedIDs(9000000000ULL, false);

    long long count = 0;

    cout << "Generating " << n << " unique records. Please wait...\n";
    auto start_time = chrono::high_resolution_clock::now();

    while (count < n)
    {
        long long randomInt = intDist(rng);

        // Map the 10-digit number (1 billion to 9.99 billion) to an index (0 to 8.99 billion)
        long long index = randomInt - 1000000000LL;

        // Check if the bit is false (meaning the integer is truly unique)
        if (!usedIDs[index])
        {
            usedIDs[index] = true; // Flip the bit to true so we never use it again

            // Generate the random 5-letter lowercase string
            string randomStr = "";
            for (int i = 0; i < 5; ++i)
            {
                randomStr += (char)charDist(rng);
            }

            // Write to the CSV file formatted as "integer,string"
            outFile << randomInt << "," << randomStr << "\n";
            count++;
        }
    }

    outFile.close();
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;

    cout << "Success! Generated " << filename << " in " << duration.count() << " seconds.\n";

    return 0;
}