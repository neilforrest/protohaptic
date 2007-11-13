#include "stdafx.h"
#include "Geometry.h"

#define SMALL_NUM  0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)  // perp product (2D)

// intersect3D_SegmentPlane(): intersect a segment and a plane
//    Input:  S = a segment, and Pn = a plane = {Point V0; Vector n;}
//    Output: *I0 = the intersect point (when it exists)
//    Return: 0 = disjoint (no intersection)
//            1 = intersection in the unique point *I0
//            2 = the segment lies in the plane
int intersect3D_SegmentPlane( Segment S, Plane Pn, Point* I )
{
    Point    u = S.P1 - S.P0;
    Vector    w = S.P0 - Pn.V0;

    float     D = dot(Pn.n, u);
    float     N = -dot(Pn.n, w);

    if (fabs(D) < SMALL_NUM) {          // segment is parallel to plane
        if (N == 0)                     // segment lies in plane
            return 2;
        else
            return 0;                   // no intersection
    }
    // they are not parallel
    // compute intersect param
    float sI = N / D;
    if (sI < 0 || sI > 1)
        return 0;                       // no intersection

	u.x= u.x * sI;
	u.y= u.y * sI;
	u.z= u.z * sI;

    *I = S.P0 + u;                 // compute segment intersect point
    return 1;
}

// Calculate the cross product of two vectors
// ------------------------------------------
void crossProduct (float *c,float a[3], float b[3])
{  
  c[0]=a[1]*b[2] - b[1]*a[2];
  c[1]=a[2]*b[0] - b[2]*a[0];
  c[2]=a[0]*b[1] - b[0]*a[1];
}

// Normalise a vector
// ------------------
void normalize (float *v)
{
	// Calculate length= square root ( A^2+B^2+C^2 )
	float length= sqrt ( pow( v[0], 2 ) +
		                 pow( v[1], 2 ) +
						 pow( v[2], 2 ) );

	// Guard against div by 0
	//ASSERT ( length != 0 );
	if ( length == 0 ) return;

	// Divide components by length
	v[0]/= length;
	v[1]/= length;
	v[2]/= length;
}

// Calculate the normal to a surface, given
// three points on its surface
// ----------------------------------------
void getSurfaceNormal ( float *normal,
					    float *pointA,
						float *pointB,
						float *pointC )
{
	// Use B as origin for two vectors: B-A and B-C on the surface

	// Define vector B-A
	float vectorBA[3]= { pointA[0] - pointB[0],
						 pointA[1] - pointB[1],
						 pointA[2] - pointB[2] };

	// Define vector C-A
	float vectorCA[3]= { pointA[0] - pointC[0],
						 pointA[1] - pointC[1],
						 pointA[2] - pointC[2] };

	// Calculate the cross product of vectors
	crossProduct ( normal, vectorBA, vectorCA );

	// Normalise the cross product
	normalize ( normal );
}

// Get projection matrix for surface defined by normal
hduMatrix getProjectionMatrix ( float* n )
{
	float a= n[0];
	float b= n[1];
	float c= n[2];

	return hduMatrix ( b*b + c*c,     -a*b,    -a*c, 0,
					   -b*a,     a*a + c*c,    -b*c, 0,
					   -c*a,          -c*b, a*a+b*b, 0,
						0,				0,		  0, 1 );
}

// Get hdu matrix from array[16]
hduMatrix getHduMatrix ( float* matrix )
{
	// Define the items rotation as an hduMatrix
	return hduMatrix  
			( matrix[0],  matrix[1],  matrix[2],  matrix[3], 
			  matrix[4],  matrix[5],  matrix[6],  matrix[7], 
			  matrix[8],  matrix[9],  matrix[10], matrix[11], 
			  matrix[12], matrix[13], matrix[14], matrix[15] );
}

// Project a point onto a positioned surface
void getProjectionOnPlane ( float* normalOfPlane, // in
						    float* pointOnPlane, // in
						    float* originalPoint, // in
							float* projectedPoint ) // out
{
	// Get projection matrix for plane with point on origin
	hduMatrix proj= getProjectionMatrix ( normalOfPlane );

	// Get matrix to translate system to origin
	hduMatrix toOrigin= hduMatrix::createTranslation ( -pointOnPlane[0],
													   -pointOnPlane[1],
													   -pointOnPlane[2] );

	// Translate the point to project to the origin
	hduVector3Dd originalPointV ( originalPoint[0], originalPoint[1], originalPoint[2] );
	hduVector3Dd originalPointOriginV;

	toOrigin.multVecMatrix ( originalPointV, originalPointOriginV );

	// Project the transformed point onto the plane centred at the origin
	hduVector3Dd out;

	proj.multVecMatrix ( originalPointOriginV, out );

	// Move back from origin
	hduMatrix invToOrigin ( toOrigin );
	invToOrigin.invert ();

	hduVector3Dd final;
	invToOrigin.multVecMatrix ( out, final );

	// Copy out answer
	projectedPoint[0]= final[0];
	projectedPoint[1]= final[1];
	projectedPoint[2]= final[2];
}