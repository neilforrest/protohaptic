#include "stdafx.h"
#include "HapticDisk.h"
#include "Geometry.h"

CHapticDisk::CHapticDisk(hduMatrix* trans)
{
	m_radius= 0.5;
	m_transform= trans;
}

CHapticDisk::~CHapticDisk(void)
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
bool CHapticDisk::intersectSurface(const HLdouble startPt[3], 
                                    const HLdouble endPt[3],
                                    HLdouble intersectionPt[3], 
                                    HLdouble intersectionNormal[3],
                                    HLenum *face,
                                    void *userdata)
{


    CHapticDisk *pThis = static_cast<CHapticDisk *>(userdata);    
	
	*face= HL_FRONT;

    hduVector3Dd startPtV(startPt);
    hduVector3Dd endPtV(endPt);
    hduVector3Dd intersectionPtV;
	hduVector3Dd tmp;
	
	hduMatrix inv= pThis->m_transform->getInverse ();

	inv.multVecMatrix ( startPtV, tmp );
	startPtV= tmp;

	inv.multVecMatrix ( endPtV, tmp );
	endPtV= tmp;

	//// Undo translation
	//startPtV[0]-= pThis->m_phSphere->getLocationX ();
	//startPtV[1]-= pThis->m_phSphere->getLocationY ();
	//startPtV[2]-= pThis->m_phSphere->getLocationZ ();

	//endPtV[0]-= pThis->m_phSphere->getLocationX ();
	//endPtV[1]-= pThis->m_phSphere->getLocationY ();
	//endPtV[2]-= pThis->m_phSphere->getLocationZ ();

	//// Transform start and end points by inv of object transformation
	//// Add current items transform to parent transform of children
	//float* transform= pThis->m_phSphere->getRotation ( );

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
	//startPtV[0]/= pThis->m_phSphere->getSizeX ();
	//startPtV[1]/= pThis->m_phSphere->getSizeY ();
	//startPtV[2]/= pThis->m_phSphere->getSizeZ ();

	//endPtV[0]/= pThis->m_phSphere->getSizeX ();
	//endPtV[1]/= pThis->m_phSphere->getSizeY ();
	//endPtV[2]/= pThis->m_phSphere->getSizeZ ();

    bool bFoundIntersection = false;
    bool bStartInside = pThis->isInside(startPtV);
    bool bEndInside = pThis->isInside(endPtV);

    // Don't proceed if the start and end are on the same side of the surface.
    if (bStartInside == bEndInside)
        return false;

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
    }

    if (bFoundIntersection)
    {
        hduVector3Dd intersectionNormalV ( 0, 1, 0 );

		//intersectionNormalV[0]/= pThis->m_phSphere->getSizeX ();
		//intersectionNormalV[1]/= pThis->m_phSphere->getSizeY ();
		//intersectionNormalV[2]/= pThis->m_phSphere->getSizeZ ();

  //      intersectionNormalV.normalize();

		hduMatrix noTranslate= *(pThis->m_transform);

		noTranslate[3][0]= 0.0;
		noTranslate[3][1]= 0.0;
		noTranslate[3][2]= 0.0;

		noTranslate.multVecMatrix ( hduVector3Dd ( intersectionNormalV ), tmp );
		intersectionNormalV= tmp;

        // If this is back face intersection, then reverse the normal.
        if (*face == HL_BACK)
        {
            intersectionNormalV *= -1;
        }

		// Apply shapes transform to intersection point
		pThis->m_transform->multVecMatrix ( hduVector3Dd ( intersectionPtV ), tmp );
		intersectionPtV= tmp;

        intersectionPt[0] = intersectionPtV[0];
        intersectionPt[1] = intersectionPtV[1];
        intersectionPt[2] = intersectionPtV[2];

        intersectionNormal[0] = intersectionNormalV[0];
        intersectionNormal[1] = intersectionNormalV[1];
        intersectionNormal[2] = intersectionNormalV[2];        

		//// Apply shapes transform to intersection point

		//// Scale
		//intersectionPt[0]*= pThis->m_phSphere->getSizeX ();
		//intersectionPt[1]*= pThis->m_phSphere->getSizeY ();
		//intersectionPt[2]*= pThis->m_phSphere->getSizeZ ();

		//// Rotate
		//rotation.multVecMatrix ( hduVector3Dd ( intersectionPt[0], intersectionPt[1], intersectionPt[2] ), tmp );
		//intersectionPt[0]= tmp[0];
		//intersectionPt[1]= tmp[1];
		//intersectionPt[2]= tmp[2];

		//// Translate
		//intersectionPt[0]+= pThis->m_phSphere->getLocationX ();
		//intersectionPt[1]+= pThis->m_phSphere->getLocationY ();
		//intersectionPt[2]+= pThis->m_phSphere->getLocationZ ();

	OutputDebugString ( "Intersect\n" );

        return true;
    }

    return false;
}

/******************************************************************************
 HapticSphere::closestSurfaceFeatures
 Finds the closest surface feature(s) to queryPt.
******************************************************************************/
bool CHapticDisk::closestSurfaceFeatures(const HLdouble queryPt[3], 
                                          const HLdouble targetPt[3],
                                          HLgeom *geom,
                                          HLdouble closestPt[3],
                                          void *userdata)
{

    return true;
}

/******************************************************************************
 HapticSphere::intersectSegmentOutIn
 Helper method to intersect sphere with line segment.
******************************************************************************/
bool CHapticDisk::intersectSegmentOutIn(
    const hduVector3Dd &startPt_LC,
    const hduVector3Dd &endPt_LC,
    hduVector3Dd &rIntersectionPt_LC) const
{
	Plane plane;
	plane.V0= Point ( 0, 0, 0 );
	plane.n= Vector ( 0, -1, 0 );

	Segment segment;
	segment.P0= Point ( startPt_LC[0], startPt_LC[1], startPt_LC[2] );
	segment.P1= Point ( endPt_LC[0], endPt_LC[1], endPt_LC[2] );

	// New point of intersection
	Point newInt;

	bool ret= intersect3D_SegmentPlane( segment, plane, &newInt );

	if ( ret )
	{
		rIntersectionPt_LC[0]= newInt.x;
		rIntersectionPt_LC[1]= newInt.y;
		rIntersectionPt_LC[2]= newInt.z;

		return ( hduVector3Dd ( newInt.x, newInt.y, newInt.z ).magnitude () < m_radius );
	}
	else
	{
		return false;
	}
}

/******************************************************************************
 HapticSphere::isInside
 Tests whether a point is inside or outside of the sphere.
******************************************************************************/
bool CHapticDisk::isInside(const hduVector3Dd &testPt) const
{
    return testPt[1] < 0;
}

/******************************************************************************/
