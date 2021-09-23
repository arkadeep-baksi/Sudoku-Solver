#include <iostream>
#include <string>
using namespace std;

const int NROWS = 9;
const int NCOLS = 9;

const int UNASSIGNED = 0;

bool solved;
long long nsols;

// zero will be unassigned

int grid[NROWS][NCOLS];

void solve();
void countSolutions();
void printSolution();
void printGrid();

string hard1  = ".....6....59.....82....8....45........3........6..3.54...325..6..................";
string easy1  = "306508400520000000087000031003010080900863005050090600130000250000000074005206300";
string grid2  = "4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......";
string otherHard = "....376.....6...9...8.....4.9......16.......93......4.7.....8...1...9.....254....";
int main() {
	string s = otherHard;

	while (s.size() !=NROWS*NCOLS) {
		cin >> s;
		cerr << "S = " << s << endl;
	}

	//convert to grid
	int sPlace = 0;
	for(int i=0;i<NROWS;++i) {
		for(int j=0;j<NCOLS;j++) {
			if (s[sPlace] == '.') {
				sPlace++;
				grid[i][j] = 0;
			} else {
				grid[i][j] = s[sPlace++] - '0';
			}
			//cout << grid[i][j] << " ";
		}
		//cout << "\n";
	}
	countSolutions();
	cout << nsols << endl;
	return 0;
}

void printSolution() {
	cout << "Solution: \n";
	printGrid();
}

bool findLocation(int &row,int &col) {
	for(int i=0;i<NROWS;i++) {
		for(int j=0;j<NCOLS;j++) {
			if (grid[i][j] == UNASSIGNED) {
				row = i;
				col = j;
				return true;
			}
		}
	}
	return false;
}

//assumes 1- ROWS as index
bool colValid(int col, int num) {
	//only one in column
	for(int row=0;row<NROWS;row++) {
		if (grid[row][col] == num) return false;
	}
	return true;
}

bool rowValid(int row, int num) {
	for(int col=0;col<NCOLS;col++) {
		if (grid[row][col] == num) return false;
	}
	return true;
}

//assums 1-9
bool squareValid(int row,int col, int num) {
	//where does it belong?
	//only one
	int rowStart = (row/3) * 3; // either 0 3 6
	int colStart = (col/3) * 3; // eigher 0 3 6
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			int value = grid[rowStart+i][colStart+j];
			if (value == num) return false;
		}
	}
	return true;
}

bool isValid(int row, int col, int num) {
	if (!colValid(col,num) ) {
		//cerr << "  not colValid" << endl;
		return false;
	}
	if (!rowValid(row,num)) {
		//cerr << "  not rowValid" << endl;
		return false;
	}
	if (!squareValid(row,col,num)) {
		//cerr << "  not squareValid" << endl;
		return false;
	}
	return true;
	return 
		colValid(col,num) && 
		rowValid(row,num) && 
		squareValid(row,col,num);
}

void printGrid() {
	for(int i=0;i<NROWS;i++) {
		for(int j=0;j<NCOLS;j++) {
			cout << grid[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}


void countSolutions() {
	//if not solved
	int row, col;
	//find acceptable location
	solved = !findLocation(row,col);
	//cerr << "solved " << solved << endl;
	if (solved) {
		printSolution();
		nsols++;
		if (nsols%100==0) cout << nsols << "\n";
		solved = false;
		return;
	}
	//cerr << row << " " << col << " unoccupied"<< endl;
	//find new location (min - max)
	for(int num = 1; num <= 9; num++) {
		//cerr << "trying num " << num << endl;
		if (isValid(row,col,num)) {
			grid[row][col] = num;
			//cerr <<"    in " << row << " " << col << " " << num << endl;
			countSolutions();
			//if (solved)return;
			//printGrid();
			grid[row][col] = UNASSIGNED;
		}
	}
}

void solve() {
	//if not solved
	int row, col;
	if (!solved) {
		//find acceptable location
		solved = !findLocation(row,col);
		//cerr << "solved " << solved << endl;
		if (solved) {
			printSolution();
			return;
		}
	}else return;
	//cerr << row << " " << col << " unoccupied"<< endl;
	//find new location (min - max)
	for(int num = 1; num <= 9; num++) {
		//cerr << "trying num " << num << endl;
		if (isValid(row,col,num)) {
			grid[row][col] = num;
			//cerr <<"    in " << row << " " << col << " " << num << endl;
			solve();
			if (solved)return;
			//printGrid();
			grid[row][col] = UNASSIGNED;
		}
	}
}