#pragma once

#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>

class Vector
{
public:
	float x, y, z;

	Vector ( ) { }

	Vector ( float a, float b, float c )
	{
		x= a; y= b; z= c;
	}

   //// Define conversion
   //operator Point () {
   //   return Point ( x, y, z ); 
   //}

	// ob + int.
   Vector operator+(Vector ob0 )
	{
	  //myClass temp;

	  //temp.count = ob.count + i;
		return Vector ( this->x + ob0.x,
			           this->y + ob0.y,
					   this->z + ob0.z );
	}

	// ob + int.
	Vector operator-(Vector ob0 )
	{
	  //myClass temp;

	  //temp.count = ob.count + i;
		return Vector ( this->x - ob0.x,
			           this->y - ob0.y,
					   this->z - ob0.z );
	}

	// ob + int.
	Vector operator*(float s )
	{
	  //myClass temp;

	  //temp.count = ob.count + i;
		return Vector ( this->x*s,
			           this->y*s,
					   this->z*s );
	}
};

class Point
{
public:

	float x, y, z;

	Point ( ) { }

	Point ( float a, float b, float c )
	{
		x= a; y= b; z= c;
	}

   // Define conversion
   operator Vector () {
      return Vector ( x, y, z ); 
   }

	// ob + int.
	Point operator+(Point ob0 )
	{
	  //myClass temp;

	  //temp.count = ob.count + i;
		return Point ( this->x + ob0.x,
			           this->y + ob0.y,
					   this->z + ob0.z );
	}

	// ob + int.
	Point operator-(Point ob0 )
	{
	  //myClass temp;

	  //temp.count = ob.count + i;
		return Point ( this->x - ob0.x,
			           this->y - ob0.y,
					   this->z - ob0.z );
	}

	// ob + int.
	Point operator*(float s )
	{
	  //myClass temp;

	  //temp.count = ob.count + i;
		return Point ( this->x*s,
			           this->y*s,
					   this->z*s );
	}

	// ob + int.
	Point operator+(float s )
	{
	  //myClass temp;

	  //temp.count = ob.count + i;
		return Point ( this->x+s,
			           this->y+s,
					   this->z+s );
	}
};

class Plane
{
public: 

	Plane ( ) { }

	Point V0;
	Vector n;
};

class Segment 
{
public: 

	Segment ( ) { }

	Point P0;
	Point P1;
};

// intersect3D_SegmentPlane(): intersect a segment and a plane
//    Input:  S = a segment, and Pn = a plane = {Point V0; Vector n;}
//    Output: *I0 = the intersect point (when it exists)
//    Return: 0 = disjoint (no intersection)
//            1 = intersection in the unique point *I0
//            2 = the segment lies in the plane
int intersect3D_SegmentPlane( Segment S, Plane Pn, Point* I );

// Calculate the cross product of two vectors
// ------------------------------------------
void crossProduct (float *c,float a[3], float b[3]);

// Normalise a vector
// ------------------
void normalize (float *v);

// Calculate the normal to a surface, given
// three points on its surface
// ----------------------------------------
void getSurfaceNormal ( float *normal,
					    float *pointA,
						float *pointB,
						float *pointC );

// Get projection matrix for surface defined by normal
// ---------------------------------------------------
hduMatrix getProjectionMatrix ( float* n );

// Project a point onto a positioned surface
// -----------------------------------------
void getProjectionOnPlane ( float* normalOfPlane, // in
						    float* pointOnPlane, // in
						    float* originalPoint, // in
							float* projectedPoint ); // out

// Get hdu matrix from array[16]
// -----------------------------
hduMatrix getHduMatrix ( float* matrix );