#include <bits/stdc++.h>

using namespace std;

typedef __int128 D;
typedef D ll;

ll pot(const ll n, const ll k, const ll m) {
	if (!k) return 1;
	if (k & 1) return (n * pot(n, k - 1, m)) % m;
	ll u = pot(n, k / 2, m);
	return (u * u) % m;
}

ll inv(const ll a, const ll n) {
	return pot(a % n, n - 2, n);
}

ll crt(vector<ll> a, vector<ll> m, ll p) {
	int n = a.size();
	ll x = 0;
	ll X = 1;
	for (ll k : m) {
		X *= k;
	}
	for (int i = 0; i < n; i++) {
		ll t = (a[i] * (X / m[i])) % X;
		t *= inv(X / m[i], m[i]);
		x += t;
		x %= X;
	}
	x %= p;
	return x;
}

void ntt_calc(vector<D> &a, D w0, D p) {
	int n = a.size();
	if (n == 1) return;
	
	vector<D> b(n);
	for (int l = 2; l <= n; l *= 2) {
		int s = n / l;
		D ww = pot(w0, s, p);
		D w = 1;
		for (int j = 0; j < l / 2; j++) {
			for (int i = 0; i < s; i++) {
				b[i+j*s] = a[i+2*j*s] + w * a[i+2*j*s+s];
				b[i+j*s+n/2] = a[i+2*j*s] - w * a[i+2*j*s+s];
			}
			w *= ww;
			w %= p;
		}
		for (int i = 0; i < n; i++) b[i] %= p;
		swap(a, b);
	}
}

const vector<D> ms = {1004535809, 1092616193, 1161822209};
vector<D> ntt(vector<D> a, vector<D> b, D p) {
	int ol = (int)a.size() + (int)b.size() - 1;
	int len = 31 - __builtin_clz((int)a.size() + (int)b.size() - 1);
	if ((1<<len) < (int)a.size() + (int)b.size() - 1) len++;
	len = 1<<len;
	vector<D> aa(len, 0);
	vector<D> bb(len, 0);
	vector<D> c[3];
	c[0] = c[1] = c[2] = aa;
	for (int mi = 0; mi < ms.size(); mi++) {
		D md = ms[mi];
		for (int i = 0; i < len; i++) aa[i] = bb[i] = 0;
		for (int i = 0; i < a.size(); i++) {
			aa[i] = a[i];
		}
		for (int i = 0; i < b.size(); i++) {
			bb[i] = b[i];
		}

		D w0 = pot(3, (md - 1) / (D)len, md);
		
		ntt_calc(aa, w0, md);
		ntt_calc(bb, w0, md);

		for (int i = 0; i < len; i++) {
			c[mi][i] = (aa[i] * bb[i]);
			c[mi][i] %= md;
		}
		ntt_calc(c[mi], inv(w0, md), md);

		for (ll& i : c[mi]) {
			i *= inv((ll)c[mi].size(), md);
			i %= md;
			i += md;
			i %= md;
		}
	}
	vector<D> d(ol, 0);
	for (int i = 0; i < ol; i++) {
		d[i] = crt({c[0][i], c[1][i], c[2][i]}, ms, p);
	}
	return d;
}

int main() {
	vector<ll> a = {5, 4, 3, 2, 1};
	vector<ll> b = {1, 2, 3, 4, 5};
	vector<ll> c = ntt(a, b, 13313);
	// expected output: 5 14 26 40 55 40 26 14 5
	for (auto i : c) cout<<(long long)i<<" ";
	cout<<endl;
}