#pragma once
#include "Shape.h"

#include <hdu/hduMatrix.h>

class CHapticDisk
{
public:
	CHapticDisk(hduMatrix* t);
	virtual ~CHapticDisk(void);

    /* Accessors for radius of cylinder. */
    double getRadius() const { return m_radius; }
    void setRadius(double r) { m_radius = r; }
    
    /* Intersects the line segment from startPt to endPt with
       the cylinder.  Returns the closest point of intersection 
       to the start point in intersectionPt.  Returns the
       surface normal at intersectionPt in intersectionNormal.
       Returns which face (HL_FRONT or HL_BACK) is being touched
       in face.
       Returns true if there is an intersection. */
    static bool HLCALLBACK intersectSurface(
        const HLdouble startPt[3], 
        const HLdouble endPt[3],
        HLdouble intersectionPt[3], 
        HLdouble intersectionNormal[3],
        HLenum *face,
        void *userdata);

    /* Finds the closest surface feature(s) to queryPt. */
    static bool HLCALLBACK closestSurfaceFeatures(
        const HLdouble queryPt[3], 
        const HLdouble targetPt[3],
        HLgeom *geom,
        HLdouble closestPt[3],
        void *userdata);

protected:

    /* Helper method to intersect cylinder with line segment. */
    bool intersectSegmentOutIn(const hduVector3Dd &startPt_LC,
                               const hduVector3Dd &endPt_LC,
                               hduVector3Dd &rIntersectionPt_LC) const;

    bool isInside(const hduVector3Dd &testPt) const;

protected:
	double m_radius;
	hduMatrix* m_transform;
};
