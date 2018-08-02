/**
    http://www.spoj.com/problems/ACQUIRE/

    M and C stores the lower envelope

    M is sorted in descending order
    query input x is in ascending order
    query finds Minimum of all the functions
**/


#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair< ll, ll > pll;

vector< ll >M;
vector< ll >C;
ll pointer;

bool useless(ll l1, ll l2, ll l3)
{
    return (C[l3]-C[l1])*(M[l1]-M[l2]) < (C[l2]-C[l1])*(M[l1]-M[l3]);
}

inline ll f(ll id, ll x)
{
    return M[id]*x+C[id];
}

void add(ll m, ll c)
{
    M.push_back(m);
    C.push_back(c);

    while (M.size() >= 3 && useless(M.size()-3, M.size()-2, M.size()-1)) {
        M.erase(M.end()-2);
        C.erase(C.end()-2);
    }
}

ll query(ll x)
{
    if (pointer >= M.size()) pointer = M.size()-1;

    while (pointer < M.size()-1 && f(pointer, x) > f(pointer+1, x)) pointer++;

    return f(pointer, x);
}

ll dp[50003];

int main()
{
    std::ios::sync_with_stdio(false);
    ll n;
    cin >> n;

    vector< pll >v(n);

    for (ll i = 0; i < n; i++) cin >> v[i].first >> v[i].second;

    sort(v.begin(), v.end());

    vector< pll >t;

    for (ll i = 0; i < n; i++) {
        while (t.size() && t[t.size()-1].second <= v[i].second)
            t.pop_back();

        t.push_back(v[i]);
    }

    n = t.size();

    dp[0] = 0;

    for (ll i = 1; i <= n; i++) {
        add(t[i-1].second, dp[i-1]);
        dp[i] = query(t[i-1].first);
        //cout << "dp[" << i << "] = " << dp[i] << endl;
    }

    cout << dp[n] << endl;

    return 0;
}

