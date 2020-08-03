#pragma once

#include<iostream>
#include<vector>
#include<iomanip>
#include<cmath>
#include<algorithm>
#include<functional>


/**
 * @brief	class for matrix
 */
class Matrix {
public:
	Matrix(void);
	Matrix(const Matrix& newMatrix);
	Matrix(int row, int col);
	Matrix(int row, int col, const std::vector<double> newData);

	inline int GetRow(void) const;
	inline int GetCol(void) const;
	inline int GetSize(void) const;
	inline double GetData(int k) const;											//Get the k-th element in the matrix (stored as a one-dimensional array)
	double& GetVal(int i, int j);												//quote of the i-th row and j-th column of the matrix, starting from 0

	Matrix& operator = (const Matrix& m);
	bool operator == (const Matrix& m) const;

	const Matrix operator + (const Matrix& m) const;
	const Matrix operator + (double x) const;
	friend const Matrix operator + (double x, const Matrix& m);
	Matrix& operator += (const Matrix& m);
	Matrix& operator += (double x);

	const Matrix operator - (const Matrix& m) const;
	const Matrix operator - (double x) const;
	Matrix& operator -= (const Matrix& m);
	Matrix& operator -= (double x);

	const Matrix operator * (const Matrix& m) const;
	const Matrix operator * (double x) const;
	friend const Matrix operator * (double x, const Matrix& m);
	Matrix& operator *= (const Matrix& m);
	Matrix& operator *= (double x);

	const Matrix operator / (const Matrix& m) const;
	const Matrix operator / (double x) const;
	Matrix& operator /= (const Matrix& m);
	Matrix& operator /= (double x);

	const Matrix LeftDiv(const Matrix& m) const;								//Left division
	const Matrix operator ^ (int power);										//power

	friend std::istream& operator >> (std::istream& input, Matrix& m);
	//friend std::ostream& operator << (std::ostream& output, const Matrix& m);

	friend double Max(const Matrix m);											//Get the maximum element in the matrix
	friend double Min(const Matrix m);											//Get the minimum element in the matrix
	friend double Sum(const Matrix m);											//Get the Sum of the elements in the matrix
	friend double Mean(const Matrix m);											//Get the Mean of the elements in the matrix
	friend Matrix Prod(const Matrix m, int type);								//Get the Product of the elements in the matrix (by column/row)
	friend void Sort(Matrix& m);												//Sort the elements in the matrix

	Matrix Reshape(int row, int col);											//Reshape the matrix with a new row and column
	static Matrix Eye(int n);													//identity matrix
	Matrix Trans(void);															//Transposed matrix
	void   SwapRow(int row1, int row2);											//swap two rows of the matrix
	void   LinearChange(int row1, int row2, double times);						//matrix linear Transformation
	int    Legalify(int col);													//Legalify the matrix
	double Det(void);															//calculate the Determinant of the matrix
	Matrix UpperTri(void);														//upper triangle of the matrix
	Matrix Inv(void);															//Inverse of matrix

protected:
	int row;
	int col;
	int size;
	std::vector<double> data;
};

/**
 * @brief	class for vector
 */
class Vec : public Matrix {
public:
	Vec(void);
	Vec(const Vec& newVec);
	Vec(int size);
	Vec(int size, const std::vector<double> newData);

	void AddData(double x);														//add element into the vector

	Vec& operator = (const Matrix& m);

	friend double Norm(const Vec v);											//2-Norm (root of square Sum)
	friend double Dot(const Vec v1, const Vec v2);								//inner Product
	friend Vec Cross(const Vec v1, const Vec v2);								//Cross Product (3D vector)
};

int Sign(double x);																//sign function

double Integrate(std::function<double(double)> f, double L, double R);			//solve intergral
Vec EquationSolve(std::function<double(double)> f, double L, double R); 		//solve equation

int Matrix::GetRow(void) const {
	return row;
}

int Matrix::GetCol(void) const {
	return col;
}

int Matrix::GetSize(void) const {
	return size;
}

double Matrix::GetData(int k) const {
	return data[k];
}
