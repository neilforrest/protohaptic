// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Rotate.cpp
// Classes: CRotate
// Purpose: Represents a rotation transformation

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Rotate.h"
#include "math.h"
#include <gl/gl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define PI 3.14159

float rad2deg(float rad)
{
	return (float)(rad*(360.0/(2*PI)));
}

CRotate::CRotate(CShape *shape, HLdouble x, HLdouble y, HLdouble z, int handle_axis, int direction)
{
	m_proxyStartPos[0]= x; m_proxyStartPos[1]= y; m_proxyStartPos[2]= z;
	m_shape= shape;
	
	int i;
	for(i= 0; i<16; i++)
		m_startRotation[i]= m_shape->getRotation()[i];

	m_handle_axis= handle_axis;
	m_direction= direction;
	rotating= false;
	m_chunking= false;
}

int CRotate::getType() { return TRANSFORM_ROTATE; }

CRotate::~CRotate()
{

}

bool CRotate::hasAxis()
{
	return rotating;
}

int CRotate:: getAxis() 
{
	return m_axis;
}

int CRotate::getHandleAxis()
{
	return m_handle_axis;
}

static void normalise(float V[3])
{
	float l= (float)sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);

	V[0]= V[0] / l;
	V[1]= V[1] / l;
	V[2]= V[2] / l;
}

static void cross(float ans[3], float A[3], float B[3])
{
	ans[0]=	A[1]*B[2]-A[2]*B[1];
	ans[1]= A[2]*B[0]-A[0]*B[2];
	ans[2]= A[0]*B[1]-A[1]*B[0];
}

static void swap(float a[3], float b[3])
{
	float tmp[3]= { a[0], a[1], a[2] };
	a[0]= b[0]; a[1]= b[1]; a[2]= b[2];
	b[0]= tmp[0]; b[1]= tmp[1]; b[2]= tmp[2];
}

void CRotate::setProxyPos(HLdouble x, HLdouble y, HLdouble z)
{
//	float R[16]; //The rotation matrix we shall construct
//
//	float c[3]= { m_shape->getLocationX(),
//				  m_shape->getLocationY(),
//				  m_shape->getLocationZ() }; //centre of object
//
//	float p[3]= { x-c[0],
//				  y-c[1],
//				  z-c[2] }; //vector from object centre to proxy
//
//	float d= sqrt(p[0]*p[0] + 
//		          p[1]*p[1] + //length of p
//				  p[2]*p[2]); //used below
//
//	float u[3]= { p[0] / d,
//		          p[1] / d,
//				  p[2] / d }; //p normalised, i.e. as unit vector
//	
//	float l= sqrt(u[0]*u[0] + u[1]*u[1]); //length of u in xy-plane
//
//	if (l == 0.0) {
///	
//		R[0]= u[2];		R[4]= 0.0;		R[8]=  0.0;
///		R[1]= 0.0;		R[5]= 1.0;		R[9]=  0.0;
///		R[2]= 0.0;		R[6]= 0.0;		R[10]= u[2];
///	
//	} else {
///	
///		R[0]= (u[2]*u[0])/l;		R[4]= -u[1]/l;		R[8]=  u[0];
///		R[1]= (u[2]*u[1])/l;		R[5]=  u[0]/l;		R[9]=  u[1];
///		R[2]= -l;					R[6]= 0.0;			R[10]= u[2];
//
//	}
//
//	R[3] = 0.0;
//	R[7] = 0.0;
//	R[11]= 0.0;
//
//	R[12]= 0.0;
//	R[13]= 0.0;
//	R[14]= 0.0;
//	R[15]= 1.0;
//
//
//	m_shape->setRotation(R);
//
//Second try

	float proxy_drag[3]= { (float)(x-m_proxyStartPos[0]),
						   (float)(y-m_proxyStartPos[1]),
						   (float)(z-m_proxyStartPos[2]) };

	float drag_dist= (float)sqrt( proxy_drag[0]*proxy_drag[0] +
						          proxy_drag[1]*proxy_drag[1] +
						          proxy_drag[2]*proxy_drag[2] );

	float c[3]= { m_shape->getLocationX(),
				  m_shape->getLocationY(),
				  m_shape->getLocationZ() }; //centre of object

	float p[3]= { (float)(x-c[0]),
				  (float)(y-c[1]),
				  (float)(z-c[2]) }; //vector from object centre to proxy

	float max= ((CProtoHapticApp*)AfxGetApp())->getRotationGuess();

	if(drag_dist>max&&rotating==false)
	{
		rotating= true;
		float n[3]= { m_startRotation[0], m_startRotation[1], m_startRotation[2] };
		//do projection
		float zy[3]= { p[0]*(n[1]*n[1]+ n[2]*n[2]) - p[1]*n[0]*n[1] -             p[2]*n[0]*n[2],
				      -p[0]*n[1]*n[0] +              p[1]*(n[0]*n[0]+n[2]*n[2]) - p[2]*n[1]*n[2],
					  -p[0]*n[0]*n[2] -              p[1]*n[1]*n[2] +             p[2]*(n[0]*n[0]+n[1]*n[1]) };
		float n1[3]= { m_startRotation[4], m_startRotation[5], m_startRotation[6] };
		//do projection
		float zx[3]= { p[0]*(n1[1]*n1[1]+ n1[2]*n1[2]) - p[1]*n1[0]*n1[1] -             p[2]*n1[0]*n1[2],
				      -p[0]*n1[1]*n1[0] +              p[1]*(n1[0]*n1[0]+n1[2]*n1[2]) - p[2]*n1[1]*n1[2],
					  -p[0]*n1[0]*n1[2] -              p[1]*n1[1]*n1[2] +               p[2]*(n1[0]*n1[0]+n1[1]*n1[1]) };
		float n2[3]= { m_startRotation[8], m_startRotation[9], m_startRotation[10] };
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
		if     (zy_length>zx_length&&zy_length>xy_length) m_axis= ROTATE_AXIS_X;
		else if(zx_length>zy_length&&zx_length>xy_length) m_axis= ROTATE_AXIS_Y;
		else                                              m_axis= ROTATE_AXIS_Z;
	
	}

	if(rotating) {

	float R[16]; //The rotation matrix we shall construct

	if(m_chunking)
	{
		p[0]= chunk(p[0]);
		p[1]= chunk(p[1]);
		p[2]= chunk(p[2]);
		normalise(p);
	}

	//project p to the apropriate plane as pr
	float pr[3];

	if(m_axis==ROTATE_AXIS_X)
	{
		//we want the local zy-plane, normal to this is the local x-axis...
		float n[3]= { m_startRotation[0], m_startRotation[1], m_startRotation[2] };
		//do projection
		pr[0]=  p[0]*(n[1]*n[1]+ n[2]*n[2]) - p[1]*n[0]*n[1] -             p[2]*n[0]*n[2];
		pr[1]= -p[0]*n[1]*n[0] +              p[1]*(n[0]*n[0]+n[2]*n[2]) - p[2]*n[1]*n[2];
		pr[2]= -p[0]*n[0]*n[2] -              p[1]*n[1]*n[2] +             p[2]*(n[0]*n[0]+n[1]*n[1]);
	}
	if(m_axis==ROTATE_AXIS_Y)
	{
		//as above...
		float n[3]= { m_startRotation[4], m_startRotation[5], m_startRotation[6] };

		pr[0]=  p[0]*(n[1]*n[1]+ n[2]*n[2]) - p[1]*n[0]*n[1] -             p[2]*n[0]*n[2];
		pr[1]= -p[0]*n[1]*n[0] +              p[1]*(n[0]*n[0]+n[2]*n[2]) - p[2]*n[1]*n[2];
		pr[2]= -p[0]*n[0]*n[2] -              p[1]*n[1]*n[2] +             p[2]*(n[0]*n[0]+n[1]*n[1]);
	}
	if(m_axis==ROTATE_AXIS_Z)
	{
		//as above...
		float n[3]= { m_startRotation[8], m_startRotation[9], m_startRotation[10] };

		pr[0]=  p[0]*(n[1]*n[1]+ n[2]*n[2]) - p[1]*n[0]*n[1] -             p[2]*n[0]*n[2];
		pr[1]= -p[0]*n[1]*n[0] +              p[1]*(n[0]*n[0]+n[2]*n[2]) - p[2]*n[1]*n[2];
		pr[2]= -p[0]*n[0]*n[2] -              p[1]*n[1]*n[2] +             p[2]*(n[0]*n[0]+n[1]*n[1]);
	}
	

	normalise(pr);           //make unit vector


	float X[3], Y[3], Z[3];

	if(m_axis==ROTATE_AXIS_X){
		Z[0]= m_direction*pr[0]; Z[1]= m_direction*pr[1]; Z[2]= m_direction*pr[2]; //the new z axis
		X[0]= m_startRotation[0]; X[1]= m_startRotation[1]; X[2]= m_startRotation[2];

		Y[0]= -(X[1]*Z[2]-X[2]*Z[1]);
		Y[1]= -(X[2]*Z[0]-X[0]*Z[2]);
		Y[2]= -(X[0]*Z[1]-X[1]*Z[0]);

		normalise(Y);			//make unit vector

		if(m_handle_axis==ROTATE_AXIS_Y) {
			swap(Z,Y);
			Z[0]= -Z[0]; Z[1]= -Z[1]; Z[2]= -Z[2];
		}
	}
	if(m_axis==ROTATE_AXIS_Y){
		X[0]= m_direction*pr[0]; X[1]= m_direction*pr[1]; X[2]= m_direction*pr[2]; //the new z axis
		Y[0]= m_startRotation[4]; Y[1]= m_startRotation[5]; Y[2]= m_startRotation[6];

		Z[0]= -(Y[1]*X[2]-Y[2]*X[1]);
		Z[1]= -(Y[2]*X[0]-Y[0]*X[2]);
		Z[2]= -(Y[0]*X[1]-Y[1]*X[0]);

		normalise(Z);			//make unit vector

		if(m_handle_axis==ROTATE_AXIS_Z) {
			swap(Z,X);
			X[0]= -X[0]; X[1]= -X[1]; X[2]= -X[2];
		}
	}
	if(m_axis==ROTATE_AXIS_Z){
		Y[0]= m_direction*pr[0]; Y[1]= m_direction*pr[1]; Y[2]= m_direction*pr[2]; //the new z axis
		Z[0]= m_startRotation[8]; Z[1]= m_startRotation[9]; Z[2]= m_startRotation[10];

		X[0]= (Y[1]*Z[2]-Y[2]*Z[1]);
		X[1]= (Y[2]*Z[0]-Y[0]*Z[2]);
		X[2]= (Y[0]*Z[1]-Y[1]*Z[0]);

		normalise(X);			//make unit vector

		if(m_handle_axis==ROTATE_AXIS_X) {
			swap(X,Y);
			Y[0]= -Y[0]; Y[1]= -Y[1]; Y[2]= -Y[2];
		}
	}


	//NEW STUFF///////////////////////////////////////

		R[0]= X[0];	R[4]= Y[0];	R[8]=  Z[0];
		R[1]= X[1];	R[5]= Y[1];	R[9]=  Z[1];
		R[2]= X[2];	R[6]= Y[2];	R[10]= Z[2];


	R[3] = 0.0;
	R[7] = 0.0;
	R[11]= 0.0;

	R[12]= 0.0;
	R[13]= 0.0;
	R[14]= 0.0;
	R[15]= 1.0;

	m_shape->setRotation(R);
	}
}

void CRotate::setChunking(bool chunking)
{
	m_chunking= chunking;
}

float CRotate::chunk(float f)
{
	if(f>0.0&&f<0.5)  return 0.0;
	if(f<0.0&&f>-0.5) return 0.0;
	if(f>0.0&&f>0.5)  return 1.0;
	if(f<0.0&&f<-0.5) return -1.0;
	else return -1000;	//To shut the compiler up!
}
