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
#include <atomic>
#include <mutex>
#include <thread>

using namespace std;

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
int row[maxN][maxN], col[maxN][maxN], reg[maxN][3][3];
vii cell;
map<int, int> pos;
int res[maxN][maxN][maxN];
atomic<bool> found;
mutex myMutex;

string s;

void backTrack(int place, int i) {
	{
		//lock_guard<mutex> myLock(myMutex);
		//cerr << "place " << place << " i = " << i << " csize "<< cell.size() << " found " << found<< endl;
	}
	if (i == cell.size()) { 
		found = true;
		lock_guard<mutex> myLock(myMutex);
		cerr << "This is solved " << found << " " << i << endl;
		if (found && i >= cell.size() ) {
			for(int i=0;i<n*n;i++) {
				if (isdigit(s[i]) ) {
					cout << s[i];
				} else {
					int r= i/n;
					int c = i%n;
					cout << pos[res[place][r][c]]+1;
				}
				if (i%n == n-1) {
					cout << endl;
				} else cout << " ";
			}
			cout << endl;
		}
	}
	//if (i > 10) cerr << "i = " << i << endl;
	if (found){
		//lock_guard<mutex> myLock(myMutex);
		//cerr << "Found it!" << endl;
		return;
	} 
	int x = cell[i].ff, y = cell[i].ss;
	//cerr << "i = " << i << " x " << x << " y " << y << endl;
	int avail = row[place][x] & col[place][y] & reg[place][x/3][y/3];
	while (avail) {
		int j = -avail & avail;
		res[place][x][y] = j;
		row[place][x] ^= j; col[place][y] ^= j; reg[place][x/3][y/3] ^= j;
		backTrack(place,i + 1);
		if (found) return;
		res[place][x][y] = 0;
		row[place][x] ^= j; col[place][y] ^= j; reg[place][x/3][y/3] ^= j;
		avail ^= j;
	}
}

void mSolveHelper(int place) {
	backTrack(place,1);
}

void multi_bit_solve(){
	int myRow,myCol;
	if (!found) {
		myRow = cell[0].first;
		myCol = cell[0].second;

		int avail = row[0][myRow] & col[0][myCol] & reg[0][myRow/3][myCol/3];
		//cout << "myRow " << myRow << " myCol " << myCol << " avail = " << avail << endl;
		int place = 0;
		vector<thread> threads;
		while (avail) {
			//cerr << "place = " << place << endl;
			int lsb = -avail & avail;
			res[place][myRow][myCol] = lsb;
			
			//cout << "lsb " << (char)(pos[res[place][myRow][myCol]] + 49) << endl;
			
			if (place > 0) {
				//memcpy(row[place], row[0], sizeof(row));
				//memcpy(col[place], col[0], sizeof(col));
				for(int i=0;i<n;i++) {
					row[place][i] = row[0][i];
					col[place][i] = col[0][i];
				}
				
				for(int i=0;i<3;i++){
					for(int j=0;j<3;j++) {
						reg[place][i][j] = reg[0][i][j];
					}
				}
			}
			//thread t = thread(backTrack,place, 1);
			//threads.emplace_back(move(t));
			avail ^=lsb;
			place++;
		}
		avail = row[0][myRow] & col[0][myCol] & reg[0][myRow/3][myCol/3];
		//cerr << "avail " << avail << endl;
		place = 0;
		//actually take it 
		while (avail) {
			//cerr << "place = " << place << endl;
			int lsb = -avail & avail;
			res[place][myRow][myCol] = lsb;
			
			row[place][myRow] ^= lsb; col[place][myCol] ^= lsb; reg[place][myRow/3][myCol/3] ^= lsb;
			thread t = thread(backTrack,place, 1);
			threads.emplace_back(move(t));
			//thread t = thread(backTrack,place, 1);
			//threads.emplace_back(move(t));
			avail ^=lsb;
			place++;
		}
		for(auto &t:threads) t.join();
		cout.flush();
		if (!found) cout <<"No sol found" << endl;
		cerr << "done" << endl;
	}
}

string newHard = "85...24..72......9..4.........1.7..23.5...9...4...........8..7..17..........36.4.";
string otherHard = "....376.....6...9...8.....4.9......16.......93......4.7.....8...1...9.....254....";

int main() {
	s=otherHard;
	rep(i, 10) pos[BIT(i)] = i;
	int cases, caseNo = 0;
	//for (scanf(" %d ", &cases); cases--; ) {
		rep(i, n) row[0][i] = col[0][i] = 511;
		rep(i, 3) rep(j, 3) reg[0][i][j] = 511;
		fill(res, 0);
		cell.clear();
		rep(i, n*n) {
			int r = i / n;
			int c = i % n;
			if (isdigit(s[i])) {
				int x = BIT(s[i] - '1');

				row[0][r] &= ~x;
				col[0][c] &= ~x;
				
				reg[0][r / 3][c / 3] &= ~x;
			}
			else {
				//cout << "r " << r << " c "<< c <<endl;
				cell.pb(ii(r, c));
			}
		}
		found = false;
		//reverse(all(cell));
		if (cell.size()) {
			multi_bit_solve();
		}
	//}
	return 0;
}
