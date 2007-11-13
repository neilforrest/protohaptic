// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: CubeControl.h
// Classes: CCubeControl
// Purpose: Represents a cube's control widget

#if !defined(AFX_CUBECONTROL_H__ECC4307D_0D47_4F5E_9E10_E3BC2B44A596__INCLUDED_)
#define AFX_CUBECONTROL_H__ECC4307D_0D47_4F5E_9E10_E3BC2B44A596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl/gl.h>
#include <gl/glu.h>

#include "Control.h"
#include "Cube.h"
#include "EditPoint.h"

/** Represents a cube's control widget */
class CCubeControl : public CControl  
{
public:
	/** Construct a new cube control from that given */
	CCubeControl(CCubeControl *c);
	
	/** Return a pointer a copy of the cube control */
	CCubeControl* clone();
	
	/** Sets the cube controls cube under control */
	void setCube(CCube *c);
	
	/** Creates a new cube control to control the cube given */
	CCubeControl(CCube* cube);
	
	/** Destroy the cue control */
	virtual ~CCubeControl();
	
	/** Overrides CDrawable in order to render the control graphically */
	void drawGLScene();
	
	/** Overrides CFeelabe in order to render the control haptically */
	void drawHapticScene(bool primary_context);
	
	/** Return the current control mode 
	    See control.h for possible values */
	int getControlMode();
private:
	/** Pointer to cube under control */
	CCube* m_cube;
	
	/** The control's edit points */
	CEditPoint* m_resize0, *m_resize1, *m_resize2, *m_resize3, *m_resize4, *m_resize5,
		*m_resize6, *m_resize7, *m_rotate0, *m_rotate1, *m_rotate2, *m_rotate3, *m_rotate4, *m_rotate5;

	/** Updates the positions of edit points based of position of cube */
	void UpdateEditPoints();
};

#endif // !defined(AFX_CUBECONTROL_H__ECC4307D_0D47_4F5E_9E10_E3BC2B44A596__INCLUDED_)
