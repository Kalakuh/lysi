#include <bits/stdc++.h>

using namespace std;

typedef long long D;
#define ll long long
D M_ = 1000000007;
D N_ = 0;
D W_ = 0;
// N_ >= M_ && N_ = kn+1

bool prime(ll n) {
	if (n < 2) return false;
	for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0) return false;
	}
	return true;
}

ll pot(const ll n, const ll k, const ll m) {
	if (!k) return 1;
	if (k & 1) return (n * pot(n, k - 1, m)) % m;
	ll u = pot(n, k / 2, m);
	return (u * u) % m;
}

ll inv(const ll a, const ll n) {
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

void ntt_calc(vector<D> &a, D w0) {
	int n = a.size();
	if (n == 1) return;
	
	vector<D> b(n);
	for (int l = 2; l <= n; l *= 2) {
		int s = n / l;
		D ww = pot(w0, s, N_);
		for (int i = 0; i < s; i++) {
			D w = 1;
			for (int j = 0; j < l / 2; j++) {
				b[i+j*s] = a[i+2*j*s] + w * a[i+2*j*s+s];
				b[i+j*s+n/2] = a[i+2*j*s] - w * a[i+2*j*s+s];
				w *= ww;
				w %= N_;
			}
		}
		swap(a, b);
	}
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

	if (!N_) {
		ll k = (M_ - 1) / (ll)len + 1;
		while (!prime(k * (ll)len + 1)) k++; 
		N_ = k * len + 1;
		ll g = generator(N_);
		W_ = pot(g, k, N_);
	}
	
	ntt_calc(aa, W_);
	ntt_calc(bb, W_);
	vector<D> c(len, 0);

	for (int i = 0; i < len; i++) {
		c[i] = (aa[i] * bb[i]);
		c[i] %= N_;
	}
	ntt_calc(c, inv(W_, N_));

	for (ll& i : c) {
		i *= inv((ll)c.size(), N_);
		i %= N_;
		i += N_;
		i %= N_;
	}
	while (c.back() == 0) c.pop_back();
	return c;
}

int main() {
	int n;
	cin>>n;
	vector<ll> a(n);
	vector<ll> b(n);
	for (int i = 0; i < n; i++) {
		a[i] = b[i] = i;
	}
	vector<ll> c = ntt(a, b);
	// expected output: 5 14 26 40 55 40 26 14 5
	int correct = 0;
	for (ll i = 0; i < n; i++) {
		if (6 * c[i] != (i * i * i - i)) {
			correct++;
		}
	}
	cout<<correct<<endl;
}