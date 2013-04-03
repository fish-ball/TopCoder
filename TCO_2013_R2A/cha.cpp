#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    int n;
    while(cin >> n) {
    for(int i = 0; i < n; ++i) {
        cout << char('a'+rand() % 3);
    }
    cout << endl;
    for(int i = 0; i < n; ++i) {
        cout << char('a'+rand() % 3);
    }
    cout << endl;
    }
}
/*

1000
2,456,23,0,875,12,68,99,100,654
852,765,23,657,222,23,863,12,555,1
1,2,3,4,5,6,2,9,1,7


*/
