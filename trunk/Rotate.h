// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Rotate.h
// Classes: CRotate
// Purpose: Represents a rotation transformation

#if !defined(AFX_ROTATE_H__4E076B0C_39D4_4F1B_9D7B_65CFB5C363DE__INCLUDED_)
#define AFX_ROTATE_H__4E076B0C_39D4_4F1B_9D7B_65CFB5C363DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Transform.h"

#define ROTATE_AXIS_X 0
#define ROTATE_AXIS_Y 1
#define ROTATE_AXIS_Z 2

/** Represents a rotation transformation */
class CRotate : public CTransform  
{
public:
	/** If chunking is set, the rotation will snap-to the world axes */
	void setChunking(bool chunking);

	/** Create a new rotation acting upon the given shape.
	    (x,y,z) is the position of the cursor at creation.
		handle_axis and direction define which control point is being used and thus which axes
		of rotation are available. Handle axis is ROTATE_AXIS_X, ROTATE_AXIS_Y or ROTATE_AXIS_Z.
		direction is 1 or -1 depending on whether the control point is at the top or bottom of the axis */
	CRotate(CShape *shape, HLdouble x, HLdouble y, HLdouble z, int handle_axis, int direction);

	/** Returns an integer constant representing the type of transformation (i.e. rotate)
	    See Transform.h for a list of possible values */
	int getType();

	/** Set the current position of the proxy.
	    The shape under control will be transformed accordingly */
	void setProxyPos(HLdouble x, HLdouble y, HLdouble z);

	/** Destroy the rotation operation */
	virtual ~CRotate();

	/** Returns true if the rotation axis is decided and rotation is occurring */
	bool hasAxis();

	/** Returns the axis about which rotation is occurring:
		ROTATE_AXIS_X, ROTATE_AXIS_Y or ROTATE_AXIS_Z.*/
	int getAxis();

	/** Returns the axis on which the handle to which the selected control point is connected lies.
		ROTATE_AXIS_X, ROTATE_AXIS_Y or ROTATE_AXIS_Z.*/
	int getHandleAxis();
protected:
	/** Rounds the float f to 1, -1 or 0 */
	float chunk(float f);

	/** Is chunking turned on? */
	bool m_chunking;

	/** Initial rotation matrix */
	float m_startRotation[16];

	/** The rotation axis, if decided */
	int m_axis;

	/** The axis on which the control point governing the rotation lies */
	int m_handle_axis;

	/** Which on of m_handle_axis the control point lies. 1 or -1 */
	int m_direction;

	/** The axis has been decided and rotation is being executed */
	bool rotating;
};

#endif // !defined(AFX_ROTATE_H__4E076B0C_39D4_4F1B_9D7B_65CFB5C363DE__INCLUDED_)
