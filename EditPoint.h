// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: EditPoint.h
// Classes: CEditPoint
// Purpose: Represents a control point

#if !defined(AFX_EDITPOINT_H__FAAE7D99_2C55_4A96_A620_D1CD21365729__INCLUDED_)
#define AFX_EDITPOINT_H__FAAE7D99_2C55_4A96_A620_D1CD21365729__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"
#include "Transform.h"
#include "PupText.h"
#include "ProtoHaptic.h"

#include <gl/gl.h>
#include <gl/glu.h>

/** Represents a control point, part of a CControl 
     with which the user may interact */
class CEditPoint : public CShape  
{
public:
	/** Sets the tooltip associated with this edit point */
	void setToolTip(CString s);
	
	/** Enable or disable the control points magnetism effect */
	void setMagnetic(bool magnetic);
	
	/** Create a new edit point */
	CEditPoint();
	
	/** Destroy the edit point */
	virtual ~CEditPoint();

	/** Overrides CDrawable to render the edit point graphically */
	void drawGLScene();
	
	/** Overrides CShape in order to return OpenGL rendering code */
	virtual CString drawGLCode();
	
	/** Overrides CFeelable in order to render the edit point haptically */
	void drawHapticScene(bool primary_context);
	
	/** Return an integer representing the type of shape (i.e. edit point)
	    See Shape.h for possible values */
	int getType();
	
	/** Return a pointer to a copy of this edit point */
	CShape* clone();

protected:
	/** Ptr to app, to get settings data */
	CProtoHapticApp* app;

	/** Require audio notification of user acquiring edit point */
	bool m_alertAquire;
	
	/** Tool tip text for edit point */
	CString m_tipText;
	
	/** Is the edit point magnetic? */
	bool m_magnetic;
	
	/** GLU Quadric representing edit point sphere */
	GLUquadric* q;
};

#endif // !defined(AFX_EDITPOINT_H__FAAE7D99_2C55_4A96_A620_D1CD21365729__INCLUDED_)
