// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: ProtoHaptic.h
// Classes: CProtoHapticApp, CAboutDlg
// Purpose: Define behaviour of ProtoHaptic application

#if !defined(AFX_PROTOHAPTIC_H__1AEBC4E0_3438_408A_8D79_979E9D4D6B94__INCLUDED_)
#define AFX_PROTOHAPTIC_H__1AEBC4E0_3438_408A_8D79_979E9D4D6B94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// sound effects
// =============

/** Edit point acquire sound effect */
#define EDIT_POINT_ON "ding.wav"

/** Touch shape sound effect */
#define TOUCH_SHAPE   "start.wav"

/** Untouch shape sound effect */
#define UNTOUCH_SHAPE   "start.wav"

/** Snap to sound effect */
#define SNAP_TO		  "snap.wav"

/** Define the behaviour of the ProtoHaptic application */
class CProtoHapticApp : public CWinApp
{
public:
	/** Returns the number of stacks and slices to be used for rendering curved shapes */
	int getStacks();

	/** Sets the number of stacks and slices to be used for rendering curved shapes */
	void setStacks(int s);

	/** The graphical view */
	CView* m_graphicalView;
	
	/** The code view */
	CView* m_codeView;
	
	/** Switch to the view v */
	CView* SwitchView( CView *v );

	/** Returns true iff audio feedback for touching a shape is on */
	bool isAudioTouch();
	
	/** Returns true iff audio feedback for edit point acquire is on */
	bool isAudioEditPoint();
	
	/** Returns true iff audio feedback for snap to is on */
	bool isAudioSnapTo();
	
	/** Set audio alerts when snapping to */
	void setAudioSnapTo(bool b);
	
	/** Set audio alerts when edit point is acquired */
	void setAudioEditPoint(bool b);
	
	/** Set audio alerts when shape is touched */
	void setAudioTouch(bool b);
	
	/** Set to true to use two haptic devices, false otherwise */
	void setTwoDevices(bool b);
	
	/** Returns true iff application is using two haptic devices */
	bool isTwoDevices();
	
	/** Set the second haptic device passive: view mode only, or
	    active: view and edit modes */
	void setPassive(bool b);
	
	/** Use the default haptic device as the primary device */
	void setUseDefault(bool b);
	
	/** Sets the name of the secondary haptic device. Must also be 
	    enabled with setTwoDevices() */
	void setSecondaryDevice(CString s);
	
	/** Sets the name of the primary haptic device. setUseDefault must
	    be set to false */
	void setPrimaryDevice(CString s);
	
	/** Returns true iff the second haptic device is passive */
	bool isPassive();
	
	/** Returns true iff the first haptic device is assigned to default */
	bool isUseDefault();

	/** Returns the name of the second haptic device, if set */
	CString getSecondaryDevice();

	/** Returns the name of the primary haptic device */
	CString getPrimaryDevice();

	/** Load the application options from the settings file: settings.dat */
	void LoadOptions();
	
	/** Save the application options to the settings file: settings.dat */
	void SaveOptions();
	
	/** Get the edit point snap distance */
	float getEditPointSnapDist();
	
	/** Sets the edit point snap distance */
	void setEditPointSnapDist(float f);
	
	/** Return the size of the edit points */
	float getEditPointSize();
	
	/** Sets the size of the edit points rendered */
	void setEditPointSize(float f);
	
	/** Gets the length of the rotation guess vector:
	    How long to wait before guessing */
	float getRotationGuess();
	
	/** Set the rotation guesvector:
	    How long to wait before guessing */
	void setRotationGuess(float f);

	/** Returns true iff the front elevation viewing pane is enabled */
	bool isFrontElevation();
	
	/** Set whether the front elevation viewing pane is enabled */
	void setFrontElevation(bool b);

	/** Returns true iff the side elevation viewing pane is enabled */
	bool isElevation();
	
	/** Set whether the side elevation viewing pane is enabled */
	void setElevation(bool b);

	/** Returns true iff the plan viewing pane is enabled */
	bool isPlan();
	
	/** Set whether the plan viewing pane is enabled */
	void setPlan(bool b);

	/** Default constructor */
	CProtoHapticApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProtoHapticApp)
	public:
	virtual BOOL InitInstance();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CProtoHapticApp)
	afx_msg void OnAppAbout();
	afx_msg void OnEditCopy();
	afx_msg void OnViewCode();
	afx_msg void OnViewGraphic();
	//afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int m_stacksSlices;
	/** Audio snap to alerts are on/off */
	bool m_audioSnapTo;
	
	/** Audio edit point acquire alerts are on/off */
	bool m_audioEditPoint;
	
	/** Audio shape touch alerts are on/off */
	bool m_audioTouch;
	
	/** Using two haptic devices: on/off */
	bool m_twoDevices;
	
	/** Second device is passive: on/off */
	bool m_passive;
	
	/** Use default PHANToM as primary device */
	bool m_useDefault;
	
	/** The name of the second haptic device */
	CString m_secondaryDevice;
	
	/** The name of the first haptic device */
	CString m_primaryDevice;
	
	/** Front elevation viewport: on/off */
	bool m_frontElevation;
	
	/** Side elevation viewport: on/off */
	bool m_elevationViewport;
	
	/** Plan viewport: on/off */
	bool m_planViewport;
	
	/** Length of the rotation guess vector */
	float m_rotationGuess;
	
	/** Distance to snap to a control point */
	float m_editPointSnapDist;
	
	/** Size of edit points */
	float m_editPointSize;
public:
	afx_msg void OnImportAbstractmodelcode();
public:
	afx_msg void OnImportGhostcode();
public:
	afx_msg void OnFileImportAm();
public:
	afx_msg void OnFileImportGhost();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROTOHAPTIC_H__1AEBC4E0_3438_408A_8D79_979E9D4D6B94__INCLUDED_)
