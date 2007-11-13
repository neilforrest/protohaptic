// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Move.h
// Classes: CMove
// Purpose: Represents move, or translation transformation

#if !defined(AFX_MOVE_H__2D34940A_5667_4478_B168_7A893ABBCF12__INCLUDED_)
#define AFX_MOVE_H__2D34940A_5667_4478_B168_7A893ABBCF12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Transform.h"
#include "ProtoHapticDoc.h"

/** Represents move, or translation transformation, by obtaining 
    user input and acting on a shape */
class CMove : public CTransform  
{
public:
	/** Set snap to other shapes boundaries */
	void setSnapto(bool snapto);
	
	/** Create a new move transformation on the given shape.
	    x, y and z represent the current proxy position and doc is
	    the current document (used by snapto) */
	CMove(CShape *shape, HLdouble x, HLdouble y, HLdouble z, CProtoHapticDoc *doc);
	
	/** Update the move by setting a new proxy position, the move will
		 update the shapes position accordingly */
	void setProxyPos(HLdouble x, HLdouble y, HLdouble z);
	
	/** Return an integer representing the type of transform (i.e. move)
		See Transform.h for return values */
	int getType();
	
	/** Destroy this move */
	virtual ~CMove();

	/** Returns the shape on which this move is acting */
	CShape *getShape();
	
protected:
	/** Have the audible snap alerts been given */
	bool m_snapAlerts[12];
	
	/** Is the snap to function activated */
	bool m_snapto;
	
	/** The document the move acts on.
		Required in order to snap to other objects in scene */
	CProtoHapticDoc *m_document;
	
	/** The starting position of the shape */
	HLdouble m_shapeStart[3];
};

#endif // !defined(AFX_MOVE_H__2D34940A_5667_4478_B168_7A893ABBCF12__INCLUDED_)
