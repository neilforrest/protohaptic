#pragma once
#include "Shape.h"

/// class to convert a shape into a string of code
/// Subclasses impliment this functionality for each language / API supported
class CShapeToCode
{
public:
	CShapeToCode(void);
	virtual ~CShapeToCode(void);

	/// Get the code required to define types and variables to aid rendering
	/// One-off setup code for the shape goes here
	virtual void GetDeclareCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember )= 0;

	/// Get the code required to generate / initialise the shape
	/// One-off setup code for the shape goes here
	virtual void GetInitCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember )= 0;

	/// Get the code required to render the shape
	/// Code required for (re-)drawing the shape goes here
	virtual void GetRenderCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember )= 0;

	// Amount by which to scale the model before output
	void SetScale ( float s );

protected:

	// Amount by which to scale the model before output
	float modelScale;
};
