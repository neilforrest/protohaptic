#include "stdafx.h"
#include "Torus.h"
#include "TorusControl.h"
#include "ProtoHaptic.h"
#include <GL/GLUT.h>

CTorus::CTorus(void)
{
	// Set Default parameters
	m_radiusMajor= 1.0;
	m_radiusMinor= 0.25;

	m_drawnFraction= 1.0; //i.e. Complete

	m_sizeX= 1.0; m_sizeY= 1.0; m_sizeZ= 1.0;
	setLocation(0.0,0.0,0.0);
	m_control= new CTorusControl(this);
}

CTorus::~CTorus(void)
{
}

/** Get radius of outer loop spine */
float CTorus::getRadiusMajor ( )
{
	return m_radiusMajor;
}

/** Set radius of outer loop spine  */
void CTorus::setRadiusMajor ( float r )
{
	m_radiusMajor= r;
}

/** Get radius of outer loop */
float CTorus::getRadiusMinor ( )
{
	return m_radiusMinor;
}

/** Set radius of outer loop */
void CTorus::setRadiusMinor ( float r )
{
	m_radiusMinor= r;
}

CShape* CTorus::clone()
{
	CTorus* cloned;

	cloned= new CTorus();
	cloned->setLocation(m_locationX,m_locationY,m_locationZ);
	cloned->setRotation(m_rotation);
	cloned->setSize(m_sizeX,m_sizeY,m_sizeZ);
	cloned->setColour(m_colourRed,m_colourGreen,m_colourBlue);
	cloned->setDampening(m_dampening);
	cloned->setDynamicFriction(m_dynamic_friction);
	cloned->setStaticFriction(m_static_friction);
	cloned->setStiffness(m_stiffness);

	cloned->m_surfaceFriction= m_surfaceFriction;
	cloned->m_mass= m_mass;
	cloned->m_proxyDynamic= m_proxyDynamic;
	cloned->m_collisionDynamic= m_collisionDynamic;

	cloned->m_logical= m_logical;
	cloned->m_stiffnessString= m_stiffnessString;
	cloned->m_dampeningString= m_dampeningString;
	cloned->m_staticFString= m_staticFString;
	cloned->m_dynamicFString= m_dynamicFString;

	cloned->m_radiusMajor= m_radiusMajor;
	cloned->m_radiusMinor= m_radiusMinor;

	cloned->m_name= m_name;

	return (CShape*)cloned;
}

/** Get radius of outer loop */
float CTorus::getRadius ( )
{
	return 0.0;
}

/** Set radius of outer loop */
void CTorus::setRadius ( float r )
{
	//m_radius= r;
}

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void
doughnut(GLfloat r, GLfloat R, GLint nsides, GLint rings, float complete)
{
  int i, j;
  GLfloat theta, phi, theta1;
  GLfloat cosTheta, sinTheta;
  GLfloat cosTheta1, sinTheta1;
  GLfloat ringDelta, sideDelta;

  ringDelta = 2.0 * M_PI / rings;
  sideDelta = 2.0 * M_PI / nsides;

  theta = 0.0;
  cosTheta = 1.0;
  sinTheta = 0.0;

	int drawnRings= rings * complete;

  for (i = drawnRings - 1; i >= 0; i--) {
    theta1 = theta + ringDelta;
    cosTheta1 = cos(theta1);
    sinTheta1 = sin(theta1);
    glBegin(GL_QUAD_STRIP);
    phi = 0.0;
    for (j = nsides; j >= 0; j--) {
      GLfloat cosPhi, sinPhi, dist;

      phi += sideDelta;
      cosPhi = cos(phi);
      sinPhi = sin(phi);
      dist = R + r * cosPhi;

      glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
      glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
      glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
      glVertex3f(cosTheta * dist, -sinTheta * dist,  r * sinPhi);
    }
    glEnd();
    theta = theta1;
    cosTheta = cosTheta1;
    sinTheta = sinTheta1;
  }
}

void CTorus::drawGLScene()
{
	PreRender();
	glPushMatrix();

	glTranslatef(m_locationX,
				 m_locationY,
				 m_locationZ);

	glMultMatrixf(m_rotation);
	
	glScalef(m_sizeX, m_sizeY, m_sizeZ );

	glColor3f(m_colourRed,
			  m_colourGreen,
			  m_colourBlue);

	int stacks= ((CProtoHapticApp*)AfxGetApp())->getStacks();

	doughnut ( m_radiusMinor, m_radiusMajor, stacks, stacks, m_drawnFraction );

	glPopMatrix();
}

void CTorus::drawHapticScene(bool primary_context)
{
	hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, m_stiffness);
	hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, m_dampening);
	hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, m_static_friction);
	hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, m_dynamic_friction);
	if(primary_context)
		hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID);
	else
		hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, m_shapeID_1);
	drawGLScene();
	hlEndShape();
}

int CTorus::getType() { return SHAPE_TORUS; }

CString CTorus::drawGLCode()
{
	CString s= "// !!Torus Code Not Implimented!! \r\n";

	return s;
}

void CTorus::Serialize(CArchive& ar)
{
	CShape::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << m_radiusMajor;
		ar << m_radiusMinor;
	}
	else
	{
		ar >> m_radiusMajor;
		ar >> m_radiusMinor;
	}
}

/** Get drawn fraction of outer loop */
float CTorus::getFraction ( )
{
	return m_drawnFraction;
}

/** Set drawn fraction of outer loop */
void CTorus::setFraction ( float f )
{	
	m_drawnFraction= f;
}