/* CREATED && DEVELOPED BY SUDARSHAN */

#include <iostream>
#include <fstream>
#include <string>
#include "huffman.hpp" 

using namespace std;

int main(int argc, char* argv[]) {

		// demonstrates basic usage of file compression and extraction
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <compress|decompress> <inputfile> <outputfile>" << endl;
        return 1;
    }

    string mode = argv[1];
    string inputFilename = argv[2];
    string outputFilename = argv[3];

    if (mode != "compress" && mode != "decompress") {
        cerr << "Invalid mode. Use 'compress' or 'decompress'." << endl;
        return 1;
    }

    // Check if the input file exists
    ifstream inputFile(inputFilename, ios::binary);
    if (!inputFile) {
        cerr << "Error: Input file \"" << inputFilename << "\" could not be opened or does not exist." << endl;
        return 1;
    }
    inputFile.close(); // Close the file since it's only being checked for existence

    Huffman huffman;

    try {
        if (mode == "compress") {
            // Compress the file
            huffman.compressFile(inputFilename, outputFilename);
            cout << "File compressed successfully!" << endl;
        } else if (mode == "decompress") {
            // Decompress the file
            huffman.decompressFile(inputFilename, outputFilename);
            cout << "File decompressed successfully!" << endl;
        }
    } catch (const ifstream::failure& e) {
        cerr << "File error: " << e.what() << endl;
        return 1;
    } catch (const runtime_error& e) {
        cerr << "Runtime error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "An unexpected error occurred." << endl;
        return 1;
    }

    return 0;
}

