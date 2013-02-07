typedef long long i64;

const int MX_PRIME = 32000;
const int MXN = 1000002;
    
class TheDivisionGame {    

public:
    i64 countWinningIntervals(int L, int R) {
        bool P[MX_PRIME] = {};
        int primes[MX_PRIME], pn = 0;
        int mp[32] = {};
        int sg[MXN] = {}, rem[MXN];
        
        for(i64 i = 2; i < MX_PRIME; ++i) {
            if(!P[i]) {
                primes[pn++] = i;
                for(i64 j = i * i; j < MX_PRIME; j += i) {
                    P[j] = true;
                }
            }
        }
        i64 ans = i64(R-L+1) * i64(R-L+2) / 2;
        for(int k = 0; L+k <= R; ++k) {
            rem[k] = L + k;
        }
        for(int i = 0; i < pn; ++i) {
            for(i64 x = primes[i]; x <= R; x *= primes[i]) {
                for(i64 y = ((L-1)/x+1)*x-L; L + y <= R; y += x) {
                    sg[y]++;
                    rem[y] /= primes[i];
                }
            }
        }
        for(int k = 0, mask = 0; L+k <= R; ++k) {
            if(rem[k] > 1) ++sg[k];
            mask ^= sg[k];
            if(mask == 0) --ans;
            ans -= mp[mask]++;
        }
        return ans;
    }
};