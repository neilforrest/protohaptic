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
class CShapeToAbstractModelCode :
	public CShapeToCode
{
public:
	CShapeToAbstractModelCode(void);
	~CShapeToAbstractModelCode(void);

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

	// Initialisation functions

	/// Get Init Code for a Cube
	void GetInitCodeCube ( CCube* cube, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Sphere
	void GetInitCodeSphere ( CSphere* sphere, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Cylinder
	void GetInitCodeCylinder ( CCylinder* cylinder, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Composite
	void GetInitCodeComposite ( CComposite* composite, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Triangle
	void GetInitCodeTriangle ( CTriangle* triangle, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Memo
	void GetInitCodeMemo ( CMemo* memo, CString *string, CString *identifier, bool compositeMember  );

	/// Get Init Code for a Composite
	void GetInitCodeTorus ( CTorus* torus, CString *string, CString *identifier, bool compositeMember  );

	/// Get code to set the shapes transform
	void GetTransform ( CShape* shape, CString *string /* in/out */, CString *identifier /* in */, bool compositeMember  );

	/// Add code to set the graphic properties
	void GetGraphicProperties ( CShape* shape, CString *string /* in/out */, CString *identifier /* in */ );

	/// Add code to set the haptic properties
	void GetHapticProperties ( CShape* shape, CString *string /* in/out */, CString *identifier /* in */ );

	// Get Euler angles from rotation matrix
	void GetEulerFromMatrix ( float* matrix, float *x, float *y, float *z );
};
