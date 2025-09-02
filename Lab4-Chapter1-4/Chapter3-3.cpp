#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
    vector<int> v = {4, 2, 5, 3, 6, 8, 9};
    int n = v.size();
    int x = 3; // key to find
    sort(v.begin(), v.end());
    for (int i = 0; i < n; i++)
    {
        cout << v[i] << " ";
    } cout << "\n";

    int a = 0, b = n - 1;
    while (a <= b)
    {
        int k = (a + b) / 2;
        if (v[k] == x)
        {
            cout<< "Found data at position " << k << "\n";
        }
        if (v[k] > x)
            b = k - 1;
        else
            a = k + 1;
    }
    return 0;
}