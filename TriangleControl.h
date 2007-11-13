#pragma once

#include "Control.h"
#include "Cube.h"
#include "EditPoint.h"
#include "Triangle.h"

#include <gl/gl.h>
#include <gl/glu.h>

// User interface widget to transform a triangle
class CTriangleControl : public CControl
{
public:

	// Create a new triangle control for the given triangle
	CTriangleControl( CTriangle* triangle );

	// Destroy the triangle control
	virtual ~CTriangleControl(void);

	// Sets the control's colour scheme. Used to 
	// distinguish primary and secondary devices controls
	void setColourScheme(int cs);
	
	// Returns an integer constant representing the current
	//  control mode. See control.h for possible values.
	int getControlMode();

	// Overrides CDrawable in order to render the control graphically 
	void drawGLScene();
	
	// Overrides CFeelabe in order to render the control haptically 
	void drawHapticScene(bool primary_context);

protected:

	// Draw a line between a and b
	static void ConnectEditPoints(CEditPoint* a, CEditPoint* b);

	// Updates the position of the edit points to match the triangle under control
	void UpdateEditPoints ( );

	// Control points for vertices
	CEditPoint* editPointA;	// Vertex A
	CEditPoint* editPointB;	// Vertex B
	CEditPoint* editPointC;	// Vertex C
	
	// Control points for rotation
	CEditPoint* rotate0;
	CEditPoint* rotate1;
	CEditPoint* rotate2;
	CEditPoint* rotate3;
	CEditPoint* rotate4;
	CEditPoint* rotate5;

	// Control points for resizing
	// (only four because a triangle is always in the plane)
	CEditPoint* resize0;
	CEditPoint* resize1;
	CEditPoint* resize2;
	CEditPoint* resize3;

	// Triangle under control
	CTriangle* controlTriangle;
};
