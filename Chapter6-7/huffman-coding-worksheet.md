# Huffman Coding Algorithm - Complete Theory & Practice Worksheet

---

## Table of Contents
1. [Theoretical Foundation](#theoretical-foundation)
2. [Algorithm Analysis](#algorithm-analysis)
3. [Complete Implementation](#complete-implementation)
4. [Practice Problems](#practice-problems)
5. [Advanced Topics](#advanced-topics)

---

## Theoretical Foundation

### What is Huffman Coding?

Huffman coding is a **lossless data compression algorithm** that uses **variable-length codes** for different characters based on their frequencies. Characters that occur more frequently are assigned shorter codes, while less frequent characters get longer codes.

### Historical Background

- **Developed by**: David A. Huffman in 1952 (while he was a PhD student at MIT)
- **Problem Origin**: Given by Professor Robert M. Fano as a term paper assignment
- **Achievement**: Huffman's solution was provably optimal and simpler than Fano's own method

### Key Concepts

#### 1. **Prefix-Free Codes (Prefix Property)**
No code word is a prefix of another code word. This ensures unique decodability without delimiters.

**Example**:
- Valid: {0, 10, 110, 111}
- Invalid: {0, 01, 10} (0 is prefix of 01)

#### 2. **Entropy and Information Theory**

**Shannon's Entropy**: The theoretical minimum average bits per symbol
```
H(X) = -Σ p(xi) × log₂(p(xi))
```

**Huffman Efficiency**: Always achieves compression within 1 bit of entropy
```
H(X) ≤ L < H(X) + 1
```
where L is the average code length

#### 3. **Optimality Conditions**

**Theorem (Huffman's Optimality)**: 
For any probability distribution, Huffman coding produces the optimal prefix-free code.

**Proof Sketch**:
1. The two least frequent symbols have the same code length
2. These symbols differ only in the last bit
3. Building the tree bottom-up ensures optimal structure

### Mathematical Foundation

#### Average Code Length
```
L = Σ (frequency[i] × codelength[i]) / Σ frequency[i]
```

#### Compression Ratio
```
Compression Ratio = (Original Size - Compressed Size) / Original Size × 100%
```

#### Space Savings
```
Original: n × ⌈log₂(alphabet_size)⌉ bits
Compressed: Σ (frequency[i] × codelength[i]) bits
```

---

## Algorithm Analysis

### The Huffman Algorithm - Step by Step

#### Phase 1: Building the Huffman Tree

1. **Count frequencies** of each character
2. **Create leaf nodes** for each character with its frequency
3. **Build a min-heap** with all leaf nodes
4. **Repeat until one node remains**:
   - Extract two nodes with minimum frequency
   - Create internal node with frequency = sum of children
   - Insert internal node back to heap
5. **Root of tree** is the final remaining node

#### Phase 2: Generating Codes

1. **Traverse tree** from root to leaves
2. **Assign codes**:
   - Left edge = 0
   - Right edge = 1
3. **Store codes** for each character

#### Phase 3: Encoding

Replace each character with its Huffman code

#### Phase 4: Decoding

Start from root, follow path based on bits until reaching leaf

### Time Complexity Analysis

| Operation | Complexity | Explanation |
|-----------|------------|-------------|
| Frequency Count | O(n) | Scan input once |
| Build Heap | O(k log k) | k unique characters |
| Build Tree | O(k log k) | k-1 merge operations |
| Generate Codes | O(k × h) | h = tree height |
| Encoding | O(n) | Process each character |
| Decoding | O(m) | m = encoded length |

**Overall**: O(n + k log k) where n = input size, k = alphabet size

### Space Complexity

- **Huffman Tree**: O(k) nodes
- **Code Table**: O(k) entries
- **Encoded Data**: Variable, typically < n

---

## Complete Implementation

### Full C++ Implementation with Detailed Comments

```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

/*
 * HUFFMAN CODING - COMPLETE IMPLEMENTATION
 * =========================================
 * This implementation includes:
 * 1. Frequency analysis
 * 2. Huffman tree construction
 * 3. Code generation
 * 4. Encoding and decoding
 * 5. Compression statistics
 * 6. Binary file I/O
 * 7. Tree visualization
 */

// Node structure for Huffman tree
struct HuffmanNode {
    char character;           // Character (for leaf nodes)
    int frequency;           // Frequency count
    HuffmanNode* left;       // Left child
    HuffmanNode* right;      // Right child
    
    // Constructor for leaf node
    HuffmanNode(char ch, int freq) {
        character = ch;
        frequency = freq;
        left = right = nullptr;
    }
    
    // Constructor for internal node
    HuffmanNode(int freq, HuffmanNode* l, HuffmanNode* r) {
        character = '\0';    // Internal nodes have no character
        frequency = freq;
        left = l;
        right = r;
    }
    
    // Check if node is leaf
    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

// Comparator for min-heap (priority queue)
struct CompareNodes {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        // Higher frequency = lower priority (min-heap)
        // If frequencies are equal, compare characters for consistency
        if (a->frequency == b->frequency) {
            return a->character > b->character;
        }
        return a->frequency > b->frequency;
    }
};

class HuffmanCoding {
private:
    HuffmanNode* root;                              // Root of Huffman tree
    unordered_map<char, string> huffmanCodes;       // Character to code mapping
    unordered_map<string, char> reverseHuffmanCodes; // Code to character mapping
    unordered_map<char, int> frequencyTable;        // Character frequencies
    string originalText;                            // Original input text
    string encodedText;                             // Encoded binary string
    
    // Helper function to build frequency table
    void buildFrequencyTable(const string& text) {
        cout << "\n=== STEP 1: FREQUENCY ANALYSIS ===" << endl;
        cout << "Analyzing " << text.length() << " characters..." << endl;
        
        frequencyTable.clear();
        for (char ch : text) {
            frequencyTable[ch]++;
        }
        
        // Display frequency table
        cout << "\nFrequency Table:" << endl;
        cout << "Char\tASCII\tFreq\tProbability" << endl;
        cout << "----\t-----\t----\t-----------" << endl;
        
        for (auto& pair : frequencyTable) {
            double probability = (double)pair.second / text.length();
            cout << "'" << pair.first << "'\t" 
                 << (int)pair.first << "\t" 
                 << pair.second << "\t" 
                 << fixed << setprecision(4) << probability << endl;
        }
        
        // Calculate entropy
        double entropy = 0;
        for (auto& pair : frequencyTable) {
            double p = (double)pair.second / text.length();
            if (p > 0) {
                entropy -= p * log2(p);
            }
        }
        cout << "\nShannon Entropy: " << fixed << setprecision(4) 
             << entropy << " bits/symbol" << endl;
        cout << "Theoretical minimum average bits: " << entropy << endl;
    }
    
    // Build Huffman tree from frequency table
    HuffmanNode* buildHuffmanTree() {
        cout << "\n=== STEP 2: BUILDING HUFFMAN TREE ===" << endl;
        
        // Create min-heap (priority queue)
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> minHeap;
        
        // Create leaf nodes for each character
        cout << "Creating leaf nodes..." << endl;
        for (auto& pair : frequencyTable) {
            HuffmanNode* node = new HuffmanNode(pair.first, pair.second);
            minHeap.push(node);
            cout << "  Added leaf: '" << pair.first << "' (freq: " 
                 << pair.second << ")" << endl;
        }
        
        cout << "\nBuilding tree (bottom-up)..." << endl;
        int step = 1;
        
        // Build tree bottom-up
        while (minHeap.size() > 1) {
            // Extract two minimum frequency nodes
            HuffmanNode* left = minHeap.top();
            minHeap.pop();
            HuffmanNode* right = minHeap.top();
            minHeap.pop();
            
            // Create new internal node
            int sumFreq = left->frequency + right->frequency;
            HuffmanNode* internal = new HuffmanNode(sumFreq, left, right);
            
            cout << "  Step " << step++ << ": Merge nodes with freq " 
                 << left->frequency << " and " << right->frequency 
                 << " -> New node with freq " << sumFreq << endl;
            
            // Add back to heap
            minHeap.push(internal);
        }
        
        cout << "Tree construction complete!" << endl;
        return minHeap.top();
    }
    
    // Generate Huffman codes by traversing tree
    void generateCodes(HuffmanNode* node, string code) {
        if (node == nullptr) return;
        
        // If leaf node, store the code
        if (node->isLeaf()) {
            huffmanCodes[node->character] = code;
            reverseHuffmanCodes[code] = node->character;
            return;
        }
        
        // Traverse left (add '0') and right (add '1')
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }
    
    // Print tree structure (for visualization)
    void printTree(HuffmanNode* node, string prefix = "", bool isLeft = true) {
        if (node == nullptr) return;
        
        cout << prefix;
        cout << (isLeft ? "├── " : "└── ");
        
        if (node->isLeaf()) {
            cout << "'" << node->character << "' (" << node->frequency << ")" << endl;
        } else {
            cout << "[" << node->frequency << "]" << endl;
        }
        
        if (!node->isLeaf()) {
            printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
            printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
        }
    }
    
    // Delete tree (cleanup)
    void deleteTree(HuffmanNode* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    
public:
    // Constructor
    HuffmanCoding() : root(nullptr) {}
    
    // Destructor
    ~HuffmanCoding() {
        deleteTree(root);
    }
    
    // Getter for encoded text (PUBLIC METHOD TO ACCESS PRIVATE MEMBER)
    string getEncodedText() const {
        return encodedText;
    }
    
    // Main encoding function
    void encode(const string& text) {
        if (text.empty()) {
            cout << "Error: Empty input text!" << endl;
            return;
        }
        
        originalText = text;
        cout << "\n========================================" << endl;
        cout << "     HUFFMAN ENCODING PROCESS" << endl;
        cout << "========================================" << endl;
        cout << "Original text: \"" << text << "\"" << endl;
        cout << "Length: " << text.length() << " characters" << endl;
        
        // Step 1: Build frequency table
        buildFrequencyTable(text);
        
        // Handle special case: only one unique character
        if (frequencyTable.size() == 1) {
            cout << "\nSpecial case: Only one unique character!" << endl;
            char ch = frequencyTable.begin()->first;
            huffmanCodes[ch] = "0";
            reverseHuffmanCodes["0"] = ch;
            
            // Create encoded text
            encodedText = string(text.length(), '0');
        } else {
            // Step 2: Build Huffman tree
            root = buildHuffmanTree();
            
            // Step 3: Generate codes
            cout << "\n=== STEP 3: GENERATING HUFFMAN CODES ===" << endl;
            generateCodes(root, "");
            
            // Step 4: Encode the text
            cout << "\n=== STEP 4: ENCODING TEXT ===" << endl;
            encodedText = "";
            for (char ch : text) {
                encodedText += huffmanCodes[ch];
            }
        }
        
        // Display results
        displayEncodingResults();
    }
    
    // Display encoding results and statistics
    void displayEncodingResults() {
        cout << "\n=== ENCODING RESULTS ===" << endl;
        
        // Display Huffman codes
        cout << "\nHuffman Code Table:" << endl;
        cout << "Char\tFreq\tCode\t\tLength" << endl;
        cout << "----\t----\t----\t\t------" << endl;
        
        // Sort by frequency for better display
        vector<pair<char, int>> sortedFreq(frequencyTable.begin(), frequencyTable.end());
        sort(sortedFreq.begin(), sortedFreq.end(), 
             [](const pair<char,int>& a, const pair<char,int>& b) {
                 return a.second > b.second;
             });
        
        for (auto& pair : sortedFreq) {
            char ch = pair.first;
            cout << "'" << ch << "'\t" 
                 << pair.second << "\t" 
                 << huffmanCodes[ch] << "\t\t" 
                 << huffmanCodes[ch].length() << endl;
        }
        
        // Calculate statistics
        cout << "\n=== COMPRESSION STATISTICS ===" << endl;
        
        int originalBits = originalText.length() * 8;  // Assuming 8-bit ASCII
        int compressedBits = encodedText.length();
        double compressionRatio = (1.0 - (double)compressedBits / originalBits) * 100;
        
        // Calculate average code length
        double avgCodeLength = 0;
        int totalChars = 0;
        for (auto& pair : frequencyTable) {
            avgCodeLength += pair.second * huffmanCodes[pair.first].length();
            totalChars += pair.second;
        }
        avgCodeLength /= totalChars;
        
        cout << "Original size: " << originalBits << " bits (" 
             << originalText.length() << " bytes)" << endl;
        cout << "Compressed size: " << compressedBits << " bits (" 
             << (compressedBits + 7) / 8 << " bytes)" << endl;
        cout << "Compression ratio: " << fixed << setprecision(2) 
             << compressionRatio << "%" << endl;
        cout << "Average code length: " << fixed << setprecision(4) 
             << avgCodeLength << " bits/symbol" << endl;
        cout << "Space saved: " << (originalBits - compressedBits) << " bits" << endl;
        
        // Show encoded text (for small inputs)
        if (originalText.length() <= 50) {
            cout << "\nEncoded binary string:" << endl;
            cout << encodedText << endl;
            
            // Show encoding process
            cout << "\nEncoding process:" << endl;
            for (char ch : originalText) {
                cout << "'" << ch << "' -> " << huffmanCodes[ch] << endl;
            }
        }
        
        // Visualize tree (for small trees)
        if (frequencyTable.size() <= 10) {
            cout << "\n=== HUFFMAN TREE STRUCTURE ===" << endl;
            printTree(root);
        }
    }
    
    // Decode function
    string decode(const string& encodedStr) {
        cout << "\n=== DECODING PROCESS ===" << endl;
        cout << "Encoded string length: " << encodedStr.length() << " bits" << endl;
        
        if (encodedStr.empty()) {
            return "";
        }
        
        string decodedText = "";
        
        // Special case: only one unique character
        if (huffmanCodes.size() == 1) {
            char ch = huffmanCodes.begin()->first;
            decodedText = string(encodedStr.length(), ch);
        } else {
            // Traverse tree based on encoded bits
            HuffmanNode* current = root;
            
            for (char bit : encodedStr) {
                if (bit == '0') {
                    current = current->left;
                } else if (bit == '1') {
                    current = current->right;
                } else {
                    cout << "Error: Invalid bit '" << bit << "' in encoded string!" << endl;
                    return "";
                }
                
                // If we reach a leaf, output character and reset
                if (current->isLeaf()) {
                    decodedText += current->character;
                    current = root;
                }
            }
            
            // Check if we ended at non-leaf (incomplete code)
            if (!current->isLeaf() && current != root) {
                cout << "Warning: Encoded string ends with incomplete code!" << endl;
            }
        }
        
        cout << "Decoded text: \"" << decodedText << "\"" << endl;
        cout << "Decoded length: " << decodedText.length() << " characters" << endl;
        
        // Verify correctness
        if (decodedText == originalText) {
            cout << "✓ Decoding successful! Text matches original." << endl;
        } else {
            cout << "✗ Decoding error! Text doesn't match original." << endl;
        }
        
        return decodedText;
    }
    
    // Save encoded data to file
    void saveToFile(const string& filename) {
        ofstream outFile(filename, ios::binary);
        if (!outFile) {
            cout << "Error opening file for writing!" << endl;
            return;
        }
        
        // Save Huffman codes table size
        int tableSize = huffmanCodes.size();
        outFile.write((char*)&tableSize, sizeof(int));
        
        // Save Huffman codes (character -> code mapping)
        for (auto& pair : huffmanCodes) {
            char ch = pair.first;
            string code = pair.second;
            int codeLength = code.length();
            
            outFile.write(&ch, sizeof(char));
            outFile.write((char*)&codeLength, sizeof(int));
            outFile.write(code.c_str(), codeLength);
        }
        
        // Save encoded text length
        int encodedLength = encodedText.length();
        outFile.write((char*)&encodedLength, sizeof(int));
        
        // Convert binary string to actual bits and save
        // Pack bits into bytes
        unsigned char buffer = 0;
        int bitCount = 0;
        
        for (char bit : encodedText) {
            buffer = (buffer << 1) | (bit - '0');
            bitCount++;
            
            if (bitCount == 8) {
                outFile.write((char*)&buffer, sizeof(char));
                buffer = 0;
                bitCount = 0;
            }
        }
        
        // Write remaining bits if any
        if (bitCount > 0) {
            buffer <<= (8 - bitCount);  // Pad with zeros
            outFile.write((char*)&buffer, sizeof(char));
        }
        
        outFile.close();
        cout << "\nEncoded data saved to " << filename << endl;
    }
    
    // Load and decode from file
    string loadFromFile(const string& filename) {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "Error opening file for reading!" << endl;
            return "";
        }
        
        // Read Huffman codes table size
        int tableSize;
        inFile.read((char*)&tableSize, sizeof(int));
        
        // Read Huffman codes and rebuild the tree
        huffmanCodes.clear();
        reverseHuffmanCodes.clear();
        
        // Create a new root for the tree
        root = new HuffmanNode(0, nullptr, nullptr);
        
        for (int i = 0; i < tableSize; i++) {
            char ch;
            int codeLength;
            inFile.read(&ch, sizeof(char));
            inFile.read((char*)&codeLength, sizeof(int));
            
            string code(codeLength, ' ');
            inFile.read(&code[0], codeLength);
            
            huffmanCodes[ch] = code;
            reverseHuffmanCodes[code] = ch;
            
            // Rebuild tree structure from code
            HuffmanNode* current = root;
            for (char bit : code) {
                if (bit == '0') {
                    if (current->left == nullptr) {
                        current->left = new HuffmanNode(0, nullptr, nullptr);
                    }
                    current = current->left;
                } else {
                    if (current->right == nullptr) {
                        current->right = new HuffmanNode(0, nullptr, nullptr);
                    }
                    current = current->right;
                }
            }
            // Mark leaf node with character
            current->character = ch;
        }
        
        // Read encoded text length
        int encodedLength;
        inFile.read((char*)&encodedLength, sizeof(int));
        
        // Read encoded bits
        encodedText = "";
        int bitsRead = 0;
        unsigned char buffer;
        
        while (bitsRead < encodedLength && inFile.read((char*)&buffer, sizeof(char))) {
            for (int i = 7; i >= 0 && bitsRead < encodedLength; i--) {
                encodedText += ((buffer >> i) & 1) ? '1' : '0';
                bitsRead++;
            }
        }
        
        inFile.close();
        
        // Decode
        return decode(encodedText);
    }
};

// Main function with test cases
int main() {
    // Redirect I/O
    freopen("huffman_input.txt", "r", stdin);
    freopen("huffman_output.txt", "w", stdout);
    
    HuffmanCoding huffman;
    
    // Test Case 1: Simple text
    cout << "╔════════════════════════════════════════╗" << endl;
    cout << "║        TEST CASE 1: SIMPLE TEXT        ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    string text1;
    getline(cin, text1);
    
    huffman.encode(text1);
    string decoded1 = huffman.decode(huffman.getEncodedText());  // USE GETTER METHOD
    
    // Test Case 2: Repeated characters
    cout << "\n\n╔════════════════════════════════════════╗" << endl;
    cout << "║     TEST CASE 2: REPEATED PATTERN      ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    string text2 = "aaaaaabbbbcccdde";
    HuffmanCoding huffman2;
    huffman2.encode(text2);
    huffman2.decode(huffman2.getEncodedText());  // USE GETTER METHOD
    
    // Test Case 3: File operations
    cout << "\n\n╔════════════════════════════════════════╗" << endl;
    cout << "║      TEST CASE 3: FILE OPERATIONS      ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    huffman.saveToFile("compressed.huf");
    
    HuffmanCoding huffman3;
    string decoded3 = huffman3.loadFromFile("compressed.huf");
    cout << "Decoded from file: \"" << decoded3 << "\"" << endl;
    
    return 0;
}
```
    
    // Helper function to build frequency table
    void buildFrequencyTable(const string& text) {
        cout << "\n=== STEP 1: FREQUENCY ANALYSIS ===" << endl;
        cout << "Analyzing " << text.length() << " characters..." << endl;
        
        frequencyTable.clear();
        for (char ch : text) {
            frequencyTable[ch]++;
        }
        
        // Display frequency table
        cout << "\nFrequency Table:" << endl;
        cout << "Char\tASCII\tFreq\tProbability" << endl;
        cout << "----\t-----\t----\t-----------" << endl;
        
        for (auto& pair : frequencyTable) {
            double probability = (double)pair.second / text.length();
            cout << "'" << pair.first << "'\t" 
                 << (int)pair.first << "\t" 
                 << pair.second << "\t" 
                 << fixed << setprecision(4) << probability << endl;
        }
        
        // Calculate entropy
        double entropy = 0;
        for (auto& pair : frequencyTable) {
            double p = (double)pair.second / text.length();
            if (p > 0) {
                entropy -= p * log2(p);
            }
        }
        cout << "\nShannon Entropy: " << fixed << setprecision(4) 
             << entropy << " bits/symbol" << endl;
        cout << "Theoretical minimum average bits: " << entropy << endl;
    }
    
    // Build Huffman tree from frequency table
    HuffmanNode* buildHuffmanTree() {
        cout << "\n=== STEP 2: BUILDING HUFFMAN TREE ===" << endl;
        
        // Create min-heap (priority queue)
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> minHeap;
        
        // Create leaf nodes for each character
        cout << "Creating leaf nodes..." << endl;
        for (auto& pair : frequencyTable) {
            HuffmanNode* node = new HuffmanNode(pair.first, pair.second);
            minHeap.push(node);
            cout << "  Added leaf: '" << pair.first << "' (freq: " 
                 << pair.second << ")" << endl;
        }
        
        cout << "\nBuilding tree (bottom-up)..." << endl;
        int step = 1;
        
        // Build tree bottom-up
        while (minHeap.size() > 1) {
            // Extract two minimum frequency nodes
            HuffmanNode* left = minHeap.top();
            minHeap.pop();
            HuffmanNode* right = minHeap.top();
            minHeap.pop();
            
            // Create new internal node
            int sumFreq = left->frequency + right->frequency;
            HuffmanNode* internal = new HuffmanNode(sumFreq, left, right);
            
            cout << "  Step " << step++ << ": Merge nodes with freq " 
                 << left->frequency << " and " << right->frequency 
                 << " -> New node with freq " << sumFreq << endl;
            
            // Add back to heap
            minHeap.push(internal);
        }
        
        cout << "Tree construction complete!" << endl;
        return minHeap.top();
    }
    
    // Generate Huffman codes by traversing tree
    void generateCodes(HuffmanNode* node, string code) {
        if (node == nullptr) return;
        
        // If leaf node, store the code
        if (node->isLeaf()) {
            huffmanCodes[node->character] = code;
            reverseHuffmanCodes[code] = node->character;
            return;
        }
        
        // Traverse left (add '0') and right (add '1')
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }
    
    // Print tree structure (for visualization)
    void printTree(HuffmanNode* node, string prefix = "", bool isLeft = true) {
        if (node == nullptr) return;
        
        cout << prefix;
        cout << (isLeft ? "├── " : "└── ");
        
        if (node->isLeaf()) {
            cout << "'" << node->character << "' (" << node->frequency << ")" << endl;
        } else {
            cout << "[" << node->frequency << "]" << endl;
        }
        
        if (!node->isLeaf()) {
            printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
            printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
        }
    }
    
    // Delete tree (cleanup)
    void deleteTree(HuffmanNode* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    
public:
    // Constructor
    HuffmanCoding() : root(nullptr) {}
    
    // Destructor
    ~HuffmanCoding() {
        deleteTree(root);
    }
    
    // Main encoding function
    void encode(const string& text) {
        if (text.empty()) {
            cout << "Error: Empty input text!" << endl;
            return;
        }
        
        originalText = text;
        cout << "\n========================================" << endl;
        cout << "     HUFFMAN ENCODING PROCESS" << endl;
        cout << "========================================" << endl;
        cout << "Original text: \"" << text << "\"" << endl;
        cout << "Length: " << text.length() << " characters" << endl;
        
        // Step 1: Build frequency table
        buildFrequencyTable(text);
        
        // Handle special case: only one unique character
        if (frequencyTable.size() == 1) {
            cout << "\nSpecial case: Only one unique character!" << endl;
            char ch = frequencyTable.begin()->first;
            huffmanCodes[ch] = "0";
            reverseHuffmanCodes["0"] = ch;
            
            // Create encoded text
            encodedText = string(text.length(), '0');
        } else {
            // Step 2: Build Huffman tree
            root = buildHuffmanTree();
            
            // Step 3: Generate codes
            cout << "\n=== STEP 3: GENERATING HUFFMAN CODES ===" << endl;
            generateCodes(root, "");
            
            // Step 4: Encode the text
            cout << "\n=== STEP 4: ENCODING TEXT ===" << endl;
            encodedText = "";
            for (char ch : text) {
                encodedText += huffmanCodes[ch];
            }
        }
        
        // Display results
        displayEncodingResults();
    }
    
    // Display encoding results and statistics
    void displayEncodingResults() {
        cout << "\n=== ENCODING RESULTS ===" << endl;
        
        // Display Huffman codes
        cout << "\nHuffman Code Table:" << endl;
        cout << "Char\tFreq\tCode\t\tLength" << endl;
        cout << "----\t----\t----\t\t------" << endl;
        
        // Sort by frequency for better display
        vector<pair<char, int>> sortedFreq(frequencyTable.begin(), frequencyTable.end());
        sort(sortedFreq.begin(), sortedFreq.end(), 
             [](const pair<char,int>& a, const pair<char,int>& b) {
                 return a.second > b.second;
             });
        
        for (auto& pair : sortedFreq) {
            char ch = pair.first;
            cout << "'" << ch << "'\t" 
                 << pair.second << "\t" 
                 << huffmanCodes[ch] << "\t\t" 
                 << huffmanCodes[ch].length() << endl;
        }
        
        // Calculate statistics
        cout << "\n=== COMPRESSION STATISTICS ===" << endl;
        
        int originalBits = originalText.length() * 8;  // Assuming 8-bit ASCII
        int compressedBits = encodedText.length();
        double compressionRatio = (1.0 - (double)compressedBits / originalBits) * 100;
        
        // Calculate average code length
        double avgCodeLength = 0;
        int totalChars = 0;
        for (auto& pair : frequencyTable) {
            avgCodeLength += pair.second * huffmanCodes[pair.first].length();
            totalChars += pair.second;
        }
        avgCodeLength /= totalChars;
        
        cout << "Original size: " << originalBits << " bits (" 
             << originalText.length() << " bytes)" << endl;
        cout << "Compressed size: " << compressedBits << " bits (" 
             << (compressedBits + 7) / 8 << " bytes)" << endl;
        cout << "Compression ratio: " << fixed << setprecision(2) 
             << compressionRatio << "%" << endl;
        cout << "Average code length: " << fixed << setprecision(4) 
             << avgCodeLength << " bits/symbol" << endl;
        cout << "Space saved: " << (originalBits - compressedBits) << " bits" << endl;
        
        // Show encoded text (for small inputs)
        if (originalText.length() <= 50) {
            cout << "\nEncoded binary string:" << endl;
            cout << encodedText << endl;
            
            // Show encoding process
            cout << "\nEncoding process:" << endl;
            for (char ch : originalText) {
                cout << "'" << ch << "' -> " << huffmanCodes[ch] << endl;
            }
        }
        
        // Visualize tree (for small trees)
        if (frequencyTable.size() <= 10) {
            cout << "\n=== HUFFMAN TREE STRUCTURE ===" << endl;
            printTree(root);
        }
    }
    
    // Decode function
    string decode(const string& encodedStr) {
        cout << "\n=== DECODING PROCESS ===" << endl;
        cout << "Encoded string length: " << encodedStr.length() << " bits" << endl;
        
        if (encodedStr.empty()) {
            return "";
        }
        
        string decodedText = "";
        
        // Special case: only one unique character
        if (huffmanCodes.size() == 1) {
            char ch = huffmanCodes.begin()->first;
            decodedText = string(encodedStr.length(), ch);
        } else {
            // Traverse tree based on encoded bits
            HuffmanNode* current = root;
            
            for (char bit : encodedStr) {
                if (bit == '0') {
                    current = current->left;
                } else if (bit == '1') {
                    current = current->right;
                } else {
                    cout << "Error: Invalid bit '" << bit << "' in encoded string!" << endl;
                    return "";
                }
                
                // If we reach a leaf, output character and reset
                if (current->isLeaf()) {
                    decodedText += current->character;
                    current = root;
                }
            }
            
            // Check if we ended at non-leaf (incomplete code)
            if (!current->isLeaf() && current != root) {
                cout << "Warning: Encoded string ends with incomplete code!" << endl;
            }
        }
        
        cout << "Decoded text: \"" << decodedText << "\"" << endl;
        cout << "Decoded length: " << decodedText.length() << " characters" << endl;
        
        // Verify correctness
        if (decodedText == originalText) {
            cout << "✓ Decoding successful! Text matches original." << endl;
        } else {
            cout << "✗ Decoding error! Text doesn't match original." << endl;
        }
        
        return decodedText;
    }
    
    // Save encoded data to file
    void saveToFile(const string& filename) {
        ofstream outFile(filename, ios::binary);
        if (!outFile) {
            cout << "Error opening file for writing!" << endl;
            return;
        }
        
        // Save frequency table size
        int tableSize = frequencyTable.size();
        outFile.write((char*)&tableSize, sizeof(int));
        
        // Save frequency table
        for (auto& pair : frequencyTable) {
            outFile.write(&pair.first, sizeof(char));
            outFile.write((char*)&pair.second, sizeof(int));
        }
        
        // Save encoded text length
        int encodedLength = encodedText.length();
        outFile.write((char*)&encodedLength, sizeof(int));
        
        // Convert binary string to actual bits and save
        // Pack bits into bytes
        unsigned char buffer = 0;
        int bitCount = 0;
        
        for (char bit : encodedText) {
            buffer = (buffer << 1) | (bit - '0');
            bitCount++;
            
            if (bitCount == 8) {
                outFile.write((char*)&buffer, sizeof(char));
                buffer = 0;
                bitCount = 0;
            }
        }
        
        // Write remaining bits if any
        if (bitCount > 0) {
            buffer <<= (8 - bitCount);  // Pad with zeros
            outFile.write((char*)&buffer, sizeof(char));
        }
        
        outFile.close();
        cout << "\nEncoded data saved to " << filename << endl;
    }
    
    // Load and decode from file
    string loadFromFile(const string& filename) {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "Error opening file for reading!" << endl;
            return "";
        }
        
        // Read frequency table size
        int tableSize;
        inFile.read((char*)&tableSize, sizeof(int));
        
        // Read frequency table
        frequencyTable.clear();
        for (int i = 0; i < tableSize; i++) {
            char ch;
            int freq;
            inFile.read(&ch, sizeof(char));
            inFile.read((char*)&freq, sizeof(int));
            frequencyTable[ch] = freq;
        }
        
        // Rebuild Huffman tree
        root = buildHuffmanTree();
        generateCodes(root, "");
        
        // Read encoded text length
        int encodedLength;
        inFile.read((char*)&encodedLength, sizeof(int));
        
        // Read encoded bits
        encodedText = "";
        int bitsRead = 0;
        unsigned char buffer;
        
        while (bitsRead < encodedLength && inFile.read((char*)&buffer, sizeof(char))) {
            for (int i = 7; i >= 0 && bitsRead < encodedLength; i--) {
                encodedText += ((buffer >> i) & 1) ? '1' : '0';
                bitsRead++;
            }
        }
        
        inFile.close();
        
        // Decode
        return decode(encodedText);
    }
};

// Main function with test cases
int main() {
    // Redirect I/O
    freopen("huffman_input.txt", "r", stdin);
    freopen("huffman_output.txt", "w", stdout);
    
    HuffmanCoding huffman;
    
    // Test Case 1: Simple text
    cout << "╔════════════════════════════════════════╗" << endl;
    cout << "║        TEST CASE 1: SIMPLE TEXT        ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    string text1;
    getline(cin, text1);
    
    huffman.encode(text1);
    string decoded1 = huffman.decode(huffman.encodedText);
    
    // Test Case 2: Repeated characters
    cout << "\n\n╔════════════════════════════════════════╗" << endl;
    cout << "║     TEST CASE 2: REPEATED PATTERN      ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    string text2 = "aaaaaabbbbcccdde";
    HuffmanCoding huffman2;
    huffman2.encode(text2);
    huffman2.decode(huffman2.encodedText);
    
    // Test Case 3: File operations
    cout << "\n\n╔════════════════════════════════════════╗" << endl;
    cout << "║      TEST CASE 3: FILE OPERATIONS      ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    huffman.saveToFile("compressed.huf");
    
    HuffmanCoding huffman3;
    string decoded3 = huffman3.loadFromFile("compressed.huf");
    cout << "Decoded from file: \"" << decoded3 << "\"" << endl;
    
    return 0;
}
```

### Sample Input File (huffman_input.txt):
```
hello world
```

---

## Practice Problems

### Problem Set 1: Basic Implementation

#### Problem 1.1: Character Frequency
**Task**: Write a function that counts character frequencies and displays them sorted by frequency.

```cpp
void analyzeFrequency(const string& text) {
    // TODO: Count frequencies
    // TODO: Sort by frequency (descending)
    // TODO: Display with percentages
}
```

#### Problem 1.2: Binary Tree Construction
**Task**: Build a Huffman tree for the frequencies: A(5), B(9), C(12), D(13), E(16), F(45)

**Solution Steps**:
1. Create leaf nodes
2. Build min-heap
3. Merge nodes step by step
4. Draw final tree

#### Problem 1.3: Code Generation
**Task**: Given a Huffman tree, generate codes for all leaf nodes.

```cpp
void generateAllCodes(HuffmanNode* root) {
    // TODO: Implement recursive traversal
    // TODO: Store codes in map
    // TODO: Print code table
}
```

### Problem Set 2: Analysis Problems

#### Problem 2.1: Compression Ratio
**Given**: Text with 100 A's, 50 B's, 25 C's, 25 D's
**Calculate**:
1. Huffman codes for each character
2. Total compressed size
3. Compression ratio

#### Problem 2.2: Optimal Code Length
**Prove**: For n symbols with equal probability, the average code length approaches log₂(n).

#### Problem 2.3: Worst Case Analysis
**Question**: What input distribution gives the worst compression ratio?
**Answer**: When all characters have equal frequency (uniform distribution).

### Problem Set 3: Advanced Implementation

#### Problem 3.1: Adaptive Huffman Coding
Implement dynamic Huffman coding that updates the tree as new symbols arrive.

```cpp
class AdaptiveHuffman {
    // TODO: Implement NYT (Not Yet Transmitted) node
    // TODO: Update tree after each symbol
    // TODO: Implement sibling property maintenance
};
```

#### Problem 3.2: Canonical Huffman Codes
Generate canonical codes (lexicographically ordered for same length).

```cpp
map<char, string> generateCanonicalCodes(map<char, int> frequencies) {
    // TODO: Generate initial codes
    // TODO: Sort by code length, then alphabetically
    // TODO: Reassign codes maintaining lengths
}
```

#### Problem 3.3: Limited Code Length
Modify algorithm to ensure no code exceeds length L.

```cpp
void limitedLengthHuffman(map<char, int> freq, int maxLength) {
    // TODO: Use Package-Merge algorithm
    // TODO: Ensure all codes ≤ maxLength
}
```

### Problem Set 4: Optimization Problems

#### Problem 4.1: Memory-Efficient Implementation
Implement Huffman coding using bit manipulation for space efficiency.

```cpp
class BitEfficientHuffman {
    // Use bit vectors instead of strings
    // Pack codes into integers
    // Implement bit-level operations
};
```

#### Problem 4.2: Parallel Huffman Encoding
Design a parallel algorithm for encoding large texts.

```cpp
string parallelEncode(const string& text, int numThreads) {
    // TODO: Divide text into chunks
    // TODO: Encode chunks in parallel
    // TODO: Merge results
}
```

### Problem Set 5: Real-World Applications

#### Problem 5.1: File Compression Utility
Create a complete file compression program.

```cpp
class FileCompressor {
public:
    void compressFile(const string& inputFile, const string& outputFile);
    void decompressFile(const string& inputFile, const string& outputFile);
    void displayStatistics();
};
```

#### Problem 5.2: Image Compression
Apply Huffman coding to compress grayscale images.

```cpp
class ImageCompressor {
    // Read pixel values
    // Build frequency table for pixel intensities
    // Compress using Huffman coding
    // Save compressed image format
};
```

#### Problem 5.3: Network Protocol
Design a protocol for transmitting Huffman-encoded data over network.

```cpp
class HuffmanProtocol {
    // Header format
    // Tree transmission
    // Data packets
    // Error handling
};
```

---

## Advanced Topics

### 1. Variations of Huffman Coding

#### 1.1 Adaptive Huffman Coding (Dynamic Huffman)
- Updates tree during encoding/decoding
- No need to transmit frequency table
- Used in real-time compression

#### 1.2 Canonical Huffman Codes
- Codes of same length are consecutive integers
- Reduces tree storage requirements
- Faster decoding

#### 1.3 Length-Limited Huffman Coding
- Restricts maximum code length
- Important for hardware implementations
- Package-Merge algorithm

### 2. Theoretical Extensions

#### 2.1 Huffman Coding for Non-Binary Alphabets
```cpp
// Ternary Huffman (base-3 codes)
class TernaryHuffman {
    // Use 3-way tree nodes
    // Codes use digits 0, 1, 2
};
```

#### 2.2 Multi-dimensional Huffman Coding
- For 2D data (images)
- Consider spatial correlation

#### 2.3 Arithmetic Coding vs Huffman
- Arithmetic coding can achieve better compression
- Huffman is simpler and patent-free

### 3. Implementation Optimizations

#### 3.1 Table Lookup Decoding
```cpp
class FastDecoder {
    // Pre-compute decoding tables
    // Trade memory for speed
    unsigned char decodeTable[256];  // For 8-bit chunks
};
```

#### 3.2 SIMD Optimizations
- Use vector instructions for parallel bit operations
- Process multiple symbols simultaneously

#### 3.3 Cache-Efficient Implementation
- Optimize tree layout for cache locality
- Use array-based tree representation

### 4. Common Pitfalls and Solutions

#### Pitfall 1: Integer Overflow
```cpp
// Problem: Frequency sum exceeds int range
// Solution: Use long long for frequencies
long long totalFrequency = 0;
```

#### Pitfall 2: Memory Leaks
```cpp
// Problem: Not deleting tree nodes
// Solution: Implement proper destructor
~HuffmanCoding() {
    deleteTree(root);
}
```

#### Pitfall 3: Incomplete Codes
```cpp
// Problem: Encoded string doesn't end at leaf
// Solution: Padding or storing exact bit count
```

#### Pitfall 4: Single Character Input
```cpp
// Problem: Can't build tree with one node
// Solution: Special case handling
if (uniqueChars == 1) {
    // Assign arbitrary code like "0"
}
```

### 5. Performance Analysis

#### Compression Effectiveness
| Data Type | Typical Compression |
|-----------|-------------------|
| English Text | 40-50% |
| Source Code | 50-60% |
| Binary Data | 10-20% |
| Random Data | ~0% (may expand) |

#### Comparison with Other Algorithms
| Algorithm | Compression | Speed | Memory |
|-----------|------------|--------|--------|
| Huffman | Good | Fast | Low |
| LZW | Better | Medium | Medium |
| Arithmetic | Best | Slow | Low |
| Burrows-Wheeler | Excellent | Slow | High |

### 6. Real-World Applications

1. **JPEG Image Compression**: Uses Huffman for entropy coding
2. **MP3 Audio**: Huffman coding for quantized values
3. **ZIP Files**: DEFLATE algorithm combines LZ77 and Huffman
4. **Fax Machines**: Modified Huffman for document transmission
5. **Data Transmission**: Reduces bandwidth requirements

---

## Debugging Guide

### Common Issues and Solutions

#### 1. Incorrect Decoding
**Symptoms**: Decoded text doesn't match original
**Debugging Steps**:
```cpp
// Add debug output
void debugDecode(const string& encoded) {
    cout << "Decoding bit by bit:" << endl;
    HuffmanNode* current = root;
    for (char bit : encoded) {
        cout << "Bit: " << bit << ", ";
        if (bit == '0') current = current->left;
        else current = current->right;
        
        if (current->isLeaf()) {
            cout << "Found: '" << current->character << "'" << endl;
            current = root;
        }
    }
}
```

#### 2. Tree Building Errors
**Verification**:
```cpp
bool verifyTree(HuffmanNode* node) {
    if (node->isLeaf()) return true;
    
    // Internal node frequency should equal sum of children
    int expectedFreq = node->left->frequency + node->right->frequency;
    if (node->frequency != expectedFreq) {
        cout << "Error: Frequency mismatch at internal node!" << endl;
        return false;
    }
    
    return verifyTree(node->left) && verifyTree(node->right);
}
```

#### 3. Memory Issues
**Use Valgrind or AddressSanitizer**:
```bash
g++ -g -fsanitize=address huffman.cpp -o huffman
./huffman
```

---

## Summary and Best Practices

### Visual Summary of Huffman Coding

```
COMPLETE HUFFMAN CODING WORKFLOW:
═══════════════════════════════════════════════════════════════

┌─────────────┐     ┌──────────────┐     ┌──────────────┐
│   INPUT     │ --> │  FREQUENCY   │ --> │ BUILD TREE   │
│   "AABBC"   │     │  ANALYSIS    │     │              │
└─────────────┘     └──────────────┘     └──────────────┘
                           │                      │
                    A:2 B:2 C:1                   ↓
                                            Min Heap Ops
                                                  │
┌─────────────┐     ┌──────────────┐     ┌──────────────┐
│   OUTPUT    │ <-- │   ENCODING   │ <-- │ GENERATE     │
│  "001011"   │     │              │     │   CODES      │
└─────────────┘     └──────────────┘     └──────────────┘
                                                  ↑
                                                Tree
                                                (5)
                                               /   \
                                              A:2  (3)
                                                   / \
                                                 B:2 C:1

COMPRESSION ACHIEVED:
Original: 5 chars × 8 bits = 40 bits
Encoded:  2×1 + 2×2 + 1×2 = 8 bits
Savings:  80% compression!
```

### Key Takeaways - Visual Mind Map

```
                    HUFFMAN CODING
                         │
        ┌────────────────┼────────────────┐
        │                │                │
    PRINCIPLES      ALGORITHM         APPLICATIONS
        │                │                │
    ┌───┼───┐      ┌─────┼─────┐    ┌────┼────┐
    │   │   │      │     │     │    │    │    │
Prefix Variable Greedy Build Generate ZIP  JPEG MP3
Free   Length   Optimal Tree  Codes  Files
Codes  Codes    Solution

KEY INSIGHTS:
═════════════
✓ Optimal for symbol-by-symbol encoding
✓ Greedy algorithm yields global optimum
✓ Compression ratio depends on frequency distribution
✓ Trade-off: Better compression vs. encoding time
✓ Not suitable for small data or uniform distribution
```

### Best Practices

```
DO's                           DON'Ts
═══════════════════           ═══════════════════════
✓ Validate input              ✗ Ignore edge cases
✓ Handle single character     ✗ Forget to free memory
✓ Use appropriate types       ✗ Use on small files
✓ Test edge cases            ✗ Assume compression
✓ Document bit operations    ✗ Mix up 0/1 encoding
✓ Profile performance        ✗ Hardcode tree depth
```

### Complexity Quick Reference

```
┌──────────────┬────────────┬─────────────┐
│  Operation   │    Time    │    Space    │
├──────────────┼────────────┼─────────────┤
│ Count Freq   │    O(n)    │    O(k)     │
│ Build Tree   │  O(k log k)│    O(k)     │
│ Gen Codes    │    O(k)    │    O(k)     │
│ Encode       │    O(n)    │    O(n)     │
│ Decode       │    O(m)    │    O(1)     │
└──────────────┴────────────┴─────────────┘
n = input size, k = alphabet size, m = encoded size
```

### When to Use Huffman Coding

```
DECISION TREE:
══════════════
                Is data size > 1KB?
                      /  \
                    No    Yes
                    /      \
              Don't use    Is frequency
              Huffman      distribution skewed?
                             /  \
                           No    Yes
                           /      \
                    Consider      Use
                    other        Huffman!
                    methods

IDEAL USE CASES:           AVOID WHEN:
• Text files               • Data < 100 bytes
• Log files                • Random/encrypted data
• Source code              • Already compressed
• Network packets          • Uniform distribution
• Database storage         • Real-time constraints
```

### Further Study

```
LEARNING PATH:
══════════════
1. Master Huffman ──→ 2. Study LZ Family ──→ 3. Learn Arithmetic
      ↓                      ↓                      ↓
   [This worksheet]      [LZ77, LZW]          [Range coding]
                              ↓                      ↓
                    4. Hybrid Methods ←─────── 5. Modern Algos
                         [DEFLATE]              [Zstd, Brotli]
                              ↓
                    6. Specialized Compression
                    [Video: H.264, Audio: AAC]
```

### Final Exercise: Complete Compression System

```
BUILD YOUR OWN FILE COMPRESSOR:
════════════════════════════════

Requirements Checklist:
□ Read any file type
□ Analyze byte frequencies
□ Build Huffman tree
□ Generate optimal codes
□ Encode to binary file
□ Store tree for decoding
□ Decompress accurately
□ Display statistics
□ Handle errors gracefully
□ Support large files

Bonus Features:
□ Progress bar
□ Multiple file support
□ Compression levels
□ Compare with ZIP
□ GUI interface
```

---

## Quick Reference Card

```
╔═══════════════════════════════════════════════════════════╗
║                 HUFFMAN CODING QUICK REFERENCE            ║
╠═══════════════════════════════════════════════════════════╣
║ 1. Count frequencies: map<char, int> freq                 ║
║ 2. Build min-heap: priority_queue<Node*, ..., Compare>   ║
║ 3. Build tree: while(heap.size() > 1) merge_two_min()    ║
║ 4. Generate codes: traverse(root, "", codes_map)         ║
║ 5. Encode: for(char c : text) output += codes[c]        ║
║ 6. Decode: traverse tree following bits until leaf       ║
╠═══════════════════════════════════════════════════════════╣
║ Time: O(n + k log k)  |  Space: O(k)  |  Optimal: Yes   ║
╚═══════════════════════════════════════════════════════════╝
```

This comprehensive worksheet with visual diagrams provides everything needed to master Huffman coding from theory to practice!