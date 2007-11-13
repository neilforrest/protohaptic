#include "stdafx.h"
#include "TriVertexMove.h"
#include "Geometry.h"

// Create a new move transformation on the given shape.
// x, y and z represent the current proxy position and doc is
// the current document (used by snapto)
CTriVertexMove::CTriVertexMove(CTriangle *triangle, 
	           HLdouble x, HLdouble y, HLdouble z, 
			   CProtoHapticDoc *doc,
			   int vertex)	// the vertex to move 1, 2 or 3
{
	// The triangle being transformed
	activeTriangle= triangle;

	// The vertex to act on
	activeVertex= vertex;

	// The document
	activeDoc= doc;

	// By default snapto is not enabled
	snapTo= false;
}

// Destroy this vertex move
CTriVertexMove::~CTriVertexMove()
{

}

// Set snap to other shapes boundaries
void CTriVertexMove::setSnapto(bool snap)
{
	snapTo= snap;
}

// Lock the proxy position to nearby objects
void CTriVertexMove::SnapPointTo ( HLdouble* x, HLdouble* y, HLdouble* z )
{
	// For each shape in document
	for ( int i= 0; i < activeDoc->GetShapeCount (); i++ )
	{
		// Dont snap to ourselves
		if ( activeDoc->GetShape ( i ) != activeTriangle )
		{
			// Snap point to that shape
			SnapPointToShape ( x, y, z, activeDoc->GetShape ( i ) );
		}
	}
}

// Lock the proxy position to nearby objects on specified shape
void CTriVertexMove::SnapPointToShape ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape )
{
	// Lock the proxy position to nearby objects on specified shape's bounding box
	SnapPointToBoundingBox ( x, y, z, shape );

	// If the shape is a triangle
	if ( shape->getType () == SHAPE_TRIANGLE )
	{
		// Lock the proxy position to nearby specified triangles verticies
		SnapPointToTriVertex ( x, y, z, (CTriangle*)shape );
	}
}

// Lock the point to point if distance is small
void CTriVertexMove::SnapPointToPoint ( HLdouble* x, HLdouble* y, HLdouble* z, 
										HLdouble x1, HLdouble y1, HLdouble z1 )
{
	HLdouble dist[3]= 
	{
		*x - x1, *y - y1, *z - z1
	};

	// Snap to if less that threshold distance
	if ( sqrt ( pow ( dist[0], 2 ) + pow ( dist[1], 2 ) + pow ( dist[2], 2 ) ) < snapDistance )
	{
		*x= x1;
		*y= y1;
		*z= z1;
	}
}

// Lock the proxy position to nearby objects on specified shape's bounding box corners
void CTriVertexMove::SnapPointToBoundingBoxVertices ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape )
{
	// Calculate the box corners in model coords
	shape->getCorners ();

	// For each corner
	for ( int i= 0; i < 8; i++ )
	{
		// Snap to point to corner
		SnapPointToPoint ( x, y, z, 
			shape->getCorner ( i )[0], shape->getCorner ( i )[1], shape->getCorner ( i )[2] );
	}
}

// Lock the proxy position to nearby objects on specified shape's bounding box surface
void CTriVertexMove::SnapPointToBoundingBoxSurface ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape, 
										 hduVector3Dd dispToSurface, // Vector from centre to surface
										 int vertexA, int vertexB, int vertexC ) // Vertices defining surface
{
	// Proxy pos
	float originalPoint [3]= { *x, *y, *z };

	// Projected proxy pos
	float projectedPoint [3];

	// normal defining current plane
	float n[3];

	// Rotation of current shape
	hduMatrix rotation;

	// Displacement from shape centre to surface of plane
	hduVector3Dd vect;		// Local
	hduVector3Dd vect_out;	// Rotated

	// Point on plane
	float pointOnPlane[3];

	// Calculate the box corners in model coords
	shape->getCorners ();

	// Get normal to plane
	getSurfaceNormal ( n, shape->getCorner ( vertexA ), shape->getCorner ( vertexB ), shape->getCorner ( vertexC ) );

	// Get a point on the plane

	// Get rotation of shape
	rotation= getHduMatrix ( shape->getRotation () );

	// Get rotated displacement from shape centre to surface of plane
	vect= dispToSurface;		// Local surface

	rotation.multVecMatrix ( vect, vect_out );			// Rotate surface 

	// Add to this the shapes translation
	vect_out+= hduVector3Dd ( shape->getLocationX (), shape->getLocationY (), shape->getLocationZ () );
														// Global surface

	// Create point on plane
	pointOnPlane[0]= vect_out[0];
	pointOnPlane[1]= vect_out[1];
	pointOnPlane[2]= vect_out[2];

	// Project onto plane
	getProjectionOnPlane ( n, pointOnPlane, originalPoint, projectedPoint );

	// Check the boundaries of the surface

	// Translate projected point to origin
	hduMatrix toOrigin ( hduMatrix::createTranslation ( -shape->getLocationX (), -shape->getLocationY (), -shape->getLocationZ () ) );

	// Un-rotate the projected point
	hduMatrix invRotation ( rotation );
	invRotation.invert ();
	
	toOrigin.multRight ( invRotation );

	hduVector3Dd localProjectedPointV;
	toOrigin.multVecMatrix ( hduVector3Dd ( projectedPoint[0], projectedPoint[1], projectedPoint[2] ),
		                     localProjectedPointV );

	if ( localProjectedPointV[0] <= shape->getSizeX()/2.0f + snapDistance && 
		 localProjectedPointV[0] >= -shape->getSizeX()/2.0f - snapDistance&&

		 localProjectedPointV[1] <= shape->getSizeY()/2.0f + snapDistance && 
		 localProjectedPointV[1] >= -shape->getSizeY()/2.0f - snapDistance &&

		 localProjectedPointV[2] <= shape->getSizeZ()/2.0f + snapDistance && 
		 localProjectedPointV[2] >= -shape->getSizeZ()/2.0f - snapDistance)
	{
		// Snap to projected point
		SnapPointToPoint ( x, y, z, projectedPoint[0], projectedPoint[1], projectedPoint[2] );
	}
}

// Lock the proxy position to nearby objects on specified shape's bounding box surfaces
void CTriVertexMove::SnapPointToBoundingBoxSurfaces ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape )
{
	// For triangle just use one xy plane
	if ( shape->getType () == SHAPE_TRIANGLE )
	{
		// Front face
		SnapPointToBoundingBoxSurface ( x, y, z, shape, hduVector3Dd ( 0, 0, 0 ), 0, 1, 2 );
	}
	else
	{
		// Front face
		SnapPointToBoundingBoxSurface ( x, y, z, shape, hduVector3Dd ( 0, 0, shape->getSizeZ()/2.0f ), 0, 1, 2 );

		// Back face
		SnapPointToBoundingBoxSurface ( x, y, z, shape, hduVector3Dd ( 0, 0, -shape->getSizeZ()/2.0f ), 4, 5, 6 );
	}

	// Top face
	SnapPointToBoundingBoxSurface ( x, y, z, shape, hduVector3Dd ( 0, shape->getSizeY()/2.0f, 0 ), 0, 4, 5 );

	// Bottom face
	SnapPointToBoundingBoxSurface ( x, y, z, shape, hduVector3Dd ( 0, -shape->getSizeY()/2.0f, 0 ), 0, 4, 5 );

	// Right face
	SnapPointToBoundingBoxSurface ( x, y, z, shape, hduVector3Dd ( shape->getSizeX()/2.0f, 0, 0 ), 1, 5, 6 );

	// Left face
	SnapPointToBoundingBoxSurface ( x, y, z, shape, hduVector3Dd ( -shape->getSizeX()/2.0f, 0, 0 ), 1, 5, 6 );
}

// Lock the proxy position to nearby objects on specified shape's bounding box
void CTriVertexMove::SnapPointToBoundingBox ( HLdouble* x, HLdouble* y, HLdouble* z, CShape* shape )
{
	// Snap to corners of rotated bounding box
	//SnapPointToBoundingBoxVertices ( x, y, z, shape );

	// Snap to surfaces of box
	SnapPointToBoundingBoxSurfaces ( x, y, z, shape );

/*
	// normal defining a plane
	float n[3];

	// Get front facing plane
	getSurfaceNormal ( n, shape->getCorner ( 0 ), shape->getCorner ( 1 ), shape->getCorner ( 2 ) );

	hduMatrix proj= getProjectionMatrix ( n );

	// Translate system to origin
	hduMatrix toOrigin;
	toOrigin.multRight ( hduMatrix::createTranslation ( -shape->getLocationX(),
														-shape->getLocationY(),
														-shape->getLocationZ() ) );
	hduVector3Dd vect ( 0, 0, -shape->getSizeZ()/2.0 );
	hduVector3Dd vect_out;

	// Define the items rotation as an hduMatrix
	hduMatrix rotation 
			( shape->getRotation()[0],  shape->getRotation()[1],  shape->getRotation()[2],  shape->getRotation()[3], 
			  shape->getRotation()[4],  shape->getRotation()[5],  shape->getRotation()[6],  shape->getRotation()[7], 
			  shape->getRotation()[8],  shape->getRotation()[9],  shape->getRotation()[10], shape->getRotation()[11], 
			  shape->getRotation()[12], shape->getRotation()[13], shape->getRotation()[14], shape->getRotation()[15] );

	rotation.multVecMatrix ( vect, vect_out );

	toOrigin.multRight ( hduMatrix::createTranslation ( vect_out ) );

	hduVector3Dd proxyPos ( *x, *y, *z );
	hduVector3Dd proxyOriginPos;

	toOrigin.multVecMatrix ( proxyPos, proxyOriginPos );

	//do projection
	hduVector3Dd in ( proxyOriginPos[0], proxyOriginPos[1], proxyOriginPos[2] );
	hduVector3Dd out;

	proj.multVecMatrix ( in, out );

	// Move back from origin
	hduMatrix invToOrigin ( toOrigin );
	invToOrigin.invert ();

	hduVector3Dd final;
	invToOrigin.multVecMatrix ( out, final );

	SnapPointToPoint ( x, y, z, final[0], final[1], final[2] );

	char s[256];
	sprintf ( s, "%f, %f, %f ----- %f, %f, %f\n", *x, *y, *z, out[0], out[1], out[2] );
	OutputDebugString ( s );

/*	float zy[3]= { p[0]*(n[1]*n[1]+ n[2]*n[2]) - p[1]*n[0]*n[1] -             p[2]*n[0]*n[2],
			      -p[0]*n[1]*n[0] +              p[1]*(n[0]*n[0]+n[2]*n[2]) - p[2]*n[1]*n[2],
				  -p[0]*n[0]*n[2] -              p[1]*n[1]*n[2] +             p[2]*(n[0]*n[0]+n[1]*n[1]) };*/
}

// Lock the proxy position to nearby specified triangles verticies
void CTriVertexMove::SnapPointToTriVertex ( HLdouble* x, HLdouble* y, HLdouble* z, CTriangle* tri )
{
	// Get triangle vertices in model coords
	hduVector3Dd a_in ( tri->GetVertexA ()[0], tri->GetVertexA ()[1], tri->GetVertexA ()[2] );
	hduVector3Dd b_in ( tri->GetVertexB ()[0], tri->GetVertexB ()[1], tri->GetVertexB ()[2] );
	hduVector3Dd c_in ( tri->GetVertexC ()[0], tri->GetVertexC ()[1], tri->GetVertexC ()[2] );

	hduVector3Dd a_out;
	hduVector3Dd b_out;
	hduVector3Dd c_out;

	// Get the shapes current transform
	hduMatrix transform;

	// Define the items rotation as an hduMatrix
	hduMatrix rotation 
			( tri->getRotation()[0],  tri->getRotation()[1],  tri->getRotation()[2],  tri->getRotation()[3], 
			  tri->getRotation()[4],  tri->getRotation()[5],  tri->getRotation()[6],  tri->getRotation()[7], 
			  tri->getRotation()[8],  tri->getRotation()[9],  tri->getRotation()[10], tri->getRotation()[11], 
			  tri->getRotation()[12], tri->getRotation()[13], tri->getRotation()[14], tri->getRotation()[15] );

	// Define the items translation as an hduMatrix
	hduMatrix translation= hduMatrix::createTranslation ( tri->getLocationX (),
														  tri->getLocationY (),
														  tri->getLocationZ () );

	// Create combined transform
	transform.multRight ( rotation );
	transform.multRight ( translation );

	// Now transform the points
	transform.multVecMatrix ( a_in, a_out );
	transform.multVecMatrix ( b_in, b_out );
	transform.multVecMatrix ( c_in, c_out );

	// Snap to point to vertices
	SnapPointToPoint ( x, y, z, a_out[0], a_out[1], a_out[2] );
	SnapPointToPoint ( x, y, z, b_out[0], b_out[1], b_out[2] );
	SnapPointToPoint ( x, y, z, c_out[0], c_out[1], c_out[2] );
}

// Update the move by setting a new proxy position, the move will
// update the shapes position accordingly
void CTriVertexMove::setProxyPos(HLdouble x, HLdouble y, HLdouble z)
{
	// If snap to option is enabled
	if ( snapTo )
	{
		// Snap the proxy [psotion to nearby objects
		SnapPointTo ( &x, &y, &z );
	}

	// Get the shapes current transform
	hduMatrix transform;

	// Define the items scaling as an hduMatrix
	hduMatrix scaling= hduMatrix::createScale ( activeTriangle->getSizeX (),
												activeTriangle->getSizeY (),
												activeTriangle->getSizeZ () );

	// Define the items rotation as an hduMatrix
	hduMatrix rotation 
			( activeTriangle->getRotation()[0],  activeTriangle->getRotation()[1],  activeTriangle->getRotation()[2],  activeTriangle->getRotation()[3], 
			  activeTriangle->getRotation()[4],  activeTriangle->getRotation()[5],  activeTriangle->getRotation()[6],  activeTriangle->getRotation()[7], 
			  activeTriangle->getRotation()[8],  activeTriangle->getRotation()[9],  activeTriangle->getRotation()[10], activeTriangle->getRotation()[11], 
			  activeTriangle->getRotation()[12], activeTriangle->getRotation()[13], activeTriangle->getRotation()[14], activeTriangle->getRotation()[15] );

	// Define the items translation as an hduMatrix
	hduMatrix translation= hduMatrix::createTranslation ( activeTriangle->getLocationX (),
														  activeTriangle->getLocationY (),
														  activeTriangle->getLocationZ () );

	// Create combined transform
	//transform.multRight ( scaling );
	transform.multRight ( rotation );
	transform.multRight ( translation );

	// Invert the shapes transform
	transform.invert ();

	// Apply the inverse of shape's transform to the proxy position
	hduVector3Dd proxy ( x, y, z );
	hduVector3Dd proxyLocal;
	transform.multVecMatrix ( proxy, proxyLocal );

	// Update the position of the triangle's vertex to match local proxy
	if ( activeVertex == 1 )	// Vertex A == 1
	{
		activeTriangle->SetVertexA ( proxyLocal[0], proxyLocal[1], proxyLocal[2] );
	}
	if ( activeVertex == 2 )	// Vertex B == 2
	{
		activeTriangle->SetVertexB ( proxyLocal[0], proxyLocal[1], proxyLocal[2] );
	}
	if ( activeVertex == 3 )	// Vertex C == 3
	{
		activeTriangle->SetVertexC ( proxyLocal[0], proxyLocal[1], proxyLocal[2] );
	}

	// Now that the vertices have moved, re-calculate the centre of the triangle
	activeTriangle->ReCentreVertices ();
}

// Return an integer representing the type of transform (i.e. move)
// See Transform.h for return values
int CTriVertexMove::getType()
{
	return TRANSFORM_TRI_VERTEX_MOVE;
}

// Returns the shape on which this vertex move is acting
CShape* CTriVertexMove::getShape()
{
	return NULL;
}