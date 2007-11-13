// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Shape.h
// Classes: CShape
// Purpose: Represents a shape

#ifndef SHAPE
#define SHAPE

#include "Drawable.h"
#include "Feelable.h"
#include "Control.h"

#define SHAPE_CUBE      0
#define SHAPE_SPHERE    1
#define SHAPE_CYLINDER  2
#define SHAPE_COMPOSITE 3
#define SHAPE_MEMO		4
#define SHAPE_TRIANGLE	5
#define SHAPE_TORUS		6

/** Represents a shape in a document or scene */
class CShape : public CDrawable, public CFeelable, public CObject
{
protected:
	/** Name / Identifier used to refer to the shape */
	CString m_name;

	/** The sound notification for user touching shape has been given */
	bool m_touchAlert;

	/** The shape's rotation in dynamics is fixed */
	bool m_rotationFixed;

	/** The shape is affected by collisions with other shapes and by gravity */
	bool m_collisionDynamic;

	/** The shape is affected by the proxy in dynamics */
	bool m_proxyDynamic;

	/** The shape's surface friction used in dynamics */
	float m_surfaceFriction;

	/** The mass of the shape used by dynamics */
	float m_mass;

	/** True if the shape is a member of a composite */
	bool m_isCompositeMember;

	/** The shape's x position */
	float m_locationX;

	/** The shape's y position */
	float m_locationY;

	/** The shape's z position */
	float m_locationZ;

	/** The shape's rotation matrix */
	float m_rotation[16];

	/** The shape's size in x */
	float m_sizeX;

	/** The shape's size in y */
	float m_sizeY; 

	/** The shape's size in z */
	float m_sizeZ;

	/** The shape's control widget */
	CControl* m_control;

	/** The rotated corners of the shape */
	float m_corners[8][3];

public:
	/** Call before the shape is rendered */
	void PreRender();

	/** Returns the OpenGL drawing code required to render the shape graphically */
	virtual CString drawGLCode()= 0;

	/** Sets that the shape's rotation in dynamics is fixed */
	void setFixedRotation(bool fr);

	/** Sets that the shape is affected by collisions with other shapes and by gravity */
	void setCollisionDynamic(bool cd);

	/** Sets the shape is affected by the proxy in dynamics */
	void setProxyDynamic(bool pd);

	/** Sets the shape's surface friction used in dynamics */
	void setSurfaceFriction(float sf);

	/** Sets the mass of the shape used by dynamics */
	void setMass(float m);

	/** True iff the shape's rotation in dynamics is fixed */
	bool isFixedRotation();

	/** True iff the shape is affected by collisions with other shapes and by gravity */
	bool isCollisionDynamic();

	/** Sets the shape is affected by the proxy in dynamics */
	bool isProxyDynamic();

	/** Returns the shape's surface friction used in dynamics */
	float getSurfaceFriction();

	/** Returns the mass of the shape used by dynamics */
	float getMass();

	/** (Call getCorners() first) Get the shapes rotated corners, indexed from 0 to 7 */
	float* getCorner(int i);

	/** Calculate the rotated corners */
	float* getCorners();

	/** Sets that the shape is a member of a composite */
	void setCompositeMember(bool composite);

	/** True iff the shape is a member of a composite */
	bool isCompositeMember();

	/** Create a new shape (abstract) */
	CShape();

	/** Destroy the shape */
	~CShape();

	/** Sets the position of the centre of the shape */
	virtual void setLocation ( float x, float y, float z );

	/** Sets the rotation matrix of the shape */
	virtual void setRotation ( float R[16] );

	/** Sets the size of the shape */
	virtual void setSize ( float x, float y, float z );

	/** Override this to return a copy of the shape */
	virtual CShape* clone()= 0;

	/** Get the position of the shape wrt the x axis */
	float getLocationX ();
	
	/** Get the position of the shape wrt the y axis */
	float getLocationY ();

	/** Get the position of the shape wrt the z axis */
	float getLocationZ ();

	/** Returns a pointer to the shapes rotation matrix */
	float *getRotation();
	
	/** Get the size of the shape wrt its own x axis */
	float getSizeX ();

	/** Get the size of the shape wrt its own y axis */
	float getSizeY ();
	
	/** Get the size of the shape wrt its own z axis */
	float getSizeZ ();

	/** Get name */
	CString GetName ( void );

	/** Set name */
	void SetName ( CString name );


	/** Return the shape's control widget */
	CControl* getControl();

	/** Override this to return an integer representing the type of this shape.
	    See top of this file (Shape.h), for list of values */
	virtual int getType()= 0;

	/** Override this to save / load shape parameters to the archive ar */
	virtual void Serialize(CArchive& ar);
};

#endif
