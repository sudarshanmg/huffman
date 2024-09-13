/* CREATED AND DEVELOPED BY SUDARSHAN 
 *
 * 1. Node class is defined to contain the nodes of the huffman tree
 * 2. Leaf nodes consist of a char and its count
 * 3. Internal nodes only consist of count (char is NULL)
 * */

#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <vector>
#include <fstream>

#define umap unordered_map
#define rcast reinterpret_cast
using namespace std;

class Node {
	public:
		char ch;
		int freq;
		Node* left;
		Node* right;

		Node(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
		Node(int frequency, Node* l, Node* r) : ch('\0'), freq(frequency), left(l), right(r) {}
};

class Huffman {
	private:

		// comparator for the priority_queue
		class Compare {
			public:
				bool operator()(Node* a, Node* b) {
					return a->freq > b->freq;
				}
		};

		// uses DFS to traverse the tree and assign the code to the char present in the leaf node
		void generateCodes(Node* root, string code, umap<char, string>& huffmanCodes) {
			if (!root) return;
			if (!root->left && !root->right) {
				huffmanCodes[root->ch] = code;
			}
			generateCodes(root->left, code + "0", huffmanCodes);
			generateCodes(root->right, code + "1", huffmanCodes);
		}

		// used in the destructor to free the memory by traversing the tree
		void deleteTree(Node* node) {
			if (!node) return;
			deleteTree(node->left);
			deleteTree(node->right);
			delete node;
		}

		// converts a binary string to a vector of bytes
		vector<uint8_t> binaryStringToBytes(const string& binaryStr) {
			vector<uint8_t> byteArray;

			for(size_t i=0; i < binaryStr.size(); i += 8) {

				// extracts the 8-bits segment
				string byteStr = binaryStr.substr(i, 8);

				// pads the last byte if its less than 8 bits of length
				if(byteStr.size() < 8) {
					byteStr.append(8 - byteStr.size(), '0');
				}
				
				// converts binary string to a byte
				uint8_t byte = bitset<8>(byteStr).to_ulong();
				byteArray.push_back(byte);
			}

			return byteArray;
		}

		Node* root;

	public:
		Huffman() : root(nullptr) {}

		~Huffman() {
			deleteTree(root);
		}

		umap<char, string> buildHuffmanTree(const string& str);

		void printHuffmanCodes(const umap<char, string>& huffmanCodes) const;

		umap<string, char> buildReverseMapping(const umap<char, string>& huffmanCodes);

		string encode(const string& str, const umap<char, string>& huffmanCodes);

		string decode(const string& encodedData, const umap<string, char>& reverseMapping);
		
		size_t calculateSize(const string& encodedData, const umap<char, string>& huffmanCodes);

		void compressFile(const string& inputFilename, const string& outputFilename);
		
		void decompressFile(const string& inputFilename, const string& outputFilename);
};

#endif
