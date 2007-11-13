// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Composite.h
// Classes: CComposite
// Purpose: Represents a composite shape

#if !defined(AFX_COMPOSITE_H__0EF26097_134D_4F0A_A221_578632B46540__INCLUDED_)
#define AFX_COMPOSITE_H__0EF26097_134D_4F0A_A221_578632B46540__INCLUDED_

/// The maximum number of shapes a composite may contain
#define MAX_COMPOSITE_SHAPE 1000

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include <gl/gl.h>
#include <gl/glu.h>

/** Represents a composite shape.
    A composite may contain no more than MAX_COMPOSITE_SHAPE member shapes.
    All shapes should be added to a composite before it is rendered */
class CComposite : public CShape 
{
private:

public:
	/** Overrides CShape to render the composite shape as OpenGL commands */
	virtual CString drawGLCode();

	/** Overrides CShape to set the colour of all member shapes
	    Components retain their original colour until this is called */
	void setColour(float r, float g, float b);

	/** Overrides CShape to get the red component of the composites colour.
	    Initially returns 1.0 when this has not been set */
	float getRed();

	/** Overrides CShape to get the green component of the composites colour.
	    Initially returns 1.0 when this has not been set */
	float getGreen();

	/** Overrides CShape to get the blue component of the composites colour.
	    Initially returns 1.0 when this has not been set */
	float getBlue();

	/** Return pointer to member shape at i */
	CShape* getShape(int i);

	/** Returns the number of shapes in the composite */
	int getShapeCount();

	/** Overrides CShape to set the location of the composite */
	void setLocation ( float x, float y, float z );

	/** Overrides CShape to set the rotation of the composite */
	void setRotation ( float R[16] );

	/** Overrides CShape to set the size of the composite.
	    Must maintain aspect in order to prevent skewing */
	void setSize ( float x, float y, float z );

	/** Create a new, empty composite */
	CComposite();

	/** Destroy the composite *and its members* */
	virtual ~CComposite();

	/** Add a shape to the composite.
	    *do not destroy shape afterwards* */
	void addShape(CShape *shape);

	/** Overrides CDrawable to render the composite graphically */
	void drawGLScene();

	/** Overrides CFeelable to render the composite haptically */
	void drawHapticScene(bool primary_context);

	/** Overrides CShape and returns an integer representing the type of shape.
	    See shape.h for constant definitions. */
	int getType();

	/** Store/Load data to/from archive ar */
	virtual void Serialize(CArchive& ar);

	/** Create and return a copy of this composite.
	    Member shapes are also cloned. */
	CShape* clone();

	// Override from feelable, apply to self and all subshapes also

	/** Set the stiffness haptic property
		 Value between 0..1 */
	void setStiffness(float stiffness);
	
	/** Set the dampening haptic property
		 Value between 0..1 */
	void setDampening(float dampening);
	
	/** Set the static friction haptic property
		 Value between 0..1 */
	void setStaticFriction(float staticFriction);
	
	/** Set the dynamic friction haptic property
		 Value between 0..1 */
	void setDynamicFriction(float dynamicFriction);

	void SetControlMembersHaptic ( bool c );

	bool IsControlMembersHaptic ( );

	/** Return true iff feelable is being touched in context c */
	bool touching ();
	
	/** Return true iff feelable is being touched in context c_1 */
	bool touching1 ();

protected:
	/// The initial sizes of composite members (when they were added)
	float m_shapesStartSize[MAX_COMPOSITE_SHAPE][3];

	/// The initial size of the composite (when last member was added)
	float m_startSize[3];

	/// The initial rotations of composite members (when they were added)
	float m_rotations[MAX_COMPOSITE_SHAPE][16];

	/** True iff the composite has not yet been rendered
	    Used to save off initial member values */
	bool m_firstTime;

	/// The initial displacement of members from the centre of the composite
	float m_offset[MAX_COMPOSITE_SHAPE][3];

	/// The number of shapes in this composite
	int m_shapeCount;

	bool m_controlMembersHaptic;

	/// An array of pointers to shapes in this composite
	CShape* m_shapes[MAX_COMPOSITE_SHAPE];
};


#endif // !defined(AFX_COMPOSITE_H__0EF26097_134D_4F0A_A221_578632B46540__INCLUDED_)
