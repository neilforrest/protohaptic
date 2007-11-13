// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Feelable.h
// Classes: CFeelable
// Purpose: Represents an object that can be rendered by OpenHaptics

#ifndef FEELABLE
#define FEELABLE

#include <HL/hl.h>
#include <HLU/hlu.h>
#include <vector>

/** Initialises the "Feelable" subsystem 
    c and c_1 are the high level rendering contexts
    returned by OpenHaptics initialisation for both
    primary and secondary haptic devices */
bool InitFeelable(HHLRC c, HHLRC c_1);

/** Represents an object that can be rendered by OpenHaptics */
class CFeelable
{
public:
	/** Evaluate the physical haptic properties from the 
		 logical equations */
	bool EvaluateLogical();
	
	/** Sets the value of the feelable's logical property control */
	void setLogicalValue(double v);
	
	/** Gets the value of the feelable's logical property control */
	double getLogicalValue();
	
	/** Activates or deactivates the logical property control */
	void setLogical(bool b);
	
	/** Returns true iff the shape is under control of logical property */
	bool isLogical();
	
	/** Set the dynamic friction formula. Logical property is 'x' and 
	    ranges between 0..1. The formula should evaluate in the same range
	    otherwise it will not be applied */
	void setDynamicFrictionFormula(CString f);
	
	/** Set the static friction formula. Logical property is 'x' and 
	    ranges between 0..1. The formula should evaluate in the same range
	    otherwise it will not be applied */
	void setStaticFrictionFormula(CString f);
	
	/** Set the dampening formula. Logical property is 'x' and 
	    ranges between 0..1. The formula should evaluate in the same range
	    otherwise it will not be applied */
	void setDampeningFormula(CString f);
	
	/** Set the stiffness formula. Logical property is 'x' and 
	    ranges between 0..1. The formula should evaluate in the same range
	    otherwise it will not be applied */
	void setStiffnessFormula(CString f);

	/** Returns the dynamic friction formula */
	CString getDynamicFrictionFormula();
	
	/** Returns the static friction formula */
	CString getStaticFrictionFormula();
	
	/** Returns the dampening formula */
	CString getDampeningFormula();
	
	/** Returns the stiffness formula */
	CString getStiffnessFormula();

	/** Activate or deactivate the feelable. Called 
	    before it is added to the scene or after it's removed */
	void setActive(bool active);
	
	/** Create a new CFeelable */
	CFeelable();
	
	/** Destroy a feelable object */
	virtual ~CFeelable();
	
	/** Override this to render the feelable object
	    If primary_context then render to context c
	    otherwise render to c_1 */
	virtual void drawHapticScene(bool primary_context)= 0;
	
	/** Return true iff feelable is being touched in context c */
	virtual bool touching ();
	
	/** Return true iff feelable is being touched in context c_1 */
	virtual bool touching1 ();
	
	/** Set the stiffness haptic property
		 Value between 0..1 */
	virtual void setStiffness(float stiffness);
	
	/** Set the dampening haptic property
		 Value between 0..1 */
	virtual void setDampening(float dampening);
	
	/** Set the static friction haptic property
		 Value between 0..1 */
	virtual void setStaticFriction(float staticFriction);
	
	/** Set the dynamic friction haptic property
		 Value between 0..1 */
	virtual void setDynamicFriction(float dynamicFriction);

	/** Get the stiffness haptic property */
	float getStiffness();
	
	/** Get the dampening haptic property */
	float getDampening();
	
	/** Get the static friction haptic property */
	float getStaticFriction();
	
	/** Get the dynamic friction haptic property */
	float getDynamicFriction();

	/** Called by the static OpenHaptics callback when this
		 feelable object is touched in context c */
	void touched();
	
	/** Called by the static OpenHaptics callback when this
		 feelable object is untouched in context c  */
	void untouched();
	
	/** Called by the static OpenHaptics callback when this
		 feelable object is touched in context c_1 */
	void touched1();
	
	/** Called by the static OpenHaptics callback when this
		 feelable object is untouched in context c_1 */
	void untouched1();

	// Enable custom (non poligonal) rendering where supported
	void EnableCustomRendering ( bool t );
	bool IsEnabledCustomRendering ();
	
	HLuint GetShapeID ();
	HLuint GetShapeID_1 ();

	// List of feelable dependants
	// Touch / Untouch events should be passed on to all
	void AddDependant ( CFeelable* d );
	void RemoveDependant ( CFeelable* d );

protected:
	// List of feelable dependants
	// Touch / Untouch events should be passed on to all
	std::vector<CFeelable*> m_dependants;

	// Enable custom (non poligonal) rendering where supported
	bool m_customRendered;

	/** Feelable's shape ID in context c_1 */
	HLuint m_shapeID_1;
	
	/** Value of logical property control. Between 0..1. */
	double m_logicalValue;
	
	/** Is the feelable under control of logical property */
	bool m_logical;
	
	/** The logical formula for dynamic friction */
	CString m_dynamicFString;
	
	/** The logical formula for static friction */
	CString m_staticFString;
	
	/** The logical formula for dampening */
	CString m_dampeningString;
	
	/** The logical formula for stiffness */
	CString m_stiffnessString;
	
	/** Feelables shape ID in context c */
	HLuint m_shapeID;
	
	/** Is proxy in contact with shape in context c */
	bool m_touching;
	
	/** Is shape in contact with proxy in context c_1 */
	bool m_touching1;
	
	/** Feelable's haptic properties */
	float m_stiffness,
		  m_dampening,
		  m_static_friction,
		  m_dynamic_friction;
};

#endif
