#include "huffman.hpp"

// builds the tree using a min-heap (priority queue)
umap<char, string> Huffman :: buildHuffmanTree(const string& str) {
	umap<char, int> freqMap;

	// step 1: create a hash table to store the char count
	for (char c : str) freqMap[c]++;
	
	// create a min-heap and add all the pairs from the hash-table to the heap
	priority_queue<Node*, vector<Node*>, Compare> pq;

	for (const auto& p : freqMap) pq.push(new Node(p.first, p.second));

	// build the tree
	// checks if only one unique char (repeating/non-repeating) is present in the input str
	// if so then, return the huffmanCodes as {<char>: '0'}
	if (pq.size() == 1) {
		Node* onlyNode = pq.top();
		pq.pop();
		root = onlyNode;

		umap<char, string> huffmanCodes;
		huffmanCodes[root->ch] = '0';
		return huffmanCodes;
	} else {
		while (pq.size() > 1) {
			Node* left = pq.top(); pq.pop();
			Node* right = pq.top(); pq.pop();

			Node* internalNode = new Node(left->freq + right->freq, left, right);
			pq.push(internalNode);
		}

		root = pq.top();
	}

	umap<char, string> huffmanCodes;
	generateCodes(root, "", huffmanCodes);

	return huffmanCodes;
}

void Huffman :: printHuffmanCodes(const umap<char, string>& huffmanCodes) const {
	cout << "Huffman codes: " << endl;
	for (const auto& p : huffmanCodes) {
		cout << p.first << ": " << p.second << endl;
	}
}

// used for decoding the string
umap<string, char> Huffman :: buildReverseMapping(const umap<char, string>& huffmanCodes) {
	umap<string, char> reverseMapping;
	for (const auto& p : huffmanCodes) {
		reverseMapping[p.second] = p.first;
	}
	return reverseMapping;
}

string Huffman :: encode(const string& str, const umap<char, string>& huffmanCodes) {
	string encodedData;
	for (char c : str) {
		encodedData += huffmanCodes.at(c);
	}
	return encodedData;
}

string Huffman :: decode(const string& encodedData, const umap<string, char>& reverseMapping) {
	string decodedString;
	string currentCode;
	for (char bit : encodedData) {
		currentCode += bit;
		if (reverseMapping.find(currentCode) != reverseMapping.end()) {
			decodedString += reverseMapping.at(currentCode);
			currentCode.clear();
		}
	}
	return decodedString;
}

size_t Huffman :: calculateSize(const string& encodedData, const umap<char, string>& huffmanCodes) {
	size_t mapSize = huffmanCodes.size();

	int bits = 0;
	for (const auto& p : huffmanCodes) {
		bits += p.second.size();
	}

	mapSize += (bits + encodedData.size()) / 8;
	return mapSize;
}

