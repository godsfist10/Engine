//======================================================================
//File: Matrix.cpp
//Author: Matt Struble
//Purpose: Matrix storage and operations
//======================================================================
#ifndef MATRIX_H
#define MATRIX_H
//======================================================================
class Vector3D;
//======================================================================

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
	float Get(const int& row, const int& column) const;

	Matrix operator+(const Matrix&) const;
	Matrix operator-(const Matrix&) const;
	Matrix operator*(const Matrix&) const;
	Matrix operator*(const float&) const;
	Matrix operator*(const Vector3D&) const;
	Matrix& operator=(const Matrix&);
	Matrix& operator*=(const Matrix&);
	Matrix& operator*=(const float&);
	Matrix& operator*=(const Vector3D&);
	Matrix& operator+=(const Matrix&);
	Matrix& operator-=(const Matrix&);
	bool operator==(const Matrix&) const;
	bool operator!=(const Matrix&) const;

	Matrix CombinedMatrix(const Matrix& matrix2, bool add) const;

	Matrix Transpose() const;
	Matrix Inverse() const;
	float Determinant() const;

	static Matrix Identity(int r, int c);

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
//======================================================================