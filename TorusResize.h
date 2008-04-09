#pragma once
#include "Resize.h"

class CTorus;
class CProtoHapticDoc;

class CTorusResize :
	public CResize
{
public:
	/** Construct a new torus resize transformation on given torus.
	    (x, y, z) is the starting position of the proxy */
	CTorusResize(CTorus *torus, HLdouble x, HLdouble y, HLdouble z, CProtoHapticDoc* doc);

	/** Returns an integer constant representing the type of transformation (i.e. ratio)
	    See Transform.h for a list of possible values */
	int getType();

	/** Destroy the ratio transform */
	virtual ~CTorusResize();
};
