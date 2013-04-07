#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

char sbuf[20];

string st[] = {
    "DOWN",
    "DOWN",
    "OFF",
    "CENTER",
    "OFF",
    
    "OFF",
    "OFF",
    "OFF",
    "OFF",
    "OFF",
    
    "OFF",
    "OFF",
    "OFF",
    "OFF",
    "OFF",
    
    "DOWN",
    "OFF",
    "CENTER",
    "OFF",
    "OFF",
    
    "UP",
    "ON"
};

class RobonautEye {
    
    int G1[2000][2000], G2[2000][2000], W, H;

public:
    vector <string> recognizeObjects(vector <int> limg, vector <int> rimg) {
        W = limg[0];
        H = limg[1];
        for(int i = 0; i < W; ++i) {
            for(int j = 0; j < H; ++j) {
                G1[i][j] = limg[2+i*H+j];
                G2[i][j] = rimg[2+i*H+j];
            }
        }
        vector<string> ret;
        for(int i = 0; i < 22; ++i) {
            sprintf(sbuf, "%s,%d,%d,%d,%d", st[i].c_str(), H/2, W/2, H/2, W/2);
            ret.push_back(sbuf);
        }
        return ret;
    }
};

RobonautEye re;

int main() {
    int len;
    cin >> len;
    vector<int> limg(len), rimg(len);
    for(int i = 0; i < len; ++i) cin >> limg[i];
    for(int i = 0; i < len; ++i) cin >> rimg[i];
    vector<string> ret = re.recognizeObjects(limg, rimg);
    for(int i = 0; i < ret.size(); ++i) {
        cout << ret[i] << endl;
    }
}
