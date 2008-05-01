// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: ProtoHapticDoc.h
// Classes: CProtoHapticDoc
// Purpose: Represents a document or model

#if !defined(AFX_PROTOHAPTICDOC_H__90146C70_9FCB_4AA9_84DE_31EFE81D4AA1__INCLUDED_)
#define AFX_PROTOHAPTICDOC_H__90146C70_9FCB_4AA9_84DE_31EFE81D4AA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ode/ode.h"
#include "shape.h"
#include "hl/hl.h"

#include <time.h>

/** The maximum number of base shapes in a document.
    Composites of any size count as one shape */
#define MAX_SHAPES 1000

/** The maximum number of undo operations to support */
#define MAX_HISTORY 20

/** Represents a ProtoHaptic document or scene model */
class CProtoHapticDoc : public CDocument
{
protected: // create from serialization only
	/** Construct a new document, from serialization only */
	CProtoHapticDoc();

	DECLARE_DYNCREATE(CProtoHapticDoc)

	// Is dynamic surface change enabled in view mode
	bool m_dynamicSurfaceChange;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProtoHapticDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Enable/Disable dynamic surface change in view mode
	void EnableDynamicSurfaceChange ( bool enable );

	// Is dynamic surface change enabled in view mode
	bool IsDynamicSurfaceChangeEnabled ( );

	/// True if the specified shape name is valid (for this root shape and sub-shapes):
	/// i.e. Is not taken by any shape or subshape in document
	bool IsShapeNameFree ( CString name, CShape* s );

	/// Returns a free shape name that is not taken by any shape or sub-shape in the document
	CString GetFreeShapeName ( );

	/// Gets the max value of x in shape name "shapeX" recursivley for all shapes and sub-shapes
	/// with s as a root
	int GetMaxShapeName ( CShape* s );

	// Convert string to int
	bool StringToInt ( char* str, int* integer );

	/// True if the specified shape name is valid:
	/// A: Has correct form (c/c++ variable name syntax, i.e. no-spaces)
	/// B: Is not taken by any shape or subshape in document
	bool IsShapeNameFree ( CString name );

	/** Reload the document creating copies of all the shapes.
	    Called after changing the haptic device configuration to ensure
		shape IDs are re-allocated following the change in context */
	void Reload();

	/** Sets the two rendering contexts for the two haptic devices.
	    If only primary device is in use b is not specified */
	void setContexts(HHLRC a, HHLRC b);

	/** Number of historic documents stored for undo purposes */
	int getHistoryCount();

	/** Returns true iff document is to hide memos when in view mode */
	bool isHideMemos();

	/** Set whether document should hide its memos when in view mode. 
	    When hidden, they may still be accessed */
	void setHideMemos(bool b);

	/** Called before entering view mode. (i.e. Before first call to SimulationStep()).
	    Must be called at beginning of each simulation cycle */
	void PreSimulation();

	/** Returns the document's speed of simulation. Related to the number of frames to
	    simulate at each call to SimulationStep() */
	float getSimSpeed();

	/** Sets the simulation speed. Related to the number of frames to
	    simulate at each call to SimulationStep() */
	void setSimSpeed(float s);

	/** Returns the air resistance set for this document. At zero, objects will
	    move forever once a force acts on them */
	float getAirResistance();

	/** Sets the air resistance for this document. At zero, objects will
	    move forever once a force acts on them */
	void setAirResistance(float a);

	/** Gets the gravity force for this document. This is a constant downwards force
	    designed to simulate gravity of a planet. */
	float getGravity();

	/** Sets the gravity force for this document. This is a constant downwards force
	    designed to simulate gravity of a planet. */
	void setGravity(float g);

	/** Called by collision detection engine when two objects are in close proximity and
	    may collide */
	void nearCallback(dGeomID o1, dGeomID o2);

	/** Updates the physics and collision engines with information from the document */
	void UpdateDynamics();

	/** Does a physics simulation step and updates the document base on the results of the
	    physics and collision engines */
	void SimulationStep();

	/** Scale the whole model, centred at the origin by the specified amount */
	void ScaleModel ( float f );

	/** Translate the whole model by the specified amount */
	void TranslateModel ( float x, float y, float z );

	/** Destroy the documents shapes, memos and history. Called automatically by destructors */
	void clean();

	/** Removes the given memo point from the document */
	void removeMemo(CShape *memo);

	/** Adds the specified memo to the document. Object must persist for lifespan of document */
	int addMemo(CShape *memo);

	/** Returns the memo at index ID. ID is between 0 and getMemoCount() */
	CShape* getMemo(int id);

	/** Returns the number of memos in the document */
	int getMemoCount();

	/** Rolls the document back to the state it was in last time SaveState() was called */
	void undo();

	/** Saves the state of the document to top of history stack. The state may be recovered with undo() */
	void saveState();

	/** Return a copy of this document. Used by saveState() for undo() */
	CProtoHapticDoc* clone();

	/** Removes the shape specified from this document */
	void RemoveShape(CShape* shape);

	/** Destroys the current document. Calls clean() */
	virtual ~CProtoHapticDoc();

	/** Returns the number of shapes in the document */
	int GetShapeCount();

	/** Returns the shape at index. Index is between 0 and getShapeCount() */
	CShape* GetShape(int index);

	/** Adds a shape to the document */
	int AddShape(CShape* shape);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	/** Number of shapes in document */
	int m_shapeCount;

	/** An array of shapes in document */
	CShape* m_shapes[MAX_SHAPES];

	/** Number of clock ticks at last simulation step */
	clock_t m_lastSimStep;

	/** Haptic rendering context for primary device */
	HHLRC m_context;

	/** Haptic rendering context for secondary device. Only
	    defined if the application is using two devices */
	HHLRC m_context_1;

// Generated message map functions
protected:
	/** Hide memos in view mode */
	bool m_hideMemos;

	/** Document's air resistance */
	float m_airResistance;

	/** Document's simulation speed */
	float m_simSpeed;

	/** Document's gravity force */
	float m_gravity;

	/** Joint group for collision contact */
	dJointGroupID m_jointGroup;

	/** Collision models for shapes */
	dGeomID m_geoms[MAX_SHAPES];

	/** Space ID allocated by collision engine */
	dSpaceID m_spaceID;

	/** Body IDs allocated by physics engine */
	dBodyID bodies[MAX_SHAPES];

	/** World ID allocated by physics engine */
	dWorldID m_worldID;

	/** Number of memos in this document */
	int m_memoCount;

	/** An array of memos in this document */
	CShape* m_memos[MAX_SHAPES];

	/** Number of historic documents stored */
	int m_historyCount;

	/** Pop and return topmost document of history stack */
	CProtoHapticDoc* popHistory();

	/** Push document to history stack */
	void pushHistory(CProtoHapticDoc* doc);

	/** Array representing the history stack */
	CProtoHapticDoc* m_history[MAX_HISTORY];

	//{{AFX_MSG(CProtoHapticDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROTOHAPTICDOC_H__90146C70_9FCB_4AA9_84DE_31EFE81D4AA1__INCLUDED_)
