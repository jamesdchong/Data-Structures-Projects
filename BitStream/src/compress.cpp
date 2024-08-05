/**
 * File that compresses text files and writes to a different file containing a
 * header with each ascii value's frequency, then the encoding of each letter
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

#define ARG_ONE 1
#define ARG_TWO 2
#define ARG_THREE 3
#define ASCII_COUNT 256

/* Uses pseudo compression to create header of each ascii value's frequency,
 * then the encoding of each letter into the compressed file
 * inFileName: file to be compressed
 * outFileName: compressed file this function is writing to
 */
void pseudoCompression(string inFileName, string outFileName) {
    unsigned char nextChar;
    int nextByte;

    // Open file to be compressed
    ifstream inFile;
    inFile.open(inFileName, ios::binary);

    // Vector to store frequencies of each ascii value in input file
    vector<unsigned int> freqs(ASCII_COUNT);

    // Add frequencies of each char into freqs vector
    while ((nextByte = inFile.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        freqs[nextChar]++;
    }

    // Open compressed file
    ofstream outFile;
    outFile.open(outFileName, ios::binary);

    // Write frequencies of each ascii value to compressed file as a header
    for (int i = 0; i < freqs.size(); i++) {
        outFile << freqs[i] << endl;
    }

    // Close input file
    inFile.close();

    // Reopen input file to start reading again
    inFile.open(inFileName, ios::binary);

    // Build HCTree using freqs vector
    HCTree tree;
    tree.build(freqs);

    // Encode each character and write to compressed file
    while ((nextByte = inFile.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        tree.encode(nextChar, outFile);
        outFile << endl;
    }

    // Close input and output files
    outFile.close();
    inFile.close();
}

/* True compression with bitwise i/o and small header
 * inFileName: file to be compressed
 * outFileName: file this function is writing to */
void trueCompression(string inFileName, string outFileName) {
    unsigned char nextChar;
    int nextByte;

    // Open file to be compressed
    ifstream inFile;
    inFile.open(inFileName, ios::binary);

    // Vector to store frequencies of each ascii value in input file
    vector<unsigned int> freqs(ASCII_COUNT);

    // Add frequencies of each char into freqs vector
    while ((nextByte = inFile.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        freqs[nextChar]++;
    }

    // Open compressed file
    ofstream outFile;
    outFile.open(outFileName, ios::binary);

    // Write frequencies of each ascii value to compressed file as a header
    for (int i = 0; i < freqs.size(); i++) {
        outFile << freqs[i] << endl;
    }

    // Close input file
    inFile.close();

    // Reopen input file to start reading again
    inFile.open(inFileName, ios::binary);

    // Build HCTree using freqs vector
    HCTree tree;
    tree.build(freqs);

    // Create BitOutputStream to encode chars
    BitOutputStream bos(outFile);

    // Encode each character and write to compressed file
    while ((nextByte = inFile.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        tree.encode(nextChar, bos);
    }

    // Flush the final buffer to ostream
    bos.flush();

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
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");
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

    // If ascii flag appears, perform pseudoCompression
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

        // Perform pseudoCompression with the two files given
        pseudoCompression(argv[ARG_TWO], argv[ARG_THREE]);

        // If no ascii flag, perform true compression
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

        // Performs true compression
        trueCompression(argv[ARG_ONE], argv[ARG_TWO]);
    }

    return 0;
}