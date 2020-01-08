#include <bits/stdc++.h>

#define ll long long
using namespace std;


ll pot(const ll n, const ll k, const ll m) {
	if (!k) return 1;
	if (k & 1) return (n * pot(n, k - 1, m)) % m;
	ll u = pot(n, k / 2, m);
	return (u * u) % m;
}

ll inv(const ll a, const ll n) {
	return pot(a, n - 2, n);
}

bool prime(ll n) {
	if (n <= 2) return n == 2;
	if (pot(2, n - 1, n) != 1) return false;
	ll d = n - 1;
	ll r = 0;
	while (!(d & 1)) {
		d /= 2;
		r++;
	}
	const vector<ll> A_ = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	for (ll a : A_) {
		if (n == a) return true;
		ll x = pot(a, d, n);
		if (x == 1 || x == n - 1) continue;
		bool b = false;
		for (int i = 1; i < r; i++) {
			x = (x * x) % n;
			if (x == n - 1) {
				b = true;
				break;
			}
		}
		if (b) continue;
		return false;
	}
	return true;
}

int main() {
	cout<<prime(2)<<endl;
	cout<<prime(3)<<endl;
	cout<<prime(1000000007)<<endl;
	cout<<prime(1000000009)<<endl;
	cout<<prime(1000000011)<<endl;
}