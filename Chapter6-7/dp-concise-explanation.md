# Dynamic Programming - Concise Theory & Examples

## What is Dynamic Programming?

**Definition**: Dynamic Programming (DP) solves complex problems by breaking them into smaller subproblems, solving each subproblem only once, and storing results to avoid redundant calculations.

### Core Principle
```
BIG PROBLEM
     ↓
SMALLER SUBPROBLEMS (Overlapping)
     ↓
STORE SOLUTIONS
     ↓
REUSE STORED SOLUTIONS
     ↓
FINAL ANSWER
```

---

## The DP Theorem

### **Bellman's Principle of Optimality**
> "An optimal solution contains optimal solutions to subproblems"

### Visual Representation
```
                 Optimal Path A→D
                /                \
    Optimal A→B                    Optimal B→D
    (Subproblem 1)                (Subproblem 2)
    
If A→B→D is optimal, then A→B must also be optimal
```

### Two Key Properties Required

```
1. OVERLAPPING SUBPROBLEMS
   ════════════════════════
   F(5)
   ├── F(4)
   │   ├── F(3)  ←┐
   │   └── F(2)   │ Same subproblems
   └── F(3) ──────┘ appear multiple times!
   
2. OPTIMAL SUBSTRUCTURE
   ════════════════════
   MinCoins(11) = 1 + MinCoins(10)  [use 1 coin]
                = 1 + MinCoins(6)   [use 5 coin]
                = 1 + MinCoins(1)   [use 10 coin]
                        ↑
                Choose minimum
```

---

## Example 1: Coin Change Problem

### Problem Statement
```
Given: Coins = [1, 5, 10, 25]
Find: Minimum coins to make 30 cents

Without DP: Try all combinations = exponential time
With DP: Build solution from 0 to 30 = linear time
```

### DP Solution Visualization

```
Building Table for Amount 0 to 11 with coins [1, 5, 10]:

Amount: 0  1  2  3  4  5  6  7  8  9  10  11
        ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓   ↓
Min:    0  1  2  3  4  1  2  3  4  5  1   2

How we calculate dp[11]:
dp[11] = min(
    dp[11-1] + 1  = dp[10] + 1 = 1 + 1 = 2  ✓ (use 1)
    dp[11-5] + 1  = dp[6] + 1  = 2 + 1 = 3    (use 5)
    dp[11-10] + 1 = dp[1] + 1  = 1 + 1 = 2  ✓ (use 10)
) = 2
```

### Short Code - Coin Change
```cpp
// Minimum coins to make amount
int coinChange(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, INT_MAX);
    dp[0] = 0;  // Base case
    
    for (int i = 1; i <= amount; i++) {
        for (int coin : coins) {
            if (coin <= i && dp[i - coin] != INT_MAX) {
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }
    
    return dp[amount] == INT_MAX ? -1 : dp[amount];
}

// Example usage
int main() {
    vector<int> coins = {1, 5, 10};
    int amount = 11;
    cout << "Min coins for " << amount << ": " << coinChange(coins, amount);
    // Output: 2 (10 + 1)
}
```

---

## Example 2: Fibonacci (Classic DP)

### Without DP vs With DP
```
WITHOUT DP (Recursive):          WITH DP (Memoization):
═══════════════════════          ═════════════════════
    fib(5)                           fib(5)
    /    \                           /    \
  fib(4)  fib(3)                  fib(4)  fib(3)←cached
  /   \    /   \                  /   \    
fib(3) fib(2) fib(2) fib(1)    fib(3) fib(2)←cached
                                  ↑      ↑
Many repeated calculations!      Calculated once, reused!

Time: O(2^n) → SLOW              Time: O(n) → FAST
```

### Short Code - Fibonacci
```cpp
// Method 1: Top-down (Memoization)
int fibMemo(int n, vector<int>& memo) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];  // Already calculated
    
    memo[n] = fibMemo(n-1, memo) + fibMemo(n-2, memo);
    return memo[n];
}

// Method 2: Bottom-up (Tabulation)
int fibDP(int n) {
    if (n <= 1) return n;
    vector<int> dp(n + 1);
    dp[0] = 0; dp[1] = 1;
    
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }
    return dp[n];
}

// Method 3: Space Optimized
int fibOptimal(int n) {
    if (n <= 1) return n;
    int prev2 = 0, prev1 = 1;
    
    for (int i = 2; i <= n; i++) {
        int current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
```

---

## Example 3: 0/1 Knapsack

### Problem Visualization
```
Items: weight/value
Item 1: 1kg/$1
Item 2: 3kg/$4  
Item 3: 4kg/$5
Item 4: 5kg/$7

Knapsack Capacity: 7kg

DP Table:
        0  1  2  3  4  5  6  7 (capacity)
Item 0  0  0  0  0  0  0  0  0
Item 1  0  1  1  1  1  1  1  1
Item 2  0  1  1  4  5  5  5  5
Item 3  0  1  1  4  5  6  6  9
Item 4  0  1  1  4  5  7  8  9

Answer: dp[4][7] = $9
```

### Short Code - Knapsack
```cpp
int knapsack(vector<int>& weights, vector<int>& values, int W) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= W; w++) {
            // Don't take item i-1
            dp[i][w] = dp[i-1][w];
            
            // Take item i-1 if it fits
            if (weights[i-1] <= w) {
                dp[i][w] = max(dp[i][w], 
                              values[i-1] + dp[i-1][w - weights[i-1]]);
            }
        }
    }
    
    return dp[n][W];
}
```

---

## DP Approaches Comparison

```
TOP-DOWN (Memoization)          BOTTOM-UP (Tabulation)
══════════════════════          ══════════════════════
    Start: Main Problem             Start: Base Cases
           ↓                               ↓
    Break into Subproblems         Build up to Main Problem
           ↓                               ↓
    Store in Memo Table            Store in DP Table
           ↓                               ↓
    Recursive + Cache              Iterative + Table

When to use:                    When to use:
• Natural recursive structure   • Clear iteration order
• Not all subproblems needed   • All subproblems needed
• Easier to write              • Better space optimization
```

---

## Common DP Patterns

### 1. Linear DP
```
dp[i] depends on dp[i-1], dp[i-2], ...
Example: Fibonacci, Climbing Stairs
Pattern: dp[i] = f(dp[i-1], dp[i-2])
```

### 2. Grid DP
```
dp[i][j] depends on dp[i-1][j], dp[i][j-1]
Example: Unique Paths, Edit Distance
Pattern: dp[i][j] = f(dp[i-1][j], dp[i][j-1])
```

### 3. Interval DP
```
dp[i][j] for substring/subarray from i to j
Example: Palindrome, Matrix Chain
Pattern: dp[i][j] = f(dp[i][k], dp[k][j])
```

### 4. Tree DP
```
dp[node] depends on dp[children]
Example: Tree Diameter, Max Path Sum
Pattern: dp[node] = f(dp[left], dp[right])
```

---

## Quick Reference - When to Use DP

### ✅ Use DP When:
```
1. Problem asks for optimal (min/max)
2. Problem can be broken into subproblems
3. Subproblems overlap (appear multiple times)
4. Optimal solution uses optimal subproblem solutions
```

### ❌ Don't Use DP When:
```
1. Simple greedy solution exists
2. No overlapping subproblems
3. Need actual solution path (use backtracking)
4. Problem size too large for memory
```

---

## DP Recipe (5 Steps)

```
STEP 1: DEFINE STATE
What parameters uniquely identify a subproblem?
Example: dp[i] = min coins for amount i

STEP 2: WRITE RECURRENCE
How does current state relate to previous states?
Example: dp[i] = min(dp[i-coin] + 1) for all coins

STEP 3: IDENTIFY BASE CASES
What are the smallest subproblems?
Example: dp[0] = 0 (0 coins for amount 0)

STEP 4: DETERMINE ORDER
In what order should we solve subproblems?
Example: Calculate dp[1], dp[2], ..., dp[n]

STEP 5: IMPLEMENT
Choose top-down or bottom-up approach
```

---

## Time & Space Complexity

```
PROBLEM          TIME        SPACE      OPTIMIZED SPACE
═══════════════════════════════════════════════════════
Fibonacci        O(n)        O(n)       O(1)
Coin Change      O(n×m)      O(n)       -
0/1 Knapsack    O(n×W)      O(n×W)     O(W)
LCS             O(m×n)      O(m×n)     O(min(m,n))
Edit Distance   O(m×n)      O(m×n)     O(min(m,n))

n,m = input sizes, W = capacity
```

---

## Summary

**Dynamic Programming in One Line**: 
> *"Remember solutions to avoid recalculating"*

**Key Insight**: 
> Trade space for time by storing intermediate results

**Success Formula**:
```
Overlapping Subproblems + Optimal Substructure + Memoization = DP Solution
```

**Most Important**: 
Start with recursive solution → Add memoization → Convert to table if needed

This is Dynamic Programming - turning exponential problems into polynomial ones through intelligent caching!