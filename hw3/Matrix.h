#ifndef Matrix_h_
#define Matrix_h_

class Matrix {
	public:
		// default constructor
		Matrix();
		// constructor
		Matrix(unsigned int rows, unsigned int cols, double fill=0);
		// copy constructor
		Matrix(const Matrix& matrix);
		// destructor
		~Matrix();
		// accessors
		unsigned int size() const;
		unsigned int num_rows() const;
		unsigned int num_cols() const;
		double* get_row(unsigned int row_i) const;
		double* get_col(unsigned int col_i) const;
		Matrix* quarter() const;
		bool is_empty() const;
		bool get(unsigned int row, unsigned int col, double& val) const;
		// modifiers
		bool set(unsigned int row, unsigned int col, double val);
		bool add(const Matrix& matrix);
		bool subtract(const Matrix& matrix);
		void multiply_by_coefficient(int n);
		bool swap_row(unsigned int row1, unsigned int row2);
		void transpose();
		void resize(unsigned int rows, unsigned int cols, double fill=0);
		void clear();
		Matrix& operator= (const Matrix& m2);
	private:
		unsigned int rows_, cols_;
		double fill_;
		double** data_;
		// private helper functions
		void copy(const Matrix& matrix);
		void wipe_data();
};
// custom operators
std::ostream& operator<< (std::ostream& out, const Matrix& matrix);
bool operator== (const Matrix& m1, const Matrix& m2);
bool operator!= (const Matrix& m1, const Matrix& m2);

#endif
