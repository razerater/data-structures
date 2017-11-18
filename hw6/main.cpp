// Raz Reed
// main.cpp
// 3/23/2017

#include "board.h" // all relevant includes are done in board.h/board.cpp


// global variables
const std::string ALPHABET("abcdefghijklmnopqrstuvwxyz");

std::vector<Board> complete_solutions(std::vector<Board>& solutions,
				   const std::vector<std::string>& bad_words, Board board, int row, int col) {
	// This method returns a vector of all possible solutions for the Board object board. It should
	// be used when the Board is mostly complete but there are still one or two empty spaces left. It
	// fills these spaces by inserting every letter in the alphabet in the first available space it
	// finds, and then calling itself providing each of the resulting boards as arguments. It does this
	// until the Board has no available spaces. In each iteration, it appends the vector of Boards
	// made by the next call (solutions to the puzzle) to the temp_solutions vector, which is
	// ultimately returned by the first call on the stack.
	std::vector<Board> temp_solutions;
	// row and col are the indices of the row and column we should start the search from
	int new_row = -1;
	int new_col = -1;
	bool found_flag = false;
	for (int i = row; i < board.get_rows(); i++) {
		for (int j = col; j < board.get_cols(); j++) {
			if (board[i][j] == '0') {
				new_row = i;
				new_col = j;
				found_flag = true;
				break;
			}
		}
		if (found_flag) break;
	}

	// base case
	if (new_row == -1) {
		// check for duplicates
		// it compares the next solution we want to add to every other solution in the solutions
		// vector -- if it finds a match then we don't add it to the vector
		for (unsigned int i = 0; i < solutions.size(); i++) {
			if (board == solutions[i]) {
				return temp_solutions;
			}
		}
		// if none of the spaces are empty then this Board is a solution to the word search
		// add it to the solutions vector
		temp_solutions.push_back(board);
	}
	else {
		for (unsigned int i = 0; i < ALPHABET.size(); i++) {
			board[new_row][new_col] = ALPHABET[i];
			// new_col+1 so that we start our search beginning with the next letter in the board
			// starting from this position will save us processing time because it stops us from
			// checking positions we've already checked in previous calls of complete_solutions()
			std::vector<Board> comp_sols = 
							   complete_solutions(solutions, bad_words, board, new_row, new_col+1);
			temp_solutions.insert(temp_solutions.end(), comp_sols.begin(), comp_sols.end());
		}
	}
	return temp_solutions;
}

// the driver function that starts the recursion
void complete_solutions(std::vector<Board>& solutions, const std::vector<std::string>& bad_words,
						const Board& board) {
	std::vector<Board> comp_sols = complete_solutions(solutions, bad_words, board, 0, 0);
	solutions.insert(solutions.end(), comp_sols.begin(), comp_sols.end());
}

// this method does the actual recursion to find a vector of all possible solutions for a given vector
// of words
void find_solutions(std::vector<Board>& solutions, const std::vector<std::string>& words,
	 const std::vector<std::string>& bad_words, int word_i, int total_len, Board board) {
	// base case: if we have reached the end of the words vector
	if (word_i >= words.size()) {
		// The next if statement checks if the board being passed is a valid solution. The
		// has_space_for_word() method handles making sure that we put words in the right place, so
		// if we reach the point where the board is full and all the words are in the right places,
		// we should add such a board to the solutions vector.
		if (board.filled_size() >= total_len || total_len > board.size()) {
			// fill all empty spaces in the board
			complete_solutions(solutions, bad_words, board);
		}
		// our recursion is finished so we should exit out of the function
		return;
	}
	std::string word = words[word_i];
	int rows = board.get_rows();
	int cols = board.get_cols();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++) {
					// skip the case where k and l are 0 because it doesn't make sense to add a word
					// with no direction
					if (k == 0 && l == 0) continue;
					if (board.has_space_for_word(bad_words, word, i, j, k, l)) {
						// if space exists in the board for the word, then we:
						Board new_board(board); // copy the previous board into new_board and
						// insert the word into the board, starting from the first letter
						new_board.insert_word(word, i, j, k, l);
						find_solutions(solutions, words, bad_words, word_i+1, total_len, new_board);
					}
				}
			}
		}
	}
}

// the driver function that starts the recursion
void find_solutions(std::vector<Board>& solutions, const std::vector<std::string>& words,
					const std::vector<std::string>& bad_words, int rows, int cols) {
	Board board(rows, cols);
	int total_len = 0; // represents total length of all words in the vector
	for (int i = 0; i < words.size(); i++) {
		total_len += words[i].size();
	}
	find_solutions(solutions, words, bad_words, 0, total_len, board);
}


int main(int argc, char* argv[]) {
	if (argc != 4) {
		std::cerr << "Format: ./main.exe puzzle_file out_file one_solution/all_solutions" << std::endl;
		return 1;
	}

	std::ifstream puzzle_file(argv[1]);
	std::ofstream out_file(argv[2]);
	std::string str_solutions(argv[3]);

	if (!puzzle_file.good()) {
		std::cerr << "Could not read from the provided file: " << argv[1] << std::endl;
		return 1;
	}
	if (!out_file.good()) {
		std::cerr << "Could not write to the provided file: " << argv[2] << std::endl;
		return 1;
	}
	if (str_solutions != "one_solution" && str_solutions != "all_solutions") {
		std::cerr << "The third argument must be \"one_solution\" or \"all_solutions\"." << std::endl;
		return 1;
	}

	int cols, rows;
	puzzle_file >> cols >> rows;

	// read in the words and forbidden (bad) words
	std::vector<std::string> words, bad_words;
	std::string sign, next_word;
	while (puzzle_file >> sign) {
		puzzle_file >> next_word;
		if (sign == "+")
			words.push_back(next_word);
		else if (sign == "-")
			bad_words.push_back(next_word);
	}

	std::vector<Board> solutions;
	find_solutions(solutions, words, bad_words, rows, cols);

	if (solutions.size() == 0) {
		out_file << "No solutions found." << std::endl;
	}
	else if (str_solutions == "one_solution") {
		out_file << solutions[0];
	}
	else {
		out_file << solutions.size() << " solution(s):" << std::endl;
		for (unsigned int i = 0; i < solutions.size(); i++) {
			out_file << solutions[i];
		}
	}

	return 0;
}
