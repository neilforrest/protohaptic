// --------------------------------------------------------
// Matrix3D Utility Functions
// --------------------------------------------------------
// Part of the AbstractModel package, (c) Neil Forrest 2007
//
// A set of simple matrix functions for use with the
// AbstractModel package.

#if !defined(AFX_MATRIX3D_H__D765BFF7_C87F_4DCD_A0CF_8F702D3FA722__INCLUDED_)
#define AFX_MATRIX3D_H__D765BFF7_C87F_4DCD_A0CF_8F702D3FA722__INCLUDED_

#ifndef AM_API
#ifdef IMPORTING
#define AM_API __declspec(dllimport)
#else
#define AM_API __declspec(dllexport)
#endif
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Simple 4x4 matrix operations
// ----------------------------

// A matix is represented by a 16 element array of floats
// arranged in the same order as in OpenGL

// Multiply matrix A with B putting the result into C
void MatrixMultiply ( float* A, float* B, float* C );

// Transform the vertex by the given matrix leaving the reult in result
void MatrixVertexMultiply(float* mat, float* vert, float* result);

// Copy matrix A into matrix B
void MatrixCopy ( float* A, float* B );

// Make matrix A the identity
void MatrixIdentity ( float* A );

// Make matrix A a rotation matrix given the rotations about x, y, z
void MatrixRotation ( float* A, float x, float y, float z );

// Make matrix A a translation matrix using vector x, y, z
void MatrixTranslation ( float* A, float x, float y, float z );

// Make matrix A a scalling matrix using vector x, y, z
void MatrixScalling ( float* A, float x, float y, float z );

// Invert the given matrix
void MatrixInvert ( float* matrix, float* result );

// Normalize the given vector
// (and return its length as a by product which might be handy)
float VectorNormalize ( float* vector );

// Get the length / magnitude of the specified vector
float VectorLength ( float* vector );

#endif // !defined(AFX_MATRIX3D_H__D765BFF7_C87F_4DCD_A0CF_8F702D3FA722__INCLUDED_)
