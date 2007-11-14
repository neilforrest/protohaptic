// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: ProtoHapticDoc.cpp
// Classes: CProtoHapticDoc
// Purpose: Represents a document or model

#include "stdafx.h"
#include "ProtoHaptic.h"

#include "ProtoHapticDoc.h"
#include "ProtoHapticView.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Composite.h"
#include "Memo.h"
#include "Feelable.h"
#include "Triangle.h"
#include "Torus.h"

#include <hdu/hduVector.h>
#include <hdu/hduMatrix.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticDoc

IMPLEMENT_DYNCREATE(CProtoHapticDoc, CDocument)

BEGIN_MESSAGE_MAP(CProtoHapticDoc, CDocument)
	//{{AFX_MSG_MAP(CProtoHapticDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticDoc construction/destruction

CProtoHapticDoc::CProtoHapticDoc()
{
	// TODO: add one-time construction code here
	m_historyCount= 0;
	m_shapeCount= 0;
	m_memoCount= 0;
	m_gravity= 0;
	m_airResistance= 0.5;
	m_simSpeed= 0.3;
	m_hideMemos= false;

	m_worldID= dWorldCreate();
	m_spaceID= dHashSpaceCreate( 0 );
	m_jointGroup= dJointGroupCreate (1000);

	// Is dynamic surface change enabled in view mode (default: yes)
	m_dynamicSurfaceChange= true;
}

void CProtoHapticDoc::clean()
{
	if(m_historyCount>0) 
	{
		OutputDebugString("Destroying Root Document...\n");
	}
	else
		OutputDebugString("->Destroying Historic Document...\n");

	int i;
	for(i= 0; i<m_shapeCount; i++)
	{
		delete m_shapes[i];
	}

	for(i= 0; i<m_memoCount; i++)
	{
		delete m_memos[i];
	}

	for(i= 0; i<m_historyCount; i++)
	{
		delete m_history[i];
	}
}

// Enable/Disable dynamic surface change in view mode
void CProtoHapticDoc::EnableDynamicSurfaceChange ( bool enable )
{
	m_dynamicSurfaceChange= enable;
}

// Is dynamic surface change enabled in view mode
bool CProtoHapticDoc::IsDynamicSurfaceChangeEnabled ( )
{
	return m_dynamicSurfaceChange;
}

CProtoHapticDoc::~CProtoHapticDoc()
{	
	clean();
	//dWorldDestroy(m_worldID);
	//dSpaceDestroy(m_spaceID);
}

// Convert string to int
bool CProtoHapticDoc::StringToInt ( char* str, int* integer )
{
	// Count the number of digits before the d.p.
	int prefixCount= 0;

	// Length of whole string
	int strLength= strlen ( str );

	// Initialise answer to 0
	(*integer)= 0;

	bool reverseSign= false;
	int readFrom= 0;

	// Check for minus sign
	if ( strLength >= 1 && str[0] == '-' )
	{
		reverseSign= true;
		readFrom= 1;
	}

	// For each char in str
	for ( int i= readFrom; i < strLength; i++ )
	{
		// If char is not the d.p.
		if ( str[i] != '.' )
		{
			// Icr count
			prefixCount++;
		}
		else
		{
			// char is d.p. finnished counting
			break;
		}
	}

	// Current power of 10
	int power= prefixCount-1;

	// For each char in str
	for ( int i= readFrom; i < strLength; i++ )
	{
		// If char is valid
		if ( isdigit ( str[i] ) )
		{
			// Int value of char
			int digit= str[i] - '0';

			// Add to the answer
			(*integer)+= (int) ( ((float)digit) * pow ( (double)10, power ) );

			// decriment the power of 10
			power--;
		}
		else // invalid char
		{
			return false;
		}
	}

	// reverse sign
	if ( reverseSign )
	{
		(*integer)= 0 - (*integer);
	}

	// Success
	return true;
}

/// Gets the max value of x in shape name "shapeX" recursivley for all shapes and sub-shapes
/// with s as a root
int CProtoHapticDoc::GetMaxShapeName ( CShape* s )
{
	// If shape is composite
	if ( s->getType ( ) == SHAPE_COMPOSITE )
	{
		// Cast as composite
		CComposite* comp= (CComposite*)s;

		// max value of 'x'
		int max_x= -1;

		// For each member of composite, find the max
		for ( int i= 0; i < comp->getShapeCount (); i++ )
		{
			// Do recursive call
			int cur_x= GetMaxShapeName ( comp->getShape ( i ) );

			// Compare to max
			if ( cur_x > max_x )
			{
				max_x= cur_x;
			}
		}

		// Finally, consider the composites name
		if ( s->GetName ().Left ( 5 ) == "shape" )
		{
			// Extract 'x' from string
			CString intStr= s->GetName ().Mid ( 5, s->GetName().GetLength () - 5 );

			// Convert 'x' into an int
			int x;
			StringToInt ( intStr.GetBuffer(), &x );

			// Comapre x to current max
			if ( x > max_x )
			{
				max_x= x;
			}
		}

		// Return the absolute maximum
		return max_x;
	}
	else
	{
		// If not a composite, then max id is my own id
		if ( s->GetName ().Left ( 5 ) == "shape" )
		{
			// Extract 'x' from string
			CString intStr= s->GetName ().Mid ( 5, s->GetName().GetLength () - 5 );

			// Convert 'x' into an int
			int x;
			StringToInt ( intStr.GetBuffer(), &x );

			// Return 'x' as int
			return x;
		}
	}

	// Didn't find any names of form 'shapeX'
	return -1;
}

/// Returns a free shape name that is not taken by any shape or sub-shape in the document
CString CProtoHapticDoc::GetFreeShapeName ( )
{
	// Find max 'x' in CShape::m_name=="shape'x' for all shapes and sub shapes

	// Max 'x'
	int max_x= -1;

	// Current x
	int cur_x= 0;

	// For all shapes in document
	for ( int i= 0; i < m_shapeCount; i++ )
	{
		// Get max shape name, recursivley if it is a composite
		cur_x= GetMaxShapeName ( m_shapes [ i ] );

		// Compare to current max
		if ( cur_x > max_x )
		{
			max_x= cur_x;
		}
	}
	
	// Give next free shape name as 'shape'max_x+1'
	max_x++;

	// Construct name string
	CString name;

	name.Format ( "shape%d", max_x );

	return name;
}

/// True if the specified shape name is valid (for this root shape and sub-shapes):
/// i.e. Is not taken by any shape or subshape in document
bool CProtoHapticDoc::IsShapeNameFree ( CString name, CShape* s )
{
	// If shape is composite
	if ( s->getType ( ) == SHAPE_COMPOSITE )
	{
		// Cast as composite
		CComposite* comp= (CComposite*)s;

		// For each member of composite check if its free
		for ( int i= 0; i < comp->getShapeCount (); i++ )
		{
			// Do recursive call
			if ( !IsShapeNameFree ( name, comp->getShape ( i ) ) )
			{
				return false;
			}
		}

		// If we reach here the name is free
		return true;
	}
	else
	{
		// If name matches my name then it's not free
		// otherwise it's free (so far)
		if ( name == s->GetName ( ) )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

/// True if the specified shape name is valid:
/// A: Has correct form (c/c++ variable name syntax, i.e. no-spaces)
/// B: Is not taken by any shape or subshape in document
bool CProtoHapticDoc::IsShapeNameFree ( CString name )
{
	// Must start with letter or '_'
	if ( ! ( name.GetLength () >= 1 && ( isalpha ( name.GetBuffer ()[0] ) || name.GetBuffer ()[0] == '_' ) ) )
	{
		return false;
	}

	// All chars must be alpha numeric or '_'
	for ( int i= 0; i < name.GetLength (); i++ )
	{
		if ( ! ( isalnum ( name.GetBuffer ()[i] ) || name.GetBuffer ()[i] == '_' ) )
		{
			return false;
		}
	}
	
	// Check that name isn't already taken
	for ( int i= 0; i < m_shapeCount; i++ )
	{
		if ( ! IsShapeNameFree ( name, m_shapes[i] ) )
		{
			return false;
		}
	}

	return true;
}

BOOL CProtoHapticDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		((CProtoHapticView*)pView)->Pause();
		((CProtoHapticView*)pView)->Deselect();
		((CProtoHapticView*)pView)->Play();
	}

	clean(); //Destroy existing document

	m_shapeCount= 0;
	m_memoCount= 0;
	m_historyCount= 0;
	m_gravity= 0;
	m_airResistance= 0.5;
	m_simSpeed= 0.3;

	OutputDebugString("NewDocument");

	//dWorldDestroy(m_worldID);
	//dSpaceDestroy(m_spaceID);
	m_worldID= dWorldCreate();
	m_spaceID= dHashSpaceCreate( 0 );

	return TRUE;
}

int CProtoHapticDoc::GetShapeCount()
{
	return m_shapeCount;
}

CShape* CProtoHapticDoc::GetShape(int index)
{
	return m_shapes[index];
}

int CProtoHapticDoc::AddShape(CShape* shape)
{
	if(m_shapeCount<MAX_SHAPES) {

		// create body
		bodies[m_shapeCount]= dBodyCreate(m_worldID);

		dMass mass;

		dMassSetBox (&mass, 100,
							1, 1, 1);

		dBodySetMass (bodies[m_shapeCount], &mass);

		dBodySetPosition   (bodies[m_shapeCount], shape->getLocationX(),
												  shape->getLocationY(),
												  shape->getLocationZ());
		//dBodySetRotation   (bodies[m_shapeCount], const dMatrix3 R);
		//dBodySetQuaternion (bodies[m_shapeCount], const dQuaternion q);
		//odySetLinearVel  (bodies[m_shapeCount], 0.1, 0, 0);
		//dBodySetAngularVel (bodies[m_shapeCount], dReal x, dReal y, dReal z);

		// create geometry
		m_geoms[m_shapeCount]= dCreateBox (m_spaceID, 1,
													  1,
													  1);
//		if(m_shapeCount!=0)
			dGeomSetBody (m_geoms[m_shapeCount], bodies[m_shapeCount]);
//		else
//			dGeomSetBody (m_geoms[m_shapeCount], 0);

		dBodySetData(bodies[m_shapeCount], (void *)m_shapeCount);
		dGeomSetData(m_geoms[m_shapeCount], (void *)m_shapeCount);

		m_shapes[m_shapeCount]= shape;
		m_shapeCount++;
		return m_shapeCount-1;
	} else
		return m_shapeCount;
}

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticDoc serialization

void CProtoHapticDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << m_shapeCount;
		int i;
		for (i= 0; i<m_shapeCount; i++) {
			ar << m_shapes[i]->getType();
			m_shapes[i]->Serialize(ar);
		}

		ar << m_memoCount;
		for (i= 0; i<m_memoCount; i++) {
			m_memos[i]->Serialize(ar);
		}
		ar << m_simSpeed
		   << m_airResistance
		   << m_gravity;
		if(m_hideMemos) ar << 1; else ar << 0;

		if(m_dynamicSurfaceChange) ar << 1; else ar << 0;
	}
	else
	{
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = GetNextView(pos);
			((CProtoHapticView*)pView)->Pause();
			((CProtoHapticView*)pView)->Deselect();
			((CProtoHapticView*)pView)->Play();
		}
		
		clean(); //Destroy existing document

		m_historyCount= 0;
		m_shapeCount= 0;
		m_memoCount= 0;

		dWorldDestroy(m_worldID);
		m_worldID= dWorldCreate();

		//dSpaceDestroy(m_spaceID);
		m_spaceID= dHashSpaceCreate(0);

		int count;
		ar >> count;
		int i;
		for (i= 0; i<count; i++)
		{	
			int shapeType;
			CShape* s= NULL;
			ar >> shapeType;
			if(shapeType==SHAPE_CUBE)		s= new CCube(1,1,1);
			if(shapeType==SHAPE_SPHERE)		s= new CSphere(1,1,1);
			if(shapeType==SHAPE_CYLINDER)	s= new CCylinder(1,1,1);
			if(shapeType==SHAPE_COMPOSITE)	s= new CComposite();
			if(shapeType==SHAPE_TRIANGLE)	s= new CTriangle();
			if(shapeType==SHAPE_TORUS)		s= new CTorus();
			
			// Unknown shape type
			if ( s == NULL )
			{
				MessageBox ( 0, "There was an error whilst reading the specified file: Unknown Shape.", "Error", MB_OK );
				return;
			}
			else
			{
				s->Serialize(ar);
				AddShape(s);
			}
		}

		ar >> count;
		for(i= 0; i<count; i++)
		{
			CMemo* m;
			m= new CMemo();
			m->Serialize(ar);
			addMemo(m);
		}
		ar >> m_simSpeed
		   >> m_airResistance
		   >> m_gravity;
		ar >> i;
		if(i==1) 
			m_hideMemos= true; 
		else 
			m_hideMemos= false;

		// Dynamic surface change in view mode (Version > 4)
		ar >> i;
		if(i==1) 
			m_dynamicSurfaceChange= true; 
		else 
			m_dynamicSurfaceChange= false;

		pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = GetNextView(pos);
			((CProtoHapticView*)pView)->CenterCamera();
		} 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticDoc diagnostics

#ifdef _DEBUG
void CProtoHapticDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProtoHapticDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProtoHapticDoc commands

void CProtoHapticDoc::RemoveShape(CShape* shape)
{
	int i= 0;
	while(m_shapes[i]!=shape) i++;

	dGeomDestroy (m_geoms[i]);
	dBodyDestroy (bodies[i]);

	int j;
	for(j= i; j<m_shapeCount; j++) {
		m_shapes[j]= m_shapes[j+1];

		bodies[j]= bodies[j+1];
		m_geoms[j]= m_geoms[j+1];
	
		if(j+1<m_shapeCount) { 
			dBodySetData(bodies[j], (void *)j);
			dGeomSetData(m_geoms[j], (void *)j);
		}
	}
	m_shapeCount--;
}

void CProtoHapticDoc::pushHistory(CProtoHapticDoc *doc)
{
	if(m_historyCount<MAX_HISTORY) {
		m_history[m_historyCount]= doc;
		m_historyCount++;
	} else {
		delete m_history[0]; //delete oldest and shift along
		int i;
		for(i= 1; i<MAX_HISTORY; i++) {
			m_history[i-1]= m_history[i];
		}
		m_history[MAX_HISTORY-1]= doc;
	}
}

CProtoHapticDoc* CProtoHapticDoc::popHistory()
{
	m_historyCount--;
	return m_history[m_historyCount];
}

CProtoHapticDoc* CProtoHapticDoc::clone()
{
	OutputDebugString("Cloning");

	CProtoHapticDoc* cloned= new CProtoHapticDoc();
	int i;
	for(i= 0; i<m_shapeCount; i++) {
		cloned->AddShape(m_shapes[i]->clone());
	}

	for(i= 0; i<m_memoCount; i++) {
		cloned->m_memos[i]= m_memos[i]->clone();
	}
	cloned->m_memoCount= m_memoCount;

	return cloned;
}

void CProtoHapticDoc::saveState()
{
	pushHistory(this->clone());
}

void CProtoHapticDoc::undo()
{
	if(m_historyCount>0) {
		CProtoHapticDoc* lastState= popHistory();

		int i;
		int max= m_shapeCount;
		for(i= 0; i<max; i++)
		{
			RemoveShape(m_shapes[0]);
		}

		for(i= 0; i<lastState->m_shapeCount; i++) {
			AddShape(lastState->m_shapes[i]);
		}

		for(i= 0; i<lastState->m_memoCount; i++) {
			m_memos[i]= lastState->m_memos[i];
		}
		m_memoCount= lastState->m_memoCount;
	}
}

int CProtoHapticDoc::getMemoCount()
{
	return m_memoCount;
}

CShape* CProtoHapticDoc::getMemo(int id)
{
	return m_memos[id];
}

int CProtoHapticDoc::addMemo(CShape *memo)
{
	if(m_memoCount<MAX_SHAPES) {
		m_memos[m_memoCount]= memo;
		m_memoCount++;
		return m_memoCount-1;
	} else
		return m_memoCount;
}

void CProtoHapticDoc::removeMemo(CShape *memo)
{
	int i= 0;
	while(m_memos[i]!=memo) i++;

	int j;
	for(j= i; j<m_memoCount; j++) {
		m_memos[j]= m_memos[j+1];
	}
	m_memoCount--;
}

void nearCallbackStatic (void *data, dGeomID o1, dGeomID o2)
{
	((CProtoHapticDoc*)data)->nearCallback(o1,o2);
}

/** Scale the whole model, centred at the origin by the specified amount */
void CProtoHapticDoc::ScaleModel ( float f )
{
	// * For all shapes
	for ( int i= 0; i < m_shapeCount; i++ )
	{
		// * Scale distances
		m_shapes[i]->setLocation ( m_shapes[i]->getLocationX() * f,
							       m_shapes[i]->getLocationY() * f,
							       m_shapes[i]->getLocationZ() * f );

		// * Scale individual shapes
		m_shapes[i]->setSize ( m_shapes[i]->getSizeX() * f,
							   m_shapes[i]->getSizeY() * f,
							   m_shapes[i]->getSizeZ() * f );
	}

}

/** Translate the whole model by the specified amount */
void CProtoHapticDoc::TranslateModel ( float x, float y, float z )
{
	// * For all shapes
	for ( int i= 0; i < m_shapeCount; i++ )
	{
		// * Update position
		m_shapes[i]->setLocation ( m_shapes[i]->getLocationX() + x,
							       m_shapes[i]->getLocationY() + y,
							       m_shapes[i]->getLocationZ() + z );
	}

}

void CProtoHapticDoc::SimulationStep()
{
	int elapsed= clock() - m_lastSimStep;
	int steps= (int)((((float)elapsed)*m_simSpeed)*0.1f);

	if(steps<1) {
		m_lastSimStep= clock();
		return;
	}

	// collision detection
	for(int i= 0; i<m_shapeCount; i++) {
		if(m_shapes[i]->isCollisionDynamic())
			dGeomSetBody (m_geoms[i], bodies[i]);
		else
			dGeomSetBody (m_geoms[i], 0);
	}

	dSpaceCollide (m_spaceID,this,&nearCallbackStatic);

	dWorldQuickStep (m_worldID, steps);
	dJointGroupEmpty (m_jointGroup);
	for( int i= 0; i<m_shapeCount; i++) {
			//air resistance
			if(m_shapes[i]->isCollisionDynamic()||m_shapes[i]->isProxyDynamic()) {
				const dReal *vel;
				const dReal *angvel;
				
				vel= dBodyGetLinearVel (bodies[i]);
				dBodyAddForce (bodies[i], -vel[0]*m_airResistance, -vel[1]*m_airResistance, -vel[2]*m_airResistance);

				angvel= dBodyGetAngularVel (bodies[i]);
				dBodyAddTorque (bodies[i], -angvel[0]*m_airResistance, -angvel[1]*m_airResistance, -angvel[2]*m_airResistance);
			}

			HHLRC rc= hlGetCurrentContext();

			// proxy0
			hlMakeCurrent(m_context);
			if(m_shapes[i]->isProxyDynamic()&&m_shapes[i]->touching()) {
				HLdouble force[3];
				HLdouble pp[3];
				hlGetDoublev(HL_DEVICE_FORCE,force);
				hlGetDoublev(HL_PROXY_POSITION,pp);

				dBodyAddForceAtPos (bodies[i], -force[0], -force[1], -force[2],
											   pp[0],    pp[1],    pp[2]);
			}

			if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
				// proxy1
				hlMakeCurrent(m_context_1);
				if(m_shapes[i]->isProxyDynamic()&&m_shapes[i]->touching1()) {
					HLdouble force[3];
					HLdouble pp[3];
					hlGetDoublev(HL_DEVICE_FORCE,force);
					hlGetDoublev(HL_PROXY_POSITION,pp);

					dBodyAddForceAtPos (bodies[i], -force[0], -force[1], -force[2],
												   pp[0],    pp[1],    pp[2]);
				}
			}

			hlMakeCurrent(rc);

			// gravity
			if(m_shapes[i]->isCollisionDynamic())
				dBodyAddForce(bodies[i], 0.0, -m_shapes[i]->getMass()*(m_gravity/10.0f), 0.0);

			const dReal *pos;
			const dReal *rot;
			
			pos= dBodyGetPosition (bodies[i]);

			rot= dBodyGetRotation (bodies[i]);

			float rotation[16];
		
			rotation[0]= rot[0];  rotation[4]= rot[1];   rotation[8]= rot[2];    rotation[12]= rot[3];
			rotation[1]= rot[4];  rotation[5]= rot[5];   rotation[9]= rot[6];    rotation[13]= rot[7];
			rotation[2]= rot[8];  rotation[6]= rot[9];   rotation[10]= rot[10];  rotation[14]= rot[11];
			rotation[3]= 0.0;     rotation[7]= 0.0;      rotation[11]= 0.0;      rotation[15]= 1.0;
			m_shapes[i]->setRotation(rotation);
			m_shapes[i]->setLocation(pos[0], pos[1], pos[2]);
	}

	m_lastSimStep= clock();
}

void CProtoHapticDoc::UpdateDynamics()
{
	for(int i= 0; i<m_shapeCount; i++) {

		dGeomBoxSetLengths (m_geoms[i], m_shapes[i]->getSizeX(),
									    m_shapes[i]->getSizeY(),
									    m_shapes[i]->getSizeZ());

		dGeomSetPosition   (m_geoms[i], m_shapes[i]->getLocationX(),
									    m_shapes[i]->getLocationY(),
									    m_shapes[i]->getLocationZ());
		dGeomSetRotation   (m_geoms[i], dBodyGetRotation(bodies[i]));

		dBodySetPosition   (bodies[i], m_shapes[i]->getLocationX(),
									   m_shapes[i]->getLocationY(),
									   m_shapes[i]->getLocationZ());

		float *rotation= m_shapes[i]->getRotation();

		const dReal rot[12]=
		{ rotation[0], rotation[4], rotation[8], rotation[12],
		  rotation[1], rotation[5], rotation[9], rotation[13],
		  rotation[2], rotation[6], rotation[10], rotation[14] };

		dBodySetRotation   (bodies[i], rot);

		dMass mass;
		dMassSetBox (&mass, m_shapes[i]->getMass(),m_shapes[i]->getSizeX(),
								                   m_shapes[i]->getSizeY(),
								                   m_shapes[i]->getSizeZ());

		dBodySetMass (bodies[i], &mass);

	}
}

void CProtoHapticDoc::nearCallback(dGeomID o1, dGeomID o2)
{
	int index1= (int)dGeomGetData(o1);
	int index2= (int)dGeomGetData(o2);

	if(m_shapes[index1]->isCollisionDynamic() || 
	   m_shapes[index2]->isCollisionDynamic()) {

		int n, i;
		const int N = 50;
		dContact contact[N];
		n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
		if (n > 0) {
			OutputDebugString("Collision\n");
			for (i=0; i<n; i++) {
				contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;
				if (dGeomGetClass(o1) == dSphereClass || dGeomGetClass(o2) == dSphereClass)
					contact[i].surface.mu = 20;
				else
					contact[i].surface.mu = 0.5;
				contact[i].surface.slip1 = 1.0 - (m_shapes[index1]->getSurfaceFriction());
				contact[i].surface.slip2 = 1.0 - (m_shapes[index2]->getSurfaceFriction());
				contact[i].surface.soft_erp = 0.8;
				contact[i].surface.soft_cfm = 0.01;
				dJointID c = dJointCreateContact (m_worldID,m_jointGroup,contact+i);
				dJointAttach (c,dGeomGetBody(o1),dGeomGetBody(o2));
			}
		}
	}

}

void CProtoHapticDoc::setGravity(float g)
{
	m_gravity= g;
}

float CProtoHapticDoc::getGravity()
{
	return m_gravity;
}

void CProtoHapticDoc::setAirResistance(float a)
{
	m_airResistance= a;
}

float CProtoHapticDoc::getAirResistance()
{
	return m_airResistance;
}

void CProtoHapticDoc::setSimSpeed(float s)
{
	m_simSpeed= s;
}

float CProtoHapticDoc::getSimSpeed()
{
	return m_simSpeed;
}

void CProtoHapticDoc::PreSimulation()
{
	m_lastSimStep= clock();
}

void CProtoHapticDoc::setHideMemos(bool b)
{
	m_hideMemos= b;
}

bool CProtoHapticDoc::isHideMemos()
{
	return m_hideMemos;
}

int CProtoHapticDoc::getHistoryCount()
{
	return m_historyCount;
}

void CProtoHapticDoc::setContexts(HHLRC a, HHLRC b)
{
	m_context= a;
	m_context_1= b;
}

void CProtoHapticDoc::Reload()
{
	for(int i= 0; i<m_shapeCount; i++)
	{
		m_shapes[i]= m_shapes[i]->clone();
	}

	for( int i= 0; i<m_memoCount; i++)
	{
		m_memos[i]= m_memos[i]->clone();
	}

	m_historyCount= 0;
}