/**
 * File that decompresses a compressed file and writes to a different file
 * containing a the original data before it was compressed
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#include <fstream>
#include <iostream>
#include <string>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

#define ARG_ONE 1
#define ARG_TWO 2
#define ARG_THREE 3
#define ASCII_COUNT 256

/* Pseudo decompression with ascii encoding and naive header */
void pseudoDecompression(string inFileName, string outFileName) {
    // Open compressed file
    ifstream inFile;
    inFile.open(inFileName, ios::binary);

    // Vector to store frequencies of each ascii value
    vector<unsigned int> freqs(ASCII_COUNT);

    // Get the count of each line and add to freq vector
    for (int i = 0; i < freqs.size(); i++) {
        string s;
        getline(inFile, s);
        int frequency = stoul(s);
        freqs[i] = frequency;
    }

    // Build HCTree using freqs vector
    HCTree tree;
    tree.build(freqs);

    // Open file to write uncompressed data
    ofstream outFile;
    outFile.open(outFileName, ios::binary);

    // Decode the compressed file until EOF
    while (inFile.peek() != EOF) {
        outFile << tree.decode(inFile);
    }

    // Close input and output files
    outFile.close();
    inFile.close();
}

/* True decompression with bitwise i/o and small header */
void trueDecompression(string inFileName, string outFileName) {
    // Open compressed file
    ifstream inFile;
    inFile.open(inFileName, ios::binary);

    // Vector to store frequencies of each ascii value
    vector<unsigned int> freqs(ASCII_COUNT);

    int totalFrequency = 0;

    // Get count of each ascii value in header and add to freqs vector
    for (int i = 0; i < freqs.size(); i++) {
        string s;
        getline(inFile, s);
        int frequency = stoul(s);
        freqs[i] = frequency;
        totalFrequency += frequency;
    }

    // Build HCTree using freqs vector
    HCTree tree;
    tree.build(freqs);

    // Open file to write uncompressed data
    ofstream outFile;
    outFile.open(outFileName, ios::binary);

    // Create BitInputStream to decode
    BitInputStream bis(inFile);

    // Decode each bit until reach the total number of chars
    while (totalFrequency > 0) {
        outFile << tree.decode(bis);
        totalFrequency--;
    }

    // Close input and output files
    outFile.close();
    inFile.close();
}

/* Main program that runs the compress
 * argc: number of arguments inputted
 * argv: array containing arguments inputted
 */
int main(int argc, char* argv[]) {
    // Uses cxxopts to find --ascii flag
    cxxopts::Options options("./uncompress",
                             "Uncompresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }

    // If ascii flag appears, perfrom pseudoDecompression
    if (isAsciiOutput) {
        // Checks for valid file
        if (!FileUtils::isValidFile(argv[ARG_TWO])) {
            return -1;
        }

        // Checks if file is empty
        if (FileUtils::isEmptyFile(argv[ARG_TWO])) {
            ofstream outFile;
            outFile.open(argv[ARG_THREE], ios::binary);
            outFile.close();
            return 0;
        }

        // Perform pseudoDecompression
        pseudoDecompression(argv[ARG_TWO], argv[ARG_THREE]);

        // If no ascii flag, perfrom true decompression
    } else {
        // Checks for valid file
        if (!FileUtils::isValidFile(argv[ARG_ONE])) {
            return -1;
        }

        // Checks if file is empty
        if (FileUtils::isEmptyFile(argv[ARG_ONE])) {
            ofstream outFile;
            outFile.open(argv[ARG_TWO], ios::binary);
            outFile.close();
            return 0;
        }

        // Performs true decompression
        trueDecompression(argv[ARG_ONE], argv[ARG_TWO]);
    }

    return 0;
}
