// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Ratio.h
// Classes: CRatio
// Purpose: Represents a transformation between cylinder and cone

#if !defined(AFX_RATIO_H__F2F275F6_4D01_4CEC_BD79_E36A13313762__INCLUDED_)
#define AFX_RATIO_H__F2F275F6_4D01_4CEC_BD79_E36A13313762__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Transform.h"
#include "Shape.h"

/** Represents a transformation between cylinder and cone */
class CRatio : public CTransform  
{
public:
	/** Construct a new ratio transformation on given shape.
	    (x, y, z) is the starting position of the proxy */
	CRatio(CShape *shape, HLdouble x, HLdouble y, HLdouble z);

	/** Set the current position of the proxy.
	    The shape under control will be transformed accordingly */
	void setProxyPos(HLdouble x, HLdouble y, HLdouble z);

	/** Returns an integer constant representing the type of transformation (i.e. ratio)
	    See Transform.h for a list of possible values */
	int getType();

	/** Destroy the ratio transform */
	virtual ~CRatio();

	/** Returns the shape being transformed */
	CShape *getShape();

protected:
	/** The ratio of the shape before being transformed */
	float m_ratioStart;
};

#endif // !defined(AFX_RATIO_H__F2F275F6_4D01_4CEC_BD79_E36A13313762__INCLUDED_)
