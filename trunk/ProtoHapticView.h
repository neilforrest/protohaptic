// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: ProtoHapticView.h
// Classes: CProtoHapticView
// Purpose: Represents view onto a document or model

#if !defined(AFX_PROTOHAPTICVIEW_H__4CA2E291_84C9_4072_BA1C_47467E412CC4__INCLUDED_)
#define AFX_PROTOHAPTICVIEW_H__4CA2E291_84C9_4072_BA1C_47467E412CC4__INCLUDED_

#include "DocumentOptionsDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// OpenGL Headers
#include <gl/gl.h>
#include <gl/glu.h>

// OpenHaptics Headers
#include <HL/hl.h>

#include <set>
using std::set;

#include "Shape.h"
#include "Cube.h"
#include "Transform.h"
#include "PupText.h"
#include "GotoMemo.h"
#include "DocumentOptionsDlg.h"
#include "ShapePropertiesDlg.h"
#include "PupText.h"

#include <HL/hl.h>
#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>


/* Tool constants */
#define TOOL_SELECT		0
#define TOOL_CUBE		1
#define TOOL_SPHERE		2
#define TOOL_CYLINDER	3
#define TOOL_PASTE		4
#define TOOL_MEMO		5
#define TOOL_TRIANGLE	6
#define TOOL_TORUS		7

/** Represents a graphic and haptic view onto a CProtoHapticDoc document or model */
class CProtoHapticView : public CView
{
protected: // create from serialization only
	/** Create a new view, from serialization only */
	CProtoHapticView();

	DECLARE_DYNCREATE(CProtoHapticView)

	/** The currently selected tool */
	int m_tool;

	/** The goto memo dialog */
	CGotoMemo m_gotoDlg;

	/** A HL effect ID for springing to memos */
	int m_spring;

	/** Is the spring to memos effect on? */
	bool m_effectspring;

// Attributes
public:
	// Tmp
	float hapticOffset[3];

	// OpenHaptics Contexts

	// Phantom 0
	/** Phantom Device Context for primary device */
	HHD m_hHD;

	/** Phantom HL Context for primary device */
	HHLRC m_hHLRC;

	// Phantom 1
	/** Phantom Device Context for second device */
	HHD m_hHD_1;

	/** Phantom HL Context for second device */
	HHLRC m_hHLRC_1;

	/** Are we in edit or view mode? */
	bool m_edit_mode;

	/** Returns the document used by this view */
	CProtoHapticDoc* GetDocument();

	/** Get the shape currently selected by device 1 */
	CShape* getSelectedShape();

	/** Get the shape currently selected by device 2 */
	CShape* getSelectedShape1();

	/** The currently active shape transformation for device 1. */
	CTransform* m_transform;

	/** Sets the currently active tool for both devices */
	void setTool(int tool_id);

	/** Gets the currently active tool for both devices */
	int getTool();

// Operations
public:
	/** Test and return integer score for the camera rotation matF. 
		prev_count being the previous best score (for optimisation) */
	int CProtoHapticView::tryCameraRotation ( hduMatrix matF, int prev_count );

	/** Set of shapes currently selected by device 1 */
	std::set<CShape*> *m_selectedShapes;

	/** Set of shapes currently selected by device 2 */
	std::set<CShape*> *m_selectedShapes1;

	/** If true, will safely clear shapes selected by device 1 */
	bool m_deselect;

	/** If true, will safely clear shapes selected by device 2 */
	bool m_deselect1;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProtoHapticView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void Deselect();
	/** Sets the associated code view so that we may update it */
	void SetCodeView(CView *v);

	/** The transformation currently in progress by device 2. NULL if none */
	CTransform *m_transform1;

	/** Sets the initial drag location for second device */
	void setDragStart1(double start[]);

	/** Sets whether the second device is dragging */
	void setDragging1(bool b);

	/** Begin rendering loop. By default loop is running. Only call after a call
	    to Pause() */
	void Play();

	/** Pause the rendering loop. Used to prevent exceptions when modifying rendering 
	    contexts and other sensitive properties of the rendering loop. Restart the loop after
		modification with Play() */
	void Pause();

	/** Shut down and restart haptic rendering. Reload haptic configuration from application.
	    Rendering loop must be paused */
	void DeInitHaptics();

	/** Top level OpenGL rendering procedure. Called from OnTimer() */
	void DrawGLViewports(bool swap_buffers);

	/** Sets the fly-to shape used to change the camera angle to focus on a shape */
	void flyToShape(CShape *s);

	/** The shape properties dialog */
	CShapePropertiesDlg shapeDlg;

	/** Move the camera to a "sensible"  position */
	void CenterCamera();

	/** Take device 1 to memo id given */
	void OnGotoMemo(int id);

	/** True iff currently going to memo */
	bool m_goingToMemo;

	/** The name to give to the new  */
	CString m_newMemoName;

	/** Load a new, empty document */
	void NewDocument();

	/** Display tooltips for edit points */
	void setToolTipVisable(bool v);

	/** Set the tooltip text to be displayed next to
	    the cursor */
	void showToolTip(CString s);

	/** Centre the camera of the currently selected shape */
	void resetCamera();

	/** Returns true iff the haptic device is controlling the desktop mouse */
	bool isHapticMouseOn();

	/** Returns a pointer the the shape currently in the view's clipboard */
	CShape* getClipboardShape();

	/** Set the location of the first devices drag start */
	void setDragStart(double start[3]);

	/** Set to signify that the first device is dragging */
	void setDragging(bool d);

	/** Rename shape and all sub-shapes recursivley with new default names */
	void ReDefaultNames ( CShape* s );

	/** Destroy the view */
	virtual ~CProtoHapticView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// OpenGL Contexts
	/** OpenGL GDI Device Context */
	HDC m_hgldc;
	/** OpenGL Rendering Context */
	HGLRC m_hglrc;

	/** Cursor local scaling factor */
	double m_gCursorScale;

	/** Display list to render cursor */
	GLuint gCursorDisplayList;

	/** Effect ID for dragging effect */
	HLuint m_effectName, 
		
	/** Effect ID for dragging effect */
		m_effectName1;

	/** OpenGL initialisation procedure */
	BOOL SetPixelformat(HDC hdc);

	/** Resizes OpenGL viewport and recalculates projection matrix */
	void ReSizeGLScene(int x, int y, int width, int height);

	/** Render the scene graphically in the current viewport 
	    Set swap buffers to true if the result is to be visible straight away */
	void DrawGLScene(bool swap_buffers);

	/** Render the scene haptically to either primary or secondary rendering context */
	void DrawHapticScene(bool primary_context);

	/** Initialise OpenGL for graphical rendering */
	int InitGL(void);

	/** Initialise Haptics loading settings from the application */
	int InitHaptics(void);

	/** Update the haptic workspace mapping following a change to the projection matrix */
	void UpdateWorkspace();

	/** Render the haptic cursor of the current context visually */
	void DrawCursor();

	/** Applies new camera transformations */
	void updateCamera();

	/** Implement any shape transformations currently ongoing */	
	void DoTransformations();

	/** Draws the selection cube between dragStart and current proxy position */
	void DrawDragArea1();

// Generated message map functions
protected:
	/** Device 2 is dragging and drag effect is on */
	bool isDrag1;

	/** The associated code view */
	CView* m_codeView;

	/** The location of drag start for device 2 */
	float m_dragStart1[3];

	/** Device 2 is dragging */
	bool m_dragging1;

	/** Using two haptic devices */
	bool m_usingTwoDevices;

	/** Main OnTimer callback ID */
	UINT_PTR m_timer;

	/** The shape on which to centre the camera */
	CShape *m_flyToShape;

	/** A composite object containing all the shapes in the scene.
	    Used to centre the camera */
	CShape *m_sceneObject;

	/** Dialog to control document options */
	CDocumentOptionsDlg docOptionsDlg;

	/** The original touch workspace matrix save once before first render */
	HLdouble m_matrix[16];

	/** Indicates that the touch workspace matrix has been saved and need not be again */
	bool m_loaded;

	/** The index of the currently active memo point */
	int m_activeMemoID;

	/** Edit point tool tips are on */
	bool m_toolTipOn;

	/** The tool tip should be displayed */
	bool m_showToolTip;

	/** The pop up text window for the tool tip */
	CPopupText m_wndText;

	/** The haptic device is controlling the desktop mouse */
	bool m_hapticMouseOn;

	/** The alt key is depressed */
	bool m_alt;

	/** The shift key is depressed */
	bool m_shift;

	/** The control key is depressed */
	bool m_ctrl;

	/** The clipboard shape, from local copy and cut commands */
	CShape* m_clipboardShape;

	/** Device 1 is dragging and the drag effect is on */
	bool isDrag;

	/** Draw the selection cube for device 1, from dragStart to current position */
	void DrawDragArea();

	/** Device 1 is dragging */
	bool m_dragging;

	/** Device 1 drag start location */
	float m_dragStart[3];

	/** The camera translation into/out-of the screen */
	float gCameraTranslationZ;

	// Camera "Fly To Shape" or "Centre on Shape" variables
	//=====================================================

	/** When fly to shape != NULL, m_initFlyTo is false until
	    the fly to variables have been calculated. Done when 
		fly to shape is first set */
	bool m_initFlyTo;

	/** The number of draws since fly to shape began */
	int m_flyToStep;

	/** The amount to increment camera x position by each draw */
	float m_flyToDeltaX;

	/** The amount to increment camera y position by each draw */
	float m_flyToDeltaY;

	/** The amount to increment camera z position by each draw */
	float m_flyToDeltaZ;

	/** The amount to increment camera scale by each draw */
	float m_flyToDeltaScale;

	/** The axis about which to rotate the camera during fly to */
	hduVector3Dd m_flyToAxis;

	/** The total angle to rotate camera during fly to */
	double m_flyToAngle;
	
	/** The angle to rotate camera each draw step of the fly to */
	double m_flyToDeltaAngle;

	/** The index, in the document, of the fly to shape */
	int m_flyToIndex;

	//{{AFX_MSG(CProtoHapticView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSelectTool();
	afx_msg void OnCubeTool();
	afx_msg void OnSphereTool();
	afx_msg void OnCylinderTool();
	afx_msg void OnTriangleTool();
	afx_msg void OnTorusTool();
	afx_msg void OnShapeProperties();
	afx_msg void OnEditMode();
	afx_msg void OnViewMode();
	afx_msg void OnCenter();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonView();
	afx_msg void OnGroup();
	afx_msg void OnUnGroup();
	afx_msg void OnDelete();
	afx_msg void OnEditUndo();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditbutton();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnRestoreCamera();
	afx_msg void OnMemo();
	afx_msg void OnGotoMemo();
	afx_msg void OnDocumentOptions();
	afx_msg void OnUpdateTriangleButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTriangleTool(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTorusButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTorusTool(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButton32771(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButton32772(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButton32777(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButton32778(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButton32785(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButton32786(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtondelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonmemo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDocoptions(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditShapeproperties(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolCube(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolCylindercone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolDeleteshape(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolGroupshapes(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolSelectiontool(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolSphere(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolUngroupshape(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonedit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonview(CCmdUI* pCmdUI);
	afx_msg void OnAppOptions();
	afx_msg void OnUpdateEditAppoptions(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnToolGroupall();
};

#ifndef _DEBUG  // debug version in ProtoHapticView.cpp
inline CProtoHapticDoc* CProtoHapticView::GetDocument()
   { return (CProtoHapticDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROTOHAPTICVIEW_H__4CA2E291_84C9_4072_BA1C_47467E412CC4__INCLUDED_)
