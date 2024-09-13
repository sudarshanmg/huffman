/* DESIGNED && DEVELOPED BY SUDARSHAN
 *
 * Functions to do the following operations:
 * 1. Build the tree
 * 2. Traverse the tree and generate the huffman codes for each char (useing DFS)
 * 3. Print the huffman codes
 * 4. Encode/Decode a string (yes you can use it to compress a string as well)
 * 5. Build reverse map used to decompress a file/string
 * 6. Encode/Decode a file
 * 7. Calculate the compression ratio of the encoded string and the original string
 * */

#include "huffman.hpp"

// builds the tree using a min-heap (priority queue)
umap<char, string> Huffman :: buildHuffmanTree(const string& str) {
	umap<char, int> freqMap;

	// create a hash table to store the char count
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

void Huffman :: compressFile(const string& inputFilename, const string& outputFilename) {

		// reads the file content and stores it into a string
		ifstream inputFile(inputFilename, ios::binary);
		string inputData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
		inputFile.close();
		
		// builds the tree and encodes the input string
		umap<char, string> huffmanCodes = buildHuffmanTree(inputData);
		string encodedData = encode(inputData, huffmanCodes);

		// converts binary string to bytes
		vector<uint8_t> compressedData = binaryStringToBytes(encodedData);
		
		// writes the huffmanCodes and compressData to the output file
		ofstream outputFile(outputFilename, ios::binary);

		// writes the map size to the o/p file
		size_t freqMapSize = huffmanCodes.size();
		outputFile.write(rcast<const char*>(&freqMapSize), sizeof(freqMapSize));

		// writes the frequency map to the o/p file (char and its code length)
		for(auto& p: huffmanCodes) {
			outputFile.write(rcast<const char*> (&p.first), sizeof(p.first));
			size_t codeLength = p.second.size();
			outputFile.write(rcast<const char*> (&codeLength), sizeof(codeLength));
			outputFile.write(p.second.data(), codeLength);
		}

		outputFile.write(rcast<const char*>(compressedData.data()), compressedData.size());
		outputFile.close();

}

void Huffman :: decompressFile(const string& inputFilename, const string& outputFilename) {

	ifstream inputFile(inputFilename, ios::binary);

	size_t freqMapSize;
	inputFile.read(rcast<char *> (&freqMapSize), sizeof(freqMapSize));

	umap<char, string> huffmanCodes;
	for(size_t i=0; i< freqMapSize; ++i) {
		char ch;
		size_t codeLength;
		inputFile.read(rcast<char *>(&ch), sizeof(ch));
		inputFile.read(rcast<char *>(&codeLength), sizeof(codeLength));
		
		string code(codeLength, '0');
		inputFile.read(&code[0], codeLength);
		huffmanCodes[ch] = code;
	}

	umap<string, char> reverseMapping = buildReverseMapping(huffmanCodes);

	vector<uint8_t> compressedData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
	inputFile.close();

	string encodedBinaryString;
	for(uint8_t byte : compressedData) {
		encodedBinaryString += bitset<8>(byte).to_string();
	}

	string decodedData = decode(encodedBinaryString, reverseMapping);

	ofstream outputFile(outputFilename, ios::binary);
	outputFile.write(decodedData.c_str(), decodedData.size());
	outputFile.close();
}
