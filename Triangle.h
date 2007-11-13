#pragma once
#include "shape.h"

// Represents a triangle polygon shape
class CTriangle : public CShape
{
public:

	// Create a new triangle
	CTriangle(void);

	// Destroy the triangle
	virtual ~CTriangle(void);

	// Overrides CDrawable in order to render the cube graphically
	void drawGLScene();
	
	// Overrides CShape in order to return OpenGL code for rendering the cube
	virtual CString drawGLCode();
	
	// Overrides CFeelable to render the cube haptically
	void drawHapticScene(bool primary_context);
	
	// Store/Load the cube's data to the archive ar
	virtual void Serialize(CArchive& ar);
	
	// Return a pointer to a copy of this cube
	CShape* clone();
	
	// Return an integer representing the type of shape (i.e. cube)
    //   See shape.h for possible values
	int getType();

	// Get the local coordinates of vertex A
	float* GetVertexA ();

	// Get the local coordinates of vertex B
	float* GetVertexB ();

	// Get the local coordinates of vertex C
	float* GetVertexC ();

	// Set the local coordinates of vertex A
	void SetVertexA ( float x, float y, float z );

	// Set the local coordinates of vertex B
	void SetVertexB ( float x, float y, float z );

	// Set the local coordinates of vertex C
	void SetVertexC ( float x, float y, float z );

	// Move vertices (local) and triangle position (model) so that the centroid of
	// the triangle is at origin of local coords system
	void ReCentreVertices ( );

	// Calculate the centriod of the triangle in local coords
	void GetCentre ( float* centre );

	/** Sets the size of the shape */
	void setSize ( float x, float y, float z );

protected:

	// Position of vertices in local coordinates
	float a[3];
	float b[3];
	float c[3];

};
