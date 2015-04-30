#ifndef MATRIX_H
#define MATRIX_H

class Vector3D;

class Matrix
{
public:
	Matrix();
	Matrix(const Matrix& rhs);
	Matrix(int rows, int columns);
	Matrix(int length, bool identiy);
	Matrix(int rows, int columns, float* matrixValues);
	~Matrix();

	void  Set(int row, int column, float value);
	void Set(int index, float val);
	void Set(Vector3D a, Vector3D b, Vector3D c);
	float Get(const int& row, const int& column) const;
	float Get(const int& index) const;

	Matrix operator+(const Matrix&) const;
	Matrix operator-(const Matrix&) const;
	Matrix operator*(const Matrix&) const;
	Matrix operator*(const float&) const;
	Vector3D operator*(const Vector3D&) const;
	Matrix& operator=(const Matrix&);
	Matrix& operator*=(const Matrix&);
	Matrix& operator*=(const float&);
	Matrix& operator+=(const Matrix&);
	Matrix& operator-=(const Matrix&);
	bool operator==(const Matrix&) const;
	bool operator!=(const Matrix&) const;

	Matrix CombinedMatrix(const Matrix& matrix2, bool add) const;

	Matrix Transpose() const;
	Matrix Inverse() const;
	float Determinant() const;

	static Matrix Identity(int r, int c);

	Vector3D Transform(const Vector3D &vector);
	Vector3D TransformInverse(const Vector3D &vector);
	Vector3D GetAxisVector(unsigned int index) const;

	inline int GetNumRows() const { return m_NumRows; };
	inline int GetNumColumns() const { return m_NumColumns; };
	inline bool SameSize(const Matrix& rhs) const { return rhs.GetNumRows() == m_NumRows && rhs.GetNumColumns() == m_NumColumns; };

private:
	void initalizeMatrix(int rows, int columns);

private:
	float* mp_Matrix;
	int  m_NumRows;
	int  m_NumColumns;
	int m_Size;
};
#endif