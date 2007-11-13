// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Feelable.cpp
// Classes: CFeelable
// Purpose: Represents an object that can be rendered by OpenHaptics

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Feelable.h"
#include "FormelParser.h"

#define MAX_IDS	999999

int g_idCount;
HLint g_shapeIDs[MAX_IDS];
HLint g_shapeIDs_1[MAX_IDS];
HHLRC g_context;
HHLRC g_context_1;

HLint g_firstID;
HLint g_firstID_1;
HLint g_nextID;
HLint g_nextID_1;

bool InitFeelable(HHLRC c, HHLRC c_1)
{
/*	
	// The old (but proven) slow way

	g_idCount= MAX_IDS;

	g_context= c;
	g_context_1= c_1;

	for(int i= 0; i<MAX_IDS; i++)
	{
		hlMakeCurrent(g_context);
		g_shapeIDs[i]= hlGenShapes(1);

		if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
			hlMakeCurrent(g_context_1);
			g_shapeIDs_1[i]= hlGenShapes(1);
			hlMakeCurrent(g_context);
		}
	}
*/

	// This way is faster
	g_idCount= 0;

	g_context= c;
	g_context_1= c_1;

	hlMakeCurrent(g_context);
	g_firstID= hlGenShapes(MAX_IDS);

	if(((CProtoHapticApp*)AfxGetApp())->isTwoDevices()) {
		hlMakeCurrent(g_context_1);
		g_firstID_1= hlGenShapes(MAX_IDS);
		hlMakeCurrent(g_context);
	}

	g_nextID= g_firstID;
	g_nextID_1= g_firstID_1;

	return true;
}

void AllocateShape(HLuint *id, HLuint *id_1)
{
	if(g_idCount<MAX_IDS) {
		g_idCount++;;
		*id= g_nextID;
		*id_1= g_nextID_1;

		g_nextID++;
		g_nextID_1++;
	} else {
		::MessageBox(NULL, "Error: Ran out of shape IDs! Can't Allocate.", "Error", MB_OK); 
	}
}

void DeallocateShape(HLuint id, HLuint id_1)
{
	g_shapeIDs[g_idCount]= id;
	g_shapeIDs_1[g_idCount]= id_1;
	g_idCount++;
}

void HLCALLBACK onTouched(HLenum event, HLuint object,
								   HLenum thread,
								   HLcache *cache,
								   void *userdata)
{
	((CFeelable*)userdata)->touched();
}

void HLCALLBACK onUntouched(HLenum event, HLuint object,
								   HLenum thread,
								   HLcache *cache,
								   void *userdata)
{
	((CFeelable*)userdata)->untouched();
}

void HLCALLBACK onTouched1(HLenum event, HLuint object,
								   HLenum thread,
								   HLcache *cache,
								   void *userdata)
{
	((CFeelable*)userdata)->touched1();
}

void HLCALLBACK onUntouched1(HLenum event, HLuint object,
								   HLenum thread,
								   HLcache *cache,
								   void *userdata)
{
	((CFeelable*)userdata)->untouched1();
}

CFeelable::CFeelable()
{
	AllocateShape(&m_shapeID, &m_shapeID_1);

	setActive(true);

	m_touching= false;
	m_touching1= false;

	setStiffness(0.5);
	setDampening(0.5);
	setStaticFriction(0.5);
	setDynamicFriction(0.5);

	setStiffnessFormula("x");
	setDampeningFormula("x");
	setStaticFrictionFormula("x");
	setDynamicFrictionFormula("x");

	m_logicalValue= 0.0;
	setLogical(false);

	// Enable custom (non poligonal) rendering where supported
	m_customRendered= true;
}

CFeelable::~CFeelable()
{
	DeallocateShape(m_shapeID, m_shapeID_1);
}

// List of feelable dependants
// Touch / Untouch events should be passed on to all
void CFeelable::AddDependant ( CFeelable* d )
{
	m_dependants.push_back ( d );
}

void CFeelable::RemoveDependant ( CFeelable* d )
{
	for ( int i= 0; i < m_dependants.size (); i++ )
	{
		if ( m_dependants.at ( i ) == d )
		{
			m_dependants.erase ( m_dependants.begin () + i );
		}
	}
}

// Enable custom (non poligonal) rendering where supported
void CFeelable::EnableCustomRendering ( bool t )
{
	m_customRendered= t;
}

bool CFeelable::IsEnabledCustomRendering ()
{
	return m_customRendered;
}

void CFeelable::touched()
{
	m_touching= true;

	for ( int i= 0; i < m_dependants.size (); i++ )
	{
		m_dependants.at ( i )->touched ( );
	}
}

void CFeelable::untouched()
{
	m_touching= false;

	for ( int i= 0; i < m_dependants.size (); i++ )
	{
		m_dependants.at ( i )->untouched ( );
	}
}

HLuint CFeelable::GetShapeID ()
{
	return m_shapeID;
}

HLuint CFeelable::GetShapeID_1 ()
{
	return m_shapeID_1;
}

bool CFeelable::touching()
{
	return m_touching;
}

void CFeelable::touched1()
{
	m_touching1= true;

	for ( int i= 0; i < m_dependants.size (); i++ )
	{
		m_dependants.at ( i )->touched1 ( );
	}
}

void CFeelable::untouched1()
{
	m_touching1= false;

	for ( int i= 0; i < m_dependants.size (); i++ )
	{
		m_dependants.at ( i )->untouched1 ( );
	}
}

bool CFeelable::touching1()
{
	return m_touching1;
}

void CFeelable::setStiffness(float stiffness)				{ m_stiffness= stiffness; }
void CFeelable::setDampening(float dampening)				{ m_dampening= dampening; }
void CFeelable::setStaticFriction(float staticFriction)		{ m_static_friction= staticFriction; }
void CFeelable::setDynamicFriction(float dynamicFriction)	{ m_dynamic_friction= dynamicFriction; }

float CFeelable::getStiffness()			{ return m_stiffness; }
float CFeelable::getDampening()			{ return m_dampening; }
float CFeelable::getStaticFriction()	{ return m_static_friction; }
float CFeelable::getDynamicFriction()	{ return m_dynamic_friction; }

void CFeelable::setActive(bool active)
{
	HHLRC rc= hlGetCurrentContext();

	hlMakeCurrent(g_context);
	if(active)
	{
			hlAddEventCallback(HL_EVENT_TOUCH,m_shapeID,
							   HL_COLLISION_THREAD,&onTouched,this);
			hlAddEventCallback(HL_EVENT_UNTOUCH,m_shapeID,
							   HL_COLLISION_THREAD,&onUntouched,this);
	} else {
			hlRemoveEventCallback(HL_EVENT_TOUCH,m_shapeID,
							   HL_COLLISION_THREAD,&onTouched);
			hlRemoveEventCallback(HL_EVENT_UNTOUCH,m_shapeID,
							   HL_COLLISION_THREAD,&onUntouched);
	}

	if ( ((CProtoHapticApp*)AfxGetApp())->isTwoDevices() ) {
	hlMakeCurrent(g_context_1);
		if(active)
		{
				hlAddEventCallback(HL_EVENT_TOUCH,m_shapeID_1,
								   HL_COLLISION_THREAD,&onTouched1,this);
				hlAddEventCallback(HL_EVENT_UNTOUCH,m_shapeID_1,
								   HL_COLLISION_THREAD,&onUntouched1,this);
		} else {
				hlRemoveEventCallback(HL_EVENT_TOUCH,m_shapeID_1,
								   HL_COLLISION_THREAD,&onTouched1);
				hlRemoveEventCallback(HL_EVENT_UNTOUCH,m_shapeID_1,
								   HL_COLLISION_THREAD,&onUntouched1);
		}
	}

	hlMakeCurrent(rc);
}

void CFeelable::drawHapticScene(bool primary_context)
{
	//If this is called we've gone wrong
	OutputDebugString("Error: Can't drawHapticScene from abstract class.");
	::MessageBox(::GetFocus(),"!","Error",MB_OK);
}

void CFeelable::setStiffnessFormula(CString f)
{
	m_stiffnessString= f;
}

void CFeelable::setDampeningFormula(CString f)
{
	m_dampeningString= f;
}

void CFeelable::setStaticFrictionFormula(CString f)
{
	m_staticFString= f;
}

void CFeelable::setDynamicFrictionFormula(CString f)
{
	m_dynamicFString= f;
}

CString CFeelable::getStiffnessFormula()
{
	return m_stiffnessString;
}

CString CFeelable::getDampeningFormula()
{
	return m_dampeningString;
}

CString CFeelable::getStaticFrictionFormula()
{
	return m_staticFString;
}

CString CFeelable::getDynamicFrictionFormula()
{
	return m_dynamicFString;
}

bool CFeelable::isLogical()
{
	return m_logical;
}

void CFeelable::setLogical(bool b)
{
	m_logical= b;
}

double CFeelable::getLogicalValue()
{
	return m_logicalValue;
}

void CFeelable::setLogicalValue(double v)
{
	m_logicalValue= v; 
}

bool CFeelable::EvaluateLogical()
{
	CFormulaParser  form;

	int ErrorPosition;
	CString strErrortext;
	strErrortext.Empty();

	double retValue = form.Calculation(m_stiffnessString, 
									   m_logicalValue, ErrorPosition, strErrortext);
	if(retValue>=0 && retValue<=1 && strErrortext.IsEmpty())
		m_stiffness= retValue;
	else
		return false;

	retValue = form.Calculation(m_dampeningString, 
									   m_logicalValue, ErrorPosition, strErrortext);
	if(retValue>=0 && retValue<=1 && strErrortext.IsEmpty())
		m_dampening= retValue;
	else
		return false;

	retValue = form.Calculation(m_staticFString, 
									   m_logicalValue, ErrorPosition, strErrortext);
	if(retValue>=0 && retValue<=1 && strErrortext.IsEmpty())
		m_dynamic_friction= retValue;
	else
		return false;

	retValue = form.Calculation(m_dynamicFString, 
									   m_logicalValue, ErrorPosition, strErrortext);
	if(retValue>=0 && retValue<=1 && strErrortext.IsEmpty())
		m_static_friction= retValue;
	else
		return false;

	return true;
}
