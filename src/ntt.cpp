#include <bits/stdc++.h>

using namespace std;

typedef long long D;
#define ll long long
D M_ = 1000000007;
D N_ = 0;
// N_ >= M_ && N_ = kn+1

bool prime(ll n) {
	if (n < 2) return false;
	for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0) return false;
	}
	return true;
}

ll pot(ll n, ll k, ll m) {
	if (!k) return 1;
	if (k & 1) return (n * pot(n, k - 1, m)) % m;
	ll u = pot(n, k / 2, m);
	return (u * u) % m;
}

ll inv(ll a, ll n) {
	return pot(a, n - 2, n);
}

ll generator(ll n) {
	vector<ll> fs;
	ll nn = n - 1;
	for (ll i = 2; i * i <= nn; i++) {
		if (nn % i == 0) {
			fs.push_back(i);
			while (nn % i == 0) nn /= i;
		}
	}
	if (nn != 1) fs.push_back(nn);

	for (ll i = 2;; i++) {
		if (pot(i, n - 1, n) == 1) {
			bool g = true;
			for (ll f : fs) {
				if (pot(i, (n - 1) / f, n) == 1) {
					g = false;
					break;
				}
			}
			if (g) {
				return i;
			}
		}
	}
}

vector<D> ntt_calc(vector<D> a, ll w0) {
	int n = a.size();
	if (n == 1) return a;

	vector<D> even(n / 2);
	vector<D> odd(n / 2);
	for (int i = 0; i < n / 2; i++) {
		even[i] = a[2 * i];
		odd[i] = a[2 * i + 1];
	}
	even = ntt_calc(even, (w0 * w0) % N_);
	odd = ntt_calc(odd, (w0 * w0) % N_);
	D w = 1;
	for (int i = 0; i < n / 2; i++) {
		a[i] = even[i] + w * odd[i];
		a[i] %= N_;
		a[i] += N_;
		a[i] %= N_;
		a[i + n / 2] = even[i] - w * odd[i];
		a[i + n / 2] %= N_;
		a[i + n / 2] += N_;
		a[i + n / 2] %= N_;
		w *= w0;
		w %= N_;
	}
	return a;
}

vector<D> ntt(vector<D> a, vector<D> b) {
	int len = 31 - __builtin_clz((int)a.size() + (int)b.size() - 1);
	if ((1<<len) < (int)a.size() + (int)b.size() - 1) len++;
	len = 1<<len;
	vector<D> aa(len, 0);
	vector<D> bb(len, 0);
	for (int i = 0; i < a.size(); i++) {
		aa[i] = a[i];
	}
	for (int i = 0; i < b.size(); i++) {
		bb[i] = b[i];
	}

	ll k = (M_ - 1) / (ll)len + 1;
	while (!prime(k * (ll)len + 1)) k++; 
	N_ = k * len + 1;
	ll g = generator(N_);
	ll w = pot(g, k, N_);

	aa = ntt_calc(aa, w);
	bb = ntt_calc(bb, w);
	vector<D> c(len, 0);

	for (int i = 0; i < len; i++) {
		c[i] = (aa[i] * bb[i]);
		c[i] %= N_;
	}
	c = ntt_calc(c, inv(w, N_));

	for (ll& i : c) {
		i %= N_;
		i *= inv((ll)c.size(), N_);
		i %= N_;
		i += N_;
		i %= N_;
	}
	while (c.back() == 0) c.pop_back();
	return c;
}

int main() {
	vector<ll> a = {1, 2, 3, 4, 5};
	vector<ll> b = {5, 4, 3, 2, 1};
	vector<ll> c = ntt(a, b);
	// expected output: 5 14 26 40 55 40 26 14 5
	for (long long i : c) {
		cout<<i<<" ";
	}
	cout<<endl;
}