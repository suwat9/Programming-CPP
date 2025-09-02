void f(int n) {
if (n == 1) return;
f(n-1);
}

void g(int n) {
if (n == 1) return;
g(n-1);
g(n-1);
}
int main() {
f(5); 
g(5); 
return 0;
}