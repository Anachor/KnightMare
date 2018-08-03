#include<bits/stdc++.h>
#define LL long long
using namespace std;

LL modit(LL x, LL mod) {
	if(x>=mod) x-=mod;
	//if(x<0) x+=mod;
	return x;
}

/// Use __int128 if available
LL mult(LL x, LL y, LL mod) {
	LL s=0, m=x%mod;
	while(y) {
		if(y&1) s = modit(s+m, mod);
		y>>=1;
		m = modit(m+m, mod);
	}
	return s;
}

LL power(LL x, LL p, LL mod){
	LL s=1, m=x;
	while(p) {
		if(p&1) s = mult(s, m, mod);
		p>>=1;
		m = mult(m, m, mod);
	}
	return s;
}

bool witness(LL a, LL n, LL u, int t){
	LL x = power(a,u,n);
	for(int i=0; i<t; i++) {
		LL nx = mult(x, x, n);
		if (nx==1 && x!=1 && x!=n-1) return 1;
		x = nx;
	}
	return x!=1;
}

/// Platform dependent on RAND_MAX
/// Toph: 2^31 - 1
/// CF: 2^15 - 1
/// Not random at all: Fix
LL randll()
{
    if (RAND_MAX == (1<<15) - 1){
        return (rand()*1LL<<48) + (rand()*1LL<<32) + (rand()*1LL<<16) + rand();
    }
    else if (RAND_MAX == (1LL<<32) - 1) {
        return (rand()*1LL<<32) + rand();
    }
}

/** iterate s times of witness on n
    return 1 if prime, 0 otherwise

    n < 4,759,123,141        3 :  2, 7, 61
    n < 1,122,004,669,633    4 :  2, 13, 23, 1662803
    n < 3,474,749,660,383    6 :  2, 3, 5, 7, 11, 13
    n < 2^64                 7 : 2, 325, 9375, 28178, 450775, 9780504, 1795265022

    Make sure testing integer is in range [2, n-2] if
    you want to use magic.
*/

bool miller_rabin(LL n, int s = 50) {
	if (n < 2)    return 0;
	if (n%2 == 0) return n==2;

	LL u = n-1;
	int t = 0;
	while(u%2==0)   u/=2, t++;  // n-1 = u*2^t

	while(s--) {
		LL a = randll()%(n-1) + 1;
		if(witness(a, n, u, t)) return 0;
	}
	return 1;
}


LL f(LL x, LL mod) {
	return modit(mult(x, x, mod) + 1, mod);
}

/// If n is prime, returns n
/// Otherwise returns a proper divisor of n
LL pollard_rho(LL n) {
    if (n==1)               return 1;
	if (miller_rabin(n))    return n;

    while (true) {
        LL x = randll() % (n-1) + 1;
        LL y = 2, res = 1;

        for (int sz=2; res == 1; sz*=2) {
            for (int i=0; i<sz && res<=1; i++) {
                x = f(x, n);
                res = __gcd(abs(x-y), n);
            }
            y = x;
        }
        if (res!=0 && res!=n) return res;
    }
}

const int K = 3e5+7;
vector<int> primes;
bool isp[K];

void sieve()
{
    fill(isp, isp+K, 1);
    isp[0] = isp[1] = 0;

    for (int i=2; i<K; i++)
        if (isp[i]) {
            for (int j=2*i; j<K; j+=i)    isp[j] = 0;
            primes.push_back(i);
        }
}


#define PLL pair<LL, LL>
int main()
{
    sieve();
    LL x;
    int t;
    cin>>t;

    for (int i=0; i<t; i++)
    {
        cin>>x;
        LL tt = x;
        vector<PLL> fac;

        for (auto p: primes)
            if (x%p == 0) {
                int e = 0;
                while (x%p == 0)    x/=p, e++;
                fac.push_back(PLL(p, e));
            }

        if (x > 1) {
            LL p = pollard_rho(x);
            fac.push_back(PLL(p, 1));
            if (p!=x)    fac.push_back(PLL(x/p, 1));
        }
        sort(fac.begin(), fac.end());
        cout<<tt<<" = ";
        for (int i=0; i<fac.size(); i++)
        {
            cout<<fac[i].first;
            if (fac[i].second > 1)  cout<<"^"<<fac[i].second;
            if ( i!= fac.size()-1)  cout<<" * ";
        }
        cout<<"\n";
    }
}
