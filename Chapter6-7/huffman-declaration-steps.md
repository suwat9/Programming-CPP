# Huffman Coding - Declaration & Step-by-Step Explanation

## Part I: Formal Declaration

### Definition
**Huffman Coding** is a lossless data compression algorithm that assigns variable-length binary codes to characters based on their frequency of occurrence, where more frequent characters receive shorter codes and less frequent characters receive longer codes, resulting in optimal prefix-free encoding.

### Mathematical Declaration
```
Given:
- Alphabet Σ = {c₁, c₂, ..., cₙ}
- Frequency function f: Σ → ℕ where f(cᵢ) = frequency of character cᵢ
- Text T of length m containing characters from Σ

Huffman Coding produces:
- Binary encoding function E: Σ → {0,1}* 
- Such that:
  1. No code E(cᵢ) is a prefix of another code E(cⱼ) for i ≠ j
  2. The average code length L = Σ(f(cᵢ) × |E(cᵢ)|) / m is minimized
  3. The encoding is uniquely decodable
```

### Core Properties

| Property | Description | Importance |
|----------|-------------|------------|
| **Variable-Length Codes** | Different characters have different code lengths | Enables compression |
| **Prefix-Free** | No code is a prefix of another | Ensures unique decoding |
| **Optimal** | Minimizes average code length | Best possible compression |
| **Binary Tree Based** | Codes derived from binary tree paths | Efficient structure |
| **Greedy Algorithm** | Builds optimal solution step by step | Guaranteed optimality |

### Visual Declaration
```
HUFFMAN CODING TRANSFORMS:
═══════════════════════════════════════════════════════════
                                    
Input Text                      Compressed Binary Output
"AAABBC"     ──────────►       "0001010110"
(48 bits)    Huffman Coding    (10 bits)
             
             Using Frequency-Based
             Variable-Length Codes:
             A(freq:3) → 0 (1 bit)
             B(freq:2) → 10 (2 bits)
             C(freq:1) → 11 (2 bits)
═══════════════════════════════════════════════════════════
```

---

## Part II: The Algorithm Steps

### Overview of the Process
```
┌──────────────────────────────────────────────────────┐
│                  HUFFMAN ALGORITHM                    │
│                                                       │
│  INPUT ──► STEP 1 ──► STEP 2 ──► STEP 3 ──► OUTPUT  │
│  Text      Count      Build      Generate    Codes   │
│           Frequency    Tree       Codes              │
└──────────────────────────────────────────────────────┘
```

---

## STEP 1: Frequency Analysis

### Purpose
Count how many times each character appears in the input text.

### Process
```
Input Text: "AAAAABBBCCD"

Character Scanning:
Position:  1 2 3 4 5 6 7 8 9 10 11
Character: A A A A A B B B C C  D
           ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓  ↓
Count:     1 2 3 4 5 1 2 3 1 2  1
```

### Result - Frequency Table
```
┌─────────┬───────────┬────────────────┐
│ Symbol  │ Frequency │ Visual         │
├─────────┼───────────┼────────────────┤
│   A     │     5     │ ████████████   │
│   B     │     3     │ ███████        │
│   C     │     2     │ █████          │
│   D     │     1     │ ██             │
└─────────┴───────────┴────────────────┘
```

### Implementation
```cpp
// Step 1 Implementation
unordered_map<char, int> frequency;
for (char c : text) {
    frequency[c]++;
}
```

---

## STEP 2: Build Huffman Tree

### Purpose
Create a binary tree where characters are leaves, with path length inversely proportional to frequency.

### Sub-steps

#### 2.1: Create Leaf Nodes
```
Create a node for each character with its frequency:

   [A:5]    [B:3]    [C:2]    [D:1]
     ○        ○        ○        ○
  (leaf)   (leaf)   (leaf)   (leaf)
```

#### 2.2: Initialize Priority Queue (Min-Heap)
```
Priority Queue (ordered by frequency, ascending):
┌─────────────────────────────────┐
│ Front → [D:1] [C:2] [B:3] [A:5] │
└─────────────────────────────────┘
```

#### 2.3: Build Tree (Iterative Merging)

**Iteration 1**: Merge two smallest (D:1 and C:2)
```
Extract: [D:1] [C:2]
Create:     [3]
           /   \
        [D:1] [C:2]

Queue now: [B:3] [3] [A:5]
```

**Iteration 2**: Merge two smallest (B:3 and [3])
```
Extract: [B:3] [3]
Create:     [6]
           /   \
        [B:3]  [3]
              /   \
           [D:1] [C:2]

Queue now: [A:5] [6]
```

**Iteration 3**: Final merge (A:5 and [6])
```
Extract: [A:5] [6]
Create:     [11]  ← ROOT
           /    \
        [A:5]   [6]
               /   \
            [B:3]  [3]
                  /   \
               [D:1] [C:2]

Queue now: [11] (only root remains)
```

### Final Tree Structure
```
           (11)
          0/  \1
         /     \
      [A:5]    (6)
              0/ \1
              /   \
          [B:3]   (3)
                 0/ \1
                 /   \
             [D:1]  [C:2]
```

### Implementation
```cpp
// Step 2 Implementation
priority_queue<Node*, vector<Node*>, CompareFreq> pq;

// Add all leaf nodes
for (auto& pair : frequency) {
    pq.push(new Node(pair.first, pair.second));
}

// Build tree
while (pq.size() > 1) {
    Node* left = pq.top(); pq.pop();
    Node* right = pq.top(); pq.pop();
    
    Node* parent = new Node('\0', 
                           left->freq + right->freq,
                           left, right);
    pq.push(parent);
}

Node* root = pq.top();
```

---

## STEP 3: Generate Binary Codes

### Purpose
Traverse the tree to assign binary codes to each character.

### Process
Navigate from root to each leaf, recording the path:
- **Left edge = 0**
- **Right edge = 1**

### Tree Traversal for Code Generation
```
Starting from root (11):

Path to A:  Root → Left           = 0
Path to B:  Root → Right → Left   = 10  
Path to D:  Root → Right → Right → Left  = 110
Path to C:  Root → Right → Right → Right = 111

Visual Traversal:
           (11)
          ↙0  ↘1
         /     \
      [A:5]    (6)
      "0"     ↙0  ↘1
              /    \
          [B:3]    (3)
          "10"    ↙0  ↘1
                  /    \
              [D:1]   [C:2]
              "110"   "111"
```

### Generated Code Table
```
┌────────┬────────┬───────────┬─────────────────────┐
│ Symbol │ Freq   │ Code      │ Bits Saved          │
├────────┼────────┼───────────┼─────────────────────┤
│   A    │   5    │    0      │ 5×(8-1) = 35 bits   │
│   B    │   3    │   10      │ 3×(8-2) = 18 bits   │
│   D    │   1    │  110      │ 1×(8-3) = 5 bits    │
│   C    │   2    │  111      │ 2×(8-3) = 10 bits   │
└────────┴────────┴───────────┴─────────────────────┘
                                 Total Saved: 68 bits
```

### Implementation
```cpp
// Step 3 Implementation
void generateCodes(Node* node, string code, 
                  unordered_map<char, string>& codes) {
    if (!node) return;
    
    // If leaf, store code
    if (node->isLeaf()) {
        codes[node->character] = code;
        return;
    }
    
    // Traverse left with '0', right with '1'
    generateCodes(node->left, code + "0", codes);
    generateCodes(node->right, code + "1", codes);
}
```

---

## STEP 4: Encoding Process

### Purpose
Replace each character in the original text with its binary code.

### Process
```
Original Text: "AAAAABBBCCD"

Character-by-Character Encoding:
A → 0
A → 0
A → 0
A → 0
A → 0
B → 10
B → 10
B → 10
C → 111
C → 111
D → 110

Concatenated Result: 00000101010111111110
```

### Visual Encoding
```
Original:  A    A    A    A    A    B    B    B    C    C    D
           ↓    ↓    ↓    ↓    ↓    ↓    ↓    ↓    ↓    ↓    ↓
Codes:     0    0    0    0    0   10   10   10  111  111  110
           └────────┬────────┘ └───────┬──────┘ └────┬────┘ │
                5 bits              6 bits        6 bits   3 bits

Encoded Binary: 00000101010111111110 (20 bits)
```

### Compression Analysis
```
Original Size:    11 characters × 8 bits = 88 bits
Compressed Size:  20 bits
Compression Ratio: (88-20)/88 × 100% = 77.3%
Space Saved:      68 bits
```

### Implementation
```cpp
// Step 4 Implementation
string encode(string text, unordered_map<char, string>& codes) {
    string encoded = "";
    for (char c : text) {
        encoded += codes[c];
    }
    return encoded;
}
```

---

## STEP 5: Decoding Process

### Purpose
Convert the binary string back to original text using the Huffman tree.

### Process
Navigate the tree from root following the bits:
- **Bit = 0**: Go left
- **Bit = 1**: Go right
- **Reach leaf**: Output character and restart from root

### Decoding Example
```
Encoded: 00000101010111111110

Bit-by-bit traversal:
Bit  Action          Current Position    Output
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
0    Go left         Reached A           A
0    Go left         Reached A           A
0    Go left         Reached A           A
0    Go left         Reached A           A
0    Go left         Reached A           A
1    Go right        At internal (6)     -
0    Go left         Reached B           B
1    Go right        At internal (6)     -
0    Go left         Reached B           B
1    Go right        At internal (6)     -
0    Go left         Reached B           B
1    Go right        At internal (6)     -
1    Go right        At internal (3)     -
1    Go right        Reached C           C
1    Go right        At internal (6)     -
1    Go right        At internal (3)     -
1    Go right        Reached C           C
1    Go right        At internal (6)     -
1    Go right        At internal (3)     -
0    Go left         Reached D           D

Decoded: AAAAABBBCCD ✓
```

### Implementation
```cpp
// Step 5 Implementation
string decode(string encoded, Node* root) {
    string decoded = "";
    Node* current = root;
    
    for (char bit : encoded) {
        // Navigate tree
        if (bit == '0')
            current = current->left;
        else
            current = current->right;
        
        // If leaf reached
        if (current->isLeaf()) {
            decoded += current->character;
            current = root;  // Reset to root
        }
    }
    
    return decoded;
}
```

---

## Complete Working Example

### Full C++ Implementation
```cpp
#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

// Node structure
struct Node {
    char character;
    int frequency;
    Node *left, *right;
    
    Node(char c, int f, Node* l = nullptr, Node* r = nullptr) 
        : character(c), frequency(f), left(l), right(r) {}
    
    bool isLeaf() { return !left && !right; }
};

// Comparator for min-heap
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};

class HuffmanCoding {
private:
    Node* root;
    unordered_map<char, string> codes;
    
    // Step 3: Generate codes
    void generateCodes(Node* node, string code) {
        if (!node) return;
        
        if (node->isLeaf()) {
            codes[node->character] = code.empty() ? "0" : code;
            return;
        }
        
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }
    
public:
    void buildTree(string text) {
        // Step 1: Count frequencies
        unordered_map<char, int> freq;
        for (char c : text) {
            freq[c]++;
        }
        
        // Step 2: Build Huffman tree
        priority_queue<Node*, vector<Node*>, Compare> pq;
        
        // Create leaf nodes
        for (auto& p : freq) {
            pq.push(new Node(p.first, p.second));
        }
        
        // Build tree
        while (pq.size() > 1) {
            Node* left = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();
            
            Node* parent = new Node('\0', 
                                   left->frequency + right->frequency,
                                   left, right);
            pq.push(parent);
        }
        
        root = pq.top();
        
        // Generate codes
        generateCodes(root, "");
    }
    
    // Step 4: Encode
    string encode(string text) {
        string result = "";
        for (char c : text) {
            result += codes[c];
        }
        return result;
    }
    
    // Step 5: Decode
    string decode(string encoded) {
        string result = "";
        Node* current = root;
        
        for (char bit : encoded) {
            current = (bit == '0') ? current->left : current->right;
            
            if (current->isLeaf()) {
                result += current->character;
                current = root;
            }
        }
        
        return result;
    }
    
    void printCodes() {
        cout << "\nGenerated Huffman Codes:\n";
        cout << "Character | Frequency | Code\n";
        cout << "----------|-----------|--------\n";
        for (auto& p : codes) {
            cout << "    " << p.first << "     |     ";
            cout << "?     |  " << p.second << "\n";
        }
    }
};

int main() {
    string text = "AAAAABBBCCD";
    
    cout << "HUFFMAN CODING DEMONSTRATION\n";
    cout << "============================\n\n";
    cout << "Original Text: " << text << "\n";
    cout << "Original Size: " << text.length() * 8 << " bits\n";
    
    HuffmanCoding huffman;
    huffman.buildTree(text);
    huffman.printCodes();
    
    string encoded = huffman.encode(text);
    cout << "\nEncoded: " << encoded << "\n";
    cout << "Encoded Size: " << encoded.length() << " bits\n";
    
    string decoded = huffman.decode(encoded);
    cout << "\nDecoded: " << decoded << "\n";
    
    double ratio = 100.0 * (1 - (double)encoded.length() / (text.length() * 8));
    cout << "\nCompression Ratio: " << ratio << "%\n";
    
    if (decoded == text) {
        cout << "✓ Success: Decoded text matches original!\n";
    }
    
    return 0;
}
```

---

## Summary of All Steps

```
┌────────────────────────────────────────────────────────────┐
│                    HUFFMAN CODING STEPS                     │
├────────────────────────────────────────────────────────────┤
│                                                            │
│  STEP 1: COUNT FREQUENCIES                                │
│  "AAAAABBBCCD" → A:5, B:3, C:2, D:1                      │
│                                                            │
│  STEP 2: BUILD TREE                                       │
│  Merge nodes with minimum frequency repeatedly            │
│  Result: Binary tree with characters as leaves            │
│                                                            │
│  STEP 3: GENERATE CODES                                   │
│  Traverse tree: Left=0, Right=1                          │
│  Result: A=0, B=10, D=110, C=111                         │
│                                                            │
│  STEP 4: ENCODE TEXT                                      │
│  Replace each character with its code                     │
│  Result: "00000101010111111110"                          │
│                                                            │
│  STEP 5: DECODE BINARY                                    │
│  Traverse tree following bits until leaf                  │
│  Result: "AAAAABBBCCD" (original text)                   │
│                                                            │
└────────────────────────────────────────────────────────────┘
```

This completes the clear declaration and step-by-step explanation of Huffman Coding!