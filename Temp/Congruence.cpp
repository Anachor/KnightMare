// This is a collection of useful code for solving problems that
// involve modular linear equations.  Note that all of the
// algorithms described here work on nonnegative integers.

#include <iostream>
#include <vector>
#include <algorithm>
#define LL long long
using namespace std;

typedef vector<LL> VL;
typedef pair<LL, LL> PLL;

// return a % b (positive value)
// Range: LL
LL mod(LL a, LL b) {
	return ((a%b) + b) % b;
}

// computes gcd(a,b)
// Range: LL
LL gcd(LL a, LL b) {
	while (b) { LL t = a%b; a = b; b = t; }
	return a;
}

// computes lcm(a,b)
// Range: int
LL lcm(LL a, LL b) {
	return (a/gcd(a, b))*b;
}

// (a^b) mod m via successive squaring
// Range: int
LL powermod(LL a, LL b, LL m)
{
	LL ret = 1;
	while (b) {
		if (b & 1) ret = mod(ret*a, m);
		a = mod(a*a, m);
		b >>= 1;
	}
	return ret;
}

// returns g = gcd(a, b); finds x, y such that d = ax + by
// Range: int (tested on CF 982E :( )
LL extended_euclid(LL a, LL b, LL &x, LL &y) {
	LL xx = y = 0;
	LL yy = x = 1;
	while (b) {
		LL q = a / b;
		LL t = b; b = a%b; a = t;
		t = xx; xx = x - q*xx; x = t;
		t = yy; yy = y - q*yy; y = t;
	}
	return a;
}

// finds all solutions to ax = b (mod n)
// Range: int (not tested)
VL modular_linear_equation_solver(LL a, LL b, LL n) {
	LL x, y;
	VL ret;
	LL g = extended_euclid(a, n, x, y);
	if (!(b%g)) {
		x = mod(x*(b / g), n);
		for (LL i = 0; i < g; i++)
			ret.push_back(mod(x + i*(n / g), n));
	}
	return ret;
}

// computes b such that ab = 1 (mod n), returns -1 on failure
// Range: int
LL mod_inverse(LL a, LL n) {
	LL x, y;
	LL g = extended_euclid(a, n, x, y);
	if (g > 1) return -1;
	return mod(x, n);
}

// Chinese remainder theorem (special case): find z such that
// z % m1 = r1, z % m2 = r2.  Here, z is unique modulo M = lcm(m1, m2).
// Return (z, M).  On failure, M = -1.
// Range: int (tested on CF 982E :( )
PLL chinese_remainder_theorem(LL m1, LL r1, LL m2, LL r2) {
	LL s, t;
	LL g = extended_euclid(m1, m2, s, t);
	if (r1%g != r2%g) return make_pair(0, -1);
	LL M = m1*m2;
	LL ss = ((s*r2)%m2)*m1;
	LL tt = ((t*r1)%m1)*m2;
	return make_pair(mod(ss+tt, M) / g, M / g);
}


// Chinese remainder theorem: find z such that
// z % m[i] = r[i] for all i.  Note that the solution is
// unique modulo M = lcm_i (m[i]).  Return (z, M). On
// failure, M = -1. Note that we do not require the a[i]'s
// to be relatively prime.
// Range: int
PLL chinese_remainder_theorem(const VL &m, const VL &r) {
	PLL ret = make_pair(r[0], m[0]);
	for (LL i = 1; i < m.size(); i++) {
		ret = chinese_remainder_theorem(ret.second, ret.first, m[i], r[i]);
		if (ret.second == -1) break;
	}
	return ret;
}

// computes x and y such that ax + by = c
// returns whether the solution exists
// Range: int
bool linear_diophantine(LL a, LL b, LL c, LL &x, LL &y) {
	if (!a && !b)
	{
		if (c) return false;
		x = 0; y = 0;
		return true;
	}
	if (!a)
	{
		if (c % b) return false;
		x = 0; y = c / b;
		return true;
	}
	if (!b)
	{
		if (c % a) return false;
		x = c / a; y = 0;
		return true;
	}
	LL g = gcd(a, b);
	if (c % g) return false;
	x = c / g * mod_inverse(a / g, b / g);
	y = (c - a*x) / b;
	return true;
}

int main() {
	// expected: 2
	cout << gcd(14, 30) << endl;

	// expected: 2 -2 1
	LL x, y;
	LL g = extended_euclid(14, 30, x, y);
	cout << g << " " << x << " " << y << endl;

	// expected: 95 45
	VL sols = modular_linear_equation_solver(14, 30, 100);
	for (LL i = 0; i < sols.size(); i++) cout << sols[i] << " ";
	cout << endl;

	// expected: 8
	cout << mod_inverse(8, 9) << endl;

	// expected: 23 105
	//           11 12
	PLL ret = chinese_remainder_theorem(VL({ 3, 5, 7 }), VL({ 2, 3, 2 }));
	cout << ret.first << " " << ret.second << endl;
	ret = chinese_remainder_theorem(VL({ 4, 6 }), VL({ 3, 5 }));
	cout << ret.first << " " << ret.second << endl;

	// expected: 5 -15
	if (!linear_diophantine(7, 2, 5, x, y)) cout << "ERROR" << endl;
	cout << x << " " << y << endl;
	return 0;
}
