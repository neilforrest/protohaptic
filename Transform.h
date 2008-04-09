// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Transform.h
// Classes: CTransform
// Purpose: Represents the transformation of a shape

#if !defined(AFX_TRANSFORM_H__BED39A90_49F4_481E_AC67_33BC0E5C77FE__INCLUDED_)
#define AFX_TRANSFORM_H__BED39A90_49F4_481E_AC67_33BC0E5C77FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// OpenHaptics Headers
#include <HL/hl.h>
#include <HLU/hlu.h>

#include "Shape.h"

#define TRANSFORM_RESIZE					0
#define TRANSFORM_ROTATE					1
#define TRANSFORM_TRANSLATE					2
#define TRANSFORM_RATIO						3
#define TRANSFORM_TRI_VERTEX_MOVE			4
#define TRANSFORM_TORUS_RADIUS_RESIZE_MAJOR	5
#define TRANSFORM_TORUS_RADIUS_RESIZE_MINOR	6
#define TRANSFORM_TORUS_RESIZE				7

/** Represents the transformation of a shape */
class CTransform  
{
public:
	/** Returns the shape being transformed */
	CShape* getShape();

	/** Create a new transformation */
	CTransform();

	/** Sets the current proxy position, used to specify the transformation */
	virtual void setProxyPos(HLdouble x, HLdouble y, HLdouble z)= 0;

	/** Override this to return an integer representing the type of this transformation.
	    See top of this file (Transform.h), for list of values */
	virtual int getType()= 0;

	/** Destroy this transform */
	virtual ~CTransform();
protected:

	/** The shape being transformed */
	CShape *m_shape;

	/** The original position of the proxy */
	HLdouble m_proxyStartPos[3];

	/** The current position of the proxy */
	HLdouble m_proxyNowPos[3];

};

#endif // !defined(AFX_TRANSFORM_H__BED39A90_49F4_481E_AC67_33BC0E5C77FE__INCLUDED_)
