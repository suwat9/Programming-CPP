#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <iomanip>  // for std::setw
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
    unordered_map<char, int> frequencies;  // 🔹 store frequencies
    
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
        for (char c : text) {
            frequencies[c]++;   // 🔹 save frequencies
        }
        
        // Step 2: Build Huffman tree
        priority_queue<Node*, vector<Node*>, Compare> pq;
        
        // Create leaf nodes
        for (auto& p : frequencies) {
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
            cout << std::setw(4) << frequencies[p.first] <<"  |  " << p.second << "\n";
        }
    }
};

int main() {
    string text = "Phetchaburi Rajabhat University";
    
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
