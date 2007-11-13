#pragma once
#include "feelable.h"
#include "HapticDisk.h"

class CDisk :
	public CFeelable
{
public:
	CDisk(void);
	virtual ~CDisk(void);

	/** Override this to render the feelable object
	    If primary_context then render to context c
	    otherwise render to c_1 */
	virtual void drawHapticScene(bool primary_context);
};
