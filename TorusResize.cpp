#include "stdafx.h"
#include "TorusResize.h"
#include "Torus.h"
#include "ProtoHapticDoc.h"

/** Construct a new torus resize transformation on given torus.
    (x, y, z) is the starting position of the proxy */
CTorusResize::CTorusResize(CTorus *torus, HLdouble x, HLdouble y, HLdouble z, CProtoHapticDoc* doc)
: CResize ( torus, x, y, z, doc )
{
	CTorus* t= (CTorus*)torus;

	float xySize= 2.0 * ( t->getRadiusMajor() + t->getRadiusMinor() );
	float zSize = 2.0 * ( t->getRadiusMinor() );

	setResizeBoxDimentions ( xySize, xySize, zSize );
}

/** Returns an integer constant representing the type of transformation (i.e. ratio)
    See Transform.h for a list of possible values */
int CTorusResize::getType()
{
	return TRANSFORM_TORUS_RESIZE;
}

/** Destroy the ratio transform */
CTorusResize::~CTorusResize()
{

}