#include <cassert>
#include <cstdio>
#include <iostream>

#include "board.h"

using namespace std;

// ==========================================================================
// CONSTRUCTOR
Board::Board(int xx, int yy, vector<char> &rowsvec, vector<char> &colsvec) {
	/*
	 * Initialize variable for board class
	 */

	board = vector<vector<char> >(xx + 2, vector<char>(yy + 2, '0'));
	rows = rowsvec;
	cols = colsvec;
	currows = vector<char>(yy, '0');
	curcols = vector<char>(xx, '0');
	avlrows = vector<char>(yy, '0' + xx);
	avlcols = vector<char>(xx, '0' + yy);

	PRINT_LIST['l'] = '<';
	PRINT_LIST['r'] = '>';
	PRINT_LIST['c'] = 'X';
	PRINT_LIST['u'] = '^';
	PRINT_LIST['d'] = 'v';
	PRINT_LIST['o'] = 'o';
	PRINT_LIST['<'] = '<';
	PRINT_LIST['>'] = '>';
	PRINT_LIST['^'] = '^';
	PRINT_LIST['V'] = 'v';
	PRINT_LIST['X'] = 'X';
	PRINT_LIST['O'] = 'o';

	for (int i = 0; i < xx + 2; i++) {
		board[i][0] = 0;
		board[i][yy + 1] = 0;
	}
	for (int i = 1; i < yy + 1; i++) {
		board[0][i] = 0;
		board[xx + 1][i] = 0;
	}
}

// ==========================================================================
// ACCESSORS
bool Board::available(int xx, int yy, int len, bool horz) const {
	/*
	 * Check whether it is available to place a ship at certain location
	 */

	xx++;
	yy++;
	if (horz) {
		if (!(board[xx][yy] == '0' || (board[xx][yy] == '<' && len != 1))) {
			return false;
		}
		if (!(board[xx + len - 1][yy] == '0'
				|| (board[xx + len - 1][yy] == '>' && len != 1))) {
			return false;
		}

		int num_cons = 0;
		for (int i = 0; i < len; i++) {
			if (board[xx + i][yy] > '9' && board[xx + i][yy] < 'a') {
				num_cons++;
			} else {
				if (curcols[xx + i - 1] + 1 > cols[xx + i - 1]) {
					return false;
				}
			}
		}
		if (currows[yy - 1] + len - num_cons > rows[yy - 1]) {
			return false;
		}
		for (int i = 1; i <= len - 2; i++) {
			if (!(board[xx + i][yy] == '0' || board[xx + i][yy] == 'X')) {
				return false;
			}
		}
	} else {
		if (!(board[xx][yy] == '0' || (board[xx][yy] == '^' && len != 1))) {
			return false;
		}
		if (!(board[xx][yy + len - 1] == '0'
				|| (board[xx][yy + len - 1] == 'V' && len != 1))) {
			return false;
		}

		int num_cons = 0;
		for (int i = 0; i < len; i++) {

			if (board[xx][yy + i] > '9' && board[xx][yy + i] < 'a') {
				num_cons++;
			} else {
				if (currows[yy + i - 1] + 1 > rows[yy + i - 1]) {
					return false;
				}
			}
		}
		if (curcols[xx - 1] + len - num_cons > cols[xx - 1]) {
			return false;
		}
		for (int i = 1; i <= len - 2; i++) {
			if (!(board[xx][yy + i] == '0' || board[xx][yy + i] == 'X')) {
				return false;
			}
		}
	}
	return true;
}

bool Board::available_fast(int xx, int yy, int len, bool horz) const {
	/*
	 * Check whether it is available to place a ship at certain location
	 */

	xx++;
	yy++;
	if (horz) {
		for (int i = 0; i <= len - 1; i++) {
			if (board[xx + i][yy] != '0') {
				return false;
			}
		}
		for (int i = 0; i < len; i++) {
			if (curcols[xx + i - 1] + 1 > cols[xx + i - 1]) {
				return false;
			}
		}
	} else {

		for (int i = 0; i < len; i++) {
			if (board[xx][yy + i] != '0') {
				return false;
			}
		}
		for (int i = 0; i < len; i++) {
			if (currows[yy + i - 1] + 1 > rows[yy + i - 1]) {
				return false;
			}
		}
	}
	return true;
}

bool Board::enough_space() const {
	for (int i = 0; i < numRows(); i++) {
		if (avlrows[i] - 48 < rows[i] - currows[i])
			return false;
	}
	for (int i = 0; i < numCols(); i++) {
		if (avlcols[i] - 48 < cols[i] - curcols[i])
			return false;
	}
	return true;
}

bool Board::available_space(const int &length, const int &last_length,
		const int &last_x, const int &last_y, vector<int> &rr,
		vector<int> &cc) const {
	if (length != last_length) {
		for (int i = 0; i < numRows(); i++) {
			if (avlrows[i] - 48 < rows[i] - currows[i])
				return false;
			if (avlrows[i] - 48 >= length)
				rr.push_back(i);
		}
		if (!rr.size())
			return false;
		for (int i = 0; i < numCols(); i++) {
			if (avlcols[i] - 48 < cols[i] - curcols[i])
				return false;
			if (avlcols[i] - 48 >= length)
				cc.push_back(i);
		}
		if (!cc.size())
			return false;
	} else {
		for (int i = last_y + 1; i < numRows(); i++) {
			if (avlrows[i] - 48 < rows[i] - currows[i])
				return false;
			if (avlrows[i] - 48 >= length)
				rr.push_back(i);
		}
		for (int i = last_x + 1; i < numCols(); i++) {
			if (avlcols[i] - 48 < cols[i] - curcols[i])
				return false;
			if (avlcols[i] - 48 >= length)
				cc.push_back(i);
		}
	}
	return true;
}

// ==========================================================================
// ACCESSORS
void Board::add(int xx, int yy, int len, bool horz) {
	/*
	 * Add a battleship to the given location
	 */

	xx++;
	yy++;
	int temp_len = 0;
	if (horz) {
		for (int ii = xx - 1; ii <= xx + len; ii++) {
			for (int jj = yy - 1; jj <= yy + 1; jj++) {
				//check boundary
				if (ii == xx - 1 || ii == xx + len || jj == yy - 1
						|| jj == yy + 1) {
					if (board[ii][jj] == '0') {
						avlrows[jj - 1]--;
						avlcols[ii - 1]--;
					}
					board[ii][jj]++;
					//check constrains
				} else if (board[ii][jj] == '0') {
					if (len == 1)
						board[ii][jj] = 'o';
					else if (ii == xx)
						board[ii][jj] = 'l';
					else if (ii == xx + len - 1)
						board[ii][jj] = 'r';
					else
						board[ii][jj] = 'c';
					curcols[ii - 1]++;
					avlrows[jj - 1]--;
					avlcols[ii - 1]--;
					temp_len++;
				}
			}
		}
		currows[yy - 1] += temp_len;
	} else {
		for (int ii = xx - 1; ii <= xx + 1; ii++) {
			for (int jj = yy - 1; jj <= yy + len; jj++) {
				//check boundary
				if (ii == xx - 1 || ii == xx + 1 || jj == yy - 1
						|| jj == yy + len) {
					if (board[ii][jj] == '0') {
						avlrows[jj - 1]--;
						avlcols[ii - 1]--;
					}
					board[ii][jj]++;
					//check constrains
				} else if (board[ii][jj] == '0') {
					if (len == 1)
						board[ii][jj] = 'o';
					else if (jj == yy)
						board[ii][jj] = 'u';
					else if (jj == yy + len - 1)
						board[ii][jj] = 'd';
					else
						board[ii][jj] = 'c';
					currows[jj - 1]++;
					avlrows[jj - 1]--;
					avlcols[ii - 1]--;
					temp_len++;
				}
			}
		}
		curcols[xx - 1] += temp_len;
	}
}

void Board::remove(int xx, int yy, int len, bool horz) {
	/*
	 * Remove a battleship from the given location
	 */

	xx++;
	yy++;
	int temp_len = 0;
	if (horz) {
		for (int ii = xx - 1; ii <= xx + len; ii++) {
			for (int jj = yy - 1; jj <= yy + 1; jj++) {
				//check boundary
				if (ii == xx - 1 || ii == xx + len || jj == yy - 1
						|| jj == yy + 1) {
					board[ii][jj]--;
					if (board[ii][jj] == '0') {
						avlrows[jj - 1]++;
						avlcols[ii - 1]++;
					}
					//check constrains
				} else if (board[ii][jj] > 'a') {
					board[ii][jj] = '0';
					curcols[ii - 1]--;
					avlrows[jj - 1]++;
					avlcols[ii - 1]++;
					temp_len++;
				}
			}
		}
		currows[yy - 1] -= temp_len;
	} else {
		for (int ii = xx - 1; ii <= xx + 1; ii++) {
			for (int jj = yy - 1; jj <= yy + len; jj++) {
				//check boundary
				if (ii == xx - 1 || ii == xx + 1 || jj == yy - 1
						|| jj == yy + len) {
					board[ii][jj]--;
					if (board[ii][jj] == '0') {
						avlrows[jj - 1]++;
						avlcols[ii - 1]++;
					}
					//check constrains
				} else if (board[ii][jj] > 'a') {
					board[ii][jj] = '0';
					currows[jj - 1]--;
					avlrows[jj - 1]++;
					avlcols[ii - 1]++;
					temp_len++;
				}
			}
		}
		curcols[xx - 1] -= temp_len;
	}
}

void Board::set_constraint(pair<pair<char, char>, char> cons) {
	/*
	 * Set specific constraint to the board
	 */

	board[cons.first.first - 47][cons.first.second - 47] = toupper(cons.second);
	currows[cons.first.second - 48]++;
	curcols[cons.first.first - 48]++;
	avlrows[cons.first.second - 48]--;
	avlcols[cons.first.first - 48]--;
}

// ==========================================================================
// PRINTING
void Board::print() {
	/*
	 * Print out the board through ASCII art
	 */

	cout << '+' << string(numCols(), '-') << '+' << endl;
	for (int i = 1; i < numRows() + 1; i++) {
		cout << '|';
		for (int j = 1; j < numCols() + 1; j++) {
			if (board[j][i] < '9')
				cout << ' ';
			else {
				cout << PRINT_LIST[board[j][i]];
			}
		}
		cout << '|' << (currows[i - 1] - 48) % 10 << endl;
	}
	cout << '+' << string(numCols(), '-') << "+\n ";
	for (int i = 1; i < numCols() + 1; i++)
		cout << (curcols[i - 1] - 48) % 10;
	cout << endl << endl;

	fflush(stdout);
}

void Board::test_print() const {
	/*
	 * Print out the board through ASCII art
	 */

	cout << '+' << string(numCols(), '-') << '+' << endl;
	for (int i = 1; i < numRows() + 1; i++) {
		cout << '|';
		for (int j = 1; j < numCols() + 1; j++)
			cout << board[j][i];
		cout << '|' << rows[i - 1] << currows[i - 1] << avlrows[i - 1] << endl;
	}
	cout << '+' << string(numCols(), '-') << "+\n ";
	for (int i = 1; i < numCols() + 1; i++)
		cout << cols[i - 1];
	cout << endl << ' ';
	for (int i = 1; i < numCols() + 1; i++)
		cout << curcols[i - 1];
	cout << endl << ' ';
	for (int i = 1; i < numCols() + 1; i++)
		cout << avlcols[i - 1];
	cout << endl << endl;

	fflush(stdout);
}

// ==========================================================================
