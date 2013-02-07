#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

class TheJediTest {
public:
    int minimumSupervisors(vector <int>, int);
};


int TheJediTest::minimumSupervisors(vector <int> s, int K) {
    int n = s.size();
    int ans = 0;
    vector <int> z(n, 0);
    for(int i = 0; i < n-1; ++i) {
        int a = s[i] + z[i];
        int m = a - a % K;
        ans += a / K;
        int p = min(m, z[i]);
        m -= p;
        z[i] -= p;
        s[i] -= m;
        if(z[i] > 0) {
            p = min(s[i+1], K-s[i]-z[i]);
            s[i+1] -= p;
            ans++;
        }
        else {
            z[i+1] = s[i];
        }
        z[i] = s[i] = 0;
    }
    ans += (z[n-1]+s[n-1]) / K;
    if((z[n-1]+s[n-1])%K) ++ans;
    return ans;
}