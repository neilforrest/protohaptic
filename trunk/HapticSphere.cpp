/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name: 

  HapticSphere.cpp

Description:
  
  HL custom shape that haptically renders a sphere.  Designed to be used
  with hlCallback as a custom shape.

*******************************************************************************/
#include "stdafx.h"

#include "HapticSphere.h"
#include <hdu/hduMatrix.h>
#include <iostream>

/******************************************************************************
 HapticSphere::HapticSphere
******************************************************************************/
HapticSphere::HapticSphere( CShape* s ) :
    m_radius(0.5)
{  
	m_phSphere= s;
}

/******************************************************************************
 HapticSphere::intersectSurface
 Intersects the line segment from startPt to endPt with the sphere. 
 Returns the closest point of intersection to the start point in intersectionPt.
 Returns the surface normal at intersectionPt in intersectionNormal.
 Returns which face (HL_FRONT or HL_BACK) is being touched in face.
 Returns true if an intersection occured.
******************************************************************************/
bool HapticSphere::intersectSurface(const HLdouble startPt[3], 
                                    const HLdouble endPt[3],
                                    HLdouble intersectionPt[3], 
                                    HLdouble intersectionNormal[3],
                                    HLenum *face,
                                    void *userdata)
{


    HapticSphere *pThis = static_cast<HapticSphere *>(userdata);    

    hduVector3Dd startPtV(startPt);
    hduVector3Dd endPtV(endPt);
    hduVector3Dd intersectionPtV;

	// Undo translation
	startPtV[0]-= pThis->m_phSphere->getLocationX ();
	startPtV[1]-= pThis->m_phSphere->getLocationY ();
	startPtV[2]-= pThis->m_phSphere->getLocationZ ();

	endPtV[0]-= pThis->m_phSphere->getLocationX ();
	endPtV[1]-= pThis->m_phSphere->getLocationY ();
	endPtV[2]-= pThis->m_phSphere->getLocationZ ();

	// Transform start and end points by inv of object transformation
	// Add current items transform to parent transform of children
	float* transform= pThis->m_phSphere->getRotation ( );

	// Define the items transform as an hduMatrix
	hduMatrix rotation 
			( transform[0],  transform[1],  transform[2],  transform[3], 
			  transform[4],  transform[5],  transform[6],  transform[7], 
			  transform[8],  transform[9],  transform[10], transform[11], 
			  transform[12], transform[13], transform[14], transform[15] );

	hduMatrix invRot= rotation.getInverse ();

	// undo rotation
	hduVector3Dd tmp;
	invRot.multVecMatrix ( hduVector3Dd ( startPtV ), tmp );
	startPtV= tmp;

	invRot.multVecMatrix ( hduVector3Dd ( endPtV ), tmp );
	endPtV= tmp;

	// Undo scale
	startPtV[0]/= pThis->m_phSphere->getSizeX ();
	startPtV[1]/= pThis->m_phSphere->getSizeY ();
	startPtV[2]/= pThis->m_phSphere->getSizeZ ();

	endPtV[0]/= pThis->m_phSphere->getSizeX ();
	endPtV[1]/= pThis->m_phSphere->getSizeY ();
	endPtV[2]/= pThis->m_phSphere->getSizeZ ();


    bool bFoundIntersection = false;
    bool bStartInside = pThis->isInside(startPtV);
    bool bEndInside = pThis->isInside(endPtV);

    // Don't proceed if the start and end are on the same side of the surface.
    if (bStartInside == bEndInside)
	{
		//OutputDebugString ( "No Intersect Sphere...\n" );

        return false;
	}

    if (bEndInside && !bStartInside)
    {        
        // Intersect sphere from outside using start->end segment.
        bFoundIntersection = pThis->intersectSegmentOutIn(
            startPtV, endPtV, intersectionPtV);
        *face = HL_FRONT;
    }
    else
    {
        // Intersect sphere from inside using end->start segment.
        bFoundIntersection = pThis->intersectSegmentOutIn(
            endPtV, startPtV, intersectionPtV);
        *face = HL_BACK;
		return false;
    }

    if (bFoundIntersection)
    {
        hduVector3Dd intersectionNormalV = intersectionPtV;

		intersectionNormalV[0]/= pThis->m_phSphere->getSizeX ();
		intersectionNormalV[1]/= pThis->m_phSphere->getSizeY ();
		intersectionNormalV[2]/= pThis->m_phSphere->getSizeZ ();

        intersectionNormalV.normalize();

		rotation.multVecMatrix ( hduVector3Dd ( intersectionNormalV ), tmp );
		intersectionNormalV= tmp;

        // If this is back face intersection, then reverse the normal.
        if (*face == HL_BACK)
        {
            intersectionNormalV *= -1;
        }

        intersectionPt[0] = intersectionPtV[0];
        intersectionPt[1] = intersectionPtV[1];
        intersectionPt[2] = intersectionPtV[2];

        intersectionNormal[0] = intersectionNormalV[0];
        intersectionNormal[1] = intersectionNormalV[1];
        intersectionNormal[2] = intersectionNormalV[2];        

		char s[256];
		sprintf ( s, "%f, %f, %f, %f, %f, %f\n", intersectionNormal[0], intersectionNormal[1], intersectionNormal[2], intersectionPt[0], intersectionPt[1], intersectionPt[2] );
		OutputDebugString ( s );

		// Apply shapes transform to intersection point

		// Scale
		intersectionPt[0]*= pThis->m_phSphere->getSizeX ();
		intersectionPt[1]*= pThis->m_phSphere->getSizeY ();
		intersectionPt[2]*= pThis->m_phSphere->getSizeZ ();

		// Rotate
		rotation.multVecMatrix ( hduVector3Dd ( intersectionPt[0], intersectionPt[1], intersectionPt[2] ), tmp );
		intersectionPt[0]= tmp[0];
		intersectionPt[1]= tmp[1];
		intersectionPt[2]= tmp[2];

		// Translate
		intersectionPt[0]+= pThis->m_phSphere->getLocationX ();
		intersectionPt[1]+= pThis->m_phSphere->getLocationY ();
		intersectionPt[2]+= pThis->m_phSphere->getLocationZ ();

        return true;
    }

	//OutputDebugString ( "No Intersect Sphere...\n" );

    return false;
}

/******************************************************************************
 HapticSphere::closestSurfaceFeatures
 Finds the closest surface feature(s) to queryPt.
******************************************************************************/
bool HapticSphere::closestSurfaceFeatures(const HLdouble queryPt[3], 
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
	//hduVector3Dd centre ( pThis->m_phSphere->getLocationX (), 
	//					  pThis->m_phSphere->getLocationY (), 
	//					  pThis->m_phSphere->getLocationZ () );

	//hduVector3Dd normal;

	//normal= q - centre;

	////normal[0]*= pThis->m_phSphere->getSizeX ();
	////normal[1]*= pThis->m_phSphere->getSizeY ();
	////normal[2]*= pThis->m_phSphere->getSizeZ ();
	//
	//normal.normalize();

 //   hduVector3Dd point = (normal * pThis->getRadius());

	//point[0]*= pThis->m_phSphere->getSizeX ()*1.9;
	//point[1]*= pThis->m_phSphere->getSizeY ()*1.9;
	//point[2]*= pThis->m_phSphere->getSizeZ ()*1.9;

	//point+= centre;

 //   hlLocalFeature2dv(geom, HL_LOCAL_FEATURE_PLANE, normal, point);
 //   memcpy(closestPt, point, sizeof(point));

    return true;
}

/******************************************************************************
 HapticSphere::intersectSegmentOutIn
 Helper method to intersect sphere with line segment.
******************************************************************************/
bool HapticSphere::intersectSegmentOutIn(
    const hduVector3Dd &startPt_LC,
    const hduVector3Dd &endPt_LC,
    hduVector3Dd &rIntersectionPt_LC) const
{
    hduVector3Dd p = startPt_LC;
    hduVector3Dd v = endPt_LC - startPt_LC;

    // Solve the intersection implicitly using the quadratic formula.
    double a = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    double b = 2 * (p[0]*v[0] + p[1]*v[1] + p[2]*v[2]);
    double c = p[0]*p[0] + p[1]*p[1] + p[2]*p[2] - m_radius * m_radius;

    double disc = b*b - 4*a*c;

    // The scale factor that must be applied to v so that p + nv is
    // on the sphere.
    double n;
    if(disc == 0.0)
    {
        n = (-b)/(2*a);
    }
    else if(disc > 0.0)
    {
        double posN = (-b + sqrt(disc))/(2*a);
        double negN = (-b - sqrt(disc))/(2*a);
        n = posN < negN ? posN : negN;
    }
    else
    {
        return false;
    }

    // n greater than one means that the ray defined by the two points
    // intersects the sphere, but beyond the end point of the segment.
    // n less than zero means that the intersection is 'behind' the
    // start point.
    if(n > 1.0 || n < 0.0)
    {
        return false;
    }

    rIntersectionPt_LC = p + n*v;
    return true;
}

/******************************************************************************
 HapticSphere::isInside
 Tests whether a point is inside or outside of the sphere.
******************************************************************************/
bool HapticSphere::isInside(const hduVector3Dd &testPt) const
{
    return testPt.magnitude() < m_radius;
}

/******************************************************************************/
