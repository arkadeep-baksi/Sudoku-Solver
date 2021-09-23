#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cstring>
#include <thread>
#include <mutex>
#include <atomic>
using namespace std;

const int NROWS = 9;
const int NCOLS = 9;

const int UNASSIGNED = 0;

atomic<bool> solved;

// zero will be unassigned

int grid[NROWS][NCOLS];

void mPrintSolution(const int grid[NROWS][NCOLS]);
void mPrintGrid(const int grid[NROWS][NCOLS]);

int unassignedCellsPlace;
vector<pair<int,int>>unassignedCells;

string hard1 = ".....6....59.....82....8....45........3........6..3.54...325..6..................";
string easy1 = "306508400520000000087000031003010080900863005050090600130000250000000074005206300";
string grid2 = "4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......";
string grid3 = "........................................................................987654321";
string newHard = "85...24..72......9..4.........1.7..23.5...9...4...........8..7..17..........36.4.";
string otherHard = "....376.....6...9...8.....4.9......16.......93......4.7.....8...1...9.....254....";

mutex myMutex;
mutex solveMutex;

void mPrintSolution(const int grid[NROWS][NCOLS]) {
	{
		//lock_guard<mutex> myLock(myMutex);
		cout << "Solution: " << endl;
		mPrintGrid(grid);
	}
}

//assumes 1- ROWS as index
bool mColValid(int col, int num, const int grid[NROWS][NCOLS]) {
	//only one in column
	for(int row=0;row<NROWS;row++) {
		if (grid[row][col] == num) return false;
	}
	return true;
}

bool mRowValid(int row, int num, const int grid[NROWS][NCOLS]) {
	for(int col=0;col<NCOLS;col++) {
		if (grid[row][col] == num) return false;
	}
	return true;
}

//assums 1-9
bool mSquareValid(int row,int col, int num, const int grid[NROWS][NCOLS]) {
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

bool mIsValid(int row, int col, int num, const int grid[NROWS][NCOLS]) {
	if (!mColValid(col,num,grid) ) {
		//cerr << "  not colValid" << endl;
		return false;
	}
	if (!mRowValid(row,num,grid)) {
		//cerr << "  not rowValid" << endl;
		return false;
	}
	if (!mSquareValid(row,col,num,grid)) {
		//cerr << "  not squareValid" << endl;
		return false;
	}
	return true;
}

void mPrintGrid(const int grid[NROWS][NCOLS]) {
	{
		//lock_guard<mutex> myLock(myMutex);
		for(int i=0;i<NROWS;i++) {
			for(int j=0;j<NCOLS;j++) {
				cout << grid[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void mFindLocation(int &row,int &col,const vector<pair<int,int>> &mUnassignedCells,const int mUnassignedCellsPlace) {
	if (mUnassignedCellsPlace < mUnassignedCells.size()) {
		row = mUnassignedCells[mUnassignedCellsPlace].first;
		col = mUnassignedCells[mUnassignedCellsPlace].second;
	} else solved = true;
}

void trueSolve(int mGrid[NROWS][NCOLS], int mUnassignedCellsPlace) {
	//if not solved

	int row, col;
	if (!solved) {
		//find acceptable location
		mFindLocation(row,col,unassignedCells, mUnassignedCellsPlace);
		/*{
			lock_guard<mutex> myLock(myMutex);
			cerr << this_thread::get_id() << " solved " <<solved <<" up = " << mUnassignedCellsPlace<< endl;
			cerr << row << " " << col << " unoccupied"<< endl;
			mPrintGrid(mGrid);
		}*/
		if (solved) {
			{
				lock_guard<mutex> myLock(myMutex);
				cerr << "This is solved " << solved << " " << mUnassignedCellsPlace << endl;
				if (solved && mUnassignedCellsPlace >= unassignedCells.size() ) {
					//race cond: sae solution at same time?
					mPrintSolution(mGrid);
					//return;
				}
			}
			return;
		}
	}else return;
	
	//find new location (min - max)
	for(int num = 1; num <= 9; num++) {
		//cerr << "trying num " << num << endl;
		if (mIsValid(row,col,num,mGrid)) {
			mGrid[row][col] = num;
			//cerr <<"    in " << row << " " << col << " " << num << endl;
			trueSolve(mGrid, mUnassignedCellsPlace+1);
			if (solved)return;
			//mPrintGrid(mGrid);
			mGrid[row][col] = UNASSIGNED;
		}
	}
}

void mSolveHelper(int mGrid[][NCOLS]) {
	//initialize local shit 
	trueSolve(mGrid,1);
}
////////////////////////////////////SINGLE THREADED //////////////////
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

////////////////////////END SINGLE THREADED/////////////////////////////////////////

//if not solved
int mGrid[NROWS][NROWS][NCOLS];
void solveIt() {	
	int row, col;
	if (!solved) {
		//find acceptable location
		row = unassignedCells[0].first;
		col = unassignedCells[0].second;

		int place = 0;
		vector<thread> threads;
		for(int i=1;i<=9;++i) {
			if (isValid(row,col,i)) {
				grid[row][col] = i;
				//int ** mGrid= new int*[NROWS];
				//for(int i=0;i<NROWS;++i) mGrid[i] = new int[NCOLS];
				//int mGrid[NROWS][NCOLS];
				memcpy(mGrid[place],grid,sizeof(grid));
				/*
				{
					lock_guard<mutex> myLock(myMutex);
					cerr << i << " at " << row << " " << col << endl;
				}
				*/
				//mPrintGrid(mGrid[place]);
				//break;
				thread t = thread(mSolveHelper,mGrid[place]);
				threads.emplace_back(move(t));
				//grid[row][col] = UNASSIGNED;
				place++;
			}
		}
		/*
		{
			lock_guard<mutex> myLock(myMutex);
			cerr << "nthreads "<< threads.size() << endl;
		}
		*/
		for(auto &t:threads) t.join();
		cout.flush();
		cerr << "done" << endl;
	}
}
void multithreadedSolve() {
	//for each start place create thread
	solveIt();
}

int main() {
	string s = otherHard;
	//while (cin >> s) {
		unassignedCells.clear();
		unassignedCellsPlace = 0;
		solved = false;
	
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
				if (grid[i][j]==UNASSIGNED) {
					unassignedCells.push_back(make_pair(i,j));
				}
				//cout << grid[i][j] << " ";
			}
			//cout << "\n";
		}
		multithreadedSolve();
		cout << "All done after multi" << endl;
	//}
	return 0;
}
