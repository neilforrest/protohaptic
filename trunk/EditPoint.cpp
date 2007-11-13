// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: EditPoint.cpp
// Classes: CEditPoint
// Purpose: Represents a control point

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "EditPoint.h"

#include "ProtoHapticDoc.h"
#include "ProtoHapticView.h"

#include <mmsystem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEditPoint::CEditPoint()
{
	m_magnetic= true;
	q= gluNewQuadric();
	m_control= NULL;
	m_alertAquire= false;

	app= (CProtoHapticApp*)AfxGetApp();
}

CEditPoint::~CEditPoint()
{

}

CShape* CEditPoint::clone()
{
	return NULL;
}

int CEditPoint::getType() { return -1; }

void CEditPoint::drawGLScene()
{
	double color[4];
	glGetDoublev(GL_CURRENT_COLOR,color);

	if(touching() || touching1()){
		glColor3d(1.0,1.0,1.0);

		((CProtoHapticView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->showToolTip(m_tipText);
		((CProtoHapticView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->setToolTipVisable(true);
	}
	//else
		//((CProtoHapticView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->setToolTipVisable(false);

	glPushMatrix();
		glTranslatef(m_locationX,m_locationY,m_locationZ);
		gluSphere  ( q, ((CProtoHapticApp*)AfxGetApp())->getEditPointSize(), 20 , 20 );
	glPopMatrix();
	glColor3d(color[0],color[1],color[2]);

	hlCheckEvents();
}

void CEditPoint::drawHapticScene(bool primary_context)
{
	if(m_alertAquire!=touching() && app->isAudioEditPoint())
	{
		if(touching())
			PlaySound(EDIT_POINT_ON, NULL, SND_FILENAME | SND_ASYNC );
		//else
			//PlaySound(UNTOUCH_SHAPE, NULL, SND_FILENAME | SND_ASYNC );
		m_alertAquire= touching();
	}
	if(m_magnetic) {
		hlEnable(HL_PROXY_RESOLUTION);
		hlTouchModel(HL_CONSTRAINT);
		hlTouchModelf(HL_SNAP_DISTANCE, app->getEditPointSnapDist());
	}
	if(primary_context)
		hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID);
	else
		hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID_1);
	glPushMatrix();
	glTranslatef(m_locationX,m_locationY,m_locationZ);
	gluSphere  ( q, app->getEditPointSize(), 5 , 5 );
	glPopMatrix();
	hlEndShape();
	hlTouchModel(HL_CONTACT);
}

void CEditPoint::setMagnetic(bool magnetic)
{
	m_magnetic= magnetic;
}

void CEditPoint::setToolTip(CString s)
{
	m_tipText= s;
}

CString CEditPoint::drawGLCode()
{
	return "DrawEditPoint()\r\n";
}