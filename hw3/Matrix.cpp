#include <cmath>
#include <cstdlib>
#include <ostream>
#include "Matrix.h"

Matrix::Matrix() {
	// default constructor
	// just defines everything as 0 or null
	data_ = NULL;
	fill_ = rows_ = cols_ = 0;
}

Matrix::Matrix(unsigned int rows, unsigned int cols, double fill) {
	// constructor
	if (rows == 0 || cols == 0) {
		// you can't have a 3x0 matrix, so set rows_ and cols_ to 0 if either is 0
		rows_ = cols_ = 0;
	} else {
		rows_ = rows;
		cols_ = cols;
	}
	fill_ = fill;
	// populate matrix with the fill double
	// allocate memory to data_ for a pointer array of length=rows_
	data_ = new double*[rows_];
	for (unsigned int i = 0; i < rows_; i++) {
		// allocate memory to data_[i] for an array of doubles, each of length=cols_
		data_[i] = new double[cols_];
		for (unsigned int j = 0; j < cols_; j++) {
			// assign fill_ to each item
			data_[i][j] = fill_;
		}
	}
}

Matrix::Matrix(const Matrix& matrix) {
	copy(matrix);
}

Matrix::~Matrix() {
	// the destructor
	// custom implementation is necessary so that we correctly free dynamic memory
	// we do that by deleting each row in data_ then deleting the pointer itself
	wipe_data();
}

unsigned int Matrix::size() const {
	// this isn't actually needed for anything but it seems like something a matrix should have
	return rows_ * cols_;
}

unsigned int Matrix::num_rows() const {
	// self-explanatory function -- returns number of rows in the matrix
	return rows_;
}

unsigned int Matrix::num_cols() const {
	// self-explanatory function -- returns number of rows in the matrix
	return cols_;
}

double* Matrix::get_row(unsigned int row_i) const {
	// returns a "row" of the matrix at index row, i.e. returns a pointer to an array of doubles
	// return null if row_i is out of bounds
	if (row_i >= rows_) return NULL;
	double* row = new double[cols_];
	for (unsigned int i = 0; i < cols_; i++) {
		row[i] = data_[row_i][i];
	}
	return row;
}

double* Matrix::get_col(unsigned int col_i) const {
	// returns a "row" of the matrix at index row, i.e. returns a pointer to an array of doubles
	// return null if row_i is out of bounds
	if (col_i >= cols_) return NULL;
	double* col = new double[rows_];
	for (unsigned int i = 0; i < rows_; i++) {
		col[i] = data_[i][col_i];
	}
	return col;
}

Matrix* Matrix::quarter() const {
	// matrices is the array of Matrix objects returned by this function
	Matrix* matrices = new Matrix[4];
	// the following variables are used to denote where in the array we should start or end when
	// dividing up the data
	unsigned int upper_row_end = ceil((double)rows_/2); // round up rows_/2
	unsigned int lower_row_start = (rows_%2 == 0 ? upper_row_end : rows_/2);
	// if the number of rows in the matrix is even, then upper_row_end and lower_row_start will just
	// be the number of rows divided by 2
	// if the number of rows is odd, then upper_row_end will be rows_/2 rounded up, and
	// lower_row_start will be rows_/2 rounded down
	unsigned int left_col_end = ceil((double)cols_/2); // round up cols_/2
	unsigned int right_col_start = (cols_%2 == 0 ? left_col_end : cols_/2);
	// it works the same with left_col_end and right_col_start, just using the number of columns

	// populate the matrices with data from this matrix object
	for (unsigned int i = 0; i < 4; i++) {
		matrices[i] = Matrix(upper_row_end, left_col_end);
	}
	for (unsigned int i = 0; i < upper_row_end; i++) {
		for (unsigned int j = 0; j < left_col_end; j++) {
			matrices[0].set(i, j, data_[i][j]);
		}
		for (unsigned int j = right_col_start; j < cols_; j++) {
			// subtract right_col_start from j to offset the distance
			matrices[1].set(i, j-right_col_start, data_[i][j]);
		}
	}
	for (unsigned int i = lower_row_start; i < rows_; i++) {
		for (unsigned int j = 0; j < left_col_end; j++) {
			matrices[2].set(i-lower_row_start, j, data_[i][j]);
		}
		for (unsigned int j = right_col_start; j < cols_; j++) {
			matrices[3].set(i-lower_row_start, j-right_col_start, data_[i][j]);
		}
	}
	return matrices;
}

bool Matrix::is_empty() const {
	return rows_ == 0;
}

bool Matrix::get(unsigned int row, unsigned int col, double& val) const {
	// stores the value at data_[row][col] in val if the row and col are within the matrix bounds
	// if they aren't, this returns false
	if (row >= rows_ || col >= cols_) return false;
	val = data_[row][col];
	return true;
}

bool Matrix::set(unsigned int row, unsigned int col, double val) {
	// sets the value at data_[row][col] to val if the row and col are within the matrix bounds
	// if they aren't, this returns false
	if (row >= rows_ || col >= cols_) return false;
	data_[row][col] = val;
	return true;
}

bool Matrix::add(const Matrix& matrix) {
	// adds each element of the passed matrix to the element in the appropriate position of this one
	if (rows_ != matrix.num_rows() || cols_ != matrix.num_cols()) return false;
	for (unsigned int i = 0; i < matrix.num_rows(); i++) {
		double* curr_row = matrix.get_row(i); // copy row, then
		for (unsigned int j = 0; j < matrix.num_cols(); j++) {
			data_[i][j] += curr_row[j];
		}
		delete [] curr_row; // deallocate it
	}
	return true;
}

bool Matrix::subtract(const Matrix& matrix) {
	// same as the add member function but it subtracts
	if (rows_ != matrix.num_rows() || cols_ != matrix.num_cols()) return false;
	for (unsigned int i = 0; i < matrix.num_rows(); i++) {
		double* curr_row = matrix.get_row(i); // copy row, then
		for (unsigned int j = 0; j < matrix.num_cols(); j++) {
			data_[i][j] -= curr_row[j];
		}
		delete [] curr_row; // deallocate it
	}
	return true;
}

void Matrix::multiply_by_coefficient(int n) {
	// multiplies each item in the matrix by an integer n
	for (unsigned int i = 0; i < rows_; i++) {
		for (unsigned int j = 0; j < cols_; j++) {
			data_[i][j] *= n;
		}
	}
}

bool Matrix::swap_row(unsigned int row1, unsigned int row2) {
	// swaps two rows at the given indices: row1 and row2
	// if either row1 or row2 is out of bounds return false
	if (row1 >= rows_ || row2 >= rows_) return false;
	// allocate memory for two new double arrays, each the width (i.e. # of columns) of the matrix
	double* new_row1 = new double[cols_];
	double* new_row2 = new double[cols_];
	for (unsigned int i = 0; i < cols_; i++) {
		new_row1[i] = data_[row1][i];
		new_row2[i] = data_[row2][i];
	}
	// deallocate the data in the old rows, then replace it with the data in the new ones, switched
	delete [] data_[row1]; data_[row1] = new_row2;
	delete [] data_[row2]; data_[row2] = new_row1;
	return true;
}

void Matrix::transpose() {
	// new_data will replace data_
	double** new_data = new double*[cols_];
	for (unsigned int i = 0; i < cols_; i++) {
		new_data[i] = new double[rows_];
		for (unsigned int j = 0; j < rows_; j++) {
			// copy each item from data_ into new_data, but switch the col and row indices
			new_data[i][j] = data_[j][i];
		}
	}
	// wipe data_, then replace it with new_data
	wipe_data();
	data_ = new_data;
	// reassign rows_ and cols_ AFTER replacing data_ so that we don't try to access bad memory
	unsigned int old_rows = rows_;
	rows_ = cols_;
	cols_ = old_rows;
}

void Matrix::resize(unsigned int rows, unsigned int cols, double fill) {
	// similar implementation to transpose, first start by initializing a double pointer
	double** new_data = new double*[rows];
	for (unsigned int i = 0; i < rows; i++) {
		new_data[i] = new double[cols];
		// check that i is still in bounds so that we can use it to access elements in data_
		if (i < rows_) {
			for (unsigned int j = 0; j < cols; j++) {
				// check that j is also still in bounds
				if (j < cols_) {
					new_data[i][j] = data_[i][j];
				} else {
					// if it's not then just set to fill
					new_data[i][j] = fill;
				} 
			}
		} else {
			for (unsigned int j = 0; j < cols; j++) {
				// if i is out of bounds of the old array then set the current item to fill
				new_data[i][j] = fill;
			}
		}
	}
	// wipe data_ then replace it
	wipe_data();
	data_ = new_data;
	// reassign rows_ and cols_
	rows_ = rows;
	cols_ = cols;
}

void Matrix::clear() {
	// clears the matrix -- wipes the dynamic array, sets all other member variables to 0
	wipe_data();
	data_ = NULL;
	fill_ = rows_ = cols_ = 0;
}

void Matrix::copy(const Matrix& matrix) {
	// this member function of Matrix takes a matrix as an argument and sets the current matrix 
	// object's properties equal to the passed matrix's
	// this is necessary because data_ is dynamically allocated, so we have to copy its contents
	// individually as opposed to copying over the memory address (which would be bad)
	// this function assumes that data_ does not already have memory allocated to it
	rows_ = matrix.num_rows();
	cols_ = matrix.num_cols();
	data_ = new double*[rows_];
	for (unsigned int i = 0; i < rows_; i++) {
		data_[i] = new double[cols_];
		for (unsigned int j = 0; j < cols_; j++) {
			data_[i][j] = matrix.data_[i][j];
		}
	}
}

void Matrix::wipe_data() {
	// deallocates all memory in data_
	// checks if data_ is null before deleting so the compiler doesn't encounter null when it expects
	// a double pointer
	if (data_ != NULL) {
		for (unsigned int i = 0; i < rows_; i++) {
			delete [] data_[i];
		}
		delete [] data_;
	} else {
		delete data_;
	}
}

Matrix& Matrix::operator= (const Matrix& m2) {
	// custom assignment operator -- necessary so that we don't accidentally copy over the memory of
	// another matrix object
	// check if this matrix and m2 are stored in the same place in memory (i.e. the same)
	// if so, no need to redefine data_, just dereference and return this object
	if (this != &m2) {
		if (!this->is_empty()) wipe_data();
		copy(m2);
	}
	return *this;
}

std::ostream& operator<< (std::ostream& out, const Matrix& matrix) {
	// overloading the bit shift operator with ostream objects so that we can make output simpler
	out << '[';
	if (matrix.num_rows() > 0) {
		for (unsigned int i = 0; i < matrix.num_rows()-1; i++) {
			double* curr_row = matrix.get_row(i);
			// each row in the matrix gets copied and printed
			for (unsigned int j = 0; j < matrix.num_cols(); j++) {
				out << ' ' << curr_row[j];
			}
			delete [] curr_row; // deallocate curr_row so no memory leaks
			out << "\n ";
		}
		// special case for the last row so we don't accidentaly print out another newline
		double* last_row = matrix.get_row(matrix.num_rows()-1);
		for (unsigned int i = 0; i < matrix.num_cols(); i++) {
			out << ' ' << last_row[i];
		}
		delete [] last_row;
	}
	out << " ]" << std::endl;
	return out;
}

bool operator== (const Matrix& m1, const Matrix& m2) {
	// check if two Matrix objects are equal, i.e. each number in m1 matches each number in m2, and
	// in the correct position
	if (m1.num_rows() != m2.num_rows() || m1.num_cols() != m2.num_cols()) return false;
	for (unsigned int i = 0; i < m1.num_rows(); i++) {
		double* curr_row_m1 = m1.get_row(i);
		double* curr_row_m2 = m2.get_row(i);
		for (unsigned int j = 0; j < m2.num_cols(); j++) {
			if (curr_row_m1[j] != curr_row_m2[j]) {
				// if we find that two values in the same location of each matrix are not equivalent,
				// we return false -- but not before deallocating the memory in the curr_rows
				delete [] curr_row_m1;
				delete [] curr_row_m2;
				return false;
			}
		}
		delete [] curr_row_m1;
		delete [] curr_row_m2;
	}
	return true;
}

bool operator!= (const Matrix& m1, const Matrix& m2) {
	// simply return the opposite of m1 == m2
	return !(m1 == m2);
}
