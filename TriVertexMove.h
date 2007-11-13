#pragma once
#include "transform.h"
#include "ProtoHapticDoc.h"
#include "Triangle.h"

#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>

// Distance threshold for snap to
static const float snapDistance= 0.5;

// Represent the moving of a triangle's vertex
class CTriVertexMove : public CTransform
{
public:

	// Create a new move transformation on the given shape.
	// x, y and z represent the current proxy position and doc is
	// the current document (used by snapto)
	CTriVertexMove(CTriangle *triangle, 
		           HLdouble x, HLdouble y, HLdouble z, 
				   CProtoHapticDoc *doc,
				   int vertex);	// the vertex to move 1, 2 or 3

	// Set snap to other shapes boundaries
	void setSnapto(bool snap);

	// Update the move by setting a new proxy position, the move will
	// update the shapes position accordingly
	void setProxyPos(HLdouble x, HLdouble y, HLdouble z);
	
	// Return an integer representing the type of transform (i.e. move)
	// See Transform.h for return values
	int getType();
	
	// Destroy this vertex move
	virtual ~CTriVertexMove();

	// Returns the shape on which this vertex move is acting
	CShape *getShape();

protected:

	// Lock the proxy position to nearby objects
	void SnapPointTo ( HLdouble* x, HLdouble* y, HLdouble* z );

	// Lock the proxy position to nearby objects on specified shape
	void SnapPointToShape ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape );

	// Lock the proxy position to nearby objects on specified shape's bounding box
	void SnapPointToBoundingBox ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape );

	// Lock the proxy position to nearby objects on specified shape's bounding box corners
	void SnapPointToBoundingBoxVertices ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape );

	// Lock the proxy position to nearby objects on specified shape's bounding box surfaces
	void SnapPointToBoundingBoxSurfaces ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape );

	// Lock the proxy position to nearby objects on specified shape's bounding box surface
	void SnapPointToBoundingBoxSurface ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape, 
										 hduVector3Dd dispToSurface, // Vector from centre to surface
										 int vertexA, int vertexB, int vertexC ); // Vertices defining surface

	// Lock the proxy position to nearby specified triangles verticies
	void SnapPointToTriVertex ( HLdouble* x, HLdouble* y, HLdouble* z, CTriangle* tri );

	// Lock the point to point if distance is small
	void SnapPointToPoint ( HLdouble* x, HLdouble* y, HLdouble* z, 
							HLdouble x1, HLdouble y1, HLdouble z1 );

	// Is snap to enabled
	bool snapTo;

	// The vertex to act on
	int activeVertex;

	// The document
	CProtoHapticDoc* activeDoc;

	// The triangle being transformed
	CTriangle* activeTriangle;
};
