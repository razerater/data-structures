// Raz Reed
// board.h
// 3/23/2017

#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class Board {
	public:
		// constructors
		Board();
		Board(int rows, int cols);
		Board(const Board& board);
		// accessors
		int size() const;
		int filled_size() const;
		int get_rows() const;
		int get_cols() const;
		bool forms_bad_word(const std::vector<std::string>& bad_words, char letter,
							int row, int col) const;
		bool has_space_for_word(const std::vector<std::string>& bad_words, const std::string& word,
								int row, int col, int row_inc, int col_inc);
		// modifiers
		void insert_word(const std::string& word, int row, int col, int row_inc, int col_inc);
		// custom operators
		const std::vector<char>& operator[] (int i) const;
		std::vector<char>& operator[] (int i);
	private:
		int rows_, cols_;
		std::vector< std::vector<char> > letters_;
		bool forms_bad_word(const std::string& bad_word, /*unsigned */int word_i, int word_i_inc,
							int row, int col, int row_inc, int col_inc) const;
};
std::ofstream& operator<< (std::ofstream& out, const Board& board);
std::ostream& operator<< (std::ostream& out, const Board& board);
bool operator== (const Board& board1, const Board& board2);

#endif
