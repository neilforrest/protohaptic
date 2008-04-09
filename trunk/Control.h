// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Control.h
// Classes: CControl
// Purpose: Represents a generic shapes control widget

#if !defined(AFX_CONTROL_H__F397E9D5_45A9_4EA9_A66A_E718DFF69415__INCLUDED_)
#define AFX_CONTROL_H__F397E9D5_45A9_4EA9_A66A_E718DFF69415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Drawable.h"
#include "Feelable.h"

// Control mode constants returned by getControlMode()
#define CONTROL_MODE_NULL     0
#define CONTROL_MODE_MOVE     1
#define CONTROL_MODE_RESIZE0  2
#define CONTROL_MODE_RESIZE1  3
#define CONTROL_MODE_RESIZE2  4
#define CONTROL_MODE_RESIZE3  5
#define CONTROL_MODE_RESIZE4  6
#define CONTROL_MODE_RESIZE5  7
#define CONTROL_MODE_RESIZE6  8
#define CONTROL_MODE_RESIZE7  9
#define CONTROL_MODE_ROTATE0  10
#define CONTROL_MODE_ROTATE1  11
#define CONTROL_MODE_ROTATE2  12
#define CONTROL_MODE_ROTATE3  13
#define CONTROL_MODE_ROTATE4  14
#define CONTROL_MODE_ROTATE5  15
#define CONTROL_MODE_RATIO    16

// Triangle specific control modes
#define CONTROL_MODE_TRI_VERTEX_A     20	// Move vertex A
#define CONTROL_MODE_TRI_VERTEX_B     21	// Move vertex B
#define CONTROL_MODE_TRI_VERTEX_C     22	// Move vertex C

// Torus specific
#define CONTROL_MODE_TORUS_RADIUS_MAJOR     30	// Chnge major radius
#define CONTROL_MODE_TORUS_RADIUS_MINOR     31	// Chnge minor radius

// Control colour scheme constants passed to setColourScheme()
#define CONTROL_COLOURS_PRIMARY  	0
#define CONTROL_COLOURS_SECONDARY	1
#define CONTROL_COLOURS_BOTH			2

/** Represents a shape's control widget
    which allows the user to alter its properties */
class CControl : public CDrawable, public CFeelable
{
public:
	/** Sets the control's colour scheme. Used to 
	    distinguish primary and secondary devices controls */
	void setColourScheme(int cs);
	
	/** Construct a new CControl */
	CControl();
	
	/** Destroy a CControl */
	virtual ~CControl();
	
	/** Returns an integer constant representing the current
	    control mode. See control.h for possible values. */
	virtual int getControlMode()= 0;
protected:
	/** The control's colour scheme */
	int m_colourScheme;
};

#endif // !defined(AFX_CONTROL_H__F397E9D5_45A9_4EA9_A66A_E718DFF69415__INCLUDED_)
