#include <fstream>
#include <map>
#include <vector>
using namespace std;
// seed	mw	n	co	sa	sun	sna	blk	score
class Rec {
public:
    int seed;
    int r;
    int mw;
    int n;
    int co;
    int sa;
    int sna;
    int score;
};

int main() {
    ifstream fin("raw_out_radius.txt");
    ofstream fout("retain_radius.txt");
    fout << "seed	r	mw	n	co	sa	sna	score" << endl;
    map<int, Rec> mp;
    Rec tmp;
    int last_seed = 0;
    vector<Rec> v;
    while(fin >> tmp.seed >> tmp.r >>tmp.mw >> tmp.n
            >> tmp.co >> tmp.sa >> tmp.sna >> tmp.score) {
        if(last_seed != tmp.seed) {
            int best = 999999999;
            for(int i = 0; i + 5 < v.size(); ++i) {
                int acc = 0;
                for(int j = i; j < i + 5; ++j) {
                    acc += v[j].score;
                }
                if(acc < best) {
                    best = acc;
                    mp[last_seed] = v[i+2];
                }
            }
            v.clear();
            last_seed = tmp.seed;
        }
        else {
            v.push_back(tmp);
        }
    }
    for(map<int, Rec>::iterator it = mp.begin(); it != mp.end(); ++it) {
// seed	mw	n	co	sa	sun	sna	blk	score
        fout << it->second.seed << "\t";
        fout << it->second.r << "\t";
        fout << it->second.mw << "\t";
        fout << it->second.n << "\t";
        fout << it->second.co << "\t";
        fout << it->second.sa << "\t";
        fout << it->second.sna << "\t";
        fout << it->second.score << endl;
    }
    system("pause");
}
