#include<bits/stdc++.h>
using namespace std;

/// c vector< vector< int > > contains information about
/// rank. i th element in the k th vector contains information
/// about the substring of length 2^k at index i

vector< vector< int > >c;
vector<int>sort_cyclic_shifts(string const& s)
{
    int n = s.size();
    const int alphabet = 256;

    vector<int> p(n), cnt(alphabet, 0);

    c.clear();
    c.emplace_back();
    c[0].resize(n);

    for (int i = 0; i < n; i++)
        cnt[s[i]]++;
    for (int i = 1; i < alphabet; i++)
        cnt[i] += cnt[i-1];
    for (int i = 0; i < n; i++)
        p[--cnt[s[i]]] = i;
    c[0][p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
        if (s[p[i]] != s[p[i-1]])
            classes++;
        c[0][p[i]] = classes - 1;
    }

    vector<int> pn(n), cn(n);
    cnt.resize(n);

    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; i++) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0)
                pn[i] += n;
        }
        fill(cnt.begin(), cnt.end(), 0);

        /// radix sort
        for (int i = 0; i < n; i++)
            cnt[c[h][pn[i]]]++;
        for (int i = 1; i < classes; i++)
            cnt[i] += cnt[i-1];
        for (int i = n-1; i >= 0; i--)
            p[--cnt[c[h][pn[i]]]] = pn[i];

        cn[p[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; i++) {
            pair<int, int> cur = {c[h][p[i]], c[h][(p[i] + (1 << h)) % n]};
            pair<int, int> prev = {c[h][p[i-1]], c[h][(p[i-1] + (1 << h)) % n]};
            if (cur != prev)
                ++classes;
            cn[p[i]] = classes - 1;
        }
        c.push_back(cn);
    }

    return p;
}

vector<int> suffix_array_construction(string s)
{
    s += "!";
    vector<int> sorted_shifts = sort_cyclic_shifts(s);
    sorted_shifts.erase(sorted_shifts.begin());
    return sorted_shifts;
}

/// compare two suffixes starting at i and j with length l
/// k = floor(log2(l))
/// n = string length

int compare(int i, int j, int n, int k)
{
    pair<int, int> a = {c[k][i], c[k][(i+1-(1 << k))%n]};
    pair<int, int> b = {c[k][j], c[k][(j+1-(1 << k))%n]};
    return a == b ? 0 : a < b ? -1 : 1;
}

int lcp(int i, int j)
{
    int log_n = c.size()-1;

    int ans = 0;
    for (int k = log_n; k >= 0; k--) {
        if (c[k][i] == c[k][j]) {
            ans += 1 << k;
            i += 1 << k;
            j += 1 << k;
        }
    }
    return ans;
}

/*
    We will compute the longest common prefix for each pair of
    adjacent suffixes in the sorted order. In other words we
    construct an array lcp[0…n−2], where lcp[i] is equal to
    the length of the longest common prefix of the suffixes
    starting at p[i] and p[i+1]. This array will give us an
    answer for any two adjacent suffixes of the string. Then
    the answer for arbitrary two suffixes, not necessarily
    neighboring ones, can be obtained from this array. In fact,
    let the request be to compute the LCP of the suffixes p[i]
    and p[j]. Then the answer to this query will be
    min{lcp[p[i]], lcp[p[i+1]], …, lcp[p[j−1]]}
*/

vector<int> lcp_construction(string const& s, vector<int> const& p)
{
    int n = s.size();
    vector<int> rank(n, 0);
    for (int i = 0; i < n; i++)
        rank[p[i]] = i;

    int k = 0;
    vector<int> lcp(n-1, 0);
    for (int i = 0; i < n; i++) {
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }
        int j = p[rank[i] + 1];
        while (i + k < n && j + k < n && s[i+k] == s[j+k])
            k++;
        lcp[rank[i]] = k;
        if (k)
            k--;
    }
    return lcp;
}

int main()
{
    string s;
    cin >> s;

    vector< int >p = suffix_array_construction(s);
    for (int x : p) cout << x << "\n";

    vector< int >lcp = lcp_construction(s, p);
    int distinctSubstring = (s.size()*(s.size()+1))/2;
    for (int x : lcp) distinctSubstring -= x;
    cout << distinctSubstring << endl;

    return 0;
}
