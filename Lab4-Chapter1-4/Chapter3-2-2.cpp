#include <iostream>

int main()
{
    struct P
    {
        int x, y;
        bool operator<(const P &p)
        {
            if (x != p.x)
                return x < p.x;
            else
                return y < p.y;
        }
    };

    P a = {2, 2}, b = {2, 1};
    if (a < b)
        std::cout << "a < b\n";
    else
        std::cout << "a >= b\n";

    return 0;
}