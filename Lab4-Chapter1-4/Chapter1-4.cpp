#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> pi;
#define F first
#define S second

int main() {

ll a = 123456789;
ll b = 987654321;
vi v = {1,2,3,4,5};
pi p = {10,20};

cout << a*b << "\n";
cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " << v[4] << "\n";
cout << p.F << " " << p.S << "\n";

return 0;
}
