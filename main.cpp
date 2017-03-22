/*
 * Reference for the name of the battleship appeared in this homework
 * S	-	submarine		1
 * D	-	destroyer		2
 * C	-	cruiser			3
 * B	-	battleship	4
 * R	-	carrier			5
 * G	-	cargo				6
 * T	-	tanker			7
 */

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "board.h"

using namespace std;

void add_ship(unsigned int &ind, int &x, int &y, Board &b, vector<char> &ships,
		vector<string> &answer, int &solutions, bool all_solutions, int last_x,
		int last_y, int last_length);
void add_ship_fast(unsigned int &ind, int &x, int &y, Board &b,
		vector<char> &ships, vector<string> &answer, int &solutions,
		bool all_solutions, int last_x, int last_y, int last_length);
void add_unknown_ship(unsigned int &ind, int &x, int &y, Board &b,
		vector<char> &ships, vector<string> &answer, int &solutions,
		bool all_solutions, unsigned int unknown, int last_x, int last_y,
		int last_length);
string hashboard(const vector<string> &answer);
void print_answer(const vector<string> &answer);

// this global variable is to stored the name and size of the battleships
map<string, char> SHIP_LIST;
map<char, string> KEY_LIST;

void InitialGlobal() {
	SHIP_LIST["submarine"] = '1';
	SHIP_LIST["destroyer"] = '2';
	SHIP_LIST["cruiser"] = '3';
	SHIP_LIST["battleship"] = '4';
	SHIP_LIST["carrier"] = '5';
	SHIP_LIST["cargo"] = '6';
	SHIP_LIST["tanker"] = '7';
	SHIP_LIST["unknown"] = '?';
	KEY_LIST['1'] = "submarine";
	KEY_LIST['2'] = "destroyer";
	KEY_LIST['3'] = "cruiser";
	KEY_LIST['4'] = "battleship";
	KEY_LIST['5'] = "carrier";
	KEY_LIST['6'] = "cargo";
	KEY_LIST['7'] = "tanker";
}

string itos(const int &i) {
	std::stringstream stream;
	stream << char(i + '0');
	return stream.str();
}

int stoi(const string& s) {
	stringstream ss;
	ss << s;
	int i;
	ss >> i;
	return i;
}

// ==========================================================================
// Helper function that is called when an error in the command line
// arguments is detected.
void usage(int argc, char *argv[]) {
	cerr << "USAGE: " << endl;
	cerr << "  " << argv[0] << " <filename> " << endl;
	cerr << "  " << argv[0] << " <filename>  find_all_solutions" << endl;
	exit(1);
}

// ==========================================================================
void CommandLineHandler(int argc, char *argv[], string &filename,
		bool &all_solutions) {
	// must at least put the filename on the command line
	if (argc < 2) {
		usage(argc, argv);
	}
	filename = argv[1];

	// parse the optional arguments
	for (int i = 2; i < argc; i++) {
		if (argv[i] == string("find_all_solutions")) {
			all_solutions = true;
		} else {
			cerr << "ERROR: unknown argument '" << argv[i] << "'" << endl;
			usage(argc, argv);
		}
	}
}

// ==========================================================================
void ParseInputFile(int argc, char *argv[], const string &filename, int &x,
		int &y, vector<char> &rows, vector<char> &cols, vector<char> &ships,
		vector<pair<pair<char, char>, char> > &constraints, unsigned int &unknown,
		bool &constraint) {

	// Open the file
	ifstream istr(filename.c_str());
	if (!istr) {
		cerr << "ERROR: cannot open file '" << filename << "'" << endl;
		usage(argc, argv);
	}
	assert(istr);

	// Read each line of the file
	string token, number, xx, yy;
	while (istr >> token) {
		if (token == "board") {
			istr >> yy >> xx;
			y = atoi(yy.c_str());
			x = atoi(xx.c_str());
		} else if (token == "rows") {
			for (int i = 0; i < y; i++) {
				istr >> number;
				rows.push_back(stoi(number) + '0');
			}
		} else if (token == "cols") {
			for (int i = 0; i < x; i++) {
				istr >> number;
				cols.push_back(stoi(number) + '0');
			}
		} else if (token == "constraint") {
			istr >> yy >> xx >> number;
			constraints.push_back(
					make_pair(make_pair(xx[0], yy[0]), number[0]));
			constraint = true;
		} else if (SHIP_LIST.count(token)) {
			if (token == "unknown")
				unknown++;
			else
				ships.push_back(SHIP_LIST[token]);

		}
	}
}

int main(int argc, char *argv[]) {
	InitialGlobal();
	string filename;
	bool all_solutions = false;
	CommandLineHandler(argc, argv, filename, all_solutions);

	// Parse the input file
	int x, y;
	unsigned int ind = 0;
	unsigned int unknown = 0;
	bool constraint = false;
	vector<char> ships, rows, cols;
	vector<pair<pair<char, char>, char> > constraints;
	ParseInputFile(argc, argv, filename, x, y, rows, cols, ships, constraints,
			unknown, constraint);

	// Build board and set constraint
	Board b(x, y, rows, cols);
	vector<string> answer(ships.size(), "");
	int solutions = 0;

	// Set the constraints to the board
	if (constraint) {
		for (unsigned int i = 0; i < constraints.size(); i++)
			b.set_constraint(constraints[i]);
	}

	// Main recursion
	if (unknown) {
		add_unknown_ship(ind, x, y, b, ships, answer, solutions, all_solutions,
				unknown, 0, 0, -1);
	}
	else if (constraint) {
		// Change the order of ships list
		sort(ships.rbegin(), ships.rend());
		add_ship(ind, x, y, b, ships, answer, solutions, all_solutions, 0, 0, -1);
	}
	else {
		// Change the order of ships list
		sort(ships.rbegin(), ships.rend());
		add_ship(ind, x, y, b, ships, answer, solutions, all_solutions, 0, 0, -1);
	}

	// Print out the conclusion of the answer
	if (!solutions)
		cout << "No solutions\n";
	else if (all_solutions)
		cout << "Found " << solutions << " solution(s)\n";

	return 0;
}

void add_ship(unsigned int &ind, int &x, int &y, Board &b, vector<char> &ships,
		vector<string> &answer, int &solutions, bool all_solutions, int last_x,
		int last_y, int last_length) {
	/*
	 * Add ships to the board
	 */

	// determine recursion if get needed information
	if (!all_solutions && solutions != 0)
		return;
	// check whether have placed all the ships
	else if (ind == ships.size()) {
		// check whether have the same board before
		vector<string> tempans(answer);
		sort(tempans.begin(), tempans.end());
		solutions++;
		//cout << "Solution:\n";
		//print_answer(tempans);
		b.print();
	} else {
		if (b.enough_space()) {
			int length = ships[ind] - 48;
			int xx = 0;
			if (length == last_length) {
				xx = last_x + 1;
				int i = last_x;
				if (i < x - length + 1) {
					for (int j = last_y + 2; j < y; j++) {
						if (!all_solutions && solutions != 0)
							return;
						if (b.available(i, j, length, true)) {
							b.add(i, j, length, true);
							answer[ind] = itos(j) + itos(i) + ships[ind] + '1';
							ind++;
							add_ship(ind, x, y, b, ships, answer, solutions,
									all_solutions, i, j, length);
							ind--;
							b.remove(i, j, length, true);
						}
					}
				}
				if (length > 1) {
					for (int j = last_y + 2; j < y - length + 1; j++) {
						if (!all_solutions && solutions != 0)
							return;
						if (b.available(i, j, length, false)) {
							b.add(i, j, length, false);
							answer[ind] = itos(j) + itos(i) + ships[ind] + '0';
							ind++;
							add_ship(ind, x, y, b, ships, answer, solutions,
									all_solutions, i, j, length);
							ind--;
							b.remove(i, j, length, false);
						}
					}
				}
			}
			for (int i = xx; i < x - length + 1; i++) {
				for (int j = 0; j < y; j++) {
					if (!all_solutions && solutions != 0)
						return;
					if (b.available(i, j, length, true)) {
						b.add(i, j, length, true);
						answer[ind] = itos(j) + itos(i) + ships[ind] + '1';
						ind++;
						add_ship(ind, x, y, b, ships, answer, solutions,
								all_solutions, i, j, length);
						ind--;
						b.remove(i, j, length, true);
					}
				}
			}
			if (length > 1) {
				for (int i = xx; i < x; i++) {
					for (int j = 0; j < y - length + 1; j++) {
						if (!all_solutions && solutions != 0)
							return;
						if (b.available(i, j, length, false)) {
							b.add(i, j, length, false);
							answer[ind] = itos(j) + itos(i) + ships[ind] + '0';
							ind++;
							add_ship(ind, x, y, b, ships, answer, solutions,
									all_solutions, i, j, length);
							ind--;
							b.remove(i, j, length, false);
						}
					}
				}
			}
		}
	}
}

void add_ship_fast(unsigned int &ind, int &x, int &y, Board &b,
		vector<char> &ships, vector<string> &answer, int &solutions,
		bool all_solutions, int last_x, int last_y, int last_length) {
	/*
	 * Add ships to the board
	 */

	// determine recursion if get needed information
	if (!all_solutions && solutions != 0)
		return;
	// check whether have placed all the ships
	else if (ind == ships.size()) {
		// check whether have the same board before
		vector<string> tempans(answer);
		sort(tempans.begin(), tempans.end());
		solutions++;
		//cout << "Solution:\n";
		//print_answer(tempans);
		b.print();
	} else {
		vector<int> avlrows;
		vector<int> avlcols;
		int length = ships[ind] - 48;
		if (b.available_space(length, last_length, last_x, last_y, avlrows,
				avlcols)) {
			int xx = 0;
			if (length == last_length) {
				xx = last_x + 1;
				int i = last_x;
				if (i < x - length + 1) {
					for (int j = last_y + 2; j < y; j++) {
						if (!all_solutions && solutions != 0)
							return;
						if (b.available_fast(i, j, length, true)) {
							b.add(i, j, length, true);
							answer[ind] = itos(j) + itos(i) + ships[ind] + '1';
							ind++;
							add_ship(ind, x, y, b, ships, answer, solutions,
									all_solutions, i, j, length);
							ind--;
							b.remove(i, j, length, true);
						}
					}
				}
				if (length > 1) {
					for (int j = last_y + 2; j < y - length + 1; j++) {
						if (!all_solutions && solutions != 0)
							return;
						if (b.available_fast(i, j, length, false)) {
							b.add(i, j, length, false);
							answer[ind] = itos(j) + itos(i) + ships[ind] + '0';
							ind++;
							add_ship(ind, x, y, b, ships, answer, solutions,
									all_solutions, i, j, length);
							ind--;
							b.remove(i, j, length, false);
						}
					}
				}
			}
			for (int i = xx; i < x - length + 1; i++) {
				for (int j = 0; j < avlrows.size(); j++) {
					if (!all_solutions && solutions != 0)
						return;
					if (b.available_fast(i, avlrows[j], length, true)) {
						b.add(i, avlrows[j], length, true);
						answer[ind] = itos(avlrows[j]) + itos(i) + ships[ind] + '1';
						ind++;
						add_ship(ind, x, y, b, ships, answer, solutions,
								all_solutions, i, avlrows[j], length);
						ind--;
						b.remove(i, avlrows[j], length, true);
					}
				}
			}
			if (length > 1) {
				for (int i = 0; i < avlcols.size(); i++) {
					for (int j = 0; j < y - length + 1; j++) {
						if (!all_solutions && solutions != 0)
							return;
						if (b.available_fast(avlcols[i], j, length, false)) {
							b.add(avlcols[i], j, length, false);
							answer[ind] = itos(j) + itos(avlcols[i]) + ships[ind] + '0';
							ind++;
							add_ship(ind, x, y, b, ships, answer, solutions,
									all_solutions, avlcols[i], j, length);
							ind--;
							b.remove(avlcols[i], j, length, false);
						}
					}
				}
			}
		}
	}
}

void add_unknown_ship(unsigned int &ind, int &x, int &y, Board &b,
		vector<char> &ships, vector<string> &answer, int &solutions,
		bool all_solutions, unsigned int unknown, int last_x, int last_y,
		int last_length) {
	/*
	 * Add unknown ships to the board
	 */
	// determine recursion if get needed information
	if (!all_solutions && solutions != 0)
		return;
	// check whether have placed all the ships
	else if (ind == ships.size()) {

		// TODO add unknown ships

	} else {
		int length = ships[ind] - 48;
		int xx = 0;
		if (length == last_length) {
			xx = last_x + 1;
			int i = last_x;
			for (int j = last_y + 1; j < y; j++) {
				if (!all_solutions && solutions != 0)
					return;
				if (b.available(i, j, length, true)) {
					b.add(i, j, length, true);
					answer[ind] = itos(j) + itos(i) + ships[ind] + '1';
					ind++;
					add_ship(ind, x, y, b, ships, answer, solutions,
							all_solutions, i, j, length);
					ind--;
					b.remove(i, j, length, true);
				}
				if (length > 1 && b.available(i, j, length, false)) {
					b.add(i, j, length, false);
					answer[ind] = itos(j) + itos(i) + ships[ind] + '0';
					ind++;
					add_ship(ind, x, y, b, ships, answer, solutions,
							all_solutions, i, j, length);
					ind--;
					b.remove(i, j, length, false);
				}
			}
		}
		for (int i = xx; i < x; i++) {
			for (int j = 0; j < y; j++) {
				if (!all_solutions && solutions != 0)
					return;
				if (b.available(i, j, length, true)) {
					b.add(i, j, length, true);
					answer[ind] = itos(j) + itos(i) + ships[ind] + '1';
					ind++;
					add_ship(ind, x, y, b, ships, answer, solutions,
							all_solutions, i, j, length);
					ind--;
					b.remove(i, j, length, true);
				}
				if (length > 1 && b.available(i, j, length, false)) {
					b.add(i, j, length, false);
					answer[ind] = itos(j) + itos(i) + ships[ind] + '0';
					ind++;
					add_ship(ind, x, y, b, ships, answer, solutions,
							all_solutions, i, j, length);
					ind--;
					b.remove(i, j, length, false);
				}
			}
		}
	}
}

string hashboard(const vector<string> &answer) {
	/*
	 * Generate a pattern for the given board
	 */
	string pat = "";
	for (unsigned int i = 0; i < answer.size(); i++)
		pat += answer[i];
	return pat;
}

void print_answer(const vector<string> &answer) {
	/*
	 * Print the answer in required format
	 */
	for (unsigned int i = 0; i < answer.size(); i++) {
		cout << left << setw(11) << KEY_LIST[answer[i][2]];
		cout << answer[i][0] - 48 << ' ' << answer[i][1] - 48;
		if (answer[i][3] == '0')
			cout << ' ' << "vertical" << endl;
		else if (answer[i][2] != '1')
			cout << ' ' << "horizontal" << endl;
		else
			cout << endl;
	}
}
