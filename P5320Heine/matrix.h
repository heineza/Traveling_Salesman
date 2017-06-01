#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
//#include <initializer_list>
using namespace std;

template <typename T>
class matrix
{
public:
	// construct a (0x0) empty  matrix-of-int T.  
	matrix() :arr(0)
	{}
	// constructs rows x cols  matrix-of-int T.
	matrix(int rows, int cols) : arr(rows)
	{
		for (auto & thisRow : arr)
			thisRow.resize(cols);
	}

	//constructs an (nxm) matrix, determined by the number of elements in the initializer list { {...} ...{...}}
	matrix(initializer_list<vector<T>> lst) : arr(lst.size())
	{
		int i = 0;
		for (auto & v : lst)
			arr[i++] = std::move(v);
	}

	//Copy constructor(L - Ref parameter)
	matrix(const vector<vector<T>> & v) : arr{ v }
	{ }

	//Move constructor(R - Ref parameter)
	matrix(vector<vector<T>> && v) : arr{ std::move(v) }
	{ }
	// returns the k-th row; constversion
	const vector<T> & operator[](int row) const
	{
		return arr[row];
	}

	// returns the k-th row; npn-const version
	vector<T> & operator[](int row)
	{
		return arr[row];
	}

	int numrows() const
	{
		return arr.size();
	}
	int numcols() const
	{
		return numrows() ? arr[0].size() : 0;
	}

	matrix<T> t(); //returns a new matrix that is the transpose of *this
	const matrix<T> & operator=(const matrix<T> & rhs);  //assignment operator
	const matrix<T> operator+=(const matrix<T> & rhs);   //matrix-matrix addition (mutator)
	const matrix<T> operator+ (const matrix<T> & rhs) const; //matrix-matrix addition (non-mutator)

	const matrix<T> operator*= (const  T &c);   //scalar multiplication (mutator)
	const matrix<T> operator*(const  T &c) const;  //scalar multiplication (non-mutator)
	const matrix<T> operator* (const matrix<T> & rhs)const;  //matrix-matrix multiplication (non-mutator)
	vector<T> row(int k);		 //returns a new vector (non-mutator), a copy of the kth-row of *this
	vector<T> col(int k);		 //returns a new vector (non-mutator), a copy of the kth-row of *this
	matrix<T> deleteRow(int k); //returns a new T (non-mutator), a copy of *this with k-th row deleted
	matrix<T> deleteCol(int k); //returns a new T (non-mutator), a copy of *this with k-th column deleted
	T det();					 //returns the determinant of *this

	//Matrix input / output member functions(both use row - major order; read mutates *this T.)
	void read(istream & in = cin);
	void print(ostream & out = cout) const;
private:
	vector<vector<T>> arr;
	T det_aux();  //helper function for the public determinant function, member det(). 
};

//non-member matrix-related function: dot product of two vectors
template<typename T>
T dot(const vector<T> & lvec, const vector<T> & rvec);

//non-member matrix-related function: returns an (n x n) identity matrix 
template<typename T>
matrix<T> eye(int n);

//non-member matrix-related function: returns an (row x col) matrix filled with zeroes (0's) 
template<typename T>
matrix<T> zeroes(int rows, int cols);

//non-member matrix-related function: returns an (row x col) matrix filled with ones (1's) 
template<typename T>
matrix<T> ones(int rows, int cols);

// used to bind a matrix (in row-major order) to element values using the stream extraction operator and istream T
template<typename T>
istream& operator>>(istream &  in, matrix<T> & rhs);

// used to output a matrix (in row-major order) using the stream insertion operator and ostream T 
template<typename T>
ostream& operator<<(ostream & out, const  matrix<T> & rhs);
#include "Matrix.cpp"
#endif


