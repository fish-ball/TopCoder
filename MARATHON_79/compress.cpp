#include <fstream>
#include <map>
using namespace std;
// seed	mw	n	co	sa	sun	sna	blk	score
class Rec {
public:
    int seed;
    int mw;
    int n;
    int co;
    int sa;
    int sun;
    int sna;
    int blk;
    int score;
};

int main() {
    ifstream fin("raw_out.txt");
    ofstream fout("retain.txt");
    fout << "seed	mw	n	co	sa	sun	sna	blk	score" << endl;
    map<int, Rec> mp;
    Rec tmp;
    while(fin >> tmp.seed >> tmp.mw >> tmp.n
            >> tmp.co >> tmp.sa >> tmp.sun >> tmp.sna
            >> tmp.blk >> tmp.score) {
        if(mp.find(tmp.seed) == mp.end() ||
                mp[tmp.seed].score > tmp.score) {
            mp[tmp.seed] = tmp;
        }
    }
    for(map<int, Rec>::iterator it = mp.begin(); it != mp.end(); ++it) {
// seed	mw	n	co	sa	sun	sna	blk	score
        fout << it->second.seed << "\t";
        fout << it->second.mw << "\t";
        fout << it->second.n << "\t";
        fout << it->second.co << "\t";
        fout << it->second.sa << "\t";
        fout << it->second.sun << "\t";
        fout << it->second.sna << "\t";
        fout << it->second.blk << "\t";
        fout << it->second.score << endl;
    }
}
