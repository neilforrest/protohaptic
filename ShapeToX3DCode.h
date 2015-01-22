#pragma once
#include "shapetocode.h"
#include "cube.h"
#include "sphere.h"
#include "cylinder.h"
#include "composite.h"
#include "Triangle.h"
#include "Torus.h"
#include "memo.h"

/// Abstract class to convert a shape into a string of code
/// to compile with the AbstractModel Library API
class CShapeToX3DCode :
	public CShapeToCode
{
public:
	CShapeToX3DCode(void);
	~CShapeToX3DCode(void);

	/// Get the code required to define types and variables to aid rendering
	/// One-off setup code for the shape goes here
	void GetDeclareCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember );

	/// Get the code required to generate / initialise the shape
	/// One-off setup code for the shape goes here
	void GetInitCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember );

	/// Get the code required to render the shape
	/// Code required for (re-)drawing the shape goes here
	void GetRenderCode ( CShape* shape, CString *string, CString *identifier, bool compositeMember );

protected:

	// Declaration functions

	/// Get Init Code for a Cube
	void GetDeclareCodeCube ( CCube* cube, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Sphere
	void GetDeclareCodeSphere ( CSphere* sphere, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Cylinder
	void GetDeclareCodeCylinder ( CCylinder* cylinder, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Composite
	void GetDeclareCodeComposite ( CComposite* composite, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Triangle
	void GetDeclareCodeTriangle ( CTriangle* triangle, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Torus
	void GetDeclareCodeTorus ( CTorus* torus, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Memo
	void GetDeclareCodeMemo ( CMemo* memo, CString *string, CString *identifier, bool compositeMember  );

  void GetTransformedShape ( CShape& shape, CString& string, const CString& identifier, const CString& shapeCode, float* rotation= NULL );

};
