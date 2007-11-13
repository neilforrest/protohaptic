// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Resize.cpp
// Classes: CResize
// Purpose: Represents resizing or scaling transformation

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Resize.h"
#include "math.h"
#include "float.h"
#include "UsefullFunctions.h"

#include <mmsystem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define RESIZE_AXIS_X 0
#define RESIZE_AXIS_Y 1
#define RESIZE_AXIS_Z 2

#define CONSIDER_DIST_RESIZE 1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
	
#define SNAP_dist1 0.5
#define CONSIDER_dist1 1

float dist1(CShape* s, CShape* s1)
{
	float x= s->getLocationX()-s1->getLocationX();
	float y= s->getLocationY()-s1->getLocationY();
	float z= s->getLocationZ()-s1->getLocationZ();

	return (float)(sqrt(x*x+y*y+z*z));
}

float max31(float a, float b, float c)
{
	if(a>b&&a>c) return a;
	if(b>a&&b>c) return b;
	else return c;
}

CResize::CResize(CShape *shape, HLdouble x, HLdouble y, HLdouble z,CProtoHapticDoc *doc)
{
//	int m_axis;
	m_axis= -1;
//	bool m_snapto;
	m_snapto= false;
//	bool m_aspectFixed;
	m_aspectFixed= false;
//	bool m_orientationX, m_orientationY, m_orientationZ;
	m_orientationX= false; m_orientationY= false; m_orientationZ= false;

	for(int v= 0; v<6; v++)
		m_snapAlerts[v]= false;

	m_shape= shape;
	m_startSizeX= shape->getSizeX();
	m_startSizeY= shape->getSizeY();
	m_startSizeZ= shape->getSizeZ();
	m_startLocationX= shape->getLocationX();
	m_startLocationY= shape->getLocationY();
	m_startLocationZ= shape->getLocationZ();
	m_resizing=false;
	m_fixedaxis= false;

	//snap to corner

	m_shape->getCorners();
	int i;
	for(i= 0; i<8; i++)
	{
		if(  x<(m_shape->getCorner(i)[0]+0.2) && x>(m_shape->getCorner(i)[0]-0.2)
		  && y<(m_shape->getCorner(i)[1]+0.2) && y>(m_shape->getCorner(i)[1]-0.2)
		  && z<(m_shape->getCorner(i)[2]+0.2) && z>(m_shape->getCorner(i)[2]-0.2) )
		{
		  x= m_shape->getCorner(i)[0];
		  y= m_shape->getCorner(i)[1];
		  z= m_shape->getCorner(i)[2];
		}
	}

	m_proxyStartPos[0]= x;
	m_proxyStartPos[1]= y;
	m_proxyStartPos[2]= z;

	m_aspectFixed= false;
	m_document= doc;

	// Remember which axes are fixed
	m_fixedAxisX= false;
	m_fixedAxisY= false;
	m_fixedAxisZ= false;	// Defaults
}

// The axis specified will be fixed. E.g. Fix one axis for a 2d planar shape
void CResize::SetAxisFixed ( bool x, bool y, bool z )
{
	// Remember which axes are fixed
	m_fixedAxisX= x;
	m_fixedAxisY= y;
	m_fixedAxisZ= z;
}

int CResize::getType() { return TRANSFORM_RESIZE; }

void CResize::setOrientation(bool x, bool y, bool z)
{
	m_orientationX= x; m_orientationY= y; m_orientationZ= z;
}

#define PI 3.141592653589793238

float deg2rad(float deg)
{
	return (float)(deg*((2*PI)/360.0));
}

void CResize::setProxyPos(HLdouble x, HLdouble y, HLdouble z)
{

	float proxy_drag[3]= { (float)(x-m_proxyStartPos[0]),
						   (float)(y-m_proxyStartPos[1]),
						   (float)(z-m_proxyStartPos[2]) };

	float drag_dist= (float)sqrt( proxy_drag[0]*proxy_drag[0] +
						   proxy_drag[1]*proxy_drag[1] +
						   proxy_drag[2]*proxy_drag[2] );


	float p[3]= { (float)proxy_drag[0],
				  (float)proxy_drag[1],
				  (float)proxy_drag[2] };

	if(drag_dist>0.5&&m_resizing==false&&m_fixedaxis)
	{
		m_resizing= true;
		float n[3]= { m_shape->getRotation()[0], m_shape->getRotation()[1], m_shape->getRotation()[2] };
		//do projection
		float zy[3]= { p[0]*(n[1]*n[1]+ n[2]*n[2]) - p[1]*n[0]*n[1] -             p[2]*n[0]*n[2],
				      -p[0]*n[1]*n[0] +              p[1]*(n[0]*n[0]+n[2]*n[2]) - p[2]*n[1]*n[2],
					  -p[0]*n[0]*n[2] -              p[1]*n[1]*n[2] +             p[2]*(n[0]*n[0]+n[1]*n[1]) };
		float n1[3]= { m_shape->getRotation()[4], m_shape->getRotation()[5], m_shape->getRotation()[6] };
		//do projection
		float zx[3]= { p[0]*(n1[1]*n1[1]+ n1[2]*n1[2]) - p[1]*n1[0]*n1[1] -             p[2]*n1[0]*n1[2],
				      -p[0]*n1[1]*n1[0] +              p[1]*(n1[0]*n1[0]+n1[2]*n1[2]) - p[2]*n1[1]*n1[2],
					  -p[0]*n1[0]*n1[2] -              p[1]*n1[1]*n1[2] +               p[2]*(n1[0]*n1[0]+n1[1]*n1[1]) };
		float n2[3]= { m_shape->getRotation()[8], m_shape->getRotation()[9], m_shape->getRotation()[10] };
		//do projection
		float xy[3]= { p[0]*(n2[1]*n2[1]+ n2[2]*n2[2]) - p[1]*n2[0]*n2[1] -             p[2]*n2[0]*n2[2],
				      -p[0]*n2[1]*n2[0] +              p[1]*(n2[0]*n2[0]+n2[2]*n2[2]) - p[2]*n2[1]*n2[2],
					  -p[0]*n2[0]*n2[2] -              p[1]*n2[1]*n2[2] +               p[2]*(n2[0]*n2[0]+n2[1]*n2[1]) };
		float zy_length= (float)sqrt( zy[0]*zy[0] +
									  zy[1]*zy[1] +
						              zy[2]*zy[2] );
		float zx_length= (float)sqrt( zx[0]*zx[0] +
						              zx[1]*zx[1] +
						              zx[2]*zx[2] );
		float xy_length= (float)sqrt( xy[0]*xy[0] +
						              xy[1]*xy[1] +
						              xy[2]*xy[2] );
		if     (zy_length>xy_length&&zx_length>xy_length) m_axis= RESIZE_AXIS_Z;
		else if(zx_length>zy_length&&xy_length>zy_length) m_axis= RESIZE_AXIS_X;
		else                                              m_axis= RESIZE_AXIS_Y;
	
	}

	if(m_resizing||!m_fixedaxis)
	{

	float signX, signY, signZ;
	float offsetX, offsetY, offsetZ;

	signX= 1; signY= 1; signZ= 1;
	offsetX= 0; offsetY= 0; offsetZ= 0;
	if(!m_orientationX) {
		signX= -1;
	}
	if(!m_orientationY) {
		signY= -1;
	}
	if(!m_orientationZ) {
		signZ= -1;
	}

	double theta= 0.0;
	double sinTheta= sin(theta);
	double cosTheta= cos(theta);
	double sinmTheta= sin(-theta);
	double cosmTheta= cos(-theta);

	double A[3][3]= {
	{ m_shape->getRotation()[0], m_shape->getRotation()[1], m_shape->getRotation()[2] },
	{ m_shape->getRotation()[4], m_shape->getRotation()[5], m_shape->getRotation()[6] },
	{ m_shape->getRotation()[8], m_shape->getRotation()[9], m_shape->getRotation()[10] } };

	double B[3][3]= {
	{ m_shape->getRotation()[0], m_shape->getRotation()[4], m_shape->getRotation()[8] },
	{ m_shape->getRotation()[1], m_shape->getRotation()[5], m_shape->getRotation()[9] },
	{ m_shape->getRotation()[2], m_shape->getRotation()[6], m_shape->getRotation()[10] } };

	float newPos[3]= {(float)x, (float)y, (float)z};

	if(m_snapto)
	{
		//snap x,y,z to (a) shape(s)

		float size[3]  = { (float)(m_shape->getSizeX()/2.0), 
						   (float)(m_shape->getSizeY()/2.0),
						   (float)(m_shape->getSizeZ()/2.0) }; 

		float r_size[3] = { size[0]*m_shape->getRotation()[0] + size[1]*m_shape->getRotation()[1] + size[2]*m_shape->getRotation()[2],
							size[0]*m_shape->getRotation()[4] + size[1]*m_shape->getRotation()[5] + size[2]*m_shape->getRotation()[6],
							size[0]*m_shape->getRotation()[8] + size[1]*m_shape->getRotation()[9] + size[2]*m_shape->getRotation()[10] };

		int i;
		for(i= 0; i<m_document->GetShapeCount(); i++)
		{
			CShape *shape= m_document->GetShape(i);
			float size1[3]  = { (float)(shape->getSizeX()/2.0), 
								(float)(shape->getSizeY()/2.0),
								(float)(shape->getSizeZ()/2.0) }; 

			float r_size1[3] = { size1[0]*shape->getRotation()[0] + size1[1]*shape->getRotation()[1] + size1[2]*shape->getRotation()[2],
								 size1[0]*shape->getRotation()[4] + size1[1]*shape->getRotation()[5] + size1[2]*shape->getRotation()[6],
								 size1[0]*shape->getRotation()[8] + size1[1]*shape->getRotation()[9] + size1[2]*shape->getRotation()[10] };

			if(shape!=m_shape &&

				 distX(shape,m_shape) < positive(r_size1[0])+positive(r_size[0])+CONSIDER_DIST_RESIZE &&
				 distY(shape,m_shape) < positive(r_size1[1])+positive(r_size[1])+CONSIDER_DIST_RESIZE &&
				 distZ(shape,m_shape) < positive(r_size1[2])+positive(r_size[2])+CONSIDER_DIST_RESIZE

				)
			{
				
		float rightmost=   -FLT_MAX;;
		float leftmost=     FLT_MAX;;
		float topmost=     -FLT_MAX;;
		float bottommost=   FLT_MAX;;
		float frountmost=  -FLT_MAX;;
		float backmost=     FLT_MAX;;

		float centre1[3]= { shape->getLocationX(), shape->getLocationY(), shape->getLocationZ() };

		float box_corners1[8][3]= 
		{
			{-size1[0],+size1[1],+size1[2]},
			{+size1[0],+size1[1],+size1[2]},
			{+size1[0],-size1[1],+size1[2]},
			{-size1[0],-size1[1],+size1[2]},
			{-size1[0],+size1[1],-size1[2]},
			{+size1[0],+size1[1],-size1[2]},
			{+size1[0],-size1[1],-size1[2]},
			{-size1[0],-size1[1],-size1[2]}
		};
		
		float r_box_corners1[8][3];

		int i;
		for(i= 0; i<8; i++)
		{
			r_box_corners1[i][0]= centre1[0] + box_corners1[i][0]*shape->getRotation()[0] + box_corners1[i][1]*shape->getRotation()[1]+ box_corners1[i][2]*shape->getRotation()[2];
		    r_box_corners1[i][1]= centre1[1] + box_corners1[i][0]*shape->getRotation()[4] + box_corners1[i][1]*shape->getRotation()[5]+ box_corners1[i][2]*shape->getRotation()[6];
			r_box_corners1[i][2]= centre1[2] + box_corners1[i][0]*shape->getRotation()[8] + box_corners1[i][1]*shape->getRotation()[9]+ box_corners1[i][2]*shape->getRotation()[10];
		}
		
		for(i= 0; i<8; i++)
		{
			if(r_box_corners1[i][0]>rightmost) rightmost= r_box_corners1[i][0];
			if(r_box_corners1[i][0]<leftmost)  leftmost=  r_box_corners1[i][0];
			if(r_box_corners1[i][1]>topmost)     topmost=     r_box_corners1[i][1];
			if(r_box_corners1[i][1]<bottommost)  bottommost=  r_box_corners1[i][1];
			if(r_box_corners1[i][2]>frountmost) frountmost= r_box_corners1[i][2];
			if(r_box_corners1[i][2]<backmost)   backmost=   r_box_corners1[i][2];

		}

				bool alert= false;

				//snap along x-axis
				if(x>leftmost-SNAP_dist1 &&
				   x<leftmost+SNAP_dist1)
				{
					newPos[0]= leftmost;
					if(!m_snapAlerts[0]) { alert= true; m_snapAlerts[0]= true; }
				}
				if(x>rightmost-SNAP_dist1 &&
				   x<rightmost+SNAP_dist1)
				{
					newPos[0]= rightmost;
					if(!m_snapAlerts[1]) { alert= true; m_snapAlerts[1]= true; }
				}
				//snap along y-axis
				if(y>bottommost-SNAP_dist1 &&
				   y<bottommost+SNAP_dist1)
				{
					newPos[1]= bottommost;
					if(!m_snapAlerts[2]) { alert= true; m_snapAlerts[2]= true; }
				}
				if(y>topmost-SNAP_dist1 &&
				   y<topmost+SNAP_dist1)
				{
					newPos[1]= topmost;
					if(!m_snapAlerts[3]) { alert= true; m_snapAlerts[3]= true; }
				}
				//snap along z-axis
				if(z>backmost-SNAP_dist1 &&
				   z<backmost+SNAP_dist1)
				{
					newPos[2]= backmost;
					if(!m_snapAlerts[4]) { alert= true; m_snapAlerts[4]= true; }
				}
				if(z>frountmost-SNAP_dist1 &&
				   z<frountmost+SNAP_dist1)
				{
					newPos[2]= frountmost;
					if(!m_snapAlerts[5]) { alert= true; m_snapAlerts[5]= true; }
				}

				if(alert && ((CProtoHapticApp*)AfxGetApp())->isAudioSnapTo()) {
					PlaySound(SNAP_TO, NULL, SND_FILENAME | SND_ASYNC );
				}
		}
		}
	}

	double   pV[3]= { newPos[0]-m_proxyStartPos[0],
					  newPos[1]-m_proxyStartPos[1],
					  newPos[2]-m_proxyStartPos[2] };

	double   rV[3]= { pV[0]*A[0][0] + pV[1]*A[0][1]+ pV[2]*A[0][2],
		              pV[0]*A[1][0] + pV[1]*A[1][1]+ pV[2]*A[1][2],
					  pV[0]*A[2][0] + pV[1]*A[2][1]+ pV[2]*A[2][2] };

	double sizeX= signX*rV[0];
	double sizeY= signY*rV[1];
	double sizeZ= signZ*rV[2];

	if(m_aspectFixed)
	{
	   float mag_X, mag_Y, mag_Z;
	   if(sizeX<0) mag_X= (float)-sizeX; else mag_X= (float)sizeX;
	   if(sizeY<0) mag_Y= (float)-sizeY; else mag_Y= (float)sizeY;
	   if(sizeZ<0) mag_Z= (float)-sizeZ; else mag_Z= (float)sizeZ;

	   if(mag_X>mag_Y &&
		  mag_X>mag_Z )
	   {
		sizeX= sizeX;
		sizeY= (sizeX/m_startSizeX)*m_startSizeY;
		sizeZ= (sizeX/m_startSizeX)*m_startSizeZ;
	   }

	   if(mag_Y>mag_X &&
		  mag_Y>mag_Z )
	   {
		sizeX= sizeX;
		sizeY= (sizeX/m_startSizeX)*m_startSizeY;;
		sizeZ= (sizeX/m_startSizeX)*m_startSizeZ;;
	   }

	   if(mag_Z>mag_Y &&
		  mag_Z>mag_X )
	   {
		sizeX= sizeX;
		sizeY= (sizeX/m_startSizeX)*m_startSizeY;;
		sizeZ= (sizeX/m_startSizeX)*m_startSizeZ;;
	   }

	}

	if(m_axis==RESIZE_AXIS_X) { sizeY= 0; sizeZ= 0; }
	if(m_axis==RESIZE_AXIS_Y) { sizeX= 0; sizeZ= 0; }
	if(m_axis==RESIZE_AXIS_Z) { sizeX= 0; sizeY= 0; }

	if(m_startSizeX+sizeX<0) sizeX= -m_startSizeX;
	if(m_startSizeY+sizeY<0) sizeY= -m_startSizeY;
	if(m_startSizeZ+sizeZ<0) sizeZ= -m_startSizeZ;

//	double   sV[3]= { offsetX*sizeX*B[0][0] + offsetY*sizeY*B[0][1]+ offsetZ*sizeZ*B[0][2],
//		              offsetX*sizeX*B[1][0] + offsetY*sizeY*B[1][1]+ offsetZ*sizeZ*B[1][2],
//					  offsetX*sizeX*B[2][0] + offsetY*sizeY*B[2][1]+ offsetZ*sizeZ*B[2][2] };

	double   sV[3]= { (m_fixedAxisX ? 0 : signX*sizeX/2.0*B[0][0] ) + (m_fixedAxisY ? 0 : signY*sizeY/2.0*B[0][1] ) + (m_fixedAxisZ ? 0 : signZ*sizeZ/2.0*B[0][2] ),
		              (m_fixedAxisX ? 0 : signX*sizeX/2.0*B[1][0] ) + (m_fixedAxisY ? 0 : signY*sizeY/2.0*B[1][1] ) + (m_fixedAxisZ ? 0 : signZ*sizeZ/2.0*B[1][2] ),
					  (m_fixedAxisX ? 0 : signX*sizeX/2.0*B[2][0] ) + (m_fixedAxisY ? 0 : signY*sizeY/2.0*B[2][1] ) + (m_fixedAxisZ ? 0 : signZ*sizeZ/2.0*B[2][2] ) };

	m_shape->setLocation((float)(m_startLocationX+sV[0]),
	     			     (float)(m_startLocationY+sV[1]),
						 (float)(m_startLocationZ+sV[2]) );

	m_shape->setSize((float)(m_startSizeX  + (m_fixedAxisX ? 0 : sizeX)),
					 (float)(m_startSizeY  + (m_fixedAxisY ? 0 : sizeY)),
					 (float)(m_startSizeZ) + (m_fixedAxisZ ? 0 : sizeZ) );
	}

}

CResize::~CResize()
{

}

void CResize::setAspectFixed(bool b)
{
	m_aspectFixed= b;
}

void CResize::setSnapto(bool snapto)
{
	if(m_snapto!=snapto) {
		for(int v= 0; v<6; v++)
			m_snapAlerts[v]= false;
		m_snapto= snapto;
	}
}

void CResize::setFixedAxis(bool fixedaxis)
{
	m_fixedaxis= fixedaxis;
}
