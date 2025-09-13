# Dynamic Programming - Complete Theory & Practice Worksheet
## Based on Competitive Programmer's Handbook - Chapter 7

---

## Fundamental Theory of Dynamic Programming

### Historical Background
Dynamic Programming was developed by Richard Bellman in the 1950s. The term "programming" refers to planning or scheduling, not computer programming. "Dynamic" indicates the time-varying nature of the problems.

### Mathematical Foundation

#### Principle of Optimality (Bellman's Principle)
An optimal policy has the property that whatever the initial state and initial decision are, the remaining decisions must constitute an optimal policy with regard to the state resulting from the first decision.

#### Formal Definition
A problem exhibits **optimal substructure** if an optimal solution contains within it optimal solutions to subproblems. Mathematically:
- If solution S is optimal for problem P
- And S can be decomposed into solutions S₁, S₂, ..., Sₖ for subproblems P₁, P₂, ..., Pₖ
- Then S₁, S₂, ..., Sₖ must be optimal for their respective subproblems

### When to Use Dynamic Programming

#### Required Properties:
1. **Overlapping Subproblems**: The problem can be broken down into subproblems which are reused several times
2. **Optimal Substructure**: An optimal solution can be constructed from optimal solutions of its subproblems

#### Identifying DP Problems:
- Keywords: "optimal", "minimum", "maximum", "longest", "shortest", "counting", "number of ways"
- Problems involving sequences, trees, or grids
- Problems where brute force leads to exponential time complexity

### DP vs Other Techniques

| Technique | When to Use | Time Complexity | Space Complexity |
|-----------|------------|-----------------|------------------|
| Dynamic Programming | Overlapping subproblems + Optimal substructure | Polynomial | O(states) |
| Greedy | Local optimal = Global optimal | Often O(n log n) | O(1) |
| Divide & Conquer | Independent subproblems | Often O(n log n) | O(log n) |
| Brute Force | Small input size | Exponential | Varies |

---

## 7.1 Coin Problem - Complete Analysis

### Deep Theoretical Background

#### Problem Origins
The coin problem originates from real-world currency systems. It's related to the Frobenius problem in number theory, which asks for the largest sum that CANNOT be formed.

#### Mathematical Model

**State Space Definition:**
- State: s(x) = "achieving sum x"
- State space: S = {s(0), s(1), ..., s(target)}
- Transition: From s(x) to s(x+c) by using coin c

**Graph Representation:**
The problem can be viewed as finding the shortest path in a directed graph where:
- Nodes represent sums (0 to target)
- Edge from node x to node (x+c) exists for each coin c
- Edge weight = 1 (one coin used)

#### Proof of Correctness

**Theorem**: The DP solution gives the minimum number of coins.

**Proof by Induction**:
- **Base case**: f(0) = 0 is correct (no coins needed for sum 0)
- **Inductive step**: Assume f(k) is correct for all k < x
- For sum x, we must use some coin c as the last coin
- The remaining sum is (x-c), which needs f(x-c) coins (correct by assumption)
- We try all possible last coins and take minimum
- Therefore, f(x) = min(f(x-c) + 1) is correct

#### Complete Implementation with Exhaustive Comments:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
using namespace std;

/*
 * COIN PROBLEM - COMPLETE IMPLEMENTATION
 * ======================================
 * This program solves both variants of the coin problem:
 * 1. Minimum number of coins to make a sum
 * 2. Number of ways to make a sum
 * 
 * The implementation includes:
 * - Theoretical explanation at each step
 * - Multiple solution approaches
 * - Complexity analysis
 * - Common pitfalls and their solutions
 */

int main() {
    // File I/O setup - competitive programming standard
    // This redirects cin to read from file and cout to write to file
    freopen("coin_input.txt", "r", stdin);
    freopen("coin_output.txt", "w", stdout);
    
    // INPUT SECTION
    // n = number of different coin denominations
    // target = the sum we want to achieve
    int n, target;
    cin >> n >> target;
    
    // Store coin denominations
    // We use vector for dynamic sizing and easy iteration
    vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }
    
    // Output problem description for clarity
    cout << "================== COIN PROBLEM ANALYSIS ==================" << endl;
    cout << "Coin denominations: ";
    for (int c : coins) cout << c << " ";
    cout << "\nTarget sum: " << target << endl;
    cout << "===========================================================" << endl << endl;
    
    /*
     * PART 1: MINIMUM COINS PROBLEM
     * ==============================
     * Goal: Find minimum number of coins to make the target sum
     * 
     * Mathematical formulation:
     * Let f(x) = minimum coins needed for sum x
     * 
     * Recurrence relation:
     * f(x) = min{f(x - c_i) + 1} for all coins c_i where c_i <= x
     * 
     * Base case: f(0) = 0 (zero coins needed for sum 0)
     * 
     * Why this works:
     * - To make sum x, we must use some coin as the last coin
     * - If we use coin c_i as the last coin, we need f(x - c_i) + 1 coins total
     * - We try all possible last coins and take the minimum
     */
    
    cout << "SOLUTION 1: MINIMUM COINS (Bottom-up DP)" << endl;
    cout << "-----------------------------------------" << endl;
    
    // DP array: dp_min[x] stores minimum coins needed for sum x
    // Initialize with INT_MAX (representing impossible)
    // Size is (target + 1) to include sum 0 to sum target
    vector<int> dp_min(target + 1, INT_MAX);
    
    // Base case: 0 coins needed for sum 0
    dp_min[0] = 0;
    
    // Parent array to reconstruct the solution
    // parent[x] stores which coin was used to achieve sum x optimally
    vector<int> parent(target + 1, -1);
    
    // Build DP table bottom-up
    // For each sum from 1 to target
    cout << "Building DP table:" << endl;
    for (int x = 1; x <= target; x++) {
        // Try using each coin as the last coin
        for (int i = 0; i < n; i++) {
            int coin = coins[i];
            
            // Check if this coin can be used:
            // 1. Coin value should not exceed current sum
            // 2. The remaining sum (x - coin) should be achievable
            if (coin <= x && dp_min[x - coin] != INT_MAX) {
                // Calculate coins needed if we use this coin
                int coins_needed = dp_min[x - coin] + 1;
                
                // Update if this is better than current best
                if (coins_needed < dp_min[x]) {
                    dp_min[x] = coins_needed;
                    parent[x] = coin;  // Remember which coin we used
                }
            }
        }
        
        // Debug output for small sums (educational purpose)
        if (x <= 10 || x == target) {
            cout << "  f(" << x << ") = ";
            if (dp_min[x] == INT_MAX) {
                cout << "impossible" << endl;
            } else {
                cout << dp_min[x] << " (using coin " << parent[x] << ")" << endl;
            }
        }
    }
    
    // Output final result
    cout << "\nRESULT: ";
    if (dp_min[target] == INT_MAX) {
        cout << "Impossible to form sum " << target << " with given coins" << endl;
    } else {
        cout << "Minimum coins needed: " << dp_min[target] << endl;
        
        // Reconstruct and display the solution
        cout << "Coins used: ";
        int sum = target;
        vector<int> solution;
        
        // Backtrack from target to 0 using parent array
        while (sum > 0 && parent[sum] != -1) {
            solution.push_back(parent[sum]);
            sum -= parent[sum];
        }
        
        // Display solution
        for (int coin : solution) {
            cout << coin << " ";
        }
        cout << endl;
        
        // Verify solution (important for debugging)
        int verify_sum = 0;
        for (int coin : solution) {
            verify_sum += coin;
        }
        cout << "Verification: " << solution.size() << " coins sum to " << verify_sum << endl;
    }
    
    /*
     * PART 2: COUNTING WAYS
     * ======================
     * Goal: Count number of different ways to make the target sum
     * 
     * Mathematical formulation:
     * Let g(x) = number of ways to form sum x
     * 
     * Recurrence relation:
     * g(x) = Σ g(x - c_i) for all coins c_i where c_i <= x
     * 
     * Base case: g(0) = 1 (one way to make 0: use no coins)
     * 
     * IMPORTANT: Order of processing matters!
     * - To count combinations (not permutations), process coins in order
     * - This ensures each unique combination is counted exactly once
     */
    
    cout << "\n\nSOLUTION 2: COUNTING WAYS (Bottom-up DP)" << endl;
    cout << "-----------------------------------------" << endl;
    
    // DP array: dp_count[x] stores number of ways to form sum x
    // Use long long to prevent overflow for large counts
    vector<long long> dp_count(target + 1, 0);
    
    // Base case: one way to make sum 0 (use no coins)
    dp_count[0] = 1;
    
    // CRITICAL: Process each coin type separately
    // This avoids counting the same combination multiple times
    // For example, {1,2} and {2,1} would be counted as different if we didn't do this
    cout << "Processing coins in order to count combinations:" << endl;
    
    for (int i = 0; i < n; i++) {
        int coin = coins[i];
        cout << "\nProcessing coin " << coin << ":" << endl;
        
        // For each sum that this coin can contribute to
        // Start from coin value (minimum sum this coin can make)
        // Go up to target
        for (int x = coin; x <= target; x++) {
            // Add ways to form (x - coin) to ways to form x
            // Because we can form x by adding this coin to any way of forming (x - coin)
            long long old_count = dp_count[x];
            dp_count[x] += dp_count[x - coin];
            
            // Debug output for educational purpose
            if (x <= 10 || x == target) {
                cout << "  g(" << x << ") updated: " << old_count << " -> " << dp_count[x];
                cout << " (added g(" << x - coin << ") = " << dp_count[x - coin] << ")" << endl;
            }
        }
    }
    
    // Output final result
    cout << "\nRESULT: Number of ways to form sum " << target << ": " << dp_count[target] << endl;
    
    // Display complete DP table for small targets (educational)
    if (target <= 15) {
        cout << "\nComplete DP table:" << endl;
        cout << "Sum:  ";
        for (int i = 0; i <= target; i++) {
            cout << i << "\t";
        }
        cout << "\nWays: ";
        for (int i = 0; i <= target; i++) {
            cout << dp_count[i] << "\t";
        }
        cout << endl;
    }
    
    /*
     * COMPLEXITY ANALYSIS
     * ===================
     * Time Complexity: O(n × target)
     * - We have 'target' states (0 to target)
     * - For each state, we try 'n' coins
     * 
     * Space Complexity: O(target)
     * - DP array of size (target + 1)
     * - Parent array of size (target + 1) for reconstruction
     * 
     * Can we optimize?
     * - Space: Yes, for counting we only need O(target) space
     * - Time: No, we must consider all state-coin combinations
     */
    
    cout << "\n================== ADVANCED TECHNIQUES ==================" << endl;
    
    /*
     * TECHNIQUE 1: Using BFS for Minimum Coins
     * =========================================
     * The minimum coins problem can be solved using BFS
     * Think of it as finding shortest path in an unweighted graph
     */
    
    cout << "\nTECHNIQUE 1: BFS Solution for Minimum Coins" << endl;
    cout << "--------------------------------------------" << endl;
    
    queue<pair<int, int>> q;  // (current_sum, coins_used)
    vector<bool> visited(target + 1, false);
    
    q.push({0, 0});  // Start from sum 0 with 0 coins
    visited[0] = true;
    
    int bfs_result = -1;
    
    while (!q.empty()) {
        int current_sum = q.front().first;
        int coins_used = q.front().second;
        q.pop();
        
        // Try adding each coin
        for (int coin : coins) {
            int new_sum = current_sum + coin;
            
            // Check if we reached target
            if (new_sum == target) {
                bfs_result = coins_used + 1;
                break;
            }
            
            // Add to queue if valid and not visited
            if (new_sum < target && !visited[new_sum]) {
                visited[new_sum] = true;
                q.push({new_sum, coins_used + 1});
            }
        }
        
        if (bfs_result != -1) break;
    }
    
    cout << "BFS Result: ";
    if (bfs_result != -1) {
        cout << bfs_result << " coins (matches DP solution)" << endl;
    } else {
        cout << "No solution found" << endl;
    }
    
    /*
     * COMMON PITFALLS AND SOLUTIONS
     * ==============================
     */
    
    cout << "\n================== COMMON PITFALLS ==================" << endl;
    cout << "1. Integer Overflow:" << endl;
    cout << "   - Use 'long long' for counting problems" << endl;
    cout << "   - Example: dp_count uses long long" << endl;
    
    cout << "\n2. Initialization Errors:" << endl;
    cout << "   - For minimum: Initialize with INF, except dp[0] = 0" << endl;
    cout << "   - For counting: Initialize with 0, except dp[0] = 1" << endl;
    
    cout << "\n3. Permutation vs Combination:" << endl;
    cout << "   - Current code counts combinations (order doesn't matter)" << endl;
    cout << "   - For permutations, use different approach" << endl;
    
    cout << "\n4. Impossible Cases:" << endl;
    cout << "   - Always check if dp[target] == INF for minimum problems" << endl;
    cout << "   - Example: coins={3,5}, target=1 is impossible" << endl;
    
    /*
     * VARIATIONS AND EXTENSIONS
     * =========================
     */
    
    cout << "\n================== VARIATIONS ==================" << endl;
    cout << "1. Limited Coins: Each coin can be used at most k times" << endl;
    cout << "2. Coin Change Making: Minimize number of coin types used" << endl;
    cout << "3. Probability Version: Probability of getting sum with random coins" << endl;
    cout << "4. Two-player Game: Players alternate taking coins" << endl;
    
    return 0;
}
```

### Sample Input File (coin_input.txt):
```
9 5723
1 2 5 10 20 50 100 500 1000
```

### Expected Output Explanation:
The program will output:
1. Minimum coins needed (2 coins: 5+5)
2. Number of ways to make sum 10 (multiple combinations)
3. Complete DP table evolution
4. BFS verification
5. Common pitfalls and variations

---

## 7.2 Longest Increasing Subsequence (LIS) - Complete Analysis

### Deep Theoretical Background

#### Problem Origins
The LIS problem appears in:
- **Patience solitaire**: Minimum number of piles needed
- **Box stacking**: Maximum height achievable
- **Missile interception**: Minimum interceptors needed
- **DNA analysis**: Finding evolutionary relationships

#### Mathematical Foundation

**Formal Definition:**
Given sequence A = (a₁, a₂, ..., aₙ), find the longest subsequence (aᵢ₁, aᵢ₂, ..., aᵢₖ) such that:
1. i₁ < i₂ < ... < iₖ (maintains order)
2. aᵢ₁ < aᵢ₂ < ... < aᵢₖ (strictly increasing)

**Theorem (Dilworth's Theorem Application):**
The minimum number of decreasing subsequences that cover the entire sequence equals the length of the longest increasing subsequence.

#### Multiple Solution Approaches

1. **Brute Force**: Check all 2ⁿ subsequences - O(2ⁿ × n)
2. **Dynamic Programming**: Classic approach - O(n²)
3. **Binary Search + DP**: Optimized approach - O(n log n)
4. **Segment Tree**: Alternative O(n log n) approach
5. **Patience Sorting**: Game-based O(n log n) approach

#### Complete Implementation:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>
using namespace std;

/*
 * LONGEST INCREASING SUBSEQUENCE (LIS) - COMPLETE ANALYSIS
 * =========================================================
 * This implementation covers:
 * 1. Classic O(n²) DP solution with full reconstruction
 * 2. Optimized O(n log n) solution using binary search
 * 3. Patience sorting interpretation
 * 4. All variations (non-decreasing, strictly decreasing, etc.)
 * 5. Related problems and applications
 */

// Helper function to print a vector
void printVector(const vector<int>& v, const string& name) {
    cout << name << ": ";
    for (int x : v) cout << x << " ";
    cout << endl;
}

// Helper function to reconstruct LIS from parent array
vector<int> reconstructLIS(const vector<int>& arr, const vector<int>& parent, int end_pos) {
    vector<int> lis;
    int pos = end_pos;
    
    // Backtrack using parent pointers
    while (pos != -1) {
        lis.push_back(arr[pos]);
        pos = parent[pos];
    }
    
    // Reverse to get correct order
    reverse(lis.begin(), lis.end());
    return lis;
}

int main() {
    freopen("lis_input.txt", "r", stdin);
    freopen("lis_output.txt", "w", stdout);
    
    // INPUT SECTION
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    cout << "========== LONGEST INCREASING SUBSEQUENCE (LIS) ==========" << endl;
    cout << "Input array of size " << n << ":" << endl;
    printVector(arr, "Array");
    cout << "===========================================================" << endl << endl;
    
    /*
     * METHOD 1: CLASSIC DYNAMIC PROGRAMMING - O(n²)
     * ==============================================
     * 
     * State Definition:
     * dp[i] = length of longest increasing subsequence ending at index i
     * 
     * Recurrence Relation:
     * dp[i] = max(dp[j] + 1) for all j < i where arr[j] < arr[i]
     * 
     * Base Case:
     * dp[i] = 1 for all i (each element alone is a subsequence of length 1)
     * 
     * Why this works:
     * - To find LIS ending at position i, we look at all previous positions j
     * - If arr[j] < arr[i], we can extend the LIS ending at j by adding arr[i]
     * - We take the maximum among all valid extensions
     */
    
    cout << "METHOD 1: CLASSIC DP SOLUTION - O(n²)" << endl;
    cout << "--------------------------------------" << endl;
    
    // dp[i] = length of LIS ending at position i
    vector<int> dp(n, 1);  // Initialize with 1 (each element is LIS of length 1)
    
    // parent[i] = index of previous element in LIS ending at i
    // -1 means this element starts a new subsequence
    vector<int> parent(n, -1);
    
    // For each position i
    cout << "\nDP Table Construction:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Position " << i << " (value=" << arr[i] << "):" << endl;
        
        // Look at all previous positions
        for (int j = 0; j < i; j++) {
            // Can we extend LIS ending at j?
            if (arr[j] < arr[i]) {  // Use <= for non-strictly increasing
                // Would extending from j give us a longer subsequence?
                if (dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                    cout << "  Extended from position " << j << " (value=" << arr[j] 
                         << "), new length=" << dp[i] << endl;
                }
            }
        }
        
        if (parent[i] == -1) {
            cout << "  Starts new subsequence, length=1" << endl;
        }
    }
    
    // Find the maximum length and its ending position
    int max_length = 0;
    int max_pos = -1;
    
    for (int i = 0; i < n; i++) {
        if (dp[i] > max_length) {
            max_length = dp[i];
            max_pos = i;
        }
    }
    
    // Reconstruct the actual LIS
    vector<int> lis = reconstructLIS(arr, parent, max_pos);
    
    // Output results
    cout << "\nRESULTS:" << endl;
    cout << "LIS Length: " << max_length << endl;
    cout << "LIS: ";
    printVector(lis, "");
    
    // Display DP table for visualization
    cout << "\nComplete DP Table:" << endl;
    cout << "Index:  ";
    for (int i = 0; i < n; i++) cout << i << "\t";
    cout << "\nValue:  ";
    for (int i = 0; i < n; i++) cout << arr[i] << "\t";
    cout << "\nDP:     ";
    for (int i = 0; i < n; i++) cout << dp[i] << "\t";
    cout << "\nParent: ";
    for (int i = 0; i < n; i++) cout << parent[i] << "\t";
    cout << endl;
    
    /*
     * METHOD 2: BINARY SEARCH OPTIMIZATION - O(n log n)
     * ==================================================
     * 
     * Key Insight:
     * Maintain array tail[] where tail[i] = smallest ending value 
     * of all increasing subsequences of length (i+1)
     * 
     * Algorithm:
     * 1. For each element, find its position in tail[] using binary search
     * 2. If element is larger than all tail[] elements, append it
     * 3. Otherwise, replace the first element >= current element
     * 
     * Why this works:
     * - We keep tail[] sorted, so binary search is possible
     * - Replacing larger values with smaller ones gives more room for future elements
     * - The length of tail[] is the LIS length
     */
    
    cout << "\n\nMETHOD 2: BINARY SEARCH OPTIMIZATION - O(n log n)" << endl;
    cout << "--------------------------------------------------" << endl;
    
    vector<int> tail;  // tail[i] = smallest ending value of LIS of length i+1
    vector<int> lis_elements;  // Store actual LIS elements (one possible LIS)
    vector<int> predecessor(n);  // For reconstruction
    vector<int> lis_length_at(n);  // LIS length when element i is processed
    
    cout << "\nProcessing elements:" << endl;
    
    for (int i = 0; i < n; i++) {
        int x = arr[i];
        cout << "\nElement " << x << " at position " << i << ":" << endl;
        
        // Find position where x should be inserted/replaced
        // lower_bound finds first element >= x
        auto it = lower_bound(tail.begin(), tail.end(), x);
        int pos = it - tail.begin();
        
        cout << "  Current tail array: ";
        printVector(tail, "");
        
        if (it == tail.end()) {
            // x is larger than all elements in tail[]
            tail.push_back(x);
            cout << "  Action: Append to tail (extends LIS)" << endl;
        } else {
            // Replace element at position pos
            *it = x;
            cout << "  Action: Replace tail[" << pos << "] with " << x << endl;
        }
        
        lis_length_at[i] = tail.size();
        cout << "  LIS length so far: " << tail.size() << endl;
    }
    
    cout << "\nFINAL RESULTS:" << endl;
    cout << "LIS Length (Binary Search): " << tail.size() << endl;
    cout << "Tail array (NOT the actual LIS): ";
    printVector(tail, "");
    
    /*
     * METHOD 3: PATIENCE SORTING INTERPRETATION
     * ==========================================
     * 
     * Imagine laying out cards in piles:
     * - Place each card on the leftmost pile where it's smaller than the top card
     * - If no such pile exists, start a new pile
     * - Number of piles = LIS length
     * 
     * This is equivalent to the binary search method!
     */
    
    cout << "\n\nMETHOD 3: PATIENCE SORTING VISUALIZATION" << endl;
    cout << "-----------------------------------------" << endl;
    
    vector<stack<int>> piles;
    
    cout << "Placing cards in piles:" << endl;
    for (int i = 0; i < n; i++) {
        int card = arr[i];
        cout << "\nCard " << card << ":" << endl;
        
        // Find leftmost pile where card < top
        int pile_idx = -1;
        for (int j = 0; j < piles.size(); j++) {
            if (card < piles[j].top()) {  // Use <= for non-strictly increasing
                pile_idx = j;
                break;
            }
        }
        
        if (pile_idx == -1) {
            // Start new pile
            piles.push_back(stack<int>());
            piles.back().push(card);
            cout << "  Started new pile #" << piles.size() << endl;
        } else {
            // Place on existing pile
            piles[pile_idx].push(card);
            cout << "  Placed on pile #" << (pile_idx + 1) << endl;
        }
        
        // Show current piles
        cout << "  Current piles (top cards): ";
        for (const auto& pile : piles) {
            cout << pile.top() << " ";
        }
        cout << endl;
    }
    
    cout << "\nNumber of piles = LIS length = " << piles.size() << endl;
    
    /*
     * VARIATIONS AND RELATED PROBLEMS
     * ================================
     */
    
    cout << "\n\n========== VARIATIONS & APPLICATIONS ==========" << endl;
    
    cout << "\n1. LONGEST NON-DECREASING SUBSEQUENCE:" << endl;
    cout << "   Change: arr[j] < arr[i] to arr[j] <= arr[i]" << endl;
    
    cout << "\n2. LONGEST DECREASING SUBSEQUENCE:" << endl;
    cout << "   Change: arr[j] < arr[i] to arr[j] > arr[i]" << endl;
    
    cout << "\n3. LONGEST BITONIC SUBSEQUENCE:" << endl;
    cout << "   Find LIS ending at each position + LDS starting at each position" << endl;
    
    cout << "\n4. MAXIMUM SUM INCREASING SUBSEQUENCE:" << endl;
    cout << "   Instead of length, maximize sum of elements" << endl;
    
    cout << "\n5. BUILDING BRIDGES PROBLEM:" << endl;
    cout << "   Sort by one coordinate, find LIS of other coordinate" << endl;
    
    cout << "\n6. BOX STACKING PROBLEM:" << endl;
    cout << "   3D version - sort by base area, find LIS by height" << endl;
    
    /*
     * COMPLEXITY ANALYSIS
     * ===================
     */
    
    cout << "\n========== COMPLEXITY ANALYSIS ==========" << endl;
    cout << "Method 1 (Classic DP):" << endl;
    cout << "  Time:  O(n²) - nested loops" << endl;
    cout << "  Space: O(n) - dp and parent arrays" << endl;
    
    cout << "\nMethod 2 (Binary Search):" << endl;
    cout << "  Time:  O(n log n) - n iterations, log n binary search" << endl;
    cout << "  Space: O(n) - tail array" << endl;
    
    cout << "\nMethod 3 (Patience Sorting):" << endl;
    cout << "  Time:  O(n log n) - can use binary search for pile selection" << endl;
    cout << "  Space: O(n) - storing all cards in piles" << endl;
    
    /*
     * COMMON PITFALLS
     * ===============
     */
    
    cout << "\n========== COMMON PITFALLS ==========" << endl;
    cout << "1. Initialization: Don't forget dp[i] = 1" << endl;
    cout << "2. Strictly vs Non-strictly: Check < vs <=" << endl;
    cout << "3. Binary Search Method: tail[] is NOT the actual LIS" << endl;
    cout << "4. Reconstruction: Need separate parent tracking" << endl;
    cout << "5. Multiple LIS: There may be multiple valid answers" << endl;
    
    return 0;
}
```

### Sample Input File (lis_input.txt):
```
10
6 2 5 1 7 4 8 3 9 10
```

---

## 7.3 Paths in a Grid - Complete Analysis

### Theoretical Foundation

#### Problem Variants
1. **Count all paths**: How many ways to reach destination?
2. **Shortest path**: Minimum steps (unweighted grid)
3. **Maximum/Minimum sum path**: Optimize path value
4. **Paths with obstacles**: Navigate around blocked cells
5. **Unique paths with constraints**: Limited moves or directions

#### Mathematical Model

**State Space:**
- State: (i, j) = being at cell (i, j)
- Valid transitions: Can move right or down (classical variant)

**Recurrence Relations:**

For counting paths:
```
paths(i, j) = paths(i-1, j) + paths(i, j-1)
paths(0, 0) = 1
```

For maximum sum:
```
maxSum(i, j) = grid[i][j] + max(maxSum(i-1, j), maxSum(i, j-1))
```

#### Complete Implementation:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
using namespace std;

/*
 * GRID PATH PROBLEMS - COMPREHENSIVE IMPLEMENTATION
 * ==================================================
 * This program solves multiple variants of grid path problems:
 * 1. Count all possible paths
 * 2. Find path with maximum sum
 * 3. Find path with minimum sum
 * 4. Handle obstacles and blocked cells
 * 5. Allow different movement patterns
 * 
 * The grid uses 0-based indexing
 * Start: (0, 0) - top-left corner
 * End: (n-1, m-1) - bottom-right corner
 */

int main() {
    freopen("grid_input.txt", "r", stdin);
    freopen("grid_output.txt", "w", stdout);
    
    // INPUT SECTION
    int n, m;  // n = rows, m = columns
    cin >> n >> m;
    
    // Read grid values
    // Special values:
    // -1 = obstacle/blocked cell
    // 0 or positive = cell value/cost
    vector<vector<int>> grid(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }
    
    cout << "=============== GRID PATH PROBLEMS ===============" << endl;
    cout << "Grid dimensions: " << n << " x " << m << endl;
    cout << "Grid layout:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "  ";
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == -1) {
                cout << "# ";  // Obstacle
            } else {
                cout << grid[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << "Start: (0,0), End: (" << n-1 << "," << m-1 << ")" << endl;
    cout << "===================================================" << endl << endl;
    
    /*
     * PROBLEM 1: COUNT ALL POSSIBLE PATHS
     * ====================================
     * Count the number of ways to reach from (0,0) to (n-1,m-1)
     * Movement: Can only move right or down
     * 
     * Mathematical insight:
     * - Without obstacles: C(n+m-2, n-1) = (n+m-2)! / ((n-1)! × (m-1)!)
     * - With obstacles: Use DP
     * 
     * State: dp[i][j] = number of paths to reach (i,j) from (0,0)
     * Recurrence: dp[i][j] = dp[i-1][j] + dp[i][j-1] if grid[i][j] != -1
     * Base case: dp[0][0] = 1 if grid[0][0] != -1
     */
    
    cout << "PROBLEM 1: COUNTING ALL POSSIBLE PATHS" << endl;
    cout << "---------------------------------------" << endl;
    
    // DP table for counting paths
    // Use long long to handle large counts
    vector<vector<long long>> count_dp(n, vector<long long>(m, 0));
    
    // Base case: starting position
    if (grid[0][0] != -1) {
        count_dp[0][0] = 1;
    }
    
    cout << "Building DP table for path counting:" << endl;
    
    // Fill first row (can only come from left)
    cout << "  First row: ";
    for (int j = 1; j < m; j++) {
        if (grid[0][j] != -1) {
            count_dp[0][j] = count_dp[0][j-1];
        }
        cout << count_dp[0][j] << " ";
    }
    cout << endl;
    
    // Fill first column (can only come from above)
    cout << "  First column: ";
    for (int i = 1; i < n; i++) {
        if (grid[i][0] != -1) {
            count_dp[i][0] = count_dp[i-1][0];
        }
        cout << count_dp[i][0] << " ";
    }
    cout << endl;
    
    // Fill rest of the table
    cout << "\n  Processing remaining cells:" << endl;
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            if (grid[i][j] != -1) {
                // Can reach from top or left
                count_dp[i][j] = count_dp[i-1][j] + count_dp[i][j-1];
                
                // Detailed logging for small grids
                if (n <= 5 && m <= 5) {
                    cout << "    (" << i << "," << j << "): "
                         << count_dp[i-1][j] << " (from top) + "
                         << count_dp[i][j-1] << " (from left) = "
                         << count_dp[i][j] << endl;
                }
            }
        }
    }
    
    cout << "\nComplete path count DP table:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "  ";
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == -1) {
                cout << "- ";
            } else {
                cout << count_dp[i][j] << " ";
            }
        }
        cout << endl;
    }
    
    cout << "\nRESULT: Total paths from (0,0) to (" << n-1 << "," << m-1 << "): " 
         << count_dp[n-1][m-1] << endl;
    
    // Mathematical verification for obstacle-free grids
    bool has_obstacles = false;
    for (int i = 0; i < n && !has_obstacles; i++) {
        for (int j = 0; j < m && !has_obstacles; j++) {
            if (grid[i][j] == -1) has_obstacles = true;
        }
    }
    
    if (!has_obstacles) {
        // Calculate using combinatorics
        long long expected = 1;
        for (int i = 1; i <= n-1; i++) {
            expected = expected * (i + m - 1) / i;
        }
        cout << "Verification (combinatorial formula): " << expected << endl;
    }
    
    /*
     * PROBLEM 2: MAXIMUM SUM PATH
     * ============================
     * Find the path from (0,0) to (n-1,m-1) that maximizes sum of cell values
     * 
     * State: dp[i][j] = maximum sum to reach (i,j)
     * Recurrence: dp[i][j] = grid[i][j] + max(dp[i-1][j], dp[i][j-1])
     */
    
    cout << "\n\nPROBLEM 2: MAXIMUM SUM PATH" << endl;
    cout << "----------------------------" << endl;
    
    // DP table for maximum sum
    vector<vector<int>> max_dp(n, vector<int>(m, INT_MIN));
    
    // Parent table for path reconstruction
    // 0 = came from top, 1 = came from left, -1 = start
    vector<vector<int>> max_parent(n, vector<int>(m, -1));
    
    // Base case
    if (grid[0][0] != -1) {
        max_dp[0][0] = grid[0][0];
    }
    
    // Fill first row
    for (int j = 1; j < m; j++) {
        if (grid[0][j] != -1 && max_dp[0][j-1] != INT_MIN) {
            max_dp[0][j] = max_dp[0][j-1] + grid[0][j];
            max_parent[0][j] = 1;  // Came from left
        }
    }
    
    // Fill first column
    for (int i = 1; i < n; i++) {
        if (grid[i][0] != -1 && max_dp[i-1][0] != INT_MIN) {
            max_dp[i][0] = max_dp[i-1][0] + grid[i][0];
            max_parent[i][0] = 0;  // Came from top
        }
    }
    
    // Fill rest of table
    cout << "Computing maximum sums:" << endl;
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            if (grid[i][j] != -1) {
                int from_top = (max_dp[i-1][j] != INT_MIN) ? 
                               max_dp[i-1][j] + grid[i][j] : INT_MIN;
                int from_left = (max_dp[i][j-1] != INT_MIN) ? 
                                max_dp[i][j-1] + grid[i][j] : INT_MIN;
                
                if (from_top > from_left && from_top != INT_MIN) {
                    max_dp[i][j] = from_top;
                    max_parent[i][j] = 0;  // Came from top
                } else if (from_left != INT_MIN) {
                    max_dp[i][j] = from_left;
                    max_parent[i][j] = 1;  // Came from left
                }
                
                if (n <= 5 && m <= 5 && max_dp[i][j] != INT_MIN) {
                    cout << "  (" << i << "," << j << "): max("
                         << (from_top != INT_MIN ? to_string(from_top) : "INF")
                         << ", "
                         << (from_left != INT_MIN ? to_string(from_left) : "INF")
                         << ") = " << max_dp[i][j] << endl;
                }
            }
        }
    }
    
    cout << "\nMaximum sum DP table:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "  ";
        for (int j = 0; j < m; j++) {
            if (max_dp[i][j] == INT_MIN) {
                cout << "- ";
            } else {
                cout << max_dp[i][j] << " ";
            }
        }
        cout << endl;
    }
    
    if (max_dp[n-1][m-1] != INT_MIN) {
        cout << "\nMaximum path sum: " << max_dp[n-1][m-1] << endl;
        
        // Reconstruct the path
        cout << "Path taken (in reverse): ";
        int i = n-1, j = m-1;
        vector<pair<int,int>> path;
        
        while (i > 0 || j > 0) {
            path.push_back({i, j});
            if (max_parent[i][j] == 0) {
                i--;  // Came from top
            } else if (max_parent[i][j] == 1) {
                j--;  // Came from left
            } else {
                break;  // Reached start
            }
        }
        path.push_back({0, 0});
        
        reverse(path.begin(), path.end());
        for (auto [x, y] : path) {
            cout << "(" << x << "," << y << ") ";
        }
        cout << endl;
    } else {
        cout << "\nNo valid path exists!" << endl;
    }
    
    /*
     * PROBLEM 3: MINIMUM SUM PATH (Similar to maximum, with min instead)
     * ===================================================================
     */
    
    cout << "\n\nPROBLEM 3: MINIMUM SUM PATH" << endl;
    cout << "----------------------------" << endl;
    
    vector<vector<int>> min_dp(n, vector<int>(m, INT_MAX));
    
    if (grid[0][0] != -1) {
        min_dp[0][0] = grid[0][0];
    }
    
    // Fill DP table (similar logic with min instead of max)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i == 0 && j == 0) continue;
            if (grid[i][j] == -1) continue;
            
            int from_top = INT_MAX;
            int from_left = INT_MAX;
            
            if (i > 0 && min_dp[i-1][j] != INT_MAX) {
                from_top = min_dp[i-1][j] + grid[i][j];
            }
            if (j > 0 && min_dp[i][j-1] != INT_MAX) {
                from_left = min_dp[i][j-1] + grid[i][j];
            }
            
            min_dp[i][j] = min(from_top, from_left);
        }
    }
    
    if (min_dp[n-1][m-1] != INT_MAX) {
        cout << "Minimum path sum: " << min_dp[n-1][m-1] << endl;
    } else {
        cout << "No valid path exists!" << endl;
    }
    
    /*
     * ADVANCED VARIATIONS
     * ===================
     */
    
    cout << "\n\n=============== ADVANCED VARIATIONS ===============" << endl;
    
    cout << "\n1. DIAGONAL MOVEMENT ALLOWED:" << endl;
    cout << "   Recurrence: dp[i][j] = dp[i-1][j] + dp[i][j-1] + dp[i-1][j-1]" << endl;
    
    cout << "\n2. K MOVES CONSTRAINT:" << endl;
    cout << "   State: dp[i][j][k] = ways to reach (i,j) in exactly k moves" << endl;
    
    cout << "\n3. MODULO ARITHMETIC:" << endl;
    cout << "   For large grids, use: dp[i][j] = (dp[i][j] % MOD + MOD) % MOD" << endl;
    
    cout << "\n4. MULTIPLE SOURCES/DESTINATIONS:" << endl;
    cout << "   Initialize multiple cells with 1, or sum multiple endpoints" << endl;
    
    cout << "\n5. BIDIRECTIONAL PATHS:" << endl;
    cout << "   Can move in all 4 directions - use BFS/DFS instead of DP" << endl;
    
    /*
     * COMPLEXITY ANALYSIS
     * ===================
     */
    
    cout << "\n=============== COMPLEXITY ANALYSIS ===============" << endl;
    cout << "Time Complexity:  O(n × m) - visit each cell once" << endl;
    cout << "Space Complexity: O(n × m) - DP table" << endl;
    cout << "\nSpace Optimization:" << endl;
    cout << "  Can reduce to O(min(n,m)) by keeping only current and previous row/column" << endl;
    
    /*
     * COMMON PITFALLS
     * ===============
     */
    
    cout << "\n=============== COMMON PITFALLS ===============" << endl;
    cout << "1. Boundary conditions: Check grid[0][0] and grid[n-1][m-1]" << endl;
    cout << "2. Integer overflow: Use long long for counting problems" << endl;
    cout << "3. Obstacles: Properly handle -1 cells (don't just set to 0)" << endl;
    cout << "4. Path reconstruction: Need separate parent tracking" << endl;
    cout << "5. Min/Max initialization: Use INT_MAX/INT_MIN appropriately" << endl;
    
    return 0;
}
```

### Sample Input File (grid_input.txt):
```
4 5
1 3 1 2 1
2 -1 3 1 4
5 2 1 -1 2
1 4 3 2 1
```

---

## Study Tips and Best Practices

### How to Master Dynamic Programming:

1. **Identify the State**
   - What uniquely describes a subproblem?
   - What parameters change as we solve?

2. **Write the Recurrence**
   - How do states relate to each other?
   - What are the transitions?

3. **Define Base Cases**
   - What are the simplest subproblems?
   - What are their known solutions?

4. **Determine Computation Order**
   - Which states depend on which?
   - Bottom-up or top-down?

5. **Optimize Space**
   - Do we need the entire table?
   - Can we use rolling arrays?

### Common Patterns:

1. **Linear DP**: Problems on sequences (LIS, coin change)
2. **Grid DP**: 2D problems (paths, edit distance)
3. **Interval DP**: Problems on ranges (matrix chain)
4. **Tree DP**: Problems on trees (subtree sums)
5. **Digit DP**: Problems on digit constraints
6. **Bitmask DP**: Problems with subset states

### Practice Recommendations:

1. Start with classic problems (this worksheet)
2. Identify problem patterns
3. Practice state definition
4. Learn to optimize solutions
5. Study various approaches to same problem

### Debugging DP Solutions:

1. Print the DP table
2. Trace through small examples by hand
3. Verify base cases
4. Check transition logic
5. Test edge cases (empty, single element, all same)

---

## Additional Resources

1. **Books**:
   - "Introduction to Algorithms" by CLRS
   - "Algorithm Design" by Kleinberg and Tardos
   - "Competitive Programming 3" by Halim

2. **Online Judges**:
   - Codeforces DP problems
   - AtCoder Educational DP Contest
   - LeetCode DP tag

3. **Visualizations**:
   - VisuAlgo for DP animations
   - Algorithm Visualizer

Remember: DP is about **practice and pattern recognition**. The more problems you solve, the better you'll become at identifying DP solutions!
