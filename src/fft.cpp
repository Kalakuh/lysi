#include <bits/stdc++.h>

using namespace std;

typedef long double C;
typedef int D;
typedef complex<C> P;
#define X real()
#define Y imag()
const double PI = atan((long double)1) * 4;

vector<P> fft_calc(vector<P> a, int dir) {
	int n = a.size();
	if (n == 1) return a;
	vector<P> even(n / 2);
	vector<P> odd(n / 2);
	for (int i = 0; i < n / 2; i++) {
		even[i] = a[2 * i];
		odd[i] = a[2 * i + 1];
	}
	even = fft_calc(even, dir);
	odd = fft_calc(odd, dir);
	P w0 = {cos(2*PI/n), sin(2*PI/n)};
	if (dir == -1) w0 = {cos(2*PI/n), -sin(2*PI/n)};
	P w = 1;
	for (int i = 0; i < n / 2; i++) {
		a[i] = even[i] + w * odd[i];
		a[i + n / 2] = even[i] - w * odd[i];
		w *= w0;
	}
	return a;
}

vector<D> fft(vector<D> a, vector<D> b) {
	int len = 31 - __builtin_clz((int)a.size() + (int)b.size() - 1);
	if ((1<<len) < (int)a.size() + (int)b.size() - 1) len++;
	len = 1<<len;
	vector<P> aa(len, (P)0);
	vector<P> bb(len, (P)0);
	for (int i = 0; i < a.size(); i++) {
		aa[i] = (P)a[i];
	}
	for (int i = 0; i < b.size(); i++) {
		bb[i] = (P)b[i];
	}
	aa = fft_calc(aa, 1);
	bb = fft_calc(bb, 1);
	vector<P> cc(len, (P)0);
	for (int i = 0; i < len; i++) {
		cc[i] = aa[i] * bb[i];
	}
	cc = fft_calc(cc, -1);

	vector<D> c;
	for (P i : cc) {
		c.push_back((D)(round(i.X / (C)len) + 0.1));
	}
	while (c.back() == 0) c.pop_back();
	return c;
}

int main() {
	vector<int> a = {1, 2, 3, 4, 5};
	vector<int> b = {5, 4, 3, 2, 1};
	vector<int> c = fft(a, b);
	
	// expected output: 5 14 26 40 55 40 26 14 5
	for (int i : c) {
		cout<<i<<" ";
	}
	cout<<endl;
}