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

class TheDevice {
public:
    int minimumAdditional(vector <string>);
};

int TheDevice::minimumAdditional(vector <string> plates) {
    int n = plates[0].size();
    vector<int> z(n, 0), o(n, 0);
    for(int i = 0; i < plates.size(); ++i) {
        for(int j = 0; j < n; ++j) {
            if(plates[i][j] == '0') {
                ++z[j];
            }
            else ++o[j];
        }
    }
    int ans = 0;
    for(int j = 0; j < n; ++j) {
        ans = max(ans, 3-min(z[j],1)-min(o[j],2));
    }
    return ans;
}