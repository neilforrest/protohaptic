#include "stdafx.h"
#include "TriangleControl.h"

#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>

// Create a new triangle control for the given triangle
CTriangleControl::CTriangleControl( CTriangle* triangle )
{
	// Control points for vertices
	editPointA= new CEditPoint ();	// Vertex A
	editPointB= new CEditPoint ();	// Vertex B
	editPointC= new CEditPoint ();	// Vertex C

	// Control points for rotation
	rotate0= new CEditPoint ();
	rotate1= new CEditPoint ();
	rotate2= new CEditPoint ();
	rotate3= new CEditPoint ();
	rotate4= new CEditPoint ();
	rotate5= new CEditPoint ();

	// Control points for resizing
	// (only four because a triangle is always in the plane)
	resize0= new CEditPoint ();
	resize1= new CEditPoint ();
	resize2= new CEditPoint ();
	resize3= new CEditPoint ();

	// Save ptr to triangle under ctrl
	controlTriangle= triangle;

	// Position edit points
	UpdateEditPoints ( );
}

// Destroy the triangle control
CTriangleControl::~CTriangleControl(void)
{
	delete editPointA;
	delete editPointB;
	delete editPointC;
}

// Updates the position of the edit points to match the triangle under control
void CTriangleControl::UpdateEditPoints ( )
{
	// Set position of vertex edit points

	// Vertex A
	editPointA->setLocation ( controlTriangle->GetVertexA()[0],
							  controlTriangle->GetVertexA()[1],
							  controlTriangle->GetVertexA()[2] );

	// Vertex B
	editPointB->setLocation ( controlTriangle->GetVertexB()[0],
							  controlTriangle->GetVertexB()[1],
							  controlTriangle->GetVertexB()[2] );

	// Vertex C
	editPointC->setLocation ( controlTriangle->GetVertexC()[0],
							  controlTriangle->GetVertexC()[1],
							  controlTriangle->GetVertexC()[2] );

	float half_x= (float)(controlTriangle->getSizeX()/2.0);
	float half_y= (float)(controlTriangle->getSizeY()/2.0);
	float half_z= (float)(controlTriangle->getSizeZ()/2.0);

	// Set the position of the rotate edit points
	rotate0->setLocation(0, -((controlTriangle->getSizeY())/2+1), 0);
	rotate1->setLocation(0, 0, -((controlTriangle->getSizeZ())/2+1) );
	rotate2->setLocation(-((controlTriangle->getSizeX())/2+1), 0, 0);
	rotate3->setLocation(0, ((controlTriangle->getSizeY())/2+1), 0);
	rotate4->setLocation(0, 0, ((controlTriangle->getSizeZ())/2+1) );
	rotate5->setLocation(((controlTriangle->getSizeX())/2+1), 0, 0);

	// Padding between triangle and bounding box
	float padding= 0.5;

	// Set the position of the resize edit poiints
	resize0->setLocation(-half_x-padding,-half_y-padding,0);
	resize1->setLocation(half_x+padding,-half_y-padding,0);
	resize2->setLocation(half_x+padding,half_y+padding,0);
	resize3->setLocation(-half_x-padding,half_y+padding,0);
}

// Sets the control's colour scheme. Used to 
// distinguish primary and secondary devices controls
void CTriangleControl::setColourScheme(int cs)
{
	m_colourScheme= cs;
}

// Returns an integer constant representing the current
//  control mode. See control.h for possible values.
int CTriangleControl::getControlMode()
{
	// If touching edit point A
	if(editPointA->touching() || editPointA->touching1())
		return CONTROL_MODE_TRI_VERTEX_A;	// Do move vertex A operation

	// If touching edit point B
	if(editPointB->touching() || editPointB->touching1())
		return CONTROL_MODE_TRI_VERTEX_B;	// Do move vertex B operation

	// If touching edit point C
	if(editPointC->touching() || editPointC->touching1())
		return CONTROL_MODE_TRI_VERTEX_C;	// Do move vertex C operation

	// If touching the triangle
	if(controlTriangle->touching() || controlTriangle->touching1())
		return CONTROL_MODE_MOVE;	// Do move operation

	// Rotate controls
	if(rotate0->touching() || rotate0->touching1())
	   return CONTROL_MODE_ROTATE0;
	if(rotate1->touching() || rotate1->touching1())
	   return CONTROL_MODE_ROTATE1;
	if(rotate2->touching() || rotate2->touching1())
	   return CONTROL_MODE_ROTATE2;
	if(rotate3->touching() || rotate3->touching1())
	   return CONTROL_MODE_ROTATE3;
	if(rotate4->touching() || rotate4->touching1())
	   return CONTROL_MODE_ROTATE4;
	if(rotate5->touching() || rotate5->touching1())
	   return CONTROL_MODE_ROTATE5;

	// Resize Controls
	if(resize0->touching())
	   return CONTROL_MODE_RESIZE0;
	if(resize1->touching())
	   return CONTROL_MODE_RESIZE1;
	if(resize2->touching())
	   return CONTROL_MODE_RESIZE2;
	if(resize3->touching())
	   return CONTROL_MODE_RESIZE3;

	// No operation
	return CONTROL_MODE_NULL;
}

// Draw a line between a and b
void CTriangleControl::ConnectEditPoints(CEditPoint* a, CEditPoint* b)
{
	glBegin(GL_LINES);
	glVertex3d(a->getLocationX(),
			   a->getLocationY(),
			   a->getLocationZ());
	glVertex3d(b->getLocationX(),
			   b->getLocationY(),
			   b->getLocationZ());
	glEnd();
}

// Overrides CDrawable in order to render the control graphically 
void CTriangleControl::drawGLScene()
{
	glPushMatrix();

	// Apply triangles transform (without scale)
	glTranslatef(controlTriangle->getLocationX(),controlTriangle->getLocationY(),controlTriangle->getLocationZ());
	glMultMatrixf(controlTriangle->getRotation());

	// Position edit points
	UpdateEditPoints();

	// Set color for vetex edit endtrols
	glColor3d ( 0.5, 0.5, 0.5 );

	// Draw edit points
	editPointA->drawGLScene ();
	editPointB->drawGLScene ();
	editPointC->drawGLScene ();

	// Set color for rotate controls
	if(m_colourScheme==CONTROL_COLOURS_PRIMARY)
	{
		glColor3d(0.5,0.5,1.0);
	}
	else if(m_colourScheme==CONTROL_COLOURS_SECONDARY)
	{
		glColor3d(1.0,1.0,0.5);
	}
	else
	{
		glColor3d(0.7,0.2,0.0);
	}

	// Draw rotate points
	rotate0->drawGLScene ();
	rotate1->drawGLScene ();
	rotate2->drawGLScene ();
	rotate3->drawGLScene ();
	rotate4->drawGLScene ();
	rotate5->drawGLScene ();

	// Set color for resize controls
	if(m_colourScheme==CONTROL_COLOURS_PRIMARY)
	{
		glColor3d(1.0,0.5,0.5);
	}
	else if(m_colourScheme==CONTROL_COLOURS_SECONDARY)
	{
		glColor3d(0.5,1.0,0.5);
	}
	else
	{
		glColor3d(0.5,1.0,1.0);
	}

	// Draw resize points
	resize0->drawGLScene ();
	resize1->drawGLScene ();
	resize2->drawGLScene ();
	resize3->drawGLScene ();

	// Draw lines joining up edit points
	glDisable(GL_LIGHTING);
	glColor3d ( 1.0, 1.0, 1.0 );

	// Join up rotate points with centre
	ConnectEditPoints(rotate0,rotate3);
	ConnectEditPoints(rotate1,rotate4);
	ConnectEditPoints(rotate2,rotate5);

	// Join up resize points
	ConnectEditPoints(resize0,resize1);
	ConnectEditPoints(resize1,resize2);
	ConnectEditPoints(resize2,resize3);
	ConnectEditPoints(resize3,resize0);

	// Enable lighting effects again
	glEnable(GL_LIGHTING);

	glPopMatrix ();
}

// Overrides CFeelabe in order to render the control haptically 
void CTriangleControl::drawHapticScene(bool primary_context)
{
	glPushMatrix();

	// Apply triangles transform (without scale)
	glTranslatef(controlTriangle->getLocationX(),controlTriangle->getLocationY(),controlTriangle->getLocationZ());
	glMultMatrixf(controlTriangle->getRotation());

	// Position edit points
	UpdateEditPoints();

	// Draw edit points
	editPointA->drawHapticScene ( primary_context );
	editPointB->drawHapticScene ( primary_context );
	editPointC->drawHapticScene ( primary_context );

	// Draw rotate points
	rotate0->drawHapticScene ( primary_context );
	rotate1->drawHapticScene ( primary_context );
	rotate2->drawHapticScene ( primary_context );
	rotate3->drawHapticScene ( primary_context );
	rotate4->drawHapticScene ( primary_context );
	rotate5->drawHapticScene ( primary_context );

	// Draw resize points
	resize0->drawHapticScene ( primary_context );
	resize1->drawHapticScene ( primary_context );
	resize2->drawHapticScene ( primary_context );
	resize3->drawHapticScene ( primary_context );

	glPopMatrix ();
}