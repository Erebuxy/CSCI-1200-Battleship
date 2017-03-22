#ifndef __BOARD_H__
#define __BOARD_H__

#include <map>
#include <set>
#include <vector>

using namespace std;

// This class stores a grid of battleship, which are NULL if the
// grid location does not (yet) contain a tile

class Board {
public:

	// CONSTRUCTOR
	// takes in the dimensions (height & width) of the board
	Board(int xx, int yy, vector<char> &rowsvec, vector<char> &colsvec);

	// ACCESSORS
	int numRows() const {
		return board[0].size() - 2;
	}
	int numCols() const {
		return board.size() - 2;
	}
	bool available(int xx, int yy, int len, bool horz) const;
	bool available_fast(int xx, int yy, int len, bool horz) const;
	bool enough_space() const;
	bool available_space(const int &length, const int &last_length,
			const int &last_x, const int &last_y, vector<int> &rr,
			vector<int> &cc) const;

	// MODIFIERS
	void add(int xx, int yy, int len, bool horz);
	void remove(int xx, int yy, int len, bool horz);
	void set_constraint(pair<pair<char, char>, char> cons);

	// PRINTING
	void print();
	void test_print() const;

private:
	// REPRESENTATION
	vector<vector<char> > board;
	vector<char> rows, cols;
	vector<char> currows, curcols;
	vector<char> avlrows, avlcols;

	map<char, char> PRINT_LIST;
};

#endif
