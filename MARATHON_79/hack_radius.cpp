#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

char buf[200];

int main() {
    for(int seed = 4; seed <= 1000; ++seed) {
        ofstream fr("r.txt");
        fr << "-1" << endl;
        fr.close();
        sprintf(buf, "java -jar SnowCleaningVis.jar -exec SnowCleaning -delay 0 -seed %d -novis", seed);
        system(buf);
        ifstream fn("n.txt");
        int n;
        fn >> n;
        fn.close();
        for(int r = 1; r <= n + n - 2; ++r) {
            ofstream frr("r.txt");
            frr << r;
            frr.close();
            ofstream of("out.txt", ios::app);
            of << seed << '\t';
            of.close();
            sprintf(buf, "java -jar SnowCleaningVis.jar -exec SnowCleaning -delay 0 -seed %d -novis", seed);
            system(buf);
            cout << seed << endl;
        }
    }
}
