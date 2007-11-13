// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Cylinder.h
// Classes: CCylinder
// Purpose: Represents cylinder shape

#if !defined(AFX_CYLINDER_H__A05BC135_C666_47C0_B32C_831B3DA364D8__INCLUDED_)
#define AFX_CYLINDER_H__A05BC135_C666_47C0_B32C_831B3DA364D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"
#include "HapticCylinder.h"
#include "HapticDisk.h"
#include "Disk.h"

#include <gl/gl.h>
#include <gl/glu.h>

/** Represents a cylinder shape */
class CCylinder : public CShape
{
private:
	/** The ratio of one end cap to the other */
	float m_ratio;

	/** Cylinder and disk quadrics */
	GLUquadric *s, *d1, *d2;

public:
	/** Create a cylinder of the given dimensions */
	CCylinder(float x,float y,float z);

	~CCylinder ( );;
	
	/** Overrides CDrawable in order to render the cylinder graphically */
	void drawGLScene();
	
	/** Overrides CFeelable in order to render the cylinder haptically */
	void drawHapticScene(bool primary_context);
	
	/** Overrides CShape in order to return OpenGL rendering code */
	virtual CString drawGLCode();

	/** Set the ratio of one end cap to the other */
	void setRatio(float ratio);
	
	/** Get the current ratio of one end cap to the other */
	float getRatio();
	/** Return an integer representing the type of shape (i.e cylinder)
	    See Shape.h for possible values */
	int getType();
	
	/** Store/Load the cylinder's data to the archive ar */
	virtual void Serialize(CArchive& ar);
	
	/** Return a pointer to a copy of this cylinder */
	CShape* clone();

	// Return ptr to the feelable object representing disks
	CDisk* GetDiskA ();
	CDisk* GetDiskB ();

protected:
	CHapticCylinder* customCylinder;
	CHapticDisk* diskA, *diskB;
	hduMatrix* diskATrans;
	hduMatrix* diskBTrans;
	CDisk* diskAShape;
	CDisk* diskBShape;
};

#endif // !defined(AFX_CYLINDER_H__A05BC135_C666_47C0_B32C_831B3DA364D8__INCLUDED_)
