#ifndef MATRIX_CPP
#define MATRIX_CPP
#include "Matrix.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <stdlib.h> 
using namespace std;

//O(N^2)
template<typename T>
matrix<T> matrix<T>::t() {
	matrix<T> result(*this);
	for (int i = 0; i < numrows(); i++)
	{
		for (int j = 0; j < numcols(); j++)
			result.arr[i][j] = this->arr[j][i];
	}
	*this = result;
	return *this;
}

template <typename T>
const matrix<T> & matrix<T>::operator=(const matrix<T> & rhs)
{
	assert(this->numrows() == rhs.numrows() && this->numcols() == rhs.numcols());
	if (this != &rhs)
		this->arr = rhs.arr;
	return *this;
}

template<typename T>
const matrix<T> matrix<T>::operator+=(const matrix<T> & rhs){
	assert(this->numrows() == rhs.numrows() && this->numcols() == rhs.numcols());
	for (int i = 0; i < numrows(); i++)
	for (int j = 0; j < numcols(); j++)
		this->arr[i][j] += rhs.arr[i][j];
	return *this;
}

template<typename T>
const matrix<T> matrix<T>::operator+ (const matrix<T> & rhs) const{

	assert(this->numrows() == rhs.numrows() && this->numcols() == rhs.numcols());

	matrix<T> result(*this);
	result += rhs;
	return result;
}
//O(N^2)
template<typename T>
const matrix<T> matrix<T>::operator*= (const  T &c){
	for (int i = 0; i < numrows(); ++i)
	for (int j = 0; j < numcols(); ++j)
		this->arr[i][j] *= c;
	return *this;
}

//O(N^2)
template<typename T>
const matrix<T>  matrix<T>::operator* (const  T &c) const{
	matrix<T> result(*this);
	result *= c;
	return result;
}

//O(N^3)
template<typename T>
const matrix<T> matrix<T>::operator* (const matrix<T> & rhs)const{
	assert(this->numcols() == rhs.numrows());
	matrix<T> result(this->numrows(), rhs.numcols());
	for (int i = 0; i < this->numrows(); i++)
	{
		for (int j = 0; j < rhs.numcols(); j++)
		{
			T mult_res = T();
			T val1 = T();
			T val2 = T();
			for (int k = 0; k < this->numcols(); k++)
			{
				val1 = this->arr[i][k];
				val2 = rhs.arr[k][j];
				mult_res += val1 * val2;
			}
			result.arr[i][j] = mult_res;
		}
	}
	return result;
}

//O(N)
template<typename T>
vector<T> matrix<T>::row(int k){
	assert(k <= numcols());
	vector<T> result;
	for (int i = 0; i < numcols(); i++)
		result.push_back(this->arr[k][i]);
	return result;
}

//O(N)
template<typename T>
vector<T> matrix<T>::col(int k){
	assert(k <= numrows());
	vector<T> result;
	for (int i = 0; i < numrows(); i++)
		result.push_back(this->arr[i][k]);
	return result;
}

//O(N)
template<typename T>
matrix<T> matrix<T>::deleteRow(int k){
	matrix<T> result(*this);
	for (int i = 0; i < result.numrows(); i++)
	{
		if (i == k)
		{
			result.arr.erase(result.arr.begin() + k);
		}
	}
	return result;
}

//O(N^2)
template<typename T>
matrix<T> matrix<T>::deleteCol(int k){
	matrix<T> result(*this);

	for (int i = 0; i < result.numrows(); i++)
	{
		for (int j = 0; j < result.numcols(); j++)
		{
			if (j == k)
			{
				result.arr[i].erase(result.arr[i].begin() + k);
			}
		}
	}
	return result;
}

//O(N^2*N!)
template <typename T>
T matrix<T>::det_aux() {
	T sum;
	if (this->numrows() == 0)
		return 0;
	else if (this->numrows() == 1)
		return this->arr[0][0];
	else if (this->numrows() == 2)
		return this->arr[0][0] * this->arr[1][1]
		- this->arr[1][0] * this->arr[0][1];
	else{
		sum = T();
		for (int k = 0; k < this->numcols(); k++){
			sum += ((k % 2) ? -1 : 1)*(this->arr[0][k] *
				(this->deleteRow(0).deleteCol(k)).det_aux());
		}
	}
	return sum;
}

template <typename T>
T matrix<T>::det()
{
	assert(this->numrows() == this->numcols());
	return this->det_aux();
}

template<typename T>
void matrix<T>::read(istream & in = cin){

	int totalElements = numrows()*numcols();
	for (int i = 0; i < numrows(); i++)
	for (int j = 0; j < numcols(); j++)
		this->arr[i][j] = T();

	if (this->numrows() == 0) return;

	int numberOfElements = 0;
	while (numberOfElements < totalElements && in >> ws && !in.eof()){
		T element;
		numberOfElements++;
		in >> element;
		this->arr[(numberOfElements - 1) / numcols()][(numberOfElements - 1) % numcols()] = element;
	}
}

template<typename T>
void matrix<T>::print(ostream & out = cout) const{
	for (int i = 0; i < numrows(); i++) {
		cout << this->arr[i][0];
		for (int j = 1; j < numcols(); j++)
			cout << " " << this->arr[i][j];
		cout << endl;
	}
}

template<typename T>
istream& operator>>(istream &  in, matrix<T> & rhs){
	rhs.read(in);
	return in;
}

template<typename T>
ostream& operator<<(ostream & out, const  matrix<T> & rhs){
	rhs.print(out);
	return out;
}

template<typename T>
T dot(const vector<T> & lvec, const vector<T> & rvec){
	assert(lvec.size() == rvec.size());

	T result = T();
	for (unsigned i = 0; i < lvec.size(); i++)
	{
		result += lvec.at(i) * rvec.at(i);
	}
	return result;
}
template<typename T>
matrix<T> eye(int n) {
	matrix<T> result(n,n);

	for (int i = 0; i < n; i++)
		result[i][i] = 1;
	
	return result;
}

template<typename T>
matrix<T> zeroes(int rows, int cols){
	matrix<T> zeroesMat(rows, cols);
	for (int i = 0; i < rows; i++)
	for (int j = 0; j < cols; j++)
		zeroesMat[i][j] = (T)0;
	return zeroesMat;
}

template<typename T>
matrix<T> ones(int rows, int cols){
	matrix<T> onesMat(rows, cols);
	for (int i = 0; i < rows; i++)
	for (int j = 0; j < cols; j++)
		onesMat[i][j] = (T)1;
	return onesMat;
}

#endif