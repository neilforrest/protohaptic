// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: CylinderControl.h
// Classes: CCylinderControl
// Purpose: Represents cylinder's  control widget

#if !defined(AFX_CYLINDERCONTROL_H__D435C000_24E6_4572_AA5C_03218AC07126__INCLUDED_)
#define AFX_CYLINDERCONTROL_H__D435C000_24E6_4572_AA5C_03218AC07126__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"
#include "SphereControl.h"
#include "EditPoint.h"

/** Represents cylinder's  control widget 
    This class extends CSphereControl, adding a ratio control */
class CCylinderControl : public CSphereControl
{
public:
	/** Create a new cylinder control controlling the given shape */
	CCylinderControl(CShape* shape);
	
	/** Destroy the control */
	virtual ~CCylinderControl();
	
	
	/** Overrides CDrawable in order to render the control graphically */
	void drawGLScene();
	
	/** Overrides CFeelabe in order to render the control haptically */
	void drawHapticScene(bool primary_context);
	
	/** Return the current control mode 
	    See control.h for possible values */
	int getControlMode();

protected:
	/** The cylinder's ratio edit point */
	CEditPoint* m_ratioEdit;
};

#endif // !defined(AFX_CYLINDERCONTROL_H__D435C000_24E6_4572_AA5C_03218AC07126__INCLUDED_)
