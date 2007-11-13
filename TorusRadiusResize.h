#pragma once
#include "transform.h"
#include "Torus.h"

class CTorusRadiusResize :
	public CTransform
{
public:
	/** Construct a new radius transformation on given shape.
	    (x, y, z) is the starting position of the proxy */
	CTorusRadiusResize(CTorus *torus, HLdouble x, HLdouble y, HLdouble z);

	/** Set the current position of the proxy.
	    The shape under control will be transformed accordingly */
	void setProxyPos(HLdouble x, HLdouble y, HLdouble z);

	/** Returns an integer constant representing the type of transformation (i.e. ratio)
	    See Transform.h for a list of possible values */
	int getType();

	/** Destroy the ratio transform */
	virtual ~CTorusRadiusResize();

	/** Returns the shape being transformed */
	CShape *getShape();

protected:
	/** The ratio of the shape before being transformed */
	float m_radiusStart;
};
