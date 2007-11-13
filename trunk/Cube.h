// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Cube.h
// Classes: CCube
// Purpose: Represents a cube shape

#ifndef CUBE
#define CUBE

#include "Shape.h"
#include "Control.h"

/** Represents a cube shape */
class CCube : public CShape
{
public:
	/** Activate the cube. Called before it is added to the
	    scene and when it is removed */
	void setActive(bool active);
	
	/** Create a cube of the given dimensions */
	CCube(float x,float y,float z);
	
	/** Destroy the cube */
	virtual ~CCube();
	
	/** Overrides CDrawable in order to render the cube graphically */
	void drawGLScene();
	
	/** Overrides CShape in order to return OpenGL code for rendering the cube */
	virtual CString drawGLCode();
	
	/** Overrides CFeelable to render the cube haptically */
	void drawHapticScene(bool primary_context);
	
	/** Store/Load the cube's data to the archive ar */
	virtual void Serialize(CArchive& ar);
	
	/** Return a pointer to a copy of this cube */
	CShape* clone();
	
	/** Return an integer representing the type of shape (i.e. cube)
       See shape.h for possible values */
	int getType();
};

#endif
