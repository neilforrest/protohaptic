// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Sphere.h
// Classes: CSphere
// Purpose: Represents a sphere shape

#if !defined(AFX_SPHERE_H__30F1FF80_2E11_40FF_A366_06963A0A95F9__INCLUDED_)
#define AFX_SPHERE_H__30F1FF80_2E11_40FF_A366_06963A0A95F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"
#include "HapticSphere.h"

#include <gl/gl.h>
#include <gl/glu.h>

/** Represents a sphere shape */
class CSphere : public CShape
{
protected:
	/** GLU quadric to represent the sphere */
	GLUquadric* s;

	// Custom Haptic sphere
	HapticSphere* m_customSphere;

public:
	/** Create a new sphere of dimensions x, y, z */
	CSphere(float x,float y,float z);

	/** Destroy the sphere */
	virtual ~CSphere();

	/** Render the shape graphically via OpenGL */
	void drawGLScene();

	/** Return OpenGL code to render the shape */
	virtual CString drawGLCode();

	/** Render the shape haptically via OpenHaptics.
	    primary_context specifies either device 1 or 2 */
	void drawHapticScene(bool primary_context);

	/** Return an integer constant representing the type of shape */
	int getType();

	/** Return a pointer to a copy of this shape */
	CShape* clone();
};

#endif // !defined(AFX_SPHERE_H__30F1FF80_2E11_40FF_A366_06963A0A95F9__INCLUDED_)
