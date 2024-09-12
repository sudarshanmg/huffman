#ifndef HUFFMAN_HPP
#define HUFFMAN_CPP

#include <iostream>
#include <unordered_map>
#include <queue>

#define umap unordered_map

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
		class Compare {
			public:
				bool operator()(Node* a, Node* b) {
					return a->freq > b->freq;
				}
		};

		void generateCodes(Node* root, string code, umap<char, string>& huffmanCodes) {
			if (!root) return;
			if (!root->left && !root->right) {
				huffmanCodes[root->ch] = code;
			}
			generateCodes(root->left, code + "0", huffmanCodes);
			generateCodes(root->right, code + "1", huffmanCodes);
		}

		void deleteTree(Node* node) {
			if (!node) return;
			deleteTree(node->left);
			deleteTree(node->right);
			delete node;
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
};

#endif
