#pragma once
#include "codetodocument.h"

// OpenHaptics Headers
#include <HL/hl.h>
#include <HLU/hlu.h>
#include <HapticMouse/HapticMouse.h>

#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>

#include <iostream>
#include <fstream>

#include <vector>

class CGhostCodeToDocument :
	public CCodeToDocument
{
public:
	CGhostCodeToDocument(void);

	virtual ~CGhostCodeToDocument(void);

	// Reads the code in the specified file and adds appropriate objects to the
	// existing specified document
	void BuildDocument ( CString filename, CProtoHapticDoc* doc );

	// Write variable names to file
	void SaveVariableDatabase ( CString filename );

	// Load variable names from file
	void LoadVariableDatabase ( CString filename );

	// Set root separator name (default= do not use separator)
	void SetRootSeparatorName ( CString separator );

protected:

	// Defines different kinds of Ghost shapes shapes
	enum GhostShapes {
	  gstSphere,
	  gstCube,
	  gstCylinder,
	  gstSeparator
	};

	// Extracts the scale component from the transformation matrix
	void GetScaleFromMatrix ( hduMatrix* matrix, float* scale );

	// Extracts the scale component from the transformation matrix
	void GetTranslationFromMatrix ( hduMatrix* matrix, float* scale );

	// Extracts the rotation matrix component from the transformation matrix
	void GetRotationFromMatrix ( hduMatrix* matrix, hduMatrix* rotation );

	// Look-up or prompt user for value of variable
	bool GetVariableValue ( CString* identifier, CString* value );

	// Look for first variable in expression, returning index and true if found
	bool FindVariable ( CString* expression, CString* variable, int* index );

	// Evaluate the expression as a float, true if success
	bool EvaluateFloatExpression ( CString* expression, float* answer );

	// Check that the string is a valid float literal
	bool IsFloatLiteral ( CString* in );

	// Get a substring a..b of in, leave in out
	void GetSubstring ( CString* in, CString* out, int a, int b );

	// From a line of code, get the operation name, if any, being applied to the specified object
	// a give a list of its parameters. True if successfull.
	bool GetOperation ( CString* identifier,					// in
													  CString* line,						// in
													  CString* operation,					// out
													  std::vector<CString>* parameters );	// out

	// Get the transform associated with the given shape identifier in the inFile
	void GetShapeTransform ( hduMatrix* scale, hduMatrix* rotate, hduMatrix* translate, CString* identifier, std::ifstream* inFile );

	// Get the transform associated with the given shape identifier in the line of code specified
	void GetShapeTransformLine ( hduMatrix* scaleM, hduMatrix* rotateM, hduMatrix* translateM, CString* identifier, CString* line );

	// Get the radius of the sphere
	void GetSphereRadius ( float* radius, CString* identifier, std::ifstream* inFile );
	void GetSphereRadiusLine ( float* radius, CString* identifier, CString* line );

	// Get the dimentions of the cube
	void GetCubeDims ( float* x, float* y, float* z, CString* identifier, std::ifstream* inFile );
	void GetCubeDimsLine ( float* x, float* y, float* z, CString* identifier, CString* line );

	// Get the dimentions of the cylinder
	void GetCylinderDims ( float* radius, float* height, CString* identifier, std::ifstream* inFile );
	void GetCylinderDimsLine ( float* radius, float* height, CString* identifier, CString* line );

	// Get the shapes haptic properties
	void GetShapeHaptics ( float* kSpring, float* dynF, float* statF, float* damp, 
		                   CString* identifier, std::ifstream* inFile );
	void GetShapeHapticsLine ( float* kSpring, float* dynF, float* statF, float* damp, 
		                       CString* identifier, CString* line );

	// Get vector from gstVector constructor string eg. "gstVector ( 0, 0, 1 )"
	bool GetGstVector ( float* vect, CString* gstVect );

	// Read across physical lines until ';'
	void GetLogicalLine ( CString* line, std::ifstream* inFile );

	// Get shape identifier name, given a line of code, the position at which the constructor was found
	void GetShapeIdentifier ( CString* line, CString* identifier, int index );

	// Returns true if the line of code contains a shape being constructed
	// If true type is the type of shape found and parameters is an array of constructor args
	// We know what they will be based on the type of the shape
	bool GetShapeConstruction ( CString* line, CString* identifier, GhostShapes* type, std::vector<CString>* params );

	// Look-up the shape index from the identifier. True iff success.
	bool FindShapeIndex ( CString* identifier, int* index );

	// Gets a list of sub-shape identifiers for the specified composite shape identifier, in file inFile
	void GetMemberShapes ( CString* compositeIdent, 
						   std::vector<CString>* memberIdents,
						   std::ifstream* inFile );

	// Gets a list of sub-shape identifiers for the specified composite shape identifier, in string line
	void GetMemberShapesLine ( CString* compositeIdent, 
						       std::vector<CString>* memberIdents,
						       CString* line );

	// Recursivly populate the specified composite, so that only complete composites are added to a composite
	void PopulateComposite ( int compIndex, std::ifstream* inFile );

	// Position this shape by applying the correct transform
	void PositionShape ( int i, std::ifstream* inFile );

	// Transform the specified  PH shape by the specified transform
	void TransformShape ( hduMatrix* transform, int i, std::ifstream* inFile );

	// Table of variable names and values
	std::vector<CString> varNames;
	std::vector<CString> varValues;

	// currently processing this line number (for error reports)
	int lineNumber;

	// A list of all the variable names of shapes found
	std::vector<CString> identifiers;

	// A list of all the types of shapes found
	std::vector<GhostShapes> types;

	// A list of all the transforms of shapes found
	std::vector<hduMatrix> transforms;

	// A list of pointers to created PH shapes
	std::vector<CShape*> shapes;

	// A list of construction parameters for shapes found
	std::vector< std::vector<CString> > parameters;

	// If the indexed shape is a composite, the value indicates whether it has been
	// populated fully yet
	std::vector<bool> populated;

	// indicates an error in parsing the file
	bool error;

	// describes the error(s) encounterd, if error==true
	CString errorText;

	// Has the user opened a variable database
	bool varDataOpened;

	// Name of root separator
	CString m_rootSeparator;
};
