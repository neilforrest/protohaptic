#pragma once
#include "transform.h"
class CTorus;

class CTorusRadiusMinorResize :
	public CTransform
{
public:
	/** Construct a new radius transformation on given shape.
	    (x, y, z) is the starting position of the proxy */
	CTorusRadiusMinorResize(CTorus *torus, HLdouble x, HLdouble y, HLdouble z);

	/** Destroy the ratio transform */
	virtual ~CTorusRadiusMinorResize(void);

	/** Set the current position of the proxy.
	    The shape under control will be transformed accordingly */
	void setProxyPos(HLdouble x, HLdouble y, HLdouble z);

	/** Returns an integer constant representing the type of transformation (i.e. ratio)
	    See Transform.h for a list of possible values */
	int getType();

	/** Returns the shape being transformed */
	CShape *getShape();
};
