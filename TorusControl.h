#pragma once
#include "spherecontrol.h"
#include "Torus.h"

class CTorusControl :
	public CSphereControl
{
public:
	CTorusControl(void);
	CTorusControl(CTorus* shape);
	virtual ~CTorusControl(void);

	/** Overrides CDrawable in order to render the control graphically */
	void drawGLScene();
	
	/** Overrides CFeelabe in order to render the control haptically */
	void drawHapticScene(bool primary_context);
	
	/** Return the current control mode 
	    See control.h for possible values */
	int getControlMode();

protected:

	/** The cylinder's ratio edit point */
	CEditPoint* m_radiusEdit;
	CEditPoint* m_radiusMinorEdit;
};
