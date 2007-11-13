// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Drawable.cpp
// Classes: CDrawable
// Purpose: Represents an object that can be rendered by OpenGL

#include "stdafx.h"
#include "Drawable.h"

void CDrawable::setColour(float r, float g, float b) 
{
	m_colourRed= r;
	m_colourGreen= g;
	m_colourBlue= b;
}

float CDrawable::getRed()
{
	return m_colourRed;
}

float CDrawable::getGreen()
{
	return m_colourGreen;
}

float CDrawable::getBlue()
{
	return m_colourBlue;
}

CDrawable::CDrawable()
{
	setColour(1.0,1.0,1.0);
}

void CDrawable::drawGLScene()
{
	//If this is called we've gone wrong
	OutputDebugString("Error: Can't drawGLScene from abstract class.");
	::MessageBox(::GetFocus(),"!","Error",MB_OK);
}