// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Resize.h
// Classes: CResize
// Purpose: Represents resizing or scaling transformation

#if !defined(AFX_RESIZE_H__47ADAA8E_32F1_4C65_9E42_8B616109816D__INCLUDED_)
#define AFX_RESIZE_H__47ADAA8E_32F1_4C65_9E42_8B616109816D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Transform.h"
#include "ProtoHapticDoc.h"

/** Represents resizing or scaling transformation */
class CResize : public CTransform  
{
public:
	/** If the axis is fixed the transform guesses the indented axis of resize and locks to it */
	void setFixedAxis(bool fixedaxis);

	// The axis specified will be fixed. E.g. Fix one axis for a 2d planar shape
	void SetAxisFixed ( bool x, bool y, bool z );

	/** If snap-to is true the transform snaps the resized boundaries to nearby shapes in the document */
	void setSnapto(bool snapto);

	/** If the aspect is fixed, all three dimensions resize in scale to one another */
	void setAspectFixed(bool b);

	/** Create a new resize acting on the given shape.
	    (x, y, z) is the initial positions of the proxy and doc is the document in 
		which shape resides */
	CResize(CShape *shape, HLdouble x, HLdouble y, HLdouble z,CProtoHapticDoc *doc);

	/** Returns an integer constant representing the type of transformation (i.e. resize)
	    See Transform.h for a list of possible values */
	int getType();

	/** Set the current position of the proxy.
	    The shape under control will be transformed accordingly */
	void setProxyPos(HLdouble x, HLdouble y, HLdouble z);

	/** Sets the corner in which the control point being used is located.
	    This determines which corner will act as the anchor point.
		Along each axis, false is at one end and true is at the other */
	void setOrientation(bool x, bool y, bool z);

	/** Set the size of the box on whose corners the resize edit-points are placed.
	    In object coordinates. The default is a unit cube. The box is centred on the origin of object space */
	void setResizeBoxDimentions ( float x, float y, float z );

	/** Destroy the resize operation */
	virtual ~CResize();
protected:
	/** Has the snap alert been given in each possible case? */
	bool m_snapAlerts[6];

	/** Which axis, if any, is the resize fixed to? */
	int m_axis;

	/** The fixed axis guess has been made, and the object is resizing */
	bool m_resizing;

	/** The transform is fixed to a single (local) axis */
	bool m_fixedaxis;

	/** The transform must snap to other objects in the document */
	bool m_snapto;

	/** The transform must maintain the aspect ratio of the shape */
	bool m_aspectFixed;

	/** The size of the shape before resizing */
	float m_startSizeX, m_startSizeY, m_startSizeZ;

	/** The position of the centre of the shape before resizing */
	float m_startLocationX, m_startLocationY, m_startLocationZ;

	/** The corner in which the control point being used is located.
	    This determines which corner will act as the anchor point.
		Along each axis, false is at one end and true is at the other */
	bool m_orientationX, m_orientationY, m_orientationZ;

	/** The document in which the shape resides, used by the
	    snap-to feature */
	CProtoHapticDoc *m_document;

	/** Remember which axes are fixed */
	bool m_fixedAxisX, m_fixedAxisY, m_fixedAxisZ;

	/** Size of the resize edit-point box, centered on object origin, which may be different from scaling/size of shape (e.g. torus) */
	float m_resizeBoxDims[3];
};

#endif // !defined(AFX_RESIZE_H__47ADAA8E_32F1_4C65_9E42_8B616109816D__INCLUDED_)
