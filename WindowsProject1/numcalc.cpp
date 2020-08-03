#include"numcalc.h"

const int precision = 6;														//output precision
const double eps = 1e-5;														//calculation precision

void errorThrow(std::string Msg) {
	throw(Msg);
}

/**
 * @brief	quote of the i-th row and j-th column of the matrix, starting from 0
 * @param	int, the row number
 * @param	int, the column number
 * @return	double&
 */
double& Matrix::GetVal(int i, int j) {
	if (i < 0 || i >= row || j < 0 || j >= col) {
		errorThrow("Index out of bounds when GetVal.");
		return data[0];
	}
	return data[i * col + j];
}

/**
 * @brief	constructor of Matrix
 */
Matrix::Matrix() {
	row = 0;
	col = 0;
	size = 0;
}

/**
 * @brief	constructor of Matrix
 * @param	const Matrix&, construct a matrix according to this matrix
 */
Matrix::Matrix(const Matrix& m) {
	row = m.row;
	col = m.col;
	size = row * col;
	data.assign(m.data.begin(), m.data.end());
}

/**
 * @brief	constructor of Matrix
 * @param	int, row
 * @param	int, column
 */ 
Matrix::Matrix(int r, int c) {
	row = r;
	col = c;
	size = row * col;
	data.assign(size, 0);
}

/**
 * @brief	constructor of Matrix
 * @param	int, row
 * @param	int, column
 * @param	const std::vector<double>, original data in the matrix
 */ 
Matrix::Matrix(int r, int c, const std::vector<double> newData) {
	row = r;
	col = c;
	size = row * col;
	if (size) data.assign(newData.begin(), newData.end());
}

/**
 * @brief	overload operator "="
 * @param	const Matrix&, the right matrix
 * @return	Matrix&, the Left matrix
 */ 
Matrix& Matrix::operator = (const Matrix& m) {
	row = m.row;
	col = m.col;
	size = m.size;
	data.assign(m.data.begin(), m.data.end());
	return *this;
}

/**
 * @brief	overload operator "=="
 * @param	const Matrix&, the matrix at right of the "=="
 * @return	bool, whether equal
 */ 
bool Matrix::operator == (const Matrix& m) const {
	if (this->row != m.row || this->col != m.col) {
		return false;
	}
	for (int i = 0; i < this->size; ++i) {
		if (this->data[i] != m.data[i]) return false;
	}
	return true;
}

/**
 * @brief	overload operator "+"
 * @param	const Matrix&, the matrix at right of the "+"
 */ 
const Matrix Matrix::operator + (const Matrix& m) const {
	if (this->row != m.row || this->col != m.col) {								//error Detection
		errorThrow("Addition with different sizes!");							
		return *this;
	}
	Matrix temp(*this);															//create a new matrix to do addition
	for (int i = 0; i < this->size; ++i) {
		temp.data[i] += m.data[i];
	}
	return temp;
}

/**
 * @brief	overload operator "+"
 * @param	double, increase
 */ 
const Matrix Matrix::operator + (double x) const {
	Matrix temp(*this);
	for (int i = 0; i < temp.size; ++i)
		temp.data[i] += x;														//add all elements in the matrix by x
	return temp;
}

/**
 * @brief	overload operator "+"
 * @param	double, increase
 * @param	Matrix&, the matrix at right of the "+"
 */
const Matrix operator + (double x, Matrix& m) {
	Matrix temp(m);
	for (int i = 0; i < m.GetRow(); ++i)
		for (int j = 0; j < m.GetCol(); ++j)
			temp.GetVal(i, j) += x;												//use GetVal() to Get elements in the matrix
	return temp;
}

/**
 * @brief	overload operator "+="
 * @param	const Matrix&, the right matrix
 * @return	Matrix&, the result matrix
 */ 
Matrix& Matrix::operator += (const Matrix& m) {
	if (this->row != m.row || this->col != m.col) {
		errorThrow("Addition with different sizes!");							//error Detection
		return *this;
	}
	for (int i = 0; i < this->size; ++i) {
		this->data[i] += m.data[i];
	}
	return *this;
}

/**
 * @brief	overload operator "+="
 * @param	double, increase
 * @return	Matrix&, the result matrix
 */ 
Matrix& Matrix::operator += (double x) {
	for (int i = 0; i < this->size; ++i)
		this->data[i] += x;
	return *this;
}

/**
 * @brief	overload operator "-"
 * @param	const Matrix&, the right matrix
 * @return	const Matrix, the Left matrix
 */ 
const Matrix Matrix::operator - (const Matrix& m) const {
	if (this->row != m.row || this->col != m.col) {								//error Detection
		errorThrow("Substraction with different sizes!");
		return *this;
	}
	Matrix temp(*this);
	for (int i = 0; i < this->size; ++i) {
		temp.data[i] -= m.data[i];
	}
	return temp;
}

/**
 * @brief	overload operator "-"
 * @param	double, decrease
 * @return	const Matrix, the Left matrix
 */ 
const Matrix Matrix::operator - (double x) const {
	Matrix temp(*this);
	for (int i = 0; i < temp.size; ++i)
		temp.data[i] -= x;
	return temp;
}

/**
 * @brief	overload operator "-="
 * @param	const Matrix&, the right matrix
 * @return	Matrix&, the result matrix
 */ 
Matrix& Matrix::operator -= (const Matrix& m) {
	if (this->row != m.row || this->col != m.col) {								//error Detection
		errorThrow("Substraction with different sizes!");
		return *this;
	}
	for (int i = 0; i < this->size; ++i) {
		this->data[i] -= m.data[i];
	}
	return *this;
}

/**
 * @brief	overload operator "-="
 * @param	double, decrease
 * @return	Matrix&, the result matrix
 */ 
Matrix& Matrix::operator -= (double x) {
	for (int i = 0; i < this->size; ++i)
		this->data[i] -= x;
	return *this;
}


/**
 * @brief	overload operator "*"
 * @param	const Matrix&, the right matrix
 * @return	const Matrix, the Left matrix
 */ 
const Matrix Matrix::operator * (const Matrix& m) const {
	if (this->col != m.row) {													//error Detection
		errorThrow("Multiplication with mismatch sizes!");
		return *this;
	}
	Matrix temp(this->row, m.col);
	for (int i = 0; i < this->row; ++i) {										//do matrix multiplication in O(n^3) 
		for (int j = 0; j < m.col; ++j) {
			for (int k = 0; k < this->col; ++k) {
				temp.data[i * temp.col + j] += this->data[i * this->col + k] * m.data[k * m.col + j];
			}
		}
	}
	return temp;
}

/**
 * @brief	overload operator "*"
 * @param	double, multiplier
 * @return	const Matrix, the Left matrix
 */ 
const Matrix Matrix::operator * (double x) const {
	Matrix temp(*this);
	for (int i = 0; i < temp.size; ++i)											//multiply each element in the matrix by x
		temp.data[i] *= x;
	return temp;
}

/**
 * @brief	overload operator "*"
 * @param	double, mutiplier
 * @param	Matrix&, the right matrix
 * @return	const Matrix, the result matrix
 */ 
const Matrix operator * (double x, Matrix& m) {
	Matrix temp(m);
	for (int i = 0; i < m.GetRow(); ++i) 										//multiply each element in the matrix by x
		for (int j = 0; j < m.GetCol(); ++j)
			temp.GetVal(i, j) *= x;
	return temp;
}

/**
 * @brief	overload operator "*="
 * @param	const Matrix&, the right matrix
 * @return	Matrix&, the result matrix
 */ 
Matrix& Matrix::operator *= (const Matrix& m) {
	if (this->col != m.row) {													//error Detection
		errorThrow("Multiplication with mismatch sizes!");
		return *this;
	}
	Matrix temp(this->row, m.col);
	for (int i = 0; i < this->row; ++i) {										//do matrix multiplication in O(n^3)
		for (int j = 0; j < m.col; ++j) {
			for (int k = 0; k < this->col; ++k) {
				temp.data[i * temp.col + j] += this->data[i * this->col + k] * m.data[k * m.col + j];
			}
		}
	}
	*this = temp;
	return *this;
}

/**
 * @brief	overload operator "*="
 * @param	double, multiplier
 * @return	Matrix&, the result matrix
 */ 
Matrix& Matrix::operator *= (double x) {
	for (int i = 0; i < this->size; ++i)
		this->data[i] *= x;														//multiply each element in the matrix by x
	return *this;
}

/**
 * @brief	overload operator "/"
 * @param	const Matrix&, divisor
 * @return	const Matrix, the result matrix
 */ 
const Matrix Matrix::operator / (const Matrix& m) const {
	if (m.col != m.row || this->col != m.row) {									//error Detection
		errorThrow("Division with mismatch sizes!");
		return *this;
	}
	Matrix dd(m);
	dd = dd.Inv();																//Get the Inverse matrix
	Matrix temp(this->row, m.col);
	temp = (*this) * dd;
	return temp;
}

/**
 * @brief	overload operator "/"
 * @param	doubel, divisor
 * @return	const Matrix, the result matrix
 */ 
const Matrix Matrix::operator / (double x) const {
	if (fabs(x) <= eps) {														//error Detection
		errorThrow("Divisor is too small!");
		return *this;
	}
	Matrix temp(*this);
	for (int i = 0; i < temp.size; ++i)
		temp.data[i] /= x;														//divide each element in the matrix by x
	return temp;
}

/**
 * @brief	overload operator "/="
 * @param	const Matrix&, the right matrix
 * @return	Matrix&, the result matrix	
 */ 
Matrix& Matrix::operator /= (const Matrix& m) {
	if (m.col != m.row || this->col != m.row) {									//error Detection
		errorThrow("Division with mismatch sizes!");
		return *this;
	}
	Matrix dd(m);
	dd = dd.Inv();																//Get the Inverse matrix
	*this = (*this) * dd;
	return *this;
}

/**
 * @brief	overload operator "/="
 * @param	double, divisor
 * @return	Matrix&, the result matrix
 */ 
Matrix& Matrix::operator /= (double x) {
	if (fabs(x) <= eps) {														//error Detection
		errorThrow("Divisor is too small!");
		return *this;
	}
	for (int i = 0; i < this->size; ++i)
		this->data[i] /= x;														//divide each element in the matrix by x
	return *this;
}

/**
 * @brief	Left division
 * @param	const Matrix&, the right matrix
 * @return	const Matrix, the result matrix
 */ 
const Matrix Matrix::LeftDiv(const Matrix& m) const {
	if (this->col != this->row || this->col != m.row) {							//error Detection
		errorThrow("Division with mismatch sizes!");
		return *this;
	}
	Matrix dd(*this);
	dd = dd.Inv();																//Get the Inverse matrix of the Left matrix
	Matrix temp(this->row, m.col);
	temp = dd * m;													
	return temp;
}

/**
 * @brief	power of matrix
 * @param	int, the power
 * @return	const Matrix, the result matrix
 */ 
const Matrix Matrix::operator ^ (int power) {
	if (row != col) {															//error Detection
		errorThrow("Not a square matrix!");
		return *this;
	}
	if (power == 0) return Matrix::Eye(row);									//when power==0, return identity matrix
	if (power < 0) return (*this).Inv() ^ (-power);								//when power<0, return Inv^(-power)
	Matrix base(*this);
	Matrix res(Matrix::Eye(row));
	while (power) {																//use QuickPow algorithm
		if (power & 1) res *= base;
		base *= base;
		power >>= 1;
	}
	return res;
}


/**
 * @brief	overload operator ">>"
 * @param	std::istream&, the input stream
 * @param	Matrix&, the tarGet matrix
 * @return	std::istream&, return input stream
 */ 
std::istream& operator >> (std::istream& input, Matrix& m) {
	for (int i = 0; i < m.size; ++i)
		input >> m.data[i];
	return input;
}

/*
std::ostream& operator << (std::ostream& output, const Matrix& m) {
	for (int i = 0; i < m.size; ++i) {
		output << std::fixed << std::setprecision(precision) << m.data[i] << " ";
		if ((i + 1) % m.col == 0) output << std::endl;
	}
	return output;
}
*/

/**
 * @brief	Get the maximum element in the matrix
 * @param	const Matrix, the matrix
 * @return	double, the result
 */ 
double Max(const Matrix m) {
	if (m.size == 0) {															//error Detection
		errorThrow("Empty matrix/vector!");
		return 0;
	}
	double mx = m.data[0];
	for (int i = 1; i < m.size; ++i) mx = mx > m.data[i] ? mx : m.data[i];		//compare to Get the maximum element
	return mx;
}

/**
 * @brief	Get the minimum element in the matrix
 * @param	const Matrix, the matrix
 * @return	double, the result
 */ 
double Min(const Matrix m) {
	if (m.size == 0) {															//error Detection
		errorThrow("Empty matrix/vector!");
		return 0;
	}
	double mn = m.data[0];
	for (int i = 1; i < m.size; ++i) mn = mn < m.data[i] ? mn : m.data[i];		//compare to Get the minimum element
	return mn;
}

/**
 * @brief	Get the Sum of all the elements in the matrix
 * @param	const Matrix, the matrix
 * @return	double, the result
 */ 
double Sum(const Matrix m) {
	if (m.size == 0) {															//error Detection
		errorThrow("Empty matrix/vector!");
		return 0;
	}
	double tmp = 0;
	for (int i = 0; i < m.size; ++i) tmp += m.data[i];							//count up
	return tmp;
}

/**
 * @brief	Get the Mean of all the elements in the matrix
 * @param	const Matrix, the matrix
 * @return	double, the result
 */ 
double Mean(const Matrix m) {
	double tmp = Sum(m);														//Get the Sum first
	return tmp / m.size;
}

/**
 * @brief	Get the Product of the elements in the matrix (by column/row)
 * @param	const Matrix, the input matrix
 * @param	int
 *			0, do the Product by row
 *			1, do the Product by column
 * @return	Matrix, the result matrix
 */
Matrix Prod(const Matrix m, int type = 1) {
	if (m.size == 0) {															//error Detection
		errorThrow("Empty matrix/vector!");
		return m;
	}
	if (m.row == 1 || m.col == 1) {												//vector
		double tmp = 1;
		for (int i = 0; i < m.size; ++i) tmp *= m.data[i];
		Matrix pd(1, 1);
		pd.data[0] = tmp;
		return pd;
	}
	if (type == 1) {															//do the Product by column
		Matrix pd(1, m.col);
		for (int j = 0; j < m.col; ++j) {
			double tmp = 1;
			for (int i = j; i < m.size; i += m.col) tmp *= m.data[i];
			pd.data[j] = tmp;
		}
		return pd;
	}
	else {																		//do the Product by row
		Matrix pd(m.row, 1);
		for (int i = 0; i < m.row; ++i) {
			double tmp = 1;
			for (int j = i * m.col; j < (i + 1) * m.col; ++j) tmp *= m.data[j];
			pd.data[i] = tmp;
		}
		return pd;
	}
}

/**
 * @brief	Sort the elements in the matrix
 * @param	Matrix&, the matrix
 */ 
void Sort(Matrix& m) {
	sort(m.data.begin(), m.data.end());
}

/**
 * @brief	Reshape the matrix with a new row and column
 * @param	int, new row number
 * @param	int, new column number
 * @return	Matrix, the result matrix
 */
Matrix Matrix::Reshape(int r, int c) {
	if (r * c != this->size) {													//error Detection
		errorThrow("Size will be changed after reshaping!");
	}
	Matrix temp(r, c, this->data);
	return temp;
}

/**
 * @brief	Get an identity matrix
 * @param	int, the size of the identity matrix
 * @return	Matrix, the identity matrix
 */
Matrix Matrix::Eye(int n) {
	Matrix e(n, n);
	for (int i = 0; i < n; ++i)
		e.GetVal(i, i) = 1;														//set value = 1 where row == column
	return e;
}

/**
 * @brief	Get the Transposed matrix
 * @return	Matrix, the result matrix
 */
Matrix Matrix::Trans(void) {
	Matrix t(col, row);
	for (int i = 0; i < col; ++i)
		for (int j = 0; j < row; ++j)
			t.GetVal(i, j) = GetVal(j, i);
	return t;
}

/**
 * @brief	swap row1 and row2 of the matrix
 * @param	int, row1
 * @param	int, row2
 */
void Matrix::SwapRow(int r1, int r2) {
	for (int j = 0; j < col; ++j)
		std::swap(GetVal(r1, j), GetVal(r2, j));
}

/**
 * @brief	matrix linear Transformation, Val(row1, k) += Val(row2, k) * times
 * @param	int, the tarGet row (row1)
 * @param	int, the source row (row2)
 * @param	double, the times
 */
void Matrix::LinearChange(int r1, int r2, double times) {
	for (int j = 0; j < col; ++j)
		GetVal(r1, j) += times * GetVal(r2, j);
}

/**
 * @brief	Legalify the matrix, making Val(row, row) != 0
 * @param	int, the tarGet row
 * @return	int 
 *			1, no need to SwapRow
 *			0, fail to Legalify
 *			-1, succeed to Legalify
 */
int Matrix::Legalify(int c) {
	if (GetVal(c, c) != 0) return 1;
	for (int i = c + 1; i < row; ++i) {
		if (GetVal(i, c) != 0) {
			SwapRow(i, c);
			return -1;
		}
	}
	return 0;
}

/**
 * @brief	calculate the Determinant of the matrix
 * @return	double, the result
 */
double Matrix::Det(void) {
	Matrix temp(*this);
	if (temp.row != temp.col) {													//error Detection
		errorThrow("No Determinant!");
		return 0;
	}
	double times, res = 1, flag;
	for (int j = 0; j < col - 1; ++j) {
		flag = temp.Legalify(j);
		if (flag == 0) return 0.0;												//if failed to Legalify the matrix, return 0
		res *= flag;															//record the parity of row-swap times
		for (int i = j + 1; i < row; ++i) {
			if (temp.GetVal(i, j) == 0) continue;
			times = -temp.GetVal(i, j) / temp.GetVal(j, j);
			temp.LinearChange(i, j, times);
		}
	}
	for (int i = 0; i < row; ++i)
		res *= temp.GetVal(i, i);
	return res;
}

/**
 * @brief	Get the upper triangle of the matrix
 * @return	Matrix, the result matrix
 */
Matrix Matrix::UpperTri(void) {
	double times;
	Matrix temp(*this);
	for (int j = 0; j < row - 1; ++j) {
		temp.Legalify(j);
		for (int i = j + 1; i < row; ++i) {
			if (temp.GetVal(i, j) == 0) continue;
			times = -temp.GetVal(i, j) / temp.GetVal(j, j);						//calculate the times
			temp.LinearChange(i, j, times);
		}
	}
	return temp;
}

/**
 * @brief	Get the Inverse of matrix
 * @param	the input matrix
 * @return	Matrix, the result matrix
 */
Matrix Matrix::Inv(void) {
	if (this->row != this->col || this->Det() == 0) {
		errorThrow("No Inverse matrix!");
		return *this;
	}
	Matrix AE(this->row, 2 * this->col);
	for (int i = 0; i < this->row; ++i) {										//construct the augmented matrix
		for (int j = 0; j < this->col; ++j) {
			AE.GetVal(i, j) = this->GetVal(i, j);
		}
	}
	for (int i = 0; i < this->row; ++i)
		AE.GetVal(i, this->row + i) = 1;
	AE = AE.UpperTri();
	double times;
	for (int j = this->col - 1; j > 0; --j) {									//do elimination
		for (int i = j - 1; i >= 0; --i) {
			times = -AE.GetVal(i, j) / AE.GetVal(j, j);
			AE.LinearChange(i, j, times);
		}
	}
	for (int i = 0; i < AE.GetRow(); ++i) {										//do elimination
		times = AE.GetVal(i, i);
		if (times == 1) continue;
		for (int j = 0; j < AE.GetCol(); ++j)
			AE.GetVal(i, j) /= times;
	}
	Matrix Invm(this->row, this->col);					
	for (int i = 0; i < this->row; ++i)											//Get the result
		for (int j = 0; j < this->col; ++j)
			Invm.GetVal(i, j) = AE.GetVal(i, j + this->col);
	return Invm;
}

/**
 * @brief	constructor of Vector
 */
Vec::Vec(void) {
	row = col = size = 0;
}

/**
 * @brief	constructor of Vector
 * @param	int s, construct a Vector with size = s
 */
Vec::Vec(int s) {
	row = 1;
	col = size = s;
	data.assign(size, 0);
}

/**
 * @brief	constructor of Vector
 * @param	const Vec& v, construct a Vector with Vector v
 */ 
Vec::Vec(const Vec& v) {
	row = 1;
	col = size = v.size;
	data.assign(v.data.begin(), v.data.end());
}

/**
 * @brief	constructor of Vector
 * @param	int, size
 * @param	const std::vector<double>, data
 */ 
Vec::Vec(int s, const std::vector<double> newData) {
	row = 1;
	col = size = s;
	if (size) data.assign(newData.begin(), newData.end());
}

/**
 * @brief	add data into the vector
 * @param	double, the data
 */ 
void Vec::AddData(double x) {
	size++;																		//increase the size
	row++;
	data.push_back(x);
}

/**
 * @brief	overload operator "="
 * @param	const Matrix&, the right matrix
 * @return	Vec&, the Left matrix
 */ 
Vec& Vec::operator = (const Matrix& m) {
	row = m.GetRow();
	col = m.GetCol();
	size = m.GetSize();
	data.assign(size, 0);
	for (int i = 0; i < size; ++i) data[i] = m.GetData(i);
	return *this;
}

/**
 * @brief	calculate 2-Norm (root of square Sum)
 * @param	const Vec, the input vector
 * @return	doubel, the result
 */
double Norm(const Vec v) {
	double tmp = 0;
	for (int i = 0; i < v.size; ++i) tmp += pow(v.data[i], 2);					//calculate the square Sum
	return sqrt(tmp);
}

/**
 * @brief	calculate the inner Product of the vector1 and vector2
 * @param	const Vec, vector1
 * @param	const Vec, vector2
 * @return	double, the result
 */
double Dot(const Vec v1, const Vec v2) {
	if (v1.size != v2.size) {													//error Detection
		errorThrow("The length of two vectors is different!");
		return 0;
	}
	double tmp = 0;
	for (int i = 0; i < v1.size; ++i) tmp += v1.data[i] * v2.data[i];
	return tmp;
}

/**
 * @brief	calculate the Cross Product of the vector1 and vector2 (for 3D vector)
 * @param	const Vec, vector1
 * @param	const Vec, vector2
 * @return	Vec, the result vector
 */
Vec Cross(const Vec v1, const Vec v2) {
	if (v1.size != 3 || v2.size != 3) {											//error Detection
		errorThrow("Not a 3-dimensional vector!");
		return v1;
	}
	Vec v(3);
	v.data[0] = v1.data[1] * v2.data[2] - v2.data[1] * v1.data[2];
	v.data[1] = v2.data[0] * v1.data[2] - v1.data[0] * v2.data[2];
	v.data[2] = v1.data[0] * v2.data[1] - v2.data[0] * v1.data[1];
	return v;
}

/**
 * @brief	the Sign function
 * @param	double, the input x
 * @return	int, the result
 */ 
int Sign(double x) {
	if (x < 0) return -1;
	return fabs(x) > eps;
}

const double EPS = 1e-9;

/**
 * @brief	simpson function
 * @param	std::function<double(double)>, the function to Integrate
 * @param	double, the lower bound
 * @param	double, the upper bound
 * @return	the result of simpson function
 */
double simpson(std::function<double(double)> f, double L, double R)
{
	double mid = L + (R - L) / 2;
	return (R - L) * (f(L) + f(R) + 4 * (f(mid))) / 6;
}

/**
 * @brief	Adaptive simpson integration.
 * @param	std::function<double(double)>, the function to Integrate
 * @param	double, the lower bound
 * @param	double, the upper bound
 * @return	the integration result
 */
double Integrate(std::function<double(double)> f, double L, double R)
{
	if (R < L)
		std::swap(L, R);
	double mid = (L + R) / 2;
	double S = simpson(f, L, R), Sl = simpson(f, L, mid), Sr = simpson(f, mid, R);
	if (fabs(Sl + Sr - S) < EPS)												//check if (Sl + Sr) is close enough to S
		return Sl + Sr;
	return Integrate(f, L, mid) + Integrate(f, mid, R);
}

const double INF = 1e9;
const int MAX_ROOT_NUM = 20;
const int MAX_ITERATION_TIMES = 100000;

/**
 * @brief	return a random number
 * @return	the random number
 */ 
int ran(void) {
	if (RAND_MAX > (int)INF) return rand();
	return rand() * RAND_MAX + rand();
}

/**
 * @brief	chord section method
 * @param	double, x0
 * @param	double, x1
 * @param	std::function<double(double)>, the function
 * @return	the possible solution
 */ 
double chordCut(double x0, double x1, std::function<double(double)> f) {
	double L = std::min(x0, x1), R = std::max(x0, x1);
	double f0 = f(x0), f1 = f(x1), x2 = x0;
	int count = 0;
	while (fabs(x1 - x0) > EPS) {
		if (fabs(f1 - f0) < EPS) break;
		x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
		if (x2 < L || x2 > R) break;
		x0 = x1, x1 = x2;
#ifdef LOCAL_DEBUG_
		printf("%.6lf, %.6lf\n", x0, x1);
#endif
		f0 = f1, f1 = f(x1);
		if (++count > MAX_ITERATION_TIMES) break;
	}
	return x2;
}

std::vector<double> findLRM(double L, double R, double m1, int p1, double m2, int p2, std::function<double(double)> f, bool& flag);

/**
 * @brief	find one or more solution in the range(L,R)
 * @param	double L, the lower bound
 * @param	double R, the upper bound
 * @param	std::function<double(double)>, the function
 * @param	bool&, whether the solution number is enough
 * @return	std::vector<double>, the solution vector
 */ 
std::vector<double> findRoot(double L, double R, std::function<double(double)> f, bool& flag) {
	if (flag || R - L < 1) {
		std::vector<double> v(0);
		return v;
	}
	double x0 = (ran() % (int)(R - L + 1)) + L;
	double f0 = f(x0);
	if (fabs(f0) < EPS) return findLRM(L, R, x0, 1, x0, 0, f, flag);
	double x1 = (ran() % (int)(R - L + 1)) + L;
	if (fabs(x0 - x1) < eps) x1 = x0 + eps * 10;
	double f1 = f(x1);
	if (fabs(f1) < EPS) return findLRM(L, R, x1, 1, x1, 0, f, flag);
	double x2 = chordCut(x0, x1, f);
	if (x2 < L) x2 = L; if (x2 > R) x2 = R;
	double f2 = f(x2);
#ifdef LOCAL_DEBUG_
	printf("!%.8lf:%.8lf\n", x2, f2);
#endif
	if (Sign(f0) != Sign(f1)) {
		if (fabs(f2) < EPS && x2 > std::min(x0, x1) && x2 < std::max(x0, x1))
			return findLRM(L, R, x2, 1, x2, 0, f, flag);
	}
	if (Sign(x2 - x1) != Sign(x2 - x0)) {
		if (fabs(f2) < EPS) return findLRM(L, R, x2, 1, x2, 0, f, flag);
		else {
			std::vector<double> v(0);
			return v;
		}
	}
	else if (x2 < std::min(x0, x1) || fabs(x2 - std::min(x0, x1)) < eps) {
		if (x2 < L) return findRoot(std::max(x0, x1), R, f, flag);
		if (fabs(f2) < EPS) return findLRM(L, R, x2, 1, std::max(x0, x1), 0, f, flag);
		return findLRM(L, R, x2, 0, std::max(x0, x1), 0, f, flag);
	}
	else {
		if (x2 > R) return findRoot(L, std::min(x0, x1), f, flag);
		if (fabs(f2) < EPS) return findLRM(L, R, std::min(x0, x1), 0, x2, 1, f, flag);
		return findLRM(L, R, std::min(x0, x1), 0, x2, 0, f, flag);
	}
}

/**
 * @brief	check the possible interval
 * @param	double L, the lower bound
 * @param	double R, the upper bound
 * @param	double m1
 * @param	int p1
 * @param	double m2
 * @param	int p2
 * @param	std::function<double(double)>, the function
 * @param	bool&, whether the solution number is enough
 * @return	solution vector
 */
std::vector<double> findLRM(double L, double R, double m1, int p1, double m2, int p2, std::function<double(double)> f, bool& flag) {
	if (flag) {
		std::vector<double> v(0);
		return v;
	}
	std::vector<double> v1 = findRoot(L, m1 - EPS, f, flag);
	std::vector<double> v2 = findRoot(m2 + EPS, R, f, flag);
	if (p1) v1.push_back(m1);
	if (p2) v1.push_back(m2);
#ifdef LOCAL_DEBUG_
	if (p1) std::cout << "@" << m1 << std::endl;
	if (p2) std::cout << "@" << m2 << std::endl;
#endif
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	std::vector<double> v3(v1.size() + v2.size());
	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	if (v3.size()) {
		std::vector<double>::iterator i, j;
		for (i = (j = v3.begin())++; j != v3.end(); ++j) {
			if (fabs(*i - *j) > eps) *(++i) = *j;
		}
		v3.erase(i + 1, v3.end());
	}
	if (v3.size() > MAX_ROOT_NUM) flag = 1;
	return v3;
}

/**
 * @brief	solve equation.
 *			When (L, R) contains values that may make function(x) nonexistent, there will be errors.
 *			As a result, it is better to set the appropriate (L, R).
 * @param	std::function<double(double)>, equation: function(x) = 0
 * @param	double L, the lower bound of the solution
 * @param	double R, the upper bound of the solution
 * @return	Vec, the solution vector
 */
Vec EquationSolve(std::function<double(double)> f, double L = -INF, double R = INF) {
	srand(time(NULL));
	std::vector<double> ans;
	for (int i = 1; i <= 100; ++i) {
		bool flag = 0;
		std::vector<double> tmp = findRoot(L, R, f, flag);						//find solution
		ans.insert(ans.end(), tmp.begin(), tmp.end());							//insert into the answer vector
	}
	sort(ans.begin(), ans.end());
	if (ans.size()) {
		std::vector<double>::iterator i, j;										
		for (i = (j = ans.begin())++; j != ans.end(); ++j) {					//duplicate removal
			if (fabs(*i - *j) > eps) *(++i) = *j;
		}
		ans.erase(i + 1, ans.end());
	}
	Vec v(ans.size(), ans);
	return v;
}

#ifdef LOCAL_TEST_
double f1(double x) {
	return x * (x - 1)*(x - 2)*(x - 3)*(x - 4)*(x - 6.66)*(x - 6.90)*(x - 7)*(x - 7.01);
}


int main() { 
	try {
		Vec ans = EquationSolve(f1, 0, 100);
		std::cout << ans;
		Matrix m1(1, 2);
		Matrix m2(2, 3);
		std::cin >> m1 >> m2;
		std::cout << m1 + m2;
	}
	catch (std::string errMsg) {
		std::cout << "SYNTAX ERROR: " << errMsg << std::endl;
	}
	return 0;
}
#endif