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

// Used to convert a file containing abstract model code into a protohaptic document
class CAbstractModelCodeToDocument :
	public CCodeToDocument
{
public:
	// Create a new converter
	CAbstractModelCodeToDocument(void);

	// Destroy the converter
	~CAbstractModelCodeToDocument(void);

	// Reads the code in the specified file and adds appropriate objects to the
	// existing specified document
	void BuildDocument ( CString filename, CProtoHapticDoc* doc );

	// Write variable names to file
	void SaveVariableDatabase ( CString filename );

	// Load variable names from file
	void LoadVariableDatabase ( CString filename );

protected:

	// Defines different kinds of Abstract Model shapes
	enum AbstractModelShapes {
	  Cube,
	  Sphere,
	  Cylinder,
	  Polygon,
	  Composite,
	  Torus
	};

	// Look-up the shape index from the identifier. True iff success.
	bool FindShapeIndex ( CString* identifier, int* index );

	// From a line of code, get the operation name, if any, being applied to the specified object
	// a give a list of its parameters. True iff successfull
	bool GetOperation ( CString* identifier,				// in
						CString* line,						// in
						CString* operation,					// out
						std::vector<CString>* parameters );	// out

	// Returns true iff the specified shape identifier is ever added to the model in file inFile
	bool IsAddedToModel ( CString* identifier, std::ifstream* inFile );

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

	// Returns true iff the specified shape identifier is ever added to the model in string line
	bool IsAddedToModelLine ( CString* identifier, CString* line );

	// Returns true if the line of code contains a shape being constructed
	// If true type is the type of shape found and parameters is an array of constructor args
	// identifier contains the name of the variable used for the shape
	// We know what they will be based on the type of the shape
	bool GetShapeConstruction ( CString* line, CString* identifier, AbstractModelShapes* type, std::vector<CString>* params );

	// Get shape identifier name, given a line of code, the position at which the constructor was found
	void GetShapeIdentifier ( CString* line, CString* identifier, int index );

	// Get the transform associated with the given shape identifier in the line of code specified
	void GetShapeTransformLine ( hduMatrix* transform, CString* identifier, CString* line );

	// Get the transform associated with the given shape identifier in the inFile
	void GetShapeTransform ( hduMatrix* transform, CString* identifier, std::ifstream* inFile );

	// Get the color associated with the given shape identifier in the inFile
	void GetShapeColor ( float* rgb, CString* identifier, std::ifstream* inFile );

	// Get the haptic properties associated with the given shape identifier in the inFile
	void GetShapeHaptics ( float* stiffness,
						   float* staticFriction,
						   float* dynamicFriction,
						   float* damping,
						   CString* identifier, std::ifstream* inFile );

	// Get the triangle vertices associated with the given shape identifier in the inFile
	void GetTriangleVertices ( std::vector<float*>* verts,
						       CString* identifier, std::ifstream* inFile );

	// Get the triangle vertices associated with the given shape identifier in the line
	void GetTriangleVerticesLine ( std::vector<float*>* verts,
						           CString* identifier, CString* line );

	// Get the torus radius associated with the given shape identifier in the inFile
	void GetTorusRadius ( float* radius,
						  CString* identifier, std::ifstream* inFile );

	// Get the triangle vertices associated with the given shape identifier in the line
	void GetTorusRadiusLine ( float* radius,
						      CString* identifier, CString* line );

	// Get the color associated with the given shape identifier in the string
	void GetShapeColorLine ( float* rgb, CString* identifier, CString* line );

	// Get the haptic properties associated with the given shape identifier in the string
	void GetShapeHapticsLine ( float* stiffness,
						       float* staticFriction,
						       float* dynamicFriction,
						       float* damping,
						       CString* identifier, CString* line );

	// Read across physical lines until ';'
	void GetLogicalLine ( CString* line, std::ifstream* inFile );

	// Get a substring a..b of in, leave in out
	void GetSubstring ( CString* in, CString* out, int a, int b );

	// Check that the string is a valid float literal
	bool IsFloatLiteral ( CString* in );

	// Extracts the scale component from the transformation matrix
	void GetScaleFromMatrix ( hduMatrix* matrix, float* scale );

	// Extracts the scale component from the transformation matrix
	void GetTranslationFromMatrix ( hduMatrix* matrix, float* scale );

	// Extracts the rotation matrix component from the transformation matrix
	void GetRotationFromMatrix ( hduMatrix* matrix, hduMatrix* rotation );

	// Evaluate the expression as a float, true if success
	bool EvaluateFloatExpression ( CString* expression, float* answer );

	// Look-up or prompt user for value of variable
	bool GetVariableValue ( CString* identifier, CString* value );

	// Look for first variable in expression, returning index and true if found
	bool FindVariable ( CString* expression, CString* variable, int* index );

	// Table of variable names and values
	std::vector<CString> varNames;
	std::vector<CString> varValues;

	// A list of all the variable names of shapes found
	std::vector<CString> identifiers;

	// A list of all the types of shapes found
	std::vector<AbstractModelShapes> types;

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

	// currently processing this line number (for error reports)
	int lineNumber;

	// Has the user opened a variable database
	bool varDataOpened;
};
