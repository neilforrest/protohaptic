// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Drawable.h
// Classes: CDrawable
// Purpose: Represents an object that can be rendered by OpenGL

#ifndef DRAWABLE
#define DRAWABLE

/** Represents an abstract object that can be rendered by OpenGL */
class CDrawable
{
public:
	/** Default constructor */
	CDrawable();
	
	/** Sets the colour of the object */
	virtual void setColour(float r, float g, float b);
	
	/** Returns the red component of the object */
	virtual float getRed();
	
	/** Returns the green component of the object */
	virtual float getGreen();
	
	/** Returns the blue component of the object */
	virtual float getBlue();
	
	/** Is overriden to perform OpenGL rendering */
	virtual void drawGLScene()= 0;
protected:
	float m_colourRed, m_colourGreen, m_colourBlue;
};

#endif
