// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Move.cpp
// Classes: CMove
// Purpose: Represents move, or translation transformation

#include "stdafx.h"
#include "float.h"
#include "math.h"
#include "ProtoHaptic.h"
#include "Move.h"
#include "UsefullFunctions.h"

#include <mmsystem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define SNAP_DIST 0.2
#define CONSIDER_DIST 2

float max3(float a, float b, float c)
{
	if(a>b&&a>c) return a;
	if(b>a&&b>c) return b;
	else return c;
}

CMove::CMove(CShape *shape, HLdouble x, HLdouble y, HLdouble z, CProtoHapticDoc *doc)
{
	m_shape= shape;
	m_proxyStartPos[0]= x;
	m_proxyStartPos[1]= y;
	m_proxyStartPos[2]= z;
	m_shapeStart[0]= m_shape->getLocationX();
	m_shapeStart[1]= m_shape->getLocationY();
	m_shapeStart[2]= m_shape->getLocationZ();
	m_document= doc;
	m_snapto= false;
	for(int i= 0; i<12; i++)
		m_snapAlerts[i]= false;
}

int CMove::getType() { return TRANSFORM_TRANSLATE; }

void CMove::setProxyPos(HLdouble x, HLdouble y, HLdouble z)
{
	m_proxyNowPos[0]= x;
	m_proxyNowPos[1]= y;
	m_proxyNowPos[2]= z;
	
	float newPos[3]= {   (float)(m_shapeStart[0]+(x-m_proxyStartPos[0])),
						 (float)(m_shapeStart[1]+(y-m_proxyStartPos[1])),
						 (float)(m_shapeStart[2]+(z-m_proxyStartPos[2]))   };

	if(m_snapto)
	{
		float this_rightmost=   -FLT_MAX;;
		float this_leftmost=     FLT_MAX;;
		float this_topmost=     -FLT_MAX;;
		float this_bottommost=   FLT_MAX;;
		float this_frountmost=  -FLT_MAX;;
		float this_backmost=     FLT_MAX;;

		float centre[3]= { m_shape->getLocationX(), m_shape->getLocationY(), m_shape->getLocationZ() };
		float size[3]  = { (float)(m_shape->getSizeX()/2.0), 
						   (float)(m_shape->getSizeY()/2.0),
						   (float)(m_shape->getSizeZ()/2.0) }; 

		float box_corners[8][3]= 
		{
			{-size[0],+size[1],+size[2]},
			{+size[0],+size[1],+size[2]},
			{+size[0],-size[1],+size[2]},
			{-size[0],-size[1],+size[2]},
			{-size[0],+size[1],-size[2]},
			{+size[0],+size[1],-size[2]},
			{+size[0],-size[1],-size[2]},
			{-size[0],-size[1],-size[2]}
		};
		
		float r_box_corners[8][3];

		int i;
		for(i= 0; i<8; i++)
		{
			r_box_corners[i][0]= centre[0] + box_corners[i][0]*m_shape->getRotation()[0] + box_corners[i][1]*m_shape->getRotation()[4]+ box_corners[i][2]*m_shape->getRotation()[8];
		    r_box_corners[i][1]= centre[1] + box_corners[i][0]*m_shape->getRotation()[1] + box_corners[i][1]*m_shape->getRotation()[5]+ box_corners[i][2]*m_shape->getRotation()[9];
			r_box_corners[i][2]= centre[2] + box_corners[i][0]*m_shape->getRotation()[2] + box_corners[i][1]*m_shape->getRotation()[6]+ box_corners[i][2]*m_shape->getRotation()[10];
		}
		
		for(i= 0; i<8; i++)
		{
			if(r_box_corners[i][0]>this_rightmost) this_rightmost= r_box_corners[i][0];
			if(r_box_corners[i][0]<this_leftmost)  this_leftmost=  r_box_corners[i][0];
			if(r_box_corners[i][1]>this_topmost)     this_topmost=     r_box_corners[i][1];
			if(r_box_corners[i][1]<this_bottommost)  this_bottommost=  r_box_corners[i][1];
			if(r_box_corners[i][2]>this_frountmost) this_frountmost= r_box_corners[i][2];
			if(r_box_corners[i][2]<this_backmost)   this_backmost=   r_box_corners[i][2];

		}

		float r_size[3] = { size[0]*m_shape->getRotation()[0] + size[1]*m_shape->getRotation()[4] + size[2]*m_shape->getRotation()[8],
							size[0]*m_shape->getRotation()[1] + size[1]*m_shape->getRotation()[5] + size[2]*m_shape->getRotation()[9],
							size[0]*m_shape->getRotation()[2] + size[1]*m_shape->getRotation()[6] + size[2]*m_shape->getRotation()[10] };


		for(i= 0; i<m_document->GetShapeCount(); i++)
		{
			CShape *shape= m_document->GetShape(i);
			float size1[3]  = { (float)(shape->getSizeX()/2.0),
								(float)(shape->getSizeY()/2.0),
								(float)(shape->getSizeZ()/2.0) };

			float r_size1[3] = { size1[0]*shape->getRotation()[0] + size1[1]*shape->getRotation()[1] + size1[2]*shape->getRotation()[2],
								 size1[0]*shape->getRotation()[4] + size1[1]*shape->getRotation()[5] + size1[2]*shape->getRotation()[6],
								 size1[0]*shape->getRotation()[8] + size1[1]*shape->getRotation()[9] + size1[2]*shape->getRotation()[10] };


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
			r_box_corners1[i][0]= centre1[0] + box_corners1[i][0]*shape->getRotation()[0] + box_corners1[i][1]*shape->getRotation()[4]+ box_corners1[i][2]*shape->getRotation()[8];
		    r_box_corners1[i][1]= centre1[1] + box_corners1[i][0]*shape->getRotation()[1] + box_corners1[i][1]*shape->getRotation()[5]+ box_corners1[i][2]*shape->getRotation()[9];
			r_box_corners1[i][2]= centre1[2] + box_corners1[i][0]*shape->getRotation()[2] + box_corners1[i][1]*shape->getRotation()[6]+ box_corners1[i][2]*shape->getRotation()[10];
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

			if(shape!=m_shape &&

			(	   (this_rightmost>leftmost-CONSIDER_DIST &&
				    this_leftmost<rightmost+CONSIDER_DIST)
			)
			&&
			(
			       (this_topmost>bottommost-CONSIDER_DIST &&
				    this_bottommost<topmost+CONSIDER_DIST)
			)
			&&
			(
			       (this_backmost<frountmost+CONSIDER_DIST &&
				    this_frountmost>backmost-CONSIDER_DIST)
			))
			{

				bool alert= false;

				//snap along x-axis
				if(this_rightmost>leftmost-SNAP_DIST &&
				   this_rightmost<leftmost+SNAP_DIST)
				{
					newPos[0]= (float)(leftmost-(this_rightmost-this_leftmost)/2.0); //m_shape->getSizeX()/2,0;
					if(!m_snapAlerts[0]) { alert= true; m_snapAlerts[0]= true; }
				}

				if(this_leftmost>rightmost-SNAP_DIST &&
				   this_leftmost<rightmost+SNAP_DIST)
				{
					newPos[0]= rightmost+(this_rightmost-this_leftmost)/2,0;
					if(!m_snapAlerts[1]) { alert= true; m_snapAlerts[1]= true; }
				}
				
				if(this_rightmost>rightmost-SNAP_DIST &&
				   this_rightmost<rightmost+SNAP_DIST)
				{
					newPos[0]= rightmost-(this_rightmost-this_leftmost)/2,0;
					if(!m_snapAlerts[2]) { alert= true; m_snapAlerts[2]= true; }
				}

				if(this_leftmost>leftmost-SNAP_DIST &&
				   this_leftmost<leftmost+SNAP_DIST)
				{
					newPos[0]= leftmost+(this_rightmost-this_leftmost)/2,0;
					if(!m_snapAlerts[3]) { alert= true; m_snapAlerts[3]= true; }
				}

				//snap along y-axis
				if(this_topmost>bottommost-SNAP_DIST &&
				   this_topmost<bottommost+SNAP_DIST)
				{
					newPos[1]= bottommost-(this_topmost-this_bottommost)/2,0;
					if(!m_snapAlerts[4]) { alert= true; m_snapAlerts[4]= true; }
				}

				if(this_bottommost>topmost-SNAP_DIST &&
				   this_bottommost<topmost+SNAP_DIST)
				{
					newPos[1]= topmost+(this_topmost-this_bottommost)/2,0;
					if(!m_snapAlerts[5]) { alert= true; m_snapAlerts[5]= true; }
				}

				if(this_topmost>topmost-SNAP_DIST &&
				   this_topmost<topmost+SNAP_DIST)
				{
					newPos[1]= topmost-(this_topmost-this_bottommost)/2,0;
					if(!m_snapAlerts[6]) { alert= true; m_snapAlerts[6]= true; }
				}

				if(this_bottommost>bottommost-SNAP_DIST &&
				   this_bottommost<bottommost+SNAP_DIST)
				{
					newPos[1]= bottommost+(this_topmost-this_bottommost)/2,0;
					if(!m_snapAlerts[7]) { alert= true; m_snapAlerts[7]= true; }
				}

				//snap along z-axis
				if(this_frountmost>frountmost-SNAP_DIST &&
				   this_frountmost<frountmost+SNAP_DIST)
				{
					newPos[2]= frountmost-(this_frountmost-this_backmost)/2,0;
					if(!m_snapAlerts[8]) { alert= true; m_snapAlerts[8]= true; }
				}

				if(this_backmost>backmost-SNAP_DIST &&
				   this_backmost<backmost+SNAP_DIST)
				{
					newPos[2]= backmost+(this_frountmost-this_backmost)/2,0;
					if(!m_snapAlerts[9]) { alert= true; m_snapAlerts[9]= true; }
				}

				if(this_frountmost>backmost-SNAP_DIST &&
				   this_frountmost<backmost+SNAP_DIST)
				{
					newPos[2]= backmost-(this_frountmost-this_backmost)/2,0;
					if(!m_snapAlerts[10]) { alert= true; m_snapAlerts[10]= true; }
				}

				if(this_backmost>frountmost-SNAP_DIST &&
				   this_backmost<frountmost+SNAP_DIST)
				{
					newPos[2]= frountmost+(this_frountmost-this_backmost)/2,0;
					if(!m_snapAlerts[11]) { alert= true; m_snapAlerts[11]= true; }
				}

				if(alert && ((CProtoHapticApp*)AfxGetApp())->isAudioSnapTo()) {
					PlaySound(SNAP_TO, NULL, SND_FILENAME | SND_ASYNC );
				}
			}
		}
	}

	m_shape->setLocation(newPos[0],newPos[1],newPos[2]);
}

CMove::~CMove()
{

}

CShape* CMove::getShape()
{
	return m_shape;
}

void CMove::setSnapto(bool snapto)
{
	if(m_snapto!=snapto) {
		for(int i= 0; i<12; i++) {
			m_snapAlerts[i]= false;
			m_snapto= snapto;
		}
	}
}
