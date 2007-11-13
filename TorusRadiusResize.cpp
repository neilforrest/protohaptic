#include "stdafx.h"
#include "TorusRadiusResize.h"

/** Construct a new radius transformation on given shape.
    (x, y, z) is the starting position of the proxy */
CTorusRadiusResize::CTorusRadiusResize(CTorus *torus, HLdouble x, HLdouble y, HLdouble z)
{
	m_shape= torus;
	m_radiusStart= ((CTorus*)m_shape)->getRadius();
	m_proxyStartPos[0]= x;
	m_proxyStartPos[1]= y;
	m_proxyStartPos[2]= z;
}

/** Set the current position of the proxy.
    The shape under control will be transformed accordingly */
void CTorusRadiusResize::setProxyPos(HLdouble x, HLdouble y, HLdouble z)
{
	double A[3][3]= {
	{ m_shape->getRotation()[0], m_shape->getRotation()[1], m_shape->getRotation()[2] },
	{ m_shape->getRotation()[4], m_shape->getRotation()[5], m_shape->getRotation()[6] },
	{ m_shape->getRotation()[8], m_shape->getRotation()[9], m_shape->getRotation()[10] } };

	double   pV[3]= { x-m_proxyStartPos[0],
					  y-m_proxyStartPos[1],
					  z-m_proxyStartPos[2] };

	double   rV[3]= { pV[0]*A[0][0] + pV[1]*A[0][1]+ pV[2]*A[0][2],
		              pV[0]*A[1][0] + pV[1]*A[1][1]+ pV[2]*A[1][2],
					  pV[0]*A[2][0] + pV[1]*A[2][1]+ pV[2]*A[2][2] };

	float delta_ratio= -(float)(rV[0]*0.5/m_shape->getSizeX());
	float new_ratio= m_radiusStart+delta_ratio;

	static const float min= 0.001;
	static const float max= m_shape->getSizeY () / 4.0f;

	if(new_ratio<min) new_ratio= min;
	if(new_ratio>max) new_ratio= max;

	((CTorus*)m_shape)->setRadius(new_ratio);
}

/** Returns an integer constant representing the type of transformation (i.e. ratio)
    See Transform.h for a list of possible values */
int CTorusRadiusResize::getType()
{
	return TRANSFORM_TORUS_RADIUS_RESIZE;
}

/** Destroy the ratio transform */
CTorusRadiusResize::~CTorusRadiusResize()
{

}

/** Returns the shape being transformed */
CShape* CTorusRadiusResize::getShape()
{
	return m_shape;
}
