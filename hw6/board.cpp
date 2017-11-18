// Raz Reed
// board.cpp
// 3/23/2017

#include "board.h"

Board::Board() {
	rows_ = 0;
	cols_ = 0;
}
Board::Board(int rows, int cols) {
	if (rows == 0 || cols == 0) {
		rows_ = 0;
		cols_ = 0;
	} else {
		rows_ = rows;
		cols_ = cols;
	}
	for (int i = 0; i < rows_; i++) {
		letters_.push_back(std::vector<char>());
		// letters_.push_back(std::vector<char>('0', cols_));
		// the above line instead of the following for loop?
		for (int j = 0; j < cols_; j++) {
			letters_[i].push_back('0');
		}
	}
}

Board::Board(const Board& board) {
	rows_ = board.get_rows();
	cols_ = board.get_cols();
	for (int i = 0; i < rows_; i++) {
		letters_.push_back(std::vector<char>());
		// letters_.push_back(std::vector<char>(board[i][j], cols_));
		// the above line instead of the following for loop?
		for (int j = 0; j < cols_; j++) {
			letters_[i].push_back(board[i][j]);
		}
	}
}

int Board::size() const {
	return rows_*cols_;
}

int Board::filled_size() const {
	int filled_size = 0;
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			if (letters_[i][j] != '0') filled_size++;
		}
	}
	return filled_size;
}

int Board::get_rows() const {
	return rows_;
}
int Board::get_cols() const {
	return cols_;
}

bool Board::forms_bad_word(const std::vector<std::string>& bad_words, char letter,
						   int row, int col) const {
	// takes in a position in the board and checks if any bad words could
	// be formed in the board by placing a specified letter in the given position (row, col)
	for (unsigned int i = 0; i < bad_words.size(); i++) {
		// check if the letter itself is the bad word, return true if so
		if (bad_words[i] == std::string(letter, 1))
			return true;
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				// j and k represent the directions that we'll check for a potential bad word
				if (j == 0 && k == 0)
					continue;
				// if the letter is in the current bad word, search the board for the word
				/*if (bad_words[i].find(letter) != std::string::npos &&
					forms_bad_word(bad_words[i], bad_words[i].find(letter), row+j, col+k, j, k))*/
				int size = bad_words[i].size();
				if (bad_words[i][0] == letter &&
					// forms_bad_word(bad_words[i], 1, row+j, col+k, j, k))
					forms_bad_word(bad_words[i], 1, 1, row+j, col+k, j, k))
					return true;
				// uncomment this block:
				if (bad_words[i][size-1] == letter &&
					forms_bad_word(bad_words[i], size-1, -1, row+j, col+k, j, k))
					return true;
			}
		}
	}
	return false;
}

bool Board::forms_bad_word(const std::string& bad_word, int word_i, int word_i_inc,
						   int row, int col, int row_inc, int col_inc) const {
	// base cases
	if (word_i < 0 || word_i >= bad_word.size())
		return true;

	if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
		return false;

	if (letters_[row][col] != bad_word[word_i])
		return false;

	// return forms_bad_word(bad_word, word_i+1, row+row_inc, col+col_inc, row_inc, col_inc);
	return forms_bad_word(bad_word, word_i+word_i_inc, word_i_inc,
						  row+row_inc, col+col_inc, row_inc, col_inc);
}

bool Board::has_space_for_word(const std::vector<std::string>& bad_words, const std::string& word,
							   int row, int col, int row_inc, int col_inc) {
	// this method checks if it is possible to store a string "word" in this instance of a Board,
	// starting at the location board[row][col] and moving through the board with row_inc and col_inc
	// for example, if we want to go backwards horizontally, row_inc = 0 and col_inc = -1
	// row_inc = 0 bc we're not moving vertically and col_inc = 0 bc we're moving left horizontally
	// row_inc and col_inc can each only be 1, 0, or -1, and they can't both be 0 (otherwise we would
	// move nowhere)

	// we can't add a blank word to the board
	if (word.size() == 0)
		return false;
	// these variables represent the indices of the proposed location immediately AFTER where in the
	// board the final letter of the word will be placed in the board
	int final_row = row + row_inc*word.size();
	int final_col = col + col_inc*word.size();

	// check first if we would go past the bounds of the board when trying to check if the word fits
	// if we would, then there is no space for the word and we should return false
	if ((row_inc == 1 && final_row > rows_) || (row_inc == -1 && final_row < -1) ||
		(col_inc == 1 && final_col > cols_) || (col_inc == -1 && final_col < -1))
		return false;

	for (int i = row, j = col, k = 0; k < word.size(); i += row_inc, j += col_inc, k++) {
		// if we encounter a letter in the board that is not empty and does not match the letter
		// at the corresponding position of the word, the space in the board we're considering
		// placing the word in is at least partially occupied by another word (we know this because
		// a letter from another word is there).
		if (letters_[i][j] != word[k] && letters_[i][j] != '0')
			return false;

		// this method isn't working :(
		if (forms_bad_word(bad_words, word[k], i, j))
			return false;
	}
	return true;
}

void Board::insert_word(const std::string& word, int row, int col, int row_inc, int col_inc) {
	// This function inserts the given word into this instance of a Board, starting with the first
	// letter at board[row][col] and moving through the board with row_inc and col_inc. It assumes
	// that attempting this insert will not result in an out-of-bounds error.
	for (int i = row, j = col, k = 0; k < word.size(); i += row_inc, j += col_inc, k++) {
		letters_[i][j] = word[k];
	}
}

const std::vector<char>& Board::operator[] (int i) const {
	return letters_[i];
}

std::vector<char>& Board::operator[] (int i) {
	return letters_[i];
}

std::ofstream& operator<< (std::ofstream& out, const Board& board) {
	out << "Board: ";
	for (int i = 0; i < board.get_rows(); i++) {
		out << "\n  ";
		for (int j = 0; j < board.get_cols(); j++) {
			out << board[i][j];
		}
	}
	out << std::endl;
	return out;
}

std::ostream& operator<< (std::ostream& out, const Board& board) {
	out << "Board: ";
	for (int i = 0; i < board.get_rows(); i++) {
		out << "\n  ";
		for (int j = 0; j < board.get_cols(); j++) {
			out << board[i][j];
		}
	}
	out << std::endl;
	return out;
}

bool operator== (const Board& board1, const Board& board2) {
	if (board1.get_rows() != board2.get_rows() || board1.get_cols() != board2.get_cols())
		return false;
	for (int i = 0; i < board1.get_rows(); i++) {
		for (int j = 0; j < board1.get_cols(); j++) {
			if (board1[i][j] != board2[i][j]) return false;
		}
	}
	return true;
}
