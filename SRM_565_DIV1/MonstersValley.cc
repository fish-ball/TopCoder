#include <vector>
#include <algorithm>
using namespace std;

typedef long long i64;

class MonstersValley {
public:
    int minimumPrice(vector<long long> d, vector <int> p) {
        int n = d.size();
        i64 a[101] = {}, b[101] = {};
        i64 *va = a, *vb = b;
        memset(va, -1, sizeof(a));
        va[0] = 0;
        for(int i = 0; i < n; ++i) {
            memset(vb, -1, sizeof(b));
            for(int k = 0; k <= 100; ++k) {
                if(va[k] > -1) {
                    if(vb[k+p[i]] == -1 || vb[k+p[i]] < d[i]+va[k]) {
                        vb[k+p[i]] = d[i]+va[k];
                    }
                    if(va[k] >= d[i] && (vb[k] == -1 || vb[k] < va[k])) {
                        vb[k] = va[k];
                    }
                }
            }
            swap(va, vb);
        }
        int ans = -1;
        for(int i = 0; i <= 100; ++i) {
            if(va[i] > -1) {
                ans = i;
                break;
            }
        }
        return ans;
    }
};