#pragma once
#include "shape.h"

class CTorus :
	public CShape
{
public:
	CTorus(void);
	virtual ~CTorus(void);

	/** Store/Load the torus's data to the archive ar */
	virtual void Serialize(CArchive& ar);

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

	/** Get radius of outer loop DEPRECIATED */
	float getRadius ( );

	/** Set radius of outer loop DEPRECIATED */
	void setRadius ( float r );

	/** Get radius of outer loop spine */
	float getRadiusMajor ( );

	/** Set radius of outer loop spine  */
	void setRadiusMajor ( float r );

	/** Get radius of outer loop */
	float getRadiusMinor ( );

	/** Set radius of outer loop */
	void setRadiusMinor ( float r );

	/** Get drawn fraction of outer loop */
	float getFraction ( );

	/** Set drawn fraction of outer loop */
	void setFraction ( float f );

protected:
	/** Radius of ring spine */
	float m_radiusMajor;

	/** Radius of ring */
	float m_radiusMinor;

	/** Fraction of complete right to draw */
	float m_drawnFraction;
};
