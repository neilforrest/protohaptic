// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: SphereControl.h
// Classes: CSphereControl
// Purpose: Represents a Sphere's control widget

#if !defined(AFX_SPHERECONTROL_H__82FC84E7_F800_4BB4_A6B1_3DB8A73850F8__INCLUDED_)
#define AFX_SPHERECONTROL_H__82FC84E7_F800_4BB4_A6B1_3DB8A73850F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl/gl.h>
#include <gl/glu.h>

#include "Control.h"
#include "Sphere.h"
#include "EditPoint.h"

/** Represents a Sphere's control widget */
class CSphereControl : public CControl
{
public:
	/** Create a new sphere control widget to control shape */
	CSphereControl(CShape* shape);

	/** Create a new sphere control widget */
	CSphereControl();

	/** Destroy this sphere control widget */
	virtual ~CSphereControl();

	void connectEditPoints(CEditPoint* a, CEditPoint* b);

	/** Render the shape graphically via OpenGL */
	void drawGLScene();

	/** Render the shape haptically via OpenHaptics.
	    primary_context specifies either device 1 or 2 */
	void drawHapticScene(bool primary_context);

	/** Returns an integer constant representing the current
	    control mode. See control.h for possible values. */
	int getControlMode();
protected:
	/** The shape under control */
	CShape* m_sphere;

	/** An edit point controlling resizing of the sphere */
	CEditPoint* m_resize0;

	/** An edit point controlling resizing of the sphere */
	CEditPoint* m_resize1;

	/** An edit point controlling resizing of the sphere */
	CEditPoint* m_resize2;

	/** An edit point controlling resizing of the sphere */
	CEditPoint* m_resize3;

	/** An edit point controlling resizing of the sphere */
	CEditPoint* m_resize4;

	/** An edit point controlling resizing of the sphere */
	CEditPoint* m_resize5;

	/** An edit point controlling resizing of the sphere */
	CEditPoint* m_resize6;

	/** An edit point controlling resizing of the sphere */
	CEditPoint* m_resize7;

	/** An edit point controlling rotation of the sphere */
	CEditPoint* m_rotate0;

	/** An edit point controlling rotation of the sphere */
	CEditPoint* m_rotate1;

	/** An edit point controlling rotation of the sphere */
	CEditPoint* m_rotate2;

	/** An edit point controlling rotation of the sphere */
	CEditPoint* m_rotate3;

	/** An edit point controlling rotation of the sphere */
	CEditPoint* m_rotate4;

	/** An edit point controlling rotation of the sphere */
	CEditPoint* m_rotate5;
};

#endif // !defined(AFX_SPHERECONTROL_H__82FC84E7_F800_4BB4_A6B1_3DB8A73850F8__INCLUDED_)
