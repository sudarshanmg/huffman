#include <iostream>
#include <unordered_map>
#include <queue>
#include "huffman.hpp"

using namespace std;



int main() {
	string str;

	cout << "Enter string: ";
	getline(cin, str);

	Huffman huffman;
	umap<char, string> huffmanCodes = huffman.buildHuffmanTree(str);

	huffman.printHuffmanCodes(huffmanCodes);

	string encodedData = huffman.encode(str, huffmanCodes);
	size_t compressedSize = huffman.calculateSize(encodedData, huffmanCodes);


	cout << "No. of bytes (before compression): " << str.size() << endl;

	// uncomment the below line to see the encoded data
	// cout << "Encoded data: " << encodedData << endl;
	cout << "No. of bytes (after compression): " << compressedSize << endl;
	cout << "Size reduction: " << (1 - static_cast<double>(compressedSize) / static_cast<double>(str.size()))*100 << "%" << endl;

	// uncomment the below line to check if the input str is same as decoded data
	// cout << "Decoded data: " << huffman.decode(encodedData, huffman.buildReverseMapping(huffmanCodes)) << endl;

}
