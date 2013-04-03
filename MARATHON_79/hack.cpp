#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

char buf[200];

int main() {
    for(int seed = 1; seed <= 1000; ++seed) {
        for(int wk = 1; wk <= 100; ++wk) {
            ofstream fwk("wk.txt");
            fwk << wk;
            fwk.close();
            ofstream of("out.txt", ios::app);
            of << seed << '\t';
            of.close();
            sprintf(buf, "java -jar SnowCleaningVis.jar -exec SnowCleaning -delay 0 -seed %d -novis", seed);
            system(buf);
            cout << seed << endl;
        }
    }
}
