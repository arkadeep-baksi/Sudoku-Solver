#include <algorithm>
#include <bitset>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

//taken from:https://github.com/lamphanviet/competitive-programming/blob/master/light-online-judge/1397%20-%20Sudoku%20Solver.cpp

#define fr(i,a,b) for (int i = (a), _b = (b); i <= _b; i++)
#define frr(i,a,b) for (int i = (a), _b = (b); i >= _b; i--)
#define rep(i,n) for (int i = 0, _n = (n); i < _n; i++)
#define repr(i,n) for (int i = (n) - 1; i >= 0; i--)
#define foreach(it, ar) for ( typeof(ar.begin()) it = ar.begin(); it != ar.end(); it++ )
#define fill(ar, val) memset(ar, val, sizeof(ar))

#define uint64 unsigned long long
#define int64 long long
#define all(ar) ar.begin(), ar.end()
#define pb push_back
#define mp make_pair
#define ff first
#define ss second

#define BIT(n) (1<<(n))
#define AND(a,b) ((a) & (b))
#define OR(a,b) ((a) | (b))
#define XOR(a,b) ((a) ^ (b))
#define sqr(x) ((x) * (x))

typedef pair<int, int> ii;
typedef pair<int, ii> iii;
typedef vector<ii> vii;
typedef vector<int> vi;

#define PI 3.1415926535897932385
#define INF 1000111222
#define eps 1e-7
#define maxN 10

const int n = 9;
char s[maxN][maxN];
int row[maxN], col[maxN], reg[3][3];
vii cell;
map<int, int> pos;
int res[maxN][maxN];
bool found;

void backTrack(int i) {
	if (i == cell.size()) found = true;
	if (found) return;
	int x = cell[i].ff, y = cell[i].ss;
	int avail = row[x] & col[y] & reg[x/3][y/3];
	while (avail) {
		if (found) return;
		int j = -avail & avail;
		res[x][y] = j;
		row[x] ^= j; col[y] ^= j; reg[x/3][y/3] ^= j;
		backTrack(i + 1);
		row[x] ^= j; col[y] ^= j; reg[x/3][y/3] ^= j;
		avail ^= j;
	}
}

string newHard = "85...24..72......9..4.........1.7..23.5...9...4...........8..7..17..........36.4.";
string otherHard = "....376.....6...9...8.....4.9......16.......93......4.7.....8...1...9.....254....";
int main() {
	string s = otherHard;

	rep(i, 10) pos[BIT(i)] = i;
	int cases, caseNo = 0;
	//for (scanf(" %d ", &cases); cases--; ) {
		rep(i, n) row[i] = col[i] = 511;
		rep(i, 3) rep(j, 3) reg[i][j] = 511;
		fill(res, 0);
		cell.clear();
		rep(i, n*n) {
			int r = i / n;
			int c = i % n;
			if (isdigit(s[i])) {
				int x = BIT(s[i] - '1');
				row[r] &= ~x;
				col[c] &= ~x;
				
				reg[r / 3][c / 3] &= ~x;
			}
			else cell.pb(ii(r, c));
		}
		found = false;
		int r = cell[0].first;
		int c = cell[0].second;

	    int avail = row[r] & col[c] & reg[r/3][c/3];

		cout << "myRow " << cell[0].first << " myCol " << cell[0].second << " avail = " << avail << endl;
		reverse(all(cell));
		if (cell.size()) backTrack(0);
		printf("Case %d:\n", ++caseNo);
		rep(i, n*n) {
			int r = i/n;
			int c = i%n;
			if (isdigit(s[i])) {putchar(s[i]);}
			else putchar(pos[res[r][c]] + 49);

			//else putchar('*');
			if (c==n-1)
				puts("");
			else putchar(' ');
		}
	//}
	return 0;
}