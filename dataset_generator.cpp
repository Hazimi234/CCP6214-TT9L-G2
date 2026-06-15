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
#include <unordered_set>
#include <chrono>

using namespace std;

int main() {
    // 1. Initialize Random Engine with leader Student ID seed
    mt19937_64 rng(2421324461U); 

    // 2. Define ranges for the 10-digit integer and 5 lowercase letters
    uniform_int_distribution<long long> intDist(1000000000LL, 9999999999LL);
    uniform_int_distribution<int> charDist('a', 'z');

    long long n;
    cout << "Enter the desired dataset size (e.g., 1000000): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid input. Please enter a positive number." << endl;
        return 1;
    }

    // 3. Create the output file based on user input size
    string filename = "dataset_" + to_string(n) + ".csv";
    ofstream outFile(filename);

    if (!outFile.is_open()) {
        cout << "Error: Could not create the file " << filename << "\n";
        return 1;
    }

    // 4. Track generated integers to ensure 100% uniqueness
    unordered_set<long long> generatedInts;
    generatedInts.reserve(n); // Pre-allocate memory for better performance

    long long count = 0;
    
    cout << "Generating " << n << " unique records. Please wait...\n";
    auto start_time = chrono::high_resolution_clock::now();

    while (count < n) {
        long long randomInt = intDist(rng);

        // Check if the integer is truly unique before writing
        if (generatedInts.find(randomInt) == generatedInts.end()) {
            generatedInts.insert(randomInt); // Add to tracking set

            // Generate the random 5-letter lowercase string
            string randomStr = "";
            for (int i = 0; i < 5; ++i) {
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