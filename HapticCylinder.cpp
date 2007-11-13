#include "stdafx.h"
#include "HapticCylinder.h"
#include "Geometry.h"

#include <hdu/hduMatrix.h>
#include <iostream>

CHapticCylinder::CHapticCylinder(CShape* s)
{
	m_phCylinder= s;
	m_radius= 0.5;
}

CHapticCylinder::~CHapticCylinder(void)
{
}

/******************************************************************************
 HapticSphere::intersectSurface
 Intersects the line segment from startPt to endPt with the sphere. 
 Returns the closest point of intersection to the start point in intersectionPt.
 Returns the surface normal at intersectionPt in intersectionNormal.
 Returns which face (HL_FRONT or HL_BACK) is being touched in face.
 Returns true if an intersection occured.
******************************************************************************/
bool CHapticCylinder::intersectSurface(const HLdouble startPt[3], 
                                    const HLdouble endPt[3],
                                    HLdouble intersectionPt[3], 
                                    HLdouble intersectionNormal[3],
                                    HLenum *face,
                                    void *userdata)
{


 //   CHapticCylinder *pThis = static_cast<CHapticCylinder *>(userdata);    

 //   hduVector3Dd startPtV(startPt);
 //   hduVector3Dd endPtV(endPt);
 //   hduVector3Dd intersectionPtV;

	//// Undo translation
	//startPtV[0]-= pThis->m_phCylinder->getLocationX ();
	//startPtV[1]-= pThis->m_phCylinder->getLocationY ();
	//startPtV[2]-= pThis->m_phCylinder->getLocationZ ();

	//endPtV[0]-= pThis->m_phCylinder->getLocationX ();
	//endPtV[1]-= pThis->m_phCylinder->getLocationY ();
	//endPtV[2]-= pThis->m_phCylinder->getLocationZ ();

	//// Transform start and end points by inv of object transformation
	//// Add current items transform to parent transform of children
	//float* transform= pThis->m_phCylinder->getRotation ( );

	//// Define the items transform as an hduMatrix
	//hduMatrix rotation 
	//		( transform[0],  transform[1],  transform[2],  transform[3], 
	//		  transform[4],  transform[5],  transform[6],  transform[7], 
	//		  transform[8],  transform[9],  transform[10], transform[11], 
	//		  transform[12], transform[13], transform[14], transform[15] );

	//hduMatrix invRot= rotation.getInverse ();

	//// undo rotation
	//hduVector3Dd tmp;
	//invRot.multVecMatrix ( hduVector3Dd ( startPtV ), tmp );
	//startPtV= tmp;

	//invRot.multVecMatrix ( hduVector3Dd ( endPtV ), tmp );
	//endPtV= tmp;


	//// Undo scale
	//startPtV[0]/= pThis->m_phCylinder->getSizeX ();
	//startPtV[1]/= pThis->m_phCylinder->getSizeY ();
	//startPtV[2]/= pThis->m_phCylinder->getSizeZ ();

	//endPtV[0]/= pThis->m_phCylinder->getSizeX ();
	//endPtV[1]/= pThis->m_phCylinder->getSizeY ();
	//endPtV[2]/= pThis->m_phCylinder->getSizeZ ();


 //   bool bFoundIntersection = false;
 //   bool bStartInside = pThis->isInside(startPtV);
 //   bool bEndInside = pThis->isInside(endPtV);

 //   // Don't proceed if the start and end are on the same side of the surface.
 //   if (bStartInside == bEndInside)
 //       return false;

 //   if (bEndInside && !bStartInside)
 //   {        
 //       // Intersect sphere from outside using start->end segment.
 //       bFoundIntersection = pThis->intersectSegmentOutIn(
 //           startPtV, endPtV, intersectionPtV);
 //       *face = HL_FRONT;
 //   }
 //   else
 //   {
 //       // Intersect sphere from inside using end->start segment.
 //       bFoundIntersection = pThis->intersectSegmentOutIn(
 //           endPtV, startPtV, intersectionPtV);
 //       *face = HL_BACK;
 //   }

 //   if (bFoundIntersection)
 //   {
 //       hduVector3Dd intersectionNormalV = intersectionPtV;
	//	intersectionNormalV[2]= 0;

	//	intersectionNormalV[0]/= pThis->m_phCylinder->getSizeX ();
	//	intersectionNormalV[1]/= pThis->m_phCylinder->getSizeY ();
	//	intersectionNormalV[2]/= pThis->m_phCylinder->getSizeZ ();

 //       intersectionNormalV.normalize();

	//	rotation.multVecMatrix ( hduVector3Dd ( intersectionNormalV ), tmp );
	//	intersectionNormalV= tmp;

 //       // If this is back face intersection, then reverse the normal.
 //       if (*face == HL_BACK)
 //       {
 //           intersectionNormalV *= -1;
 //       }

 //       intersectionPt[0] = intersectionPtV[0];
 //       intersectionPt[1] = intersectionPtV[1];
 //       intersectionPt[2] = intersectionPtV[2];

 //       intersectionNormal[0] = intersectionNormalV[0];
 //       intersectionNormal[1] = intersectionNormalV[1];
 //       intersectionNormal[2] = intersectionNormalV[2];        

	//	// Apply shapes transform to intersection point

	//	// Scale
	//	intersectionPt[0]*= pThis->m_phCylinder->getSizeX ();
	//	intersectionPt[1]*= pThis->m_phCylinder->getSizeY ();
	//	intersectionPt[2]*= pThis->m_phCylinder->getSizeZ ();

	//	// Rotate
	//	rotation.multVecMatrix ( hduVector3Dd ( intersectionPt[0], intersectionPt[1], intersectionPt[2] ), tmp );
	//	intersectionPt[0]= tmp[0];
	//	intersectionPt[1]= tmp[1];
	//	intersectionPt[2]= tmp[2];

	//	// Translate
	//	intersectionPt[0]+= pThis->m_phCylinder->getLocationX ();
	//	intersectionPt[1]+= pThis->m_phCylinder->getLocationY ();
	//	intersectionPt[2]+= pThis->m_phCylinder->getLocationZ ();

	//	char s[256];
	//	sprintf ( s, "Intersect: %d\n", pThis  );
	//	OutputDebugString ( s );

 //       return true;
 //   }

    return false;
}

/******************************************************************************
 HapticSphere::closestSurfaceFeatures
 Finds the closest surface feature(s) to queryPt.
******************************************************************************/
bool CHapticCylinder::closestSurfaceFeatures(const HLdouble queryPt[3], 
                                          const HLdouble targetPt[3],
                                          HLgeom *geom,
                                          HLdouble closestPt[3],
                                          void *userdata)
{
	OutputDebugString ( "Surface Feature\n" );

 //   HapticSphere *pThis = static_cast<HapticSphere *>(userdata);

	//char s[256];
	//sprintf ( s, "queryPt: %f, %f, %f\n", queryPt[0], queryPt[1], queryPt[2] );
	//OutputDebugString ( s );

	//sprintf ( s, "targetPt: %f, %f, %f\n", targetPt[0], targetPt[1], targetPt[2] );
	//OutputDebugString ( s );

	//sprintf ( s, "closestPt: %f, %f, %f\n", closestPt[0], closestPt[1], closestPt[2] );
	//OutputDebugString ( s );

 //   // Return a plane tangent to the sphere as the closest 2D local feature.
 //   hduVector3Dd q(queryPt);
	//hduVector3Dd centre ( pThis->m_phCylinder->getLocationX (), 
	//					  pThis->m_phCylinder->getLocationY (), 
	//					  pThis->m_phCylinder->getLocationZ () );

	//hduVector3Dd normal;

	//normal= q - centre;

	////normal[0]*= pThis->m_phCylinder->getSizeX ();
	////normal[1]*= pThis->m_phCylinder->getSizeY ();
	////normal[2]*= pThis->m_phCylinder->getSizeZ ();
	//
	//normal.normalize();

 //   hduVector3Dd point = (normal * pThis->getRadius());

	//point[0]*= pThis->m_phCylinder->getSizeX ()*1.9;
	//point[1]*= pThis->m_phCylinder->getSizeY ()*1.9;
	//point[2]*= pThis->m_phCylinder->getSizeZ ()*1.9;

	//point+= centre;

 //   hlLocalFeature2dv(geom, HL_LOCAL_FEATURE_PLANE, normal, point);
 //   memcpy(closestPt, point, sizeof(point));

    return true;
}

/******************************************************************************
 HapticSphere::intersectSegmentOutIn
 Helper method to intersect sphere with line segment.
******************************************************************************/
bool CHapticCylinder::intersectSegmentOutIn(
    const hduVector3Dd &startPt_LC,
    const hduVector3Dd &endPt_LC,
    hduVector3Dd &rIntersectionPt_LC) const
{
 //   hduVector3Dd p = startPt_LC;
 //   hduVector3Dd v = endPt_LC - startPt_LC;

 //   // Solve the intersection implicitly using the quadratic formula.
 //   double a = v[0]*v[0] + v[1]*v[1];
 //   double b = 2 * ( v[1]*p[1] + v[0]*p[0] );
 //   double c = p[1]*p[1] + p[0]*p[0] - 1;

 //   double disc = b*b - 4*a*c;

 //   // The scale factor that must be applied to v so that p + nv is
 //   // on the sphere.
 //   double n;
 //   if(disc == 0.0)
 //   {
 //       n = (-b)/(2*a);
 //   }
 //   else if(disc > 0.0)
 //   {
 //       double posN = (-b + sqrt(disc))/(2*a);
 //       double negN = (-b - sqrt(disc))/(2*a);
 //       n = posN < negN ? posN : negN;
 //   }
 //   else
 //   {
 //       return false;
 //   }

 //   // n greater than one means that the ray defined by the two points
 //   // intersects the sphere, but beyond the end point of the segment.
 //   // n less than zero means that the intersection is 'behind' the
 //   // start point.
 //   if(n > 1.0 || n < 0.0)
 //   {
 //       return false;
 //   }

	//hduVector3Dd pointOnSurface= p + n*v;

 //   rIntersectionPt_LC[0]= pointOnSurface[0];
 //   rIntersectionPt_LC[1]= pointOnSurface[1];
 //   rIntersectionPt_LC[2]= pointOnSurface[2];

//    return true;
	return false;
}

/******************************************************************************
 HapticSphere::isInside
 Tests whether a point is inside or outside of the sphere.
******************************************************************************/
bool CHapticCylinder::isInside(const hduVector3Dd &testPt) const
{
	hduVector3Dd xyTest ( testPt[0], testPt[1], 0 );

	return xyTest.magnitude() < m_radius && abs(testPt[2]) < 0.5;

    //return testPt.magnitude() < m_radius;
}

/******************************************************************************/
