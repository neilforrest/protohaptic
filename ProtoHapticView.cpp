// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: ProtoHapticView.cpp
// Classes: CProtoHapticView
// Purpose: Represents view onto a document or model

#include "stdafx.h"
#include "ProtoHaptic.h"

#include <windows.h>
#include <math.h>

#include "ProtoHapticDoc.h"
#include "ProtoHapticView.h"
#include "Move.h"
#include "Resize.h"
#include "Rotate.h"
#include "Ratio.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Composite.h"
#include "Torus.h"
#include "Memo.h"
#include "ShapePropertiesDlg.h"
#include "MemoDlg.h"
#include "DocumentOptionsDlg.h"
#include "AppOptionsDlg.h"
#include "Control.h"
#include "CodeView.h"
#include "TriVertexMove.h"
#include "TorusRadiusResize.h"

// OpenGL Headers
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

// OpenHaptics Headers
#include <HL/hl.h>
#include <HLU/hlu.h>
#include <HapticMouse/HapticMouse.h>

#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>

#include <vector>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==============================================================================

static hduVector3Dd gCameraPosWC;
static int gWindowWidth, gWindowHeight;

#define CURSOR_SIZE_PIXELS 20
static double gCursorScale;
static GLint bob = 0;

/* Variables used by the trackball emulation. */
static hduMatrix gCameraRotation;
static double gCameraScale = 1.0;
static double gCameraTranslationX = 0;
static double gCameraTranslationY = 0;
static bool gIsRotatingCamera = false;
static bool gIsScalingCamera = false;
static bool gIsTranslatingCamera = false;
static int gLastMouseX, gLastMouseY;

static bool gIsProxyManipPoint = false;

static const double kPI = 3.1415926535897932384626433832795;

bool mouseOn= false;

// ==============================================================================

#define FLY_TO_STEPS 20

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticView

IMPLEMENT_DYNCREATE(CProtoHapticView, CView)

BEGIN_MESSAGE_MAP(CProtoHapticView, CView)
	//{{AFX_MSG_MAP(CProtoHapticView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON32771, OnSelectTool)
	ON_COMMAND(ID_BUTTON32772, OnCubeTool)
	ON_COMMAND(ID_BUTTON32777, OnSphereTool)
	ON_COMMAND(ID_BUTTON32778, OnCylinderTool)
	ON_COMMAND(ID_BUTTON_TRI, OnTriangleTool)
	ON_COMMAND(ID_BUTTON_TORUS, OnTorusTool)
	ON_COMMAND(ID_TOOL_TRIANGLE, OnTriangleTool)
	ON_COMMAND(ID_TOOL_TORUS, OnTorusTool)
	ON_COMMAND(ID_EDIT_SHAPEPROPERTIES, OnShapeProperties)
	ON_COMMAND(ID_VIEW_EDITMODE, OnEditMode)
	ON_COMMAND(ID_VIEW_VIEWMODE, OnViewMode)
	ON_COMMAND(ID_BUTTONCENTER, OnCenter)
	ON_COMMAND(ID_BUTTONEDIT, OnButtonEdit)
	ON_COMMAND(ID_BUTTONVIEW, OnButtonView)
	ON_COMMAND(ID_BUTTON32785, OnGroup)
	ON_COMMAND(ID_BUTTON32786, OnUnGroup)
	ON_COMMAND(ID_BUTTONDELETE, OnDelete)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_EDIT_COPY, OnEditbutton)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_VIEW_RESTORECAMERA, OnRestoreCamera)
	ON_COMMAND(ID_BUTTONMEMO, OnMemo)
	ON_COMMAND(ID_BUTTONGOTO, OnGotoMemo)
	ON_COMMAND(ID_EDIT_DOCOPTIONS, OnDocumentOptions)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TRI, OnUpdateTriangleButton)
	ON_UPDATE_COMMAND_UI(ID_TOOL_TRIANGLE, OnUpdateTriangleTool)
	ON_UPDATE_COMMAND_UI(ID_TOOL_TORUS, OnUpdateTorusTool)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TORUS, OnUpdateTorusButton)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32771, OnUpdateButton32771)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32772, OnUpdateButton32772)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32777, OnUpdateButton32777)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32778, OnUpdateButton32778)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32785, OnUpdateButton32785)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32786, OnUpdateButton32786)
	ON_UPDATE_COMMAND_UI(ID_BUTTONDELETE, OnUpdateButtondelete)
	ON_UPDATE_COMMAND_UI(ID_BUTTONMEMO, OnUpdateButtonmemo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DOCOPTIONS, OnUpdateEditDocoptions)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHAPEPROPERTIES, OnUpdateEditShapeproperties)
	ON_UPDATE_COMMAND_UI(ID_TOOL_CUBE, OnUpdateToolCube)
	ON_UPDATE_COMMAND_UI(ID_TOOL_CYLINDERCONE, OnUpdateToolCylindercone)
	ON_UPDATE_COMMAND_UI(ID_TOOL_DELETESHAPE, OnUpdateToolDeleteshape)
	ON_UPDATE_COMMAND_UI(ID_TOOL_GROUPSHAPES, OnUpdateToolGroupshapes)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SELECTIONTOOL, OnUpdateToolSelectiontool)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SPHERE, OnUpdateToolSphere)
	ON_UPDATE_COMMAND_UI(ID_TOOL_UNGROUPSHAPE, OnUpdateToolUngroupshape)
	ON_UPDATE_COMMAND_UI(ID_BUTTONEDIT, OnUpdateButtonedit)
	ON_UPDATE_COMMAND_UI(ID_BUTTONVIEW, OnUpdateButtonview)
	ON_COMMAND(ID_ACCEL_CUBE, OnCubeTool)
	ON_COMMAND(ID_ACCEL_CYLINDER, OnCylinderTool)
	ON_COMMAND(ID_ACCEL_DELETE, OnDelete)
	ON_COMMAND(ID_ACCEL_GROUP, OnGroup)
	ON_COMMAND(ID_ACCEL_SPHERE, OnSphereTool)
	ON_COMMAND(ID_ACCEL_UNGROUP, OnUnGroup)
	ON_COMMAND(ID_TOOL_CUBE, OnCubeTool)
	ON_COMMAND(ID_TOOL_CYLINDERCONE, OnCylinderTool)
	ON_COMMAND(ID_TOOL_DELETESHAPE, OnDelete)
	ON_COMMAND(ID_TOOL_GROUPSHAPES, OnGroup)
	ON_COMMAND(ID_TOOL_SPHERE, OnSphereTool)
	ON_COMMAND(ID_TOOL_UNGROUPSHAPE, OnUnGroup)
	ON_COMMAND(ID_ACCEL_EDITMODE, OnEditMode)
	ON_COMMAND(ID_ACCEL_VIEWMODE, OnViewMode)
	ON_COMMAND(ID_ACCEL_CENTER, OnCenter)
	ON_COMMAND(ID_ACCEL_SELECT, OnSelectTool)
	ON_COMMAND(ID_VIEW_CENTERCAMERA, OnCenter)
	ON_COMMAND(ID_TOOL_SELECTIONTOOL, OnSelectTool)
	ON_COMMAND(ID_ACCEL_SHAPEPROPERTIES, OnShapeProperties)
	ON_COMMAND(ID_EDIT_APPOPTIONS, OnAppOptions)
	ON_UPDATE_COMMAND_UI(ID_EDIT_APPOPTIONS, OnUpdateEditAppoptions)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_TOOL_GROUPALL, &CProtoHapticView::OnToolGroupall)
END_MESSAGE_MAP()

void HLCALLBACK onButtonDown(HLenum event, HLuint object,
								   HLenum thread,
								   HLcache *cache,
								   void *userdata)
{
	((CProtoHapticView*)userdata)->m_goingToMemo= false;

	if(!((CProtoHapticView*)userdata)->m_edit_mode) return;

	if(!((CProtoHapticView*)userdata)->isHapticMouseOn()) return;

	HHLRC rc= hlGetCurrentContext();
	hlMakeCurrent(((CProtoHapticView*)userdata)->m_hHLRC);

	HLdouble pp[3];
	hlGetDoublev(HL_PROXY_POSITION, pp);

	hlMakeCurrent(rc);

	((CProtoHapticView*)userdata)->setDragging(true);
	((CProtoHapticView*)userdata)->setDragStart(pp);
	
	if(((CProtoHapticView*)userdata)->getTool()!=TOOL_SELECT)
	{
		((CProtoHapticView*)userdata)->GetDocument()->saveState();

		int shape;
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_CUBE)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CCube(1,1,1));
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_SPHERE)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CSphere(1,1,1));
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_CYLINDER)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CCylinder(1,1,1));
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_TRIANGLE)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CTriangle());
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_TORUS)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CTorus());
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_PASTE)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(((CProtoHapticView*)userdata)->getClipboardShape());
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_MEMO)
		{
			shape= ((CProtoHapticView*)userdata)->GetDocument()->addMemo(new CMemo(((CProtoHapticView*)userdata)->m_newMemoName));
			(((CProtoHapticView*)userdata)->GetDocument()->getMemo(shape))->setLocation(pp[0],pp[1],pp[2]);
		} else
			(((CProtoHapticView*)userdata)->GetDocument()->GetShape(shape))->setLocation(pp[0],pp[1],pp[2]);

		// Give the new shape a default name
		CString name= ((CProtoHapticView*)userdata)->GetDocument ()->GetFreeShapeName ();
		((CProtoHapticView*)userdata)->GetDocument ()->GetShape (shape)->SetName ( name );

		((CProtoHapticView*)userdata)->setTool(TOOL_SELECT);
	}

	if(((CProtoHapticView*)userdata)->getSelectedShape()!=NULL)
	{
		CShape* shape= ((CProtoHapticView*)userdata)->getSelectedShape();
		if ( shape->getControl()==NULL ) { 
			OutputDebugString("ERROR"); 
		}
		int mode= shape->getControl()->getControlMode();

		if(mode!=CONTROL_MODE_NULL) ((CProtoHapticView*)userdata)->GetDocument()->saveState();

		if(mode==CONTROL_MODE_MOVE)
		{
			((CProtoHapticView*)userdata)->m_transform= new CMove(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
		}	
		if(mode==CONTROL_MODE_RESIZE0)
		{
			((CProtoHapticView*)userdata)->m_transform= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform)->setOrientation(false,false,false);
		}
		if(mode==CONTROL_MODE_RESIZE1)
		{
			((CProtoHapticView*)userdata)->m_transform= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform)->setOrientation(true,false,false);
		}
		if(mode==CONTROL_MODE_RESIZE2)
		{
			((CProtoHapticView*)userdata)->m_transform= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform)->setOrientation(true,true,false);
		}
		if(mode==CONTROL_MODE_RESIZE3)
		{
			((CProtoHapticView*)userdata)->m_transform= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform)->setOrientation(false,true,false);
		}
		if(mode==CONTROL_MODE_RESIZE4)
		{
			((CProtoHapticView*)userdata)->m_transform= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform)->setOrientation(false,false,true);
		}
		if(mode==CONTROL_MODE_RESIZE5)
		{
			((CProtoHapticView*)userdata)->m_transform= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform)->setOrientation(true,false,true);
		}
		if(mode==CONTROL_MODE_RESIZE6)
		{
			((CProtoHapticView*)userdata)->m_transform= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform)->setOrientation(true,true,true);
		}
		if(mode==CONTROL_MODE_RESIZE7)
		{
			((CProtoHapticView*)userdata)->m_transform= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform)->setOrientation(false,true,true);
		}
		if(mode==CONTROL_MODE_ROTATE0)
		{
			((CProtoHapticView*)userdata)->m_transform= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_Y,-1);
		}
		if(mode==CONTROL_MODE_ROTATE1)
		{
			((CProtoHapticView*)userdata)->m_transform= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_Z,-1);
		}
		if(mode==CONTROL_MODE_ROTATE4)
		{
			((CProtoHapticView*)userdata)->m_transform= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_Z,1);
		}
		if(mode==CONTROL_MODE_ROTATE3)
		{
			((CProtoHapticView*)userdata)->m_transform= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_Y,1);
		}
		if(mode==CONTROL_MODE_ROTATE2)
		{
			((CProtoHapticView*)userdata)->m_transform= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_X,-1);
		}
		if(mode==CONTROL_MODE_ROTATE5)
		{
			((CProtoHapticView*)userdata)->m_transform= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_X,1);
		}
		if(mode==CONTROL_MODE_RATIO)
		{
			((CProtoHapticView*)userdata)->m_transform= new CRatio(shape,pp[0],pp[1],pp[2]);
		}

		// Torus specific control modes
		if (mode==CONTROL_MODE_TORUS_RADIUS)
		{
			((CProtoHapticView*)userdata)->m_transform= new CTorusRadiusResize((CTorus*)shape,
																		        pp[0],pp[1],pp[2] );
		}

		// Triangle specific control modes
		if(mode==CONTROL_MODE_TRI_VERTEX_A)		// Move vertex A
		{
			((CProtoHapticView*)userdata)->m_transform= new CTriVertexMove((CTriangle*)shape,
																		    pp[0],pp[1],pp[2],
																			((CProtoHapticView*)userdata)->GetDocument(),
																			1);	// Vertex 1
		}
		if(mode==CONTROL_MODE_TRI_VERTEX_B)		// Move vertex B
		{
			((CProtoHapticView*)userdata)->m_transform= new CTriVertexMove((CTriangle*)shape,
																		    pp[0],pp[1],pp[2],
																			((CProtoHapticView*)userdata)->GetDocument(),
																			2); // Vertex 2
		}
		if(mode==CONTROL_MODE_TRI_VERTEX_C)		// Move vertex C
		{
			((CProtoHapticView*)userdata)->m_transform= new CTriVertexMove((CTriangle*)shape,
																		    pp[0],pp[1],pp[2],
																			((CProtoHapticView*)userdata)->GetDocument(),
																			3); // Vertex 3
		}

		if(mode==CONTROL_MODE_NULL)
		{
			((CProtoHapticView*)userdata)->m_deselect= true;
		}

		// If transform is re-size and shape is triangle
		if ( ((CProtoHapticView*)userdata)->m_transform !=NULL &&
			 ((CProtoHapticView*)userdata)->m_transform->getType () == TRANSFORM_RESIZE
			                                   && shape->getType () == SHAPE_TRIANGLE )
		{
			// Fix the z-axis as it is, because the triangle is flat in the xy plane
			((CResize*)((CProtoHapticView*)userdata)->m_transform)->SetAxisFixed ( false, false, true );
		}
	}	
}

void HLCALLBACK onButtonDown1(HLenum event, HLuint object,
								   HLenum thread,
								   HLcache *cache,
								   void *userdata)
{
	((CProtoHapticView*)userdata)->m_goingToMemo= false;

	if(!((CProtoHapticView*)userdata)->m_edit_mode) return;

	if(!((CProtoHapticView*)userdata)->isHapticMouseOn()) return;

	HHLRC rc= hlGetCurrentContext();
	hlMakeCurrent(((CProtoHapticView*)userdata)->m_hHLRC_1);


	HLdouble pp[3];
	hlGetDoublev(HL_PROXY_POSITION, pp);

	hlMakeCurrent(rc);

	((CProtoHapticView*)userdata)->setDragging1(true);
	((CProtoHapticView*)userdata)->setDragStart1(pp);
	
	if(((CProtoHapticView*)userdata)->getTool()!=TOOL_SELECT)
	{
		((CProtoHapticView*)userdata)->GetDocument()->saveState();

		int shape;
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_CUBE)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CCube(1,1,1));
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_SPHERE)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CSphere(1,1,1));
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_CYLINDER)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CCylinder(1,1,1));
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_TRIANGLE)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CTriangle());
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_TORUS)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(new CTorus());
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_PASTE)
			shape= ((CProtoHapticView*)userdata)->GetDocument()->AddShape(((CProtoHapticView*)userdata)->getClipboardShape());
		if(((CProtoHapticView*)userdata)->getTool()==TOOL_MEMO)
		{
			shape= ((CProtoHapticView*)userdata)->GetDocument()->addMemo(new CMemo(((CProtoHapticView*)userdata)->m_newMemoName));
			(((CProtoHapticView*)userdata)->GetDocument()->getMemo(shape))->setLocation(pp[0],pp[1],pp[2]);
		} else
			(((CProtoHapticView*)userdata)->GetDocument()->GetShape(shape))->setLocation(pp[0],pp[1],pp[2]);

		// Give the new shape a default name
		CString name= ((CProtoHapticView*)userdata)->GetDocument ()->GetFreeShapeName ();
		((CProtoHapticView*)userdata)->GetDocument ()->GetShape (shape)->SetName ( name );

		((CProtoHapticView*)userdata)->setTool(TOOL_SELECT);
	}

	if(((CProtoHapticView*)userdata)->getSelectedShape1()!=NULL)
	{
		CShape* shape= ((CProtoHapticView*)userdata)->getSelectedShape1();
		if ( shape->getControl()==NULL ) { 
			OutputDebugString("ERROR"); 
		}
		int mode= shape->getControl()->getControlMode();

		if(mode!=CONTROL_MODE_NULL) ((CProtoHapticView*)userdata)->GetDocument()->saveState();

		if(mode==CONTROL_MODE_MOVE)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CMove(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
		}	
		if(mode==CONTROL_MODE_RESIZE0)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform1)->setOrientation(false,false,false);
		}
		if(mode==CONTROL_MODE_RESIZE1)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform1)->setOrientation(true,false,false);
		}
		if(mode==CONTROL_MODE_RESIZE2)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform1)->setOrientation(true,true,false);
		}
		if(mode==CONTROL_MODE_RESIZE3)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform1)->setOrientation(false,true,false);
		}
		if(mode==CONTROL_MODE_RESIZE4)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform1)->setOrientation(false,false,true);
		}
		if(mode==CONTROL_MODE_RESIZE5)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform1)->setOrientation(true,false,true);
		}
		if(mode==CONTROL_MODE_RESIZE6)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform1)->setOrientation(true,true,true);
		}
		if(mode==CONTROL_MODE_RESIZE7)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CResize(shape,pp[0],pp[1],pp[2],((CProtoHapticView*)userdata)->GetDocument());
			((CResize*)((CProtoHapticView*)userdata)->m_transform1)->setOrientation(false,true,true);
		}
		if(mode==CONTROL_MODE_ROTATE0)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_Y,-1);
		}
		if(mode==CONTROL_MODE_ROTATE1)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_Z,-1);
		}
		if(mode==CONTROL_MODE_ROTATE4)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_Z,1);
		}
		if(mode==CONTROL_MODE_ROTATE3)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_Y,1);
		}
		if(mode==CONTROL_MODE_ROTATE2)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_X,-1);
		}
		if(mode==CONTROL_MODE_ROTATE5)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CRotate(shape,pp[0],pp[1],pp[2],ROTATE_AXIS_X,1);
		}
		if(mode==CONTROL_MODE_RATIO)
		{
			((CProtoHapticView*)userdata)->m_transform1= new CRatio(shape,pp[0],pp[1],pp[2]);
		}
		if(mode==CONTROL_MODE_NULL)
		{
			((CProtoHapticView*)userdata)->m_deselect1= true;
		}
	}	
}

void HLCALLBACK onButtonUp(HLenum event, HLuint object,
								   HLenum thread,
								   HLcache *cache,
								   void *userdata)
{
	if(((CProtoHapticView*)userdata)->m_transform!=NULL)
	{
		delete ((CProtoHapticView*)userdata)->m_transform;
		((CProtoHapticView*)userdata)->m_transform= NULL;
	}
	((CProtoHapticView*)userdata)->setDragging(false);
}

void HLCALLBACK onButtonUp1(HLenum event, HLuint object,
								   HLenum thread,
								   HLcache *cache,
								   void *userdata)
{
	if(((CProtoHapticView*)userdata)->m_transform1!=NULL)
	{
		delete ((CProtoHapticView*)userdata)->m_transform1;
		((CProtoHapticView*)userdata)->m_transform1= NULL;
	}
	((CProtoHapticView*)userdata)->setDragging1(false);
}

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticView construction/destruction

CProtoHapticView::CProtoHapticView()
{
	hapticOffset[0]= 0.0;
	hapticOffset[1]= 0.0;
	hapticOffset[2]= 0.0;

	m_selectedShapes = new set<CShape*>();
	m_selectedShapes1 = new set<CShape*>();
	m_showToolTip= false;
	m_toolTipOn= false;
	m_flyToShape= NULL;
}

CProtoHapticView::~CProtoHapticView()
{
	OutputDebugString("Destroying Haptic View");

	hmShutdownMouse ( );

	delete m_selectedShapes;
	delete m_selectedShapes1;
}

BOOL CProtoHapticView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC, 
				   ::LoadCursor(NULL, IDC_ARROW), NULL, NULL); 
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs); 
}

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticView drawing

void CProtoHapticView::OnDraw(CDC* pDC)
{
	CProtoHapticDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hglrc);
	//DrawGLViewports();
}

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticView diagnostics

#ifdef _DEBUG
void CProtoHapticView::AssertValid() const
{
	CView::AssertValid();
}

void CProtoHapticView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProtoHapticDoc* CProtoHapticView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProtoHapticDoc)));
	return (CProtoHapticDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticView message handlers

int CProtoHapticView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hgldc = ::GetDC(m_hWnd);

    if(!SetPixelformat(m_hgldc))
    {
	::MessageBox(::GetFocus(),"SetPixelformat Failed!","Error",MB_OK);
	return -1;
    }

    m_hglrc = wglCreateContext(m_hgldc);
    int i= wglMakeCurrent(m_hgldc,m_hglrc);

	InitGL();

	((CProtoHapticApp*)AfxGetApp())->LoadOptions();

	if(!InitHaptics())
	{
		::MessageBox(::GetFocus(),"Error: Failed to Initialise Haptics.\n","Error",MB_OK);
		//exit(0);
	}

	gCameraTranslationZ= 0;

	m_selectedShapes->clear();
	m_selectedShapes1->clear();
	m_transform= NULL;
	m_transform1= NULL;
	m_edit_mode= true;
	setTool(TOOL_SELECT);
	shapeDlg.Create(IDD_SHAPE,NULL);
	shapeDlg.setShape(NULL);
	m_gotoDlg.Create(IDD_GOTO_MEMO);
	m_gotoDlg.setView(this);
	m_deselect= true;
	m_deselect1= true;
	isDrag= false;
	isDrag1= false;
	m_dragging= false;
	m_dragging1= false;
	m_clipboardShape= NULL;
	m_ctrl= false;
	m_shift= false;
	m_alt= false;
	m_goingToMemo= false;
	m_loaded= false;
	m_effectspring= false;
	m_flyToShape= NULL;
	m_initFlyTo= false;
	m_flyToStep= 0;
	m_flyToIndex= -1;

	m_timer= SetTimer(1, 42, 0);

	return 0;
}

void CProtoHapticView::OnDestroy() 
{
	CView::OnDestroy();
	
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);
	
	if(m_hglrc != NULL)
	{
		wglDeleteContext(m_hglrc);
		m_hglrc = NULL;
	}
	
}

BOOL CProtoHapticView::OnEraseBkgnd(CDC* pDC) 
{
	return CView::OnEraseBkgnd(pDC);
}

void CProtoHapticView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	int width;
	int height;

	width = cx;
	height = cy;
    gWindowWidth = width;
    gWindowHeight = height;

	ReSizeGLScene(0, 0, (int) width, (int) height);
}


BOOL CProtoHapticView::SetPixelformat(HDC hdc)
{

    PIXELFORMATDESCRIPTOR *ppfd; 
    int pixelformat; 
 
    PIXELFORMATDESCRIPTOR pfd = { 
    sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
    1,                     // version number 
    PFD_DRAW_TO_WINDOW |   // support window 
    PFD_SUPPORT_OPENGL |   // support OpenGL 
    PFD_GENERIC_FORMAT |
    PFD_DOUBLEBUFFER,      // double buffered 
    PFD_TYPE_RGBA,         // RGBA type 
    32,                    // 24-bit color depth 
    0, 0, 0, 0, 0, 0,      // color bits ignored 
    8,                     // no alpha buffer 
    0,                     // shift bit ignored 
    8,                     // no accumulation buffer 
    0, 0, 0, 0,            // accum bits ignored 
    64,                    // 32-bit z-buffer	 
    8,                     // no stencil buffer 
    8,                     // no auxiliary buffer 
    PFD_MAIN_PLANE,        // main layer 
    0,                     // reserved 
    0, 0, 0                // layer masks ignored 
    }; 

   
    ppfd = &pfd;

 
    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
    { 
        ::MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        ::MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE; 

}

void CProtoHapticView::setTool(int tool_id)
{
	m_tool= tool_id;
}

int CProtoHapticView::getTool()
{
	return m_tool;
}

double nearDist;
double farDist;

void CProtoHapticView::ReSizeGLScene(int x, int y, int width, int height)
{
    static const double kFovY = 40;
    static const double kCanonicalSphereRadius = 2;

    glViewport(x, y, width-x, height-y);

    // Compute the viewing parameters based on a fixed fov and viewing
    // sphere enclosing a canonical box centered at the origin.

    nearDist = kCanonicalSphereRadius / tan((kFovY / 2.0) * kPI / 180.0);
    farDist = nearDist + 10.0 * kCanonicalSphereRadius;
   
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(kFovY, ((float)(width-x)/(float)(height-y)), nearDist, farDist);

// Place the camera down the Z axis looking at the origin.
//    gCameraPosWC[0] = 0;
//    gCameraPosWC[1] = 0;
//    gCameraPosWC[2] = nearDist + kCanonicalSphereRadius;
 
    updateCamera();
//    glMatrixMode(GL_MODELVIEW);
}

void CProtoHapticView::UpdateWorkspace()
{
	HHLRC rc= hlGetCurrentContext();
	hlMakeCurrent(m_hHLRC);

	if(!m_loaded)
	{
		hlGetDoublev(HL_TOUCHWORKSPACE_MATRIX, m_matrix);
		dMass mass;

		dMassSetBox (&mass, 100,
							1, 1, 1);		m_loaded= true;
	}

    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    hlMatrixMode(HL_TOUCHWORKSPACE);
    hlLoadIdentity();
    
    // Fit haptic workspace to view volume.
    hluFitWorkspace(projection);
	

    // Compute cursor scale.
    gCursorScale = hluScreenToModelScale(modelview, projection, viewport);
    gCursorScale *= CURSOR_SIZE_PIXELS;

	if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
		hlMakeCurrent(m_hHLRC_1);
		hlMatrixMode(HL_TOUCHWORKSPACE);
		hlLoadIdentity();
    
		// Fit haptic workspace to view volume.
		hluFitWorkspace(projection);
	}
	hlMakeCurrent(rc);
}

int CProtoHapticView::InitGL(void)
{
    static const GLfloat light_model_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
    static const GLfloat light0_diffuse[] = {0.9f, 0.9f, 0.9f, 0.9f};   
    static const GLfloat light0_direction[] = {0.0f, -0.4f, 1.0f, 0.0f};    
    
    // Enable depth buffering for hidden surface removal.
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    
    // Cull back faces.
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    // Other misc features.
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_model_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
    glEnable(GL_LIGHT0); 

	glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

   glEnableClientState (GL_VERTEX_ARRAY);
   //glEnableClientState (GL_COLOR_ARRAY);
   glEnableClientState (GL_NORMAL_ARRAY);

   return TRUE;										
}

int CProtoHapticView::InitHaptics(void)
{
   // OpenHaptics setup follows:
   
   // Create a haptic device instance.
   HDErrorInfo error;
   if(!((CProtoHapticApp*)AfxGetApp())->isUseDefault())
	m_hHD = hdInitDevice(((CProtoHapticApp*)AfxGetApp())->getPrimaryDevice());
   else {
	m_hHD = hdInitDevice(HD_DEFAULT_DEVICE);
	OutputDebugString("Using Default\n");
   }
   if (HD_DEVICE_ERROR(error = hdGetError()))
   {
       return FALSE;
   }

   m_usingTwoDevices= false;
   if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
	   m_usingTwoDevices= true;
	   m_hHD_1 = hdInitDevice(((CProtoHapticApp*)AfxGetApp())->getSecondaryDevice());
	   if (HD_DEVICE_ERROR(error = hdGetError()))
	   {
		   hdDisableDevice(m_hHD);
		   return FALSE;
	   }

	   if (HD_SUCCESS != hdGetError().errorCode)
	   {
		   hdDisableDevice(m_hHD);
		   return FALSE;;
	   }
   }

   // Create a haptic rendering context and activate it.
   m_hHLRC = hlCreateContext(m_hHD);
   hlMakeCurrent(m_hHLRC);

	// Create Secondary HL context
   if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices())
		m_hHLRC_1 = hlCreateContext(m_hHD_1);

   //register callbacks
   	hlAddEventCallback(HL_EVENT_1BUTTONDOWN,HL_OBJECT_ANY,
					   HL_COLLISION_THREAD,&onButtonDown,this);
   	hlAddEventCallback(HL_EVENT_1BUTTONUP,HL_OBJECT_ANY,
					   HL_COLLISION_THREAD,&onButtonUp,this);

	if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices() && 
		!((CProtoHapticApp*)AfxGetApp())->isPassive() ) {
		hlMakeCurrent(m_hHLRC_1);
		m_effectName1 = hlGenEffects(1);
   		hlAddEventCallback(HL_EVENT_1BUTTONDOWN,HL_OBJECT_ANY,
						   HL_COLLISION_THREAD,&onButtonDown1,this);
   		hlAddEventCallback(HL_EVENT_1BUTTONUP,HL_OBJECT_ANY,
						   HL_COLLISION_THREAD,&onButtonUp1,this);
		hlMakeCurrent(m_hHLRC);
	}

    // A drag friction effect.
    m_effectName = hlGenEffects(1);
	m_spring = hlGenEffects(1);


    hlBeginFrame();
    hlEffectd(HL_EFFECT_PROPERTY_GAIN, 0.4);
    hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, 0.1); 
	hlEndFrame();

    hlHintb(HL_SHAPE_DYNAMIC_SURFACE_CHANGE, false);
	hlEnable ( HL_HAPTIC_CAMERA_VIEW );

	if(!InitFeelable(m_hHLRC,m_hHLRC_1))
	{
		hlDeleteContext(m_hHLRC);
		hdDisableDevice(m_hHD);
		if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
			hdDisableDevice(m_hHD_1);
			hlDeleteContext(m_hHLRC_1);
		}
		return FALSE;
	}

	GetDocument()->setContexts(m_hHLRC, m_hHLRC_1);

   return TRUE;										
}

void CProtoHapticView::updateCamera()
{
	//OnCenter();

	//gCameraPosWC[0]= m_dragStart[0];
	//gCameraPosWC[1]= m_dragStart[1];
	//gCameraPosWC[2]= m_dragStart[2];

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    //gluLookAt(gCameraPosWC[0], gCameraPosWC[1], gCameraPosWC[2],
    //          0, 0, 0,
    //          0, 1, 0);
    
	glTranslatef(0, 0, gCameraScale-nearDist);
	//char s[100];
	//sprintf(s, "cam rotX: %f, %f, %f\n", gCameraRotation.get(0,0), gCameraRotation.get(0,1), gCameraRotation.get(0,2));
	//OutputDebugString(s);
    glMultMatrixd(gCameraRotation);
    glTranslatef(gCameraTranslationX, gCameraTranslationY, gCameraTranslationZ);
    //glScaled(gCameraScale, gCameraScale, gCameraScale);

    UpdateWorkspace();
}

int pixelCoverage ( int x0, int y0,
				    int x1, int y1, int prev_count )
{
	#define BLOCK_SIZE 100
	int coverage= 0;
	bool done= false;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glReadBuffer(GL_BACK_LEFT);

	for(int i= x0; i<x1 && !done; i+= BLOCK_SIZE)
	{
		for(int j= y0; j<y1 && !done; j+= BLOCK_SIZE)
		{
			GLubyte pixel[ 3*BLOCK_SIZE*BLOCK_SIZE ];

			glReadPixels(i,j,BLOCK_SIZE,BLOCK_SIZE,
			GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);

			int boundary_x= BLOCK_SIZE;
			int boundary_y= BLOCK_SIZE;

			if( i+BLOCK_SIZE > x1 ) boundary_x-= (i+BLOCK_SIZE)-x1;
			if( j+BLOCK_SIZE > y1 ) boundary_y-= (j+BLOCK_SIZE)-y1;

			for(int k= 0; k<boundary_x; k++)
			{
				for(int l= 0; l<boundary_y; l++)
				{
					if(pixel[3*((l*BLOCK_SIZE)+k)]!=0 || pixel[3*((l*BLOCK_SIZE)+k)+1]!=0 || pixel[3*((l*BLOCK_SIZE)+k)+2]!=0) coverage++;
					if(coverage>prev_count) done= true;
				}
			}
		}
	}

	return coverage;
}

int shapeCoverage ( CShape *s, int prev_count )
{
	GLdouble modelview[16];
	GLdouble projection[16];
	GLint viewport[4];
	GLdouble x, y, z;
	float max_x, min_x, max_y, min_y;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	s->getCorners();

	max_x=-FLT_MAX;
	min_x= FLT_MAX;
	max_y=-FLT_MAX;
	min_y= FLT_MAX;

	for(int i= 0; i<8; i++) {
		
		gluProject(s->getCorner(i)[0],
			       s->getCorner(i)[1],
			       s->getCorner(i)[2],
				   modelview,
				   projection,
				   viewport,
				   &x,
				   &y,
				   &z);

		if(x>max_x) max_x= x; if(x<min_x) min_x= x;
		if(y>max_y) max_y= y; if(y<min_y) min_y= y;
	}

	return pixelCoverage ( min_x, min_y,
						   max_x, max_y, prev_count );
}


void CProtoHapticView::DrawGLScene(bool swap_buffers)
{
	CPaintDC dc(this);  

   //Calculate selection
	int i;
	for (i= 0; i<GetDocument()->GetShapeCount(); i++)
	{
		GetDocument()->GetShape(i)->drawGLScene();
		if(GetDocument()->GetShape(i)->touching()) {
			std::set<CShape*>::iterator theIterator;
			bool found= false;
			for( theIterator = m_selectedShapes1->begin(); theIterator != m_selectedShapes1->end(); theIterator++ ) {
				CShape* shape= *theIterator;
				if(shape == GetDocument()->GetShape(i)) found= true;
			}
			if(!found) {
				if(!m_ctrl)
					m_selectedShapes->clear(); //Remove for multiple select
				m_selectedShapes->insert(GetDocument()->GetShape(i));
			}
		}
	}

	if(!((!m_edit_mode)&&(GetDocument()->isHideMemos())))
	{
		//again for memos
		for (i= 0; i<GetDocument()->getMemoCount(); i++)
		{
			GetDocument()->getMemo(i)->drawGLScene();
			if(GetDocument()->getMemo(i)->touching()) {
				std::set<CShape*>::iterator theIterator;
				bool found= false;
				for( theIterator = m_selectedShapes1->begin(); theIterator != m_selectedShapes1->end(); theIterator++ ) {
					CShape* shape= *theIterator;
					if(shape == GetDocument()->getMemo(i)) found= true;
				}
				if(!found) {
					if(!m_ctrl)
						m_selectedShapes->clear(); //Remove for multiple select
					m_selectedShapes->insert(GetDocument()->getMemo(i));
				}
			}
		}
	}

   //Calculate selection
	for (i= 0; i<GetDocument()->GetShapeCount(); i++)
	{
		if(GetDocument()->GetShape(i)->touching1()) {
			std::set<CShape*>::iterator theIterator;
			bool found= false;
			for( theIterator = m_selectedShapes->begin(); theIterator != m_selectedShapes->end(); theIterator++ ) {
				CShape* shape= *theIterator;
				if(shape == GetDocument()->GetShape(i)) found= true;
			}
			if(!found) {
				if(!m_ctrl)
					m_selectedShapes1->clear(); //Remove for multiple select
				m_selectedShapes1->insert(GetDocument()->GetShape(i));
			}
		}
	}

	if(!((!m_edit_mode)&&(GetDocument()->isHideMemos())))
	{
		//again for memos
		for (i= 0; i<GetDocument()->getMemoCount(); i++)
		{
			if(GetDocument()->getMemo(i)->touching1()) {
				std::set<CShape*>::iterator theIterator;
				bool found= false;
				for( theIterator = m_selectedShapes->begin(); theIterator != m_selectedShapes->end(); theIterator++ ) {
					CShape* shape= *theIterator;
					if(shape == GetDocument()->getMemo(i)) found= true;
				}
				if(!found) {
					if(!m_ctrl)
						m_selectedShapes1->clear(); //Remove for multiple select
					m_selectedShapes1->insert(GetDocument()->getMemo(i));
				}
			}
		}
	}

   std::set<CShape*>::iterator theIterator;
   for( theIterator = m_selectedShapes->begin(); theIterator != m_selectedShapes->end(); theIterator++ ) {
		CShape* shape= *theIterator;
		if(m_edit_mode&&!m_deselect&&
		   (!shape->isCompositeMember()||shape->getType()!=SHAPE_COMPOSITE)) {
			shape->getControl()->setColourScheme(CONTROL_COLOURS_PRIMARY);
			shape->getControl()->drawGLScene();
		}
   }	
   for( theIterator = m_selectedShapes1->begin(); theIterator != m_selectedShapes1->end(); theIterator++ ) {
		CShape* shape= *theIterator;
		if(m_edit_mode&&!m_deselect1&&
		   (!shape->isCompositeMember()||shape->getType()!=SHAPE_COMPOSITE)) {
			shape->getControl()->setColourScheme(CONTROL_COLOURS_SECONDARY);
			shape->getControl()->drawGLScene();
		}
   }

	if(m_dragging && m_transform==NULL) DrawDragArea();
	if(m_dragging1 && m_transform1==NULL) DrawDragArea1();

	DrawCursor();

	if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
		if((!((CProtoHapticApp*)AfxGetApp())->isPassive() && m_edit_mode) || !m_edit_mode) {
			hlMakeCurrent(m_hHLRC_1);
			DrawCursor();
			hlMakeCurrent(m_hHLRC);
		}
	}

	if(swap_buffers)
		SwapBuffers(m_hgldc);
}



void CProtoHapticView::DrawHapticScene(bool primary_context)
{
	if ( m_flyToShape != NULL ) 
	{
		hlBeginFrame ();
		hlEndFrame ();
		return;
	}

	glPushMatrix();
	
	// Haptic only transformations
	glTranslatef ( hapticOffset[0], hapticOffset[1], hapticOffset[2] );

   hlBeginFrame();

   if(primary_context) {
	   if(m_transform!=NULL&&isDrag==false) { isDrag= true; 
			hlEffectd(HL_EFFECT_PROPERTY_GAIN, 0.4);
			hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, 0.1); 
			hlStartEffect(HL_EFFECT_FRICTION, m_effectName); }
	   if(m_transform==NULL&&isDrag==true)  { isDrag= false; hlStopEffect(m_effectName); }
   } else {
	   if(m_transform1!=NULL&&isDrag1==false) { isDrag1= true; 
			hlEffectd(HL_EFFECT_PROPERTY_GAIN, 0.4);
			hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, 0.1); 
			hlStartEffect(HL_EFFECT_FRICTION, m_effectName1); }
	   if(m_transform1==NULL&&isDrag1==true)  { isDrag1= false; hlStopEffect(m_effectName1); }
   }
   
	if(!m_goingToMemo&&m_effectspring)
	{
		hlStopEffect(m_spring);
		m_effectspring= false;
	}

   if(!((primary_context&&m_dragging)||((!primary_context)&&m_dragging1))&&!m_goingToMemo) {
	  //hlEnable(HL_HAPTIC_CAMERA_VIEW); // Stop popthough

		int i;
		for(i= 0; i < GetDocument()->GetShapeCount(); i++) {

				if((!gIsRotatingCamera)&&(!gIsScalingCamera)) {
					GetDocument()->GetShape(i)->drawHapticScene(primary_context);
				}
		}	

	//again for memos
		for(i= 0; i < GetDocument()->getMemoCount(); i++) {
			if((!gIsRotatingCamera)&&(!gIsScalingCamera)) {
				GetDocument()->getMemo(i)->drawHapticScene(primary_context);
			}
		}	


		if(primary_context) {
			std::set<CShape*>::iterator theIterator;
			for( theIterator = m_selectedShapes->begin(); theIterator != m_selectedShapes->end(); theIterator++ ) {
				CShape* shape= *theIterator;

					if(m_edit_mode&&
					   (!shape->isCompositeMember()||shape->getType()!=SHAPE_COMPOSITE)) {
						shape->getControl()->drawHapticScene(primary_context);
					}
			}
		}
		else
		{

			std::set<CShape*>::iterator theIterator;
			for( theIterator = m_selectedShapes1->begin(); theIterator != m_selectedShapes1->end(); theIterator++ ) {
				CShape* shape= *theIterator;

					if(m_edit_mode&&
					   (!shape->isCompositeMember()||shape->getType()!=SHAPE_COMPOSITE)) {
						shape->getControl()->drawHapticScene(primary_context);
					}
			}
		}

   }
   //here!!
   if(mouseOn)
	hmRenderMouseScene();
   hlEndFrame();

   glPopMatrix();
}

void CProtoHapticView::DrawCursor()
{
    static const double kCursorRadius = 0.5;
    static const double kCursorHeight = 1.5;
    static const int kCursorTess = 15;
    HLdouble proxyxform[16];

    GLUquadricObj *qobj = 0;

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushMatrix();

    if (!bob)
    {
        bob = glGenLists(1);
        glNewList(bob, GL_COMPILE);
        qobj = gluNewQuadric();
               
        gluCylinder(qobj, 0.0, kCursorRadius, kCursorHeight,
                            kCursorTess, kCursorTess);
        glTranslated(0.0, 0.0, kCursorHeight);
        gluCylinder(qobj, kCursorRadius, 0.0, kCursorHeight / 5.0,
                    kCursorTess, kCursorTess);
    
        gluDeleteQuadric(qobj);
        glEndList();
    }

    // Get the proxy transform in world coordinates.
    hlGetDoublev(HL_PROXY_TRANSFORM, proxyxform);
    glMultMatrixd(proxyxform);

    // Apply the local cursor scale factor.
    glScaled(gCursorScale, gCursorScale, gCursorScale);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0.0, 0.5, 1.0);

	if(isHapticMouseOn())
      glCallList(bob);

    glPopMatrix(); 
    glPopAttrib();
}

void CProtoHapticView::DoTransformations()
{
	if(m_transform!=NULL)
	{

		HHLRC rc= hlGetCurrentContext();
		hlMakeCurrent(m_hHLRC);

		HLdouble pp[3];
		hlGetDoublev(HL_PROXY_POSITION, pp);

		hlMakeCurrent(rc);
		
		m_transform->setProxyPos(pp[0],pp[1],pp[2]);
	}

	if(m_transform1!=NULL)
	{
		HHLRC rc= hlGetCurrentContext();
		hlMakeCurrent(m_hHLRC_1);

		HLdouble pp[3];
		hlGetDoublev(HL_PROXY_POSITION, pp);

		hlMakeCurrent(rc);
		
		m_transform1->setProxyPos(pp[0],pp[1],pp[2]);
	}
}

CShape* CProtoHapticView::getSelectedShape()
{
	
	if(m_selectedShapes->size()>0) {
		std::set<CShape*>::iterator theIterator;
		return *m_selectedShapes->begin();
	} else
		return NULL;
}

CShape* CProtoHapticView::getSelectedShape1()
{
	
	if(m_selectedShapes1->size()>0) {
		std::set<CShape*>::iterator theIterator;
		return *m_selectedShapes1->begin();
	} else
		return NULL;
}

int CProtoHapticView::tryCameraRotation ( hduMatrix matF, int prev_count )
{
	int number;

	// save camera variables
	hduMatrix saved_cameraRotation; 
	saved_cameraRotation.set(gCameraRotation);
	float saved_cameraTranslationX= gCameraTranslationX;
	float saved_cameraTranslationY= gCameraTranslationY;
	float saved_cameraTranslationZ= gCameraTranslationZ;
	float saved_cameraScale= gCameraScale;
			
	// apply proposed camera variables
	gCameraRotation.set(matF);
	gCameraTranslationX= -m_flyToShape->getLocationX();
	gCameraTranslationY= -m_flyToShape->getLocationY();
	gCameraTranslationZ= -m_flyToShape->getLocationZ();
	gCameraScale= -(farDist-nearDist)/2.0;
	updateCamera();

	DrawGLViewports(false);

	// test for visibility
	number= shapeCoverage ( m_flyToShape, prev_count );

	// restore camera
	gCameraRotation.set(saved_cameraRotation);
	gCameraTranslationX= saved_cameraTranslationX;
	gCameraTranslationY= saved_cameraTranslationY;
	gCameraTranslationZ= saved_cameraTranslationZ;
	gCameraScale= saved_cameraScale;
	updateCamera();

	return number;
}

bool donethat= false;

void CProtoHapticView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	// Force update of toolbars
	AfxGetApp()->OnIdle(-1);

	// Stop CCodeView Stealing our keystrokes!
	if(!donethat) {
	CView* p= ((CProtoHapticApp*)AfxGetApp())->m_codeView;
	if(p != NULL && p->IsKindOf(RUNTIME_CLASS(CView))) {
		((CProtoHapticApp*)AfxGetApp())->SwitchView(((CProtoHapticApp*)AfxGetApp())->m_codeView);
		((CProtoHapticApp*)AfxGetApp())->SwitchView(((CProtoHapticApp*)AfxGetApp())->m_graphicalView);
		donethat= true;
	}}

	//move this:
	((CCodeView*)m_codeView)->SetDocument(GetDocument());

	if(m_flyToShape!=NULL)
	{
		if(!m_initFlyTo) {

			glDisable(GL_LIGHTING);
			m_deselect= true;

			//save colours
			int i;
			m_flyToIndex= -1;
			for (i= 0; i<GetDocument()->GetShapeCount(); i++)
				if(GetDocument()->GetShape(i) == m_flyToShape) m_flyToIndex= i;

			GetDocument()->saveState();

			//set colours
			for (i= 0; i<GetDocument()->GetShapeCount(); i++)
			{
				GetDocument()->GetShape(i)->setColour(0,0,0);
			}

			hduMatrix matF;
			matF.makeIdentity();

			if(m_flyToIndex!=-1)
			{
				GetDocument()->GetShape(m_flyToIndex)->setColour(1,1,1);

				int biggest= 0;
				hduMatrix rota;
				hduMatrix newRotation;

				newRotation= hduMatrix::createRotationAroundY(0.5);
				for( int r= 0; r<13; r++ )
				{
					int count= tryCameraRotation ( matF, biggest);

					if ( count>biggest )
					{
						biggest= count;
						rota.set(matF);
					}

					matF.multRight(	newRotation );
				}

				matF.set(rota);
			}

			m_flyToDeltaX= ((-m_flyToShape->getLocationX())-gCameraTranslationX)/FLY_TO_STEPS;
			m_flyToDeltaY= ((-m_flyToShape->getLocationY())-gCameraTranslationY)/FLY_TO_STEPS;
			m_flyToDeltaZ= ((-m_flyToShape->getLocationZ())-gCameraTranslationZ)/FLY_TO_STEPS;

			m_flyToDeltaScale= (-((farDist-nearDist)/2.0) - gCameraScale)/FLY_TO_STEPS;

			hduMatrix matS(gCameraRotation);

			matS.transpose();

			hduMatrix matT(matS.multRight(matF));
			hduQuaternion quat(matT);
			quat.toAxisAngle(m_flyToAxis, m_flyToAngle);

			m_flyToDeltaAngle= m_flyToAngle/FLY_TO_STEPS;

			//restore colours
			GetDocument()->undo();

			glEnable(GL_LIGHTING);

			m_initFlyTo= true;
		}

		if(m_flyToStep<FLY_TO_STEPS) {
			gCameraTranslationX+= m_flyToDeltaX;
			gCameraTranslationY+= m_flyToDeltaY;
			gCameraTranslationZ+= m_flyToDeltaZ;
			gCameraScale+= m_flyToDeltaScale;

			hduMatrix newRotation1;
			newRotation1 = hduMatrix::createRotation(m_flyToAxis, m_flyToDeltaAngle);            
			gCameraRotation.multRight(newRotation1);

			updateCamera();
			m_flyToStep++;
			
		} else {
			m_flyToShape= NULL;		// Reached destination
			if(m_flyToIndex!=-1)
				m_selectedShapes->insert(GetDocument()->GetShape(m_flyToIndex));
		}
	}

	if(m_transform!=NULL&&m_transform->getType()==TRANSFORM_ROTATE)
	{
		if(m_ctrl)
			((CRotate*)m_transform)->setChunking(true);
		else
			((CRotate*)m_transform)->setChunking(false);
	}

	if(m_transform!=NULL&&m_transform->getType()==TRANSFORM_TRANSLATE)
	{
		if(m_ctrl)
			((CMove*)m_transform)->setSnapto(true);
		else
			((CMove*)m_transform)->setSnapto(false);
	}

	// Set snap to for moving triangle vertex transformation
	if(m_transform!=NULL&&m_transform->getType()==TRANSFORM_TRI_VERTEX_MOVE)
	{
		if(m_ctrl)
			((CTriVertexMove*)m_transform)->setSnapto(true);
		else
			((CTriVertexMove*)m_transform)->setSnapto(false);
	}

	if(m_transform!=NULL&&m_transform->getType()==TRANSFORM_RESIZE)
	{
		if(m_ctrl&&m_shift)
			((CResize*)m_transform)->setFixedAxis(true);
		else if(m_ctrl&&!m_shift)
			((CResize*)m_transform)->setSnapto(true);
		else if(m_shift&&!m_ctrl)
			((CResize*)m_transform)->setAspectFixed(true);
		if(!(m_ctrl&&m_shift))
			((CResize*)m_transform)->setFixedAxis(false);
		if(!(m_ctrl&&!m_shift))
			((CResize*)m_transform)->setSnapto(false);
		if(!(m_shift&&!m_ctrl))
			((CResize*)m_transform)->setAspectFixed(false);
	}

	if(m_transform!=NULL &&
	   m_transform->getType()==TRANSFORM_RESIZE &&
	   getSelectedShape()->getType()==SHAPE_COMPOSITE)
	{
		((CResize*)m_transform)->setAspectFixed(true);
	}

	if(m_deselect) {
		OutputDebugString("Cleared");
		m_deselect= false;
		m_selectedShapes->clear();
	}

	if(m_deselect1) {
		OutputDebugString("Cleared");
		m_deselect1= false;
		m_selectedShapes1->clear();
	}

	if(m_selectedShapes->size()>0&&m_edit_mode) {
		std::set<CShape*>::iterator theIterator;
		shapeDlg.setShape(*m_selectedShapes->begin());
		shapeDlg.SetDocument ( GetDocument () );
	} else
	{
		shapeDlg.setShape(NULL);
	}
//here!!
	if(!m_hapticMouseOn) {
		//hmEnableMouse(HM_MOUSE_INSIDE_VIEWPORT);
		//hmEnableMouse(HM_MOUSE_OUTSIDE_VIEWPORT);
	} else {
		hmDisableMouse(HM_MOUSE_INSIDE_VIEWPORT);
		hmDisableMouse(HM_MOUSE_OUTSIDE_VIEWPORT);
	}

	if(!m_edit_mode) {
		GetDocument()->UpdateDynamics();
		GetDocument()->SimulationStep();
	}
	DrawGLViewports(true);

	DrawHapticScene(true);

	if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
		hlMakeCurrent(m_hHLRC_1);
		if((!((CProtoHapticApp*)AfxGetApp())->isPassive() && m_edit_mode) || !m_edit_mode) 
			DrawHapticScene(false);
		else
		{
			hlBeginFrame();
			hlEndFrame();
		}
		hlMakeCurrent(m_hHLRC);

	}

	DoTransformations();

	if(!mouseOn) {
		char b[100];
		char c[100];
		if(AfxGetApp()->m_pMainWnd!=NULL) {
			AfxGetApp()->m_pMainWnd->GetWindowText(b,100);
			GetClassName(AfxGetApp()->m_pMainWnd->m_hWnd,c,100);
			// here!!
			if(hmInitializeMouse(m_hHLRC,c,b)) {
				hmEnableMouse(HM_MOUSE_INSIDE_VIEWPORT);
				//hmDisableMouse(HM_MOUSE_INSIDE_VIEWPORT);
				mouseOn= true;
			}
		}
	}

	if(false) {//if(m_showToolTip) {
	    GLdouble modelview[16];
		GLdouble projection[16];
		GLint viewport[4];
		GLdouble x, y, z;
		double pp[3];

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	    glGetDoublev(GL_PROJECTION_MATRIX, projection);
	    glGetIntegerv(GL_VIEWPORT, viewport);

		hlGetDoublev(HL_PROXY_POSITION,pp);

		gluProject(pp[0],
				pp[1],
				pp[2],
				modelview,
				projection,
				viewport,
				&x,
				&y,
				&z);
		CPoint p;
		p.x= x; p.y= viewport[3]-y;
		ClientToScreen(&p);

		m_wndText.SetWindowPos(NULL, p.x+20, p.y+20, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE);
		m_wndText.ShowWindow(SW_SHOWNA);
	}

	CView::OnTimer(nIDEvent);
}

double projectToTrackball(double radius, double x, double y)
{
    static const double kUnitSphereRadius2D = sqrt(2.0);
    double z;

    double dist = sqrt(x * x + y * y);
    if (dist < radius * kUnitSphereRadius2D / 2.0)
    {
        // Solve for sphere case.
        z = sqrt(radius * radius - dist * dist);
    }
    else
    {
        // Solve for hyperbolic sheet case.
        double t = radius / kUnitSphereRadius2D;
        z = t * t / dist;
    }

    return z;
}

void CProtoHapticView::OnMouseMove(UINT nFlags, CPoint point) 
{
	int x= point.x; int y= point.y;

	if (gIsRotatingCamera)
    {
        static const double kTrackBallRadius = 0.8;   

        hduVector3Dd lastPos;
        lastPos[0] = gLastMouseX * 2.0 / gWindowWidth - 1.0;
        lastPos[1] = (gWindowHeight - gLastMouseY) * 2.0 / gWindowHeight - 1.0;
        lastPos[2] = projectToTrackball(kTrackBallRadius, lastPos[0], lastPos[1]);

        hduVector3Dd currPos;
        currPos[0] = x * 2.0 / gWindowWidth - 1.0;
        currPos[1] = (gWindowHeight - y) * 2.0 / gWindowHeight - 1.0;
        currPos[2] = projectToTrackball(kTrackBallRadius, currPos[0], currPos[1]);

        currPos.normalize();
        lastPos.normalize();

        hduVector3Dd rotateVec = lastPos.crossProduct(currPos);
        
        double rotateAngle = asin(rotateVec.magnitude());
        if (!hduIsEqual(rotateAngle, 0.0, DBL_EPSILON))
        {
            hduMatrix deltaRotation = hduMatrix::createRotation(
                rotateVec, rotateAngle);            
            gCameraRotation.multRight(deltaRotation);
        
            updateCamera();
        }
    }
    if (gIsTranslatingCamera)
    {
        gCameraTranslationX += 10 * double(x - gLastMouseX)/gWindowWidth;
        gCameraTranslationY -= 10 * double(y - gLastMouseY)/gWindowWidth;

        updateCamera();
    }
    else if (gIsScalingCamera)
    {
        float y1 = gWindowHeight - gLastMouseY;
        float y2 = gWindowHeight - y;

		gCameraScale += 10 * ((y1 - y2) / gWindowHeight);  

        updateCamera();
    }

    gLastMouseX = x;
    gLastMouseY = y;


	CView::OnMouseMove(nFlags, point);
}

void CProtoHapticView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	gIsScalingCamera = true;     
    gLastMouseX = point.x;
    gLastMouseY = point.y;

	CView::OnRButtonDown(nFlags, point);
}

void CProtoHapticView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	gIsScalingCamera = false;  

	CView::OnRButtonUp(nFlags, point);
}

void CProtoHapticView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	gIsRotatingCamera = true;
    gLastMouseX = point.x;
    gLastMouseY = point.y;

	CView::OnRButtonDown(nFlags, point);
}

void CProtoHapticView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	gIsRotatingCamera = false;

	CView::OnLButtonUp(nFlags, point);
}

void CProtoHapticView::OnSelectTool() 
{
	// TODO: Add your command handler code here
	setTool(TOOL_SELECT);
}

void CProtoHapticView::OnCubeTool() 
{
	// TODO: Add your command handler code here
	setTool(TOOL_CUBE);
}

void CProtoHapticView::OnSphereTool() 
{
	// TODO: Add your command handler code here
	setTool(TOOL_SPHERE);
}

void CProtoHapticView::OnCylinderTool() 
{
	// TODO: Add your command handler code here
	setTool(TOOL_CYLINDER);
}

void CProtoHapticView::OnTriangleTool () 
{
	// TODO: Add your command handler code here
	setTool(TOOL_TRIANGLE);
}

void CProtoHapticView::OnTorusTool () 
{
	// TODO: Add your command handler code here
	setTool(TOOL_TORUS);
}

void CProtoHapticView::OnShapeProperties() 
{
	if(shapeDlg.isVisable())
	{
		shapeDlg.ShowWindow(SW_HIDE);
		shapeDlg.setVisable(false);
	} else {
		shapeDlg.ShowWindow(SW_SHOW);
		shapeDlg.setVisable(true);
	}
}

void CProtoHapticView::OnEditMode() 
{
	// TODO: Add your command handler code here
	CMenu* pMenu = AfxGetApp()->GetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( ID_VIEW_EDITMODE, MF_CHECKED );
	pMenu->CheckMenuItem( ID_VIEW_VIEWMODE, MF_UNCHECKED );

	m_edit_mode= true;
	HHLRC rc= hlGetCurrentContext();

	hlMakeCurrent(m_hHLRC);
	hlHintb(HL_SHAPE_DYNAMIC_SURFACE_CHANGE, false);
	if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
		hlMakeCurrent(m_hHLRC_1);
		hlHintb(HL_SHAPE_DYNAMIC_SURFACE_CHANGE, false);
	}
	hlMakeCurrent(rc);
}

void CProtoHapticView::OnViewMode() 
{
	// TODO: Add your command handler code here
	GetDocument()->saveState();

	CMenu* pMenu = AfxGetApp()->GetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( ID_VIEW_EDITMODE, MF_UNCHECKED );
	pMenu->CheckMenuItem( ID_VIEW_VIEWMODE, MF_CHECKED );

	m_edit_mode= false;
	shapeDlg.setShape(NULL);

	HHLRC rc= hlGetCurrentContext();

	hlMakeCurrent(m_hHLRC);
	hlHintb(HL_SHAPE_DYNAMIC_SURFACE_CHANGE, GetDocument ()->IsDynamicSurfaceChangeEnabled ());

	if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
		hlMakeCurrent(m_hHLRC_1);
		hlHintb(HL_SHAPE_DYNAMIC_SURFACE_CHANGE, GetDocument ()->IsDynamicSurfaceChangeEnabled ());
	}
	hlMakeCurrent(rc);
	
	GetDocument()->PreSimulation();
}

void CProtoHapticView::OnCenter() 
{
	// TODO: Add your command handler code here
	if(m_selectedShapes->size()>0) {
		std::set<CShape*>::iterator theIterator;
		CShape* shape= *m_selectedShapes->begin();
		flyToShape(shape);
	}
}

void CProtoHapticView::OnButtonEdit() 
{
	// TODO: Add your command handler code here
	OnEditMode();
}

void CProtoHapticView::OnButtonView() 
{
	// TODO: Add your command handler code here
	OnViewMode();
}

void CProtoHapticView::OnGroup() 
{
	// TODO: Add your command handler code here
	if(m_selectedShapes->size()>0) {

	//::MessageBox(NULL, "Begin add composite", "Info", MB_OK); 

		if(getSelectedShape()->getType()==SHAPE_MEMO) return;

		GetDocument()->saveState();

		//crashes after here

		CComposite *c= new CComposite(); 
		
		// Create a default name for the composite
		CString name= GetDocument ()->GetFreeShapeName ();
		c->SetName ( name );

		std::set<CShape*>::iterator theIterator;
		for( theIterator = m_selectedShapes->begin(); theIterator != m_selectedShapes->end(); theIterator++ ) {
			CShape* shape= *theIterator;

			c->addShape(shape);
			GetDocument()->RemoveShape(shape);
			OutputDebugString(".");
		}

		//crashed before here

		GetDocument()->AddShape(c);
		m_selectedShapes->clear();
		m_selectedShapes->insert(c);

		//crashed before here
	}
}

void CProtoHapticView::OnUnGroup() 
{
	// TODO: Add your command handler code here
	if(m_selectedShapes->size()>0) {

		CShape *shape= *m_selectedShapes->begin();
		if(shape->getType()==SHAPE_COMPOSITE) {
			GetDocument()->saveState();
			CComposite *c= (CComposite*)shape;
		
			int i;
			for(i= 0; i<c->getShapeCount(); i++)
			{
				GetDocument()->AddShape(c->getShape(i));
			}

			GetDocument()->RemoveShape(c);
		}
	}
	m_deselect= true;
}

void CProtoHapticView::setDragging(bool d)
{
	m_dragging= d;
}

void CProtoHapticView::setDragStart(double start[])
{
	m_dragStart[0]= start[0];
	m_dragStart[1]= start[1];
	m_dragStart[2]= start[2];
}

void CProtoHapticView::DrawDragArea()
{
	HHLRC rc= hlGetCurrentContext();
	hlMakeCurrent(m_hHLRC);

	HLdouble pp[3];
	hlGetDoublev(HL_PROXY_POSITION, pp);

	hlMakeCurrent(rc);

	float v0[3]= { pp[0], pp[1], pp[2] };
	float v1[3]= { pp[0], pp[1], m_dragStart[2] };
	float v2[3]= { pp[0], m_dragStart[1], m_dragStart[2] };
	float v3[3]= { pp[0], m_dragStart[1], pp[2] };
	float v4[3]= { m_dragStart[0], pp[1], pp[2] };
	float v5[3]= { m_dragStart[0], m_dragStart[1], pp[2] };
	float v6[3]= { m_dragStart[0], m_dragStart[1], m_dragStart[2] };
	float v7[3]= { m_dragStart[0], pp[1], m_dragStart[2] };

	glDisable(GL_LIGHTING);
	glColor3d(1.0,1.0,1.0);

	glBegin(GL_LINES);
	glVertex3d(v0[0],v0[1],v0[2]);
	glVertex3d(v1[0],v1[1],v1[2]);
	glVertex3d(v1[0],v1[1],v1[2]);
	glVertex3d(v2[0],v2[1],v2[2]);
	glVertex3d(v2[0],v2[1],v2[2]);
	glVertex3d(v3[0],v3[1],v3[2]);
	glVertex3d(v3[0],v3[1],v3[2]);
	glVertex3d(v0[0],v0[1],v0[2]);
	glVertex3d(v4[0],v4[1],v4[2]);
	glVertex3d(v5[0],v5[1],v5[2]);
	glVertex3d(v5[0],v5[1],v5[2]);
	glVertex3d(v6[0],v6[1],v6[2]);
	glVertex3d(v6[0],v6[1],v6[2]);
	glVertex3d(v7[0],v7[1],v7[2]);
	glVertex3d(v7[0],v7[1],v7[2]);
	glVertex3d(v4[0],v4[1],v4[2]);
	glVertex3d(v0[0],v0[1],v0[2]);
	glVertex3d(v4[0],v4[1],v4[2]);
	glVertex3d(v3[0],v3[1],v3[2]);
	glVertex3d(v5[0],v5[1],v5[2]);
	glVertex3d(v2[0],v2[1],v2[2]);
	glVertex3d(v6[0],v6[1],v6[2]);
	glVertex3d(v1[0],v1[1],v1[2]);
	glVertex3d(v7[0],v7[1],v7[2]);
	glEnd();

	glEnable(GL_LIGHTING);

	//Calculate selection
	m_selectedShapes->clear();


	int i;
	for ( i= 0; i<GetDocument()->GetShapeCount(); i++) 
	{
		CShape* shape= GetDocument()->GetShape(i);
		float x= shape->getLocationX();
		float y= shape->getLocationY();
		float z= shape->getLocationZ();


		if( ( ((v0[0]>v4[0]) && (x>v4[0] && x<v0[0])) || 
			  ((v0[0]<v4[0]) && (x<v4[0] && x>v0[0]))    ) &&
		    ( ((v5[1]>v4[1]) && (y>v4[1] && y<v5[1])) ||
			  ((v5[1]<v4[1]) && (y<v4[1] && y>v5[1]))    ) &&
		    ( ((v0[2]>v1[2]) && (z>v1[2] && z<v0[2])) ||
			  ((v0[2]<v1[2]) && (z<v1[2] && z>v0[2]))    ) )
		{
			std::set<CShape*>::iterator theIterator;
			bool found= false;
			for( theIterator = m_selectedShapes1->begin(); theIterator != m_selectedShapes1->end(); theIterator++ ) {
				CShape* s= *theIterator;
				if(shape == s) found= true;
			}
			if(!found)
				m_selectedShapes->insert(shape);
		}

	}
}

void CProtoHapticView::DrawDragArea1()
{
	HHLRC rc= hlGetCurrentContext();
	hlMakeCurrent(m_hHLRC_1);

	HLdouble pp[3];
	hlGetDoublev(HL_PROXY_POSITION, pp);

	hlMakeCurrent(rc);

	float v0[3]= { pp[0], pp[1], pp[2] };
	float v1[3]= { pp[0], pp[1], m_dragStart1[2] };
	float v2[3]= { pp[0], m_dragStart1[1], m_dragStart1[2] };
	float v3[3]= { pp[0], m_dragStart1[1], pp[2] };
	float v4[3]= { m_dragStart1[0], pp[1], pp[2] };
	float v5[3]= { m_dragStart1[0], m_dragStart1[1], pp[2] };
	float v6[3]= { m_dragStart1[0], m_dragStart1[1], m_dragStart1[2] };
	float v7[3]= { m_dragStart1[0], pp[1], m_dragStart1[2] };

	glDisable(GL_LIGHTING);
	glColor3d(1.0,1.0,1.0);

	glBegin(GL_LINES);
	glVertex3d(v0[0],v0[1],v0[2]);
	glVertex3d(v1[0],v1[1],v1[2]);
	glVertex3d(v1[0],v1[1],v1[2]);
	glVertex3d(v2[0],v2[1],v2[2]);
	glVertex3d(v2[0],v2[1],v2[2]);
	glVertex3d(v3[0],v3[1],v3[2]);
	glVertex3d(v3[0],v3[1],v3[2]);
	glVertex3d(v0[0],v0[1],v0[2]);
	glVertex3d(v4[0],v4[1],v4[2]);
	glVertex3d(v5[0],v5[1],v5[2]);
	glVertex3d(v5[0],v5[1],v5[2]);
	glVertex3d(v6[0],v6[1],v6[2]);
	glVertex3d(v6[0],v6[1],v6[2]);
	glVertex3d(v7[0],v7[1],v7[2]);
	glVertex3d(v7[0],v7[1],v7[2]);
	glVertex3d(v4[0],v4[1],v4[2]);
	glVertex3d(v0[0],v0[1],v0[2]);
	glVertex3d(v4[0],v4[1],v4[2]);
	glVertex3d(v3[0],v3[1],v3[2]);
	glVertex3d(v5[0],v5[1],v5[2]);
	glVertex3d(v2[0],v2[1],v2[2]);
	glVertex3d(v6[0],v6[1],v6[2]);
	glVertex3d(v1[0],v1[1],v1[2]);
	glVertex3d(v7[0],v7[1],v7[2]);
	glEnd();

	glEnable(GL_LIGHTING);

	//Calculate selection
	m_selectedShapes1->clear();


	int i;
	for ( i= 0; i<GetDocument()->GetShapeCount(); i++) 
	{
		CShape* shape= GetDocument()->GetShape(i);
		float x= shape->getLocationX();
		float y= shape->getLocationY();
		float z= shape->getLocationZ();


		if( ( ((v0[0]>v4[0]) && (x>v4[0] && x<v0[0])) || 
			  ((v0[0]<v4[0]) && (x<v4[0] && x>v0[0]))    ) &&
		    ( ((v5[1]>v4[1]) && (y>v4[1] && y<v5[1])) ||
			  ((v5[1]<v4[1]) && (y<v4[1] && y>v5[1]))    ) &&
		    ( ((v0[2]>v1[2]) && (z>v1[2] && z<v0[2])) ||
			  ((v0[2]<v1[2]) && (z<v1[2] && z>v0[2]))    ) )
		{
			std::set<CShape*>::iterator theIterator;
			bool found= false;
			for( theIterator = m_selectedShapes->begin(); theIterator != m_selectedShapes->end(); theIterator++ ) {
				CShape* s= *theIterator;
				if(shape == s) found= true;
			}
			if(!found)
				m_selectedShapes1->insert(shape);
		}

	}
}

void CProtoHapticView::OnDelete() 
{
	// TODO: Add your command handler code here
	if(getSelectedShape()!=NULL) {
		GetDocument()->saveState();
		if(getSelectedShape()->getType()==SHAPE_MEMO) {
			GetDocument()->removeMemo(getSelectedShape());
			OnGotoMemo();
		} else
			GetDocument()->RemoveShape(getSelectedShape());
		m_deselect= true;
	}
}

CShape* CProtoHapticView::getClipboardShape()
{
	return m_clipboardShape;
}

void CProtoHapticView::OnEditUndo() 
{
	// TODO: Add your command handler code here
	m_deselect= true;
	GetDocument()->undo();
}

void CProtoHapticView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	hduVector3Dd rotateVec;
	hduMatrix deltaRotation;
		CPoint p;
	switch(nChar)
    {
    case VK_CONTROL:
        m_ctrl= true;
        break;
    case VK_SHIFT:
        m_shift= true;
        break;
    case VK_MENU:
        m_alt= true;
        break;
    case VK_RIGHT:
		rotateVec[0]= 0;
		rotateVec[1]= 1;
		rotateVec[2]= 0;
        deltaRotation= hduMatrix::createRotation(
            rotateVec, -3.14/8);            
        gCameraRotation.multRight(deltaRotation);
        
        updateCamera();
        break;
    case VK_LEFT:
		rotateVec[0]= 0;
		rotateVec[1]= 1;
		rotateVec[2]= 0;
        deltaRotation = hduMatrix::createRotation(
            rotateVec, 3.14/8);            
        gCameraRotation.multRight(deltaRotation);
        
        updateCamera();
        break;
    case VK_UP:
		rotateVec[0]= 1;
		rotateVec[1]= 0;
		rotateVec[2]= 0;
        deltaRotation= hduMatrix::createRotation(
            rotateVec, -3.14/8);            
        gCameraRotation.multRight(deltaRotation);
        
        updateCamera();
        break;
    case VK_DOWN:
		rotateVec[0]= 1;
		rotateVec[1]= 0;
		rotateVec[2]= 0;
        deltaRotation = hduMatrix::createRotation(
            rotateVec, 3.14/8);            
        gCameraRotation.multRight(deltaRotation);
        
        updateCamera();
        break;
	case VK_NUMPAD8:
		gCameraScale += 0.5;
		updateCamera();
		break;
	case VK_NUMPAD2:
		gCameraScale -= 0.5;
		updateCamera();
		break;
	case VK_ESCAPE:
		showToolTip("Reset Camera");
		resetCamera();
		break;
	
    }
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CProtoHapticView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
    {
    case VK_CONTROL:
        m_ctrl= false;
        break;
    case VK_SHIFT:
        m_shift= false;
        break;
    case VK_MENU:
        m_alt= false;
        break;
    }
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

bool CProtoHapticView::isHapticMouseOn()
{
	return m_hapticMouseOn;
}

void CProtoHapticView::OnEditbutton() 
{
	// TODO: Add your command handler code here
	if(getSelectedShape()!=NULL) {
		if(getSelectedShape()->getType()!=SHAPE_MEMO)
		{
			// We can delete this as it's our own copy of the shape
			if ( m_clipboardShape != NULL )
			{
				delete m_clipboardShape;
			}

			// Need to clone incase document is delete before paste
			m_clipboardShape= getSelectedShape()->clone ();
		}
	}
}

/** Rename shape and all sub-shapes recursivley with new default names */
void CProtoHapticView::ReDefaultNames ( CShape* s )
{
	// Give the new shape a default name
	s->SetName ( "" );
	CString name= GetDocument ()->GetFreeShapeName ();
	s->SetName ( name );

	// If shape is composite recurse into it
	if ( s->getType ( ) == SHAPE_COMPOSITE )
	{
		// Cast as composite
		CComposite* comp= (CComposite*)s;

		// For each sub-shape
		for ( int i= 0; i < comp->getShapeCount (); i++ )
		{
			ReDefaultNames ( comp->getShape ( i ) );
		}
	}
}

void CProtoHapticView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	if(m_clipboardShape!=NULL) 
	{
		GetDocument()->saveState();

		CShape* s= m_clipboardShape->clone();

		// Add to document
		int i= GetDocument()->AddShape(s);

		// Create new default name for shape
		// If the shape is composite then rename all sub-shapes (recursivley)
		ReDefaultNames ( s );
	
		HLdouble pp[3];
		hlGetDoublev ( HL_PROXY_POSITION, pp );

		GetDocument()->GetShape(i)->setLocation(pp[0],
												pp[1],
												pp[2]);
	}
}

void CProtoHapticView::OnEditCut() 
{
	// TODO: Add your command handler code here
	OnEditbutton();
	OnDelete();
}

void CProtoHapticView::resetCamera()
{
	if(GetDocument()->GetShapeCount()>0)
	{
		//if(m_sceneObject!=NULL) delete CShape::m_sceneObject;

		m_sceneObject= new CComposite();

		for(int i= 0; i<GetDocument()->GetShapeCount(); i++ ) {
				((CComposite*)m_sceneObject)->addShape(GetDocument()->GetShape(i));
			}

		flyToShape(m_sceneObject);
	} else {
		gCameraRotation.makeIdentity();
		gCameraScale = 1.0;
		gCameraTranslationX = 0;
		gCameraTranslationY = 0;
		gCameraTranslationZ= 0;
		updateCamera();
	}
}

void CProtoHapticView::showToolTip(CString s)
{
		if(!m_wndText)
			m_wndText.Create(CPoint(0,0),NULL,0);
		m_wndText.SetWindowText(s);
}

void CProtoHapticView::setToolTipVisable(bool v)
{
	m_showToolTip= v;
}

void CProtoHapticView::OnRestoreCamera() 
{
	// TODO: Add your command handler code here
	resetCamera();
}

void CProtoHapticView::NewDocument()
{
	m_deselect= true;
	resetCamera();
}

void CProtoHapticView::OnMemo() 
{
	// TODO: Add your command handler code here
	if(!m_edit_mode) return;
	
	CMemoDlg dlg(NULL);
	INT_PTR nRet = -1;
	nRet = dlg.DoModal();

	switch(nRet) {
	case IDOK:
		m_newMemoName= dlg.getName();
		setTool(TOOL_MEMO);
	break;
	case IDCANCEL:
		OutputDebugString("Cancelled.");
	break;
	default:
	break;
	}
}

void CProtoHapticView::OnGotoMemo() 
{
	// TODO: Add your command handler code here
	m_gotoDlg.ShowWindow(SW_SHOW);

	m_gotoDlg.clear();
	int i;
	for(i= 0; i < GetDocument()->getMemoCount(); i++) {
			m_gotoDlg.addName(((CMemo*)GetDocument()->getMemo(i))->getName());

	}
	
}

void CProtoHapticView::OnGotoMemo(int id)
{
	m_goingToMemo= true;
	hlBeginFrame();

	HLdouble wc_pos[3]={GetDocument()->getMemo(id)->getLocationX(),
						GetDocument()->getMemo(id)->getLocationY(),
						GetDocument()->getMemo(id)->getLocationZ()};

	hlProxydv(HL_PROXY_POSITION, wc_pos);
	hlPushMatrix();
	hlLoadMatrixd(m_matrix);

	//adjust for camera xfm's
	hduMatrix m;
	m.makeIdentity();
	m.multRight(gCameraRotation);
	m.invert();

	hlTranslatef(-gCameraTranslationX, -gCameraTranslationY, -gCameraTranslationZ);
	hlMultMatrixd(m);
	hlTranslatef(0,0,-(gCameraScale-gCameraPosWC[2]));

	HLdouble anchor[3];
	hlGetDoublev(HL_PROXY_POSITION, anchor);
	hlPopMatrix();

	//set spring
	hlEffectd(HL_EFFECT_PROPERTY_GAIN, 0.5);
	hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, 0.6);
	hlEffectdv(HL_EFFECT_PROPERTY_POSITION, anchor);
	//hlEffectd(HL_EFFECT_PROPERTY_DURATION, 3000.0);
	//hlTriggerEffect(HL_EFFECT_SPRING);

	if(!m_effectspring) {
		hlStartEffect(HL_EFFECT_SPRING, m_spring);
		m_effectspring= true;
	} else
		hlUpdateEffect(m_spring);

	hlEndFrame();

}

void CProtoHapticView::CenterCamera()
{
	resetCamera();
}

void CProtoHapticView::OnDocumentOptions() 
{
	// Save cpu cycles needed for generating dialog by 
	// disabling rendering of magnetic effect
	m_deselect= true;
	m_deselect1= true;

	docOptionsDlg.setDocument(GetDocument());
	docOptionsDlg.DoModal();
}

void CProtoHapticView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && GetDocument()->getHistoryCount()>0 )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );

}

void CProtoHapticView::OnUpdateButton32771(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
	pCmdUI->SetCheck(m_tool==TOOL_SELECT);
}

void CProtoHapticView::OnUpdateButton32772(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
	pCmdUI->SetCheck(m_tool==TOOL_CUBE);
}

void CProtoHapticView::OnUpdateButton32777(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
	pCmdUI->SetCheck(m_tool==TOOL_SPHERE);
}

void CProtoHapticView::OnUpdateButton32778(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
	pCmdUI->SetCheck(m_tool==TOOL_CYLINDER);
}

void CProtoHapticView::OnUpdateButton32785(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && m_selectedShapes->size()>1)
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateButton32786(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && m_selectedShapes->size()>0 && getSelectedShape()->getType()==SHAPE_COMPOSITE)
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateButtondelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && m_selectedShapes->size()>0)
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateButtonmemo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && m_selectedShapes->size()>0)
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && m_selectedShapes->size()>0)
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateEditDocoptions(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateEditShapeproperties(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && m_selectedShapes->size()>0)
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateToolCube(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateToolCylindercone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateToolDeleteshape(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && m_selectedShapes->size()>0)
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateToolGroupshapes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && m_selectedShapes->size()>1)
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateToolSelectiontool(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateToolSphere(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateToolUngroupshape(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode && m_selectedShapes->size()>0 && getSelectedShape()->getType()==SHAPE_COMPOSITE)
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateTriangleTool(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateTorusTool(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::OnUpdateTorusButton(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );

	pCmdUI->SetCheck(m_tool==TOOL_TORUS);
}

void CProtoHapticView::OnUpdateTriangleButton(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );

	pCmdUI->SetCheck(m_tool==TOOL_TRIANGLE);
}

void CProtoHapticView::OnUpdateButtonedit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_edit_mode );
}

void CProtoHapticView::OnUpdateButtonview(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( !m_edit_mode );
}

void CProtoHapticView::flyToShape(CShape *s)
{
	m_flyToShape= s;
	m_initFlyTo= false;
	m_flyToStep= 0;

	updateCamera();
}

void CProtoHapticView::OnAppOptions() 
{
	// Save cpu cycles needed for generating dialog by 
	// disabling rendering of magnetic effect
	m_deselect= true;
	m_deselect1= true;

	CAppOptionsDlg d;

	d.setView(this);
	d.DoModal();
	
	hapticOffset[0]= d.m_hapticOffsetX;
	hapticOffset[1]= d.m_hapticOffsetY;
	hapticOffset[2]= d.m_hapticOffsetZ;
}

void CProtoHapticView::OnUpdateEditAppoptions(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if( m_edit_mode )
        pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );	
}

void CProtoHapticView::DrawGLViewports(bool swap_buffers)
{
	bool plan= ((CProtoHapticApp*)AfxGetApp())->isPlan();
	bool elev= ((CProtoHapticApp*)AfxGetApp())->isElevation();
	bool front= ((CProtoHapticApp*)AfxGetApp())->isFrontElevation();

	float mainViewHeight= 1.0;
	float subViewWidth= 1.0;

	if(plan||elev||front) {
		int viewcount= 0;
		if(plan) viewcount++;
		if(elev) viewcount++;
		if(front) viewcount++;

		mainViewHeight= 0.7;
		subViewWidth= 1.0f/(float)viewcount;
	}

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

   ReSizeGLScene(0, 0, gWindowWidth,mainViewHeight*gWindowHeight);

    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
	GLdouble x, y, z;
	double pp[3];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

	HHLRC rc= hlGetCurrentContext();
	hlMakeCurrent(m_hHLRC);

	hlGetDoublev(HL_PROXY_POSITION, pp);

	gluProject(pp[0],
               pp[1],
               pp[2],
               modelview,
               projection,
               viewport,
               &x,
               &y,
               &z);

	if(x>0&&x<viewport[2]&&
		y>0&&y<viewport[3]){
		m_hapticMouseOn= true;
	}else{
		m_hapticMouseOn= false;
	}

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

	viewport[1]= -80;//Account for toolbars
	// Account for other viewports & toolbar
	if(plan||elev||front) viewport[1]= -80-((1-mainViewHeight)*gWindowHeight);
	if(mouseOn)
		hmSetMouseTransforms(modelview,projection,viewport);

	hlMakeCurrent(rc);

	// Main view
	ReSizeGLScene(0, 0, gWindowWidth,mainViewHeight*gWindowHeight); 
	DrawGLScene(false);

	if(plan||elev||front)
	{
		glPushMatrix();

		float maxsize;
		float sizeX;
		float sizeY;
		float sizeZ;
		float locationX;
		float locationY;
		float locationZ;
		float minX= FLT_MAX;
		float maxX= -FLT_MAX;
		float minY= FLT_MAX;
		float maxY=- FLT_MAX;
		float minZ= FLT_MAX;
		float maxZ= -FLT_MAX;

		if(m_transform==NULL) {
			int i;
			for(i= 0; i<GetDocument()->GetShapeCount(); i++)
			{
				//Find the extreem shapes which define the bounding box
				//Get bounding cube corners
				GetDocument()->GetShape(i)->getCorners();
				
				int j;
				for(j= 0; j<8; j++)
				{
					if(GetDocument()->GetShape(i)->getCorner(j)[0]>maxX) maxX= GetDocument()->GetShape(i)->getCorner(j)[0];
					if(GetDocument()->GetShape(i)->getCorner(j)[0]<minX) minX= GetDocument()->GetShape(i)->getCorner(j)[0];
					if(GetDocument()->GetShape(i)->getCorner(j)[1]>maxY) maxY= GetDocument()->GetShape(i)->getCorner(j)[1];
					if(GetDocument()->GetShape(i)->getCorner(j)[1]<minY) minY= GetDocument()->GetShape(i)->getCorner(j)[1];
					if(GetDocument()->GetShape(i)->getCorner(j)[2]>maxZ) maxZ= GetDocument()->GetShape(i)->getCorner(j)[2];
					if(GetDocument()->GetShape(i)->getCorner(j)[2]<minZ) minZ= GetDocument()->GetShape(i)->getCorner(j)[2];
				}
			}

			//update my position/size based on this
			sizeX= (maxX-minX);
			sizeY= (maxY-minY);
			sizeZ= (maxZ-minZ);
			locationX= (float)(minX+sizeX/2.0);
			locationY= (float)(minY+sizeY/2.0);
			locationZ= (float)(minZ+sizeZ/2.0);		

			if(sizeX>sizeY&&sizeX>sizeZ) maxsize= sizeX;
			if(sizeY>sizeX&&sizeX>sizeZ) maxsize= sizeY;
			if(sizeZ>sizeY&&sizeX>sizeX) maxsize= sizeZ;
		} else {
			locationX= getSelectedShape()->getLocationX();
			locationY= getSelectedShape()->getLocationY();
			locationZ= getSelectedShape()->getLocationZ();

			sizeX= getSelectedShape()->getSizeX();
			sizeY= getSelectedShape()->getSizeY();
			sizeZ= getSelectedShape()->getSizeZ();

			if(sizeX>sizeY&&sizeX>sizeZ) maxsize= sizeX;
			if(sizeY>sizeX&&sizeX>sizeZ) maxsize= sizeY;
			if(sizeZ>sizeY&&sizeX>sizeX) maxsize= sizeZ;
		}


		float leftside= 0.0;

		// Plan view
		if(plan) {
			ReSizeGLScene(leftside, mainViewHeight*gWindowHeight, leftside+gWindowWidth*subViewWidth,gWindowHeight);

			glLoadIdentity();
			if(GetDocument()->GetShapeCount()>0)
			{
				glTranslated(0,0,-10);
				glRotated(90,1,0,0);
				glTranslated(-locationX,-locationY,-locationZ);
			} 
			DrawGLScene(false);
			leftside+= gWindowWidth*subViewWidth;
		}

		// Elevation view
		if(elev) {
			ReSizeGLScene(leftside, mainViewHeight*gWindowHeight, leftside+gWindowWidth*subViewWidth,gWindowHeight);

			glLoadIdentity();
			if(GetDocument()->GetShapeCount()>0)
			{
				glTranslated(0,0,-10);
				//glRotated(90,0,0,0);
				glTranslated(-locationX,-locationY,-locationZ);
			}
			DrawGLScene(false);
			leftside+= gWindowWidth*subViewWidth;
		}

		// Front Elevation view
		if(front) {
			ReSizeGLScene(leftside, mainViewHeight*gWindowHeight, leftside+gWindowWidth*subViewWidth,gWindowHeight);

			glLoadIdentity();
			if(GetDocument()->GetShapeCount()>0)
			{
				glTranslated(0,0,-10);
				glRotated(90,0,1,0);
				glTranslated(-locationX,-locationY,-locationZ);
			}
			DrawGLScene(false);
			leftside+= gWindowWidth*subViewWidth;
		}

		glPopMatrix();
	}

	if(swap_buffers)
		SwapBuffers(m_hgldc);
}

void CProtoHapticView::DeInitHaptics()
{
   // OpenHaptics setup follows:
	mouseOn= false;

	hlDeleteContext(m_hHLRC);
	hdDisableDevice(m_hHD);
	if(m_usingTwoDevices) {
		hlDeleteContext(m_hHLRC_1);
		hdDisableDevice(m_hHD_1);
	}

	if(!InitHaptics())
	{
		::MessageBox(::GetFocus(),"Error: Failed to Initialise Haptics.\nAttempting to roll-back to stable configuration","Error",MB_OK);
		((CProtoHapticApp*)AfxGetApp())->LoadOptions();
		if(!InitHaptics())
			::MessageBox(::GetFocus(),"Error: Roll-back failed: FATAL ERROR!.\n","Error",MB_OK);
	}

	GetDocument()->Reload();


}

void CProtoHapticView::Pause()
{
	KillTimer(m_timer);
}

void CProtoHapticView::Play()
{
	m_timer= SetTimer(1, 42, 0);
}

void CProtoHapticView::setDragging1(bool b)
{
	m_dragging1= b;
}

void CProtoHapticView::setDragStart1(double start[])
{
	m_dragStart1[0]= start[0];
	m_dragStart1[1]= start[1];
	m_dragStart1[2]= start[2];
}

void CProtoHapticView::SetCodeView(CView *v)
{
	m_codeView= v;
}

void CProtoHapticView::Deselect()
{
	m_selectedShapes->clear();
	m_selectedShapes1->clear();
}

void CProtoHapticView::OnToolGroupall()
{
	// TODO: Add your command handler code here

		CComposite *c= new CComposite(); 
		
		// Create a default name for the composite
		CString name= GetDocument ()->GetFreeShapeName ();
		c->SetName ( name );

		int tmp= GetDocument ()->GetShapeCount ();

		for( int i= 0; i < tmp; i++ ) {
			CShape* shape= GetDocument()->GetShape ( 0 );

			c->addShape(shape);
			GetDocument()->RemoveShape(shape);
		}

		GetDocument()->AddShape(c);
		m_selectedShapes->clear();
		m_selectedShapes->insert(c);
}
