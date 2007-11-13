// --------------------------------------------------------
// Matrix3D Utility Functions
// --------------------------------------------------------
// Part of the AbstractModel package, (c) Neil Forrest 2007
//
// A set of simple matrix functions for use with the
// AbstractModel package.

#include "stdafx.h"
#include "matrix3d.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// Simple 4x4 matrix operations
// ----------------------------

// A matix is represented by a 16 element array of floats
// arranged in the same order as in OpenGL

// Multiply matrix A with B putting the result into C
// --------------------------------------------------
void MatrixMultiply ( float* A, float* B, float* C )
{
	C[0] = A[0]*B[0]  +  A[4]*B[1] +  A[8]*B[2]   +  A[12]*B[3];
	C[1] = A[1]*B[0]  +  A[5]*B[1] +  A[9]*B[2]   +  A[13]*B[3];
	C[2] = A[2]*B[0]  +  A[6]*B[1] +  A[10]*B[2]  +  A[14]*B[3];
	C[3] = A[3]*B[0]  +  A[7]*B[1] +  A[11]*B[2]  +  A[15]*B[3];
	C[4] = A[0]*B[4]  +  A[4]*B[5] +  A[8]*B[6]   +  A[12]*B[7];
	C[5] = A[1]*B[4]  +  A[5]*B[5] +  A[9]*B[6]   +  A[13]*B[7];
	C[6] = A[2]*B[4]  +  A[6]*B[5] +  A[10]*B[6]  +  A[14]*B[7];
	C[7] = A[3]*B[4]  +  A[7]*B[5] +  A[11]*B[6]  +  A[15]*B[7];
	C[8] = A[0]*B[8]  +  A[4]*B[9] +  A[8]*B[10]  + A[12]*B[11];
	C[9] = A[1]*B[8]  +  A[5]*B[9] +  A[9]*B[10]  + A[13]*B[11];
	C[10]= A[2]*B[8]  +  A[6]*B[9] +  A[10]*B[10] + A[14]*B[11];
	C[11]= A[3]*B[8]  +  A[7]*B[9] +  A[11]*B[10] + A[15]*B[11];
	C[12]= A[0]*B[12] + A[4]*B[13] + A[8]*B[14]   + A[12]*B[15];
	C[13]= A[1]*B[12] + A[5]*B[13] + A[9]*B[14]   + A[13]*B[15];
	C[14]= A[2]*B[12] + A[6]*B[13] + A[10]*B[14]  + A[14]*B[15];
	C[15]= A[3]*B[12] + A[7]*B[13] + A[11]*B[14]  + A[15]*B[15];
}

// Transform the vertex by the given matrix leaving the reult in result
void MatrixVertexMultiply(float* mat, float* vert, float* result) 
{

  result[0] = vert[0] * mat[0] +
              vert[1] * mat[4] +
              vert[2] * mat[8] +
                        mat[12];

  result[1] = vert[0] * mat[1] +
              vert[1] * mat[5] +
              vert[2] * mat[9] +
                        mat[13];

  result[2] = vert[0] * mat[2] +
              vert[1] * mat[6] +
              vert[2] * mat[10] +
                        mat[14];
} 

// Copy matrix A into matrix B
// ---------------------------
void MatrixCopy ( float* A, float* B )
{
	for(int i= 0; i < 16; i++)
	{
		B[i]= A[i];
	}
}

// Make matrix A the identity
// --------------------------
void MatrixIdentity ( float* A )
{
	// Column major?
	A[0]= 1; A[4]= 0;  A[8]= 0; A[12]= 0;
	A[1]= 0; A[5]= 1;  A[9]= 0; A[13]= 0;
	A[2]= 0; A[6]= 0; A[10]= 1; A[14]= 0;
	A[3]= 0; A[7]= 0; A[11]= 0; A[15]= 1;
}

// Invert the given matrix
// -----------------------
void MatrixInvert ( float* matrix, float* result )
{
	result[0]= matrix[0]; result[4]= matrix[1];  result[8]= matrix[2]; result[12]= matrix[3];
	result[1]= matrix[4]; result[5]= matrix[5];  result[9]= matrix[6]; result[13]= matrix[7];
	result[2]= matrix[8]; result[6]= matrix[9]; result[10]= matrix[10]; result[14]= matrix[11];
	result[3]= matrix[12]; result[7]= matrix[13]; result[11]= matrix[14]; result[15]= matrix[15];
}

// Make matrix A a rotation matrix given the rotations about x, y, z
// -----------------------------------------------------------------
void MatrixRotation ( float* A, float x, float y, float z )
{
	MatrixIdentity(A); // make identity

	if( x != 0 ) // if rotation about X
	{
		float rotX[16]; float C[16];

		// make x rotation
		rotX[0]= 1; rotX[4]= 0;                 rotX[8]= 0;                 rotX[12]= 0;
		rotX[1]= 0; rotX[5]= (float)cos(x);     rotX[9]= (float)-sin(x);    rotX[13]= 0;
		rotX[2]= 0; rotX[6]= (float)sin(x);     rotX[10]= (float)cos(x);    rotX[14]= 0;
		rotX[3]= 0; rotX[7]= 0;                 rotX[11]= 0;                rotX[15]= 1;

		// multiply in x rotation
		MatrixMultiply(rotX, A, C);

		// copy back into A
		MatrixCopy(C, A);
	}

	if( y != 0 ) // if rotation about Y
	{
		float rotY[16]; float C[16];

		// make y rotation
		rotY[0]= (float)cos(y);  rotY[4]= 0;      rotY[8]= (float)sin(y);    rotY[12]= 0;
		rotY[1]= 0;				 rotY[5]= 1;      rotY[9]= 0;                rotY[13]= 0;
		rotY[2]= (float)-sin(y); rotY[6]= 0;     rotY[10]= (float)cos(y);    rotY[14]= 0;
		rotY[3]= 0;              rotY[7]= 0;     rotY[11]= 0;                rotY[15]= 1;

		// multiply in y rotation
		MatrixMultiply(rotY, A, C);

		// copy back into A
		MatrixCopy(C, A);
	}

	if( z != 0 ) // if rotation about Z
	{
		float rotZ[16]; float C[16];

		// make z rotation
		rotZ[0]= (float)cos(z); rotZ[4]= (float)-sin(z);    rotZ[8]=  0;    rotZ[12]= 0;
		rotZ[1]= (float)sin(z); rotZ[5]= (float)cos(z);     rotZ[9]=  0;    rotZ[13]= 0;
		rotZ[2]= 0;             rotZ[6]= 0;                 rotZ[10]= 1;    rotZ[14]= 0;
		rotZ[3]= 0;             rotZ[7]= 0;                 rotZ[11]= 0;    rotZ[15]= 1;

		// multiply in z rotation
		MatrixMultiply(rotZ, A, C);

		// copy back into A
		MatrixCopy(C, A);
	}
}

// Make matrix A a translation matrix using vector x, y, z
// -------------------------------------------------------
void MatrixTranslation ( float* A, float x, float y, float z )
{
	A[0]= 1; A[4]= 0;   A[8]= 0;  A[12]= x;
	A[1]= 0; A[5]= 1;   A[9]= 0;  A[13]= y;
	A[2]= 0; A[6]= 0;  A[10]= 1;  A[14]= z;
	A[3]= 0; A[7]= 0;  A[11]= 0;  A[15]= 1;
}

// Make matrix A a scalling matrix using vector x, y, z
// ----------------------------------------------------
void MatrixScalling ( float* A, float x, float y, float z )
{
	A[0]= x; A[4]= 0;  A[8]= 0; A[12]= 0;
	A[1]= 0; A[5]= y;  A[9]= 0; A[13]= 0;
	A[2]= 0; A[6]= 0; A[10]= z; A[14]= 0;
	A[3]= 0; A[7]= 0; A[11]= 0; A[15]= 1;
}


// Normalize the given vector
// (and return its length as a by product which might be handy)
// --------------------------
float VectorNormalize ( float* vector )
{
	float length= VectorLength ( vector );

	// Do not div by zero
	if ( length != 0 )
	{
		vector [0]/= length;
		vector [1]/= length;
		vector [2]/= length;
	}
	else
	{
		MessageBox ( 0, "Div by 0", "", MB_OK );
	}

	return length; // (and return its length as a by product which might be handy)
}

// Get the length / magnitude of the specified vector
// --------------------------------------------------
float VectorLength ( float* vector )
{
	return (float) sqrt ( pow ( vector [0], 2 ) +
		                  pow ( vector [1], 2 ) +
				          pow ( vector [2], 2 ) );
}