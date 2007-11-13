// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006
// File: Memo.h
// Classes: CMemo
// Purpose: A memo point object

#if !defined(AFX_MEMO_H__F39349B1_2600_40A9_8568_3600962F6019__INCLUDED_)
#define AFX_MEMO_H__F39349B1_2600_40A9_8568_3600962F6019__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"

/** A memo point object. An external memory aid to which users
    may be actively directed to */
class CMemo : public CShape  
{
public:
	/** Create a new memo */
	CMemo();
	
	/** Get the string identifying this memo */
	CString getName();
	
	/** Create a new memo with the give string identifier */
	CMemo(CString name);
	
	/** Destroy the memo point */
	virtual ~CMemo();
	
	/** Overrides CDrawable to render the memo graphically */
	void drawGLScene();
	
	/** Overrides CShape to return the OpenGL code for rendering the memo */
	virtual CString drawGLCode();
	
	/** Overrides CFeelable to render the memo haptically */
	void drawHapticScene(bool primary_context);
	
	/** Return an integer representing the type of this shape (i.e. memo)
		See Shape.h for a list of values */
	int getType();
	
	/** Store/Load the cube's data to the archive ar */
	virtual void Serialize(CArchive& ar);
	
	/** Return a pointer to a copy of this cube */
	CShape* clone();
	
protected:
	/** The memo identifier */
	CString m_name;
};

#endif // !defined(AFX_MEMO_H__F39349B1_2600_40A9_8568_3600962F6019__INCLUDED_)
