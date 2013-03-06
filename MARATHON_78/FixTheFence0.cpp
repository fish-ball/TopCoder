#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class FixTheFence {
    
    vector<string> dg;
    
    int n, cnt[4], all;
    
    string genA(int x, int y) {
    }
    
    string genB(int x, int y) {
    }
    
    int evaluate(string result) {
    }
    
public:
    
    string findLoop(const vector<string> &diagram) {
        
        // reset the local variables.
        dg = diagram;
        n = diagram.size();
        all = 0;
        for(int i = 0; i < 4; ++i) cnt[i] = 0;
        
        // count the cells.
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                if(dg[i][j] != '-') {
                    all++;
                    cnt[dg[i][j] - '0']++;
                }
            }
        }
        
        if(cnt[2] >= cnt[0] && cnt[2] >= cnt[1]) {
            string ans = "0 0 ";
            
            int x = 0, y = 0;
            
            while(x + 2 <= n) {
                // if not begin, down 1 unit.
                if(x > 0) {
                    ans += 'D';
                    x += 1;
                }
                
                // move right most.
                while(y < n) {
                    ans += 'R';
                    y += 1;
                }
                // down 1 unit on the right side.
                ans += 'D';
                x += 1;
                // move left to 1.
                while(y > 1) {
                    ans += 'L';
                    y -= 1;
                }
            }
            
            while(y > 0) {
                ans += 'L';
                y -= 1;
            }
            
            while(x > 0) {
                ans += 'U';
                x -= 1;
            }
            
            return ans;
        }
        
        if(cnt[1] >= cnt[0] && cnt[1] >= cnt[2]) {
            string ans = "0 0 ";
            
            int x = 0, y = 0;
            
            while(x + 4 <= n) {
                // if not begin, down 2 unit.
                if(x > 0) {
                    ans += 'D';
                    ans += 'D';
                    x += 2;
                }
                // move right most.
                while(y < n) {
                    ans += 'R';
                    y += 1;
                }
                // down 2 unit on the right side.
                ans += 'D';
                ans += 'D';
                x += 2;
                // move left to 2.
                while(y > 2) {
                    ans += 'L';
                    y -= 1;
                }
            }
            
            while(y > 0) {
                ans += 'L';
                y -= 1;
            }
            
            while(x > 0) {
                ans += 'U';
                x -= 1;
            }
            
            if(n % 4 > 0) ans[0] = '1';
            
            return ans;
        }
        
        return "0 0 RDLU";
    }
};
