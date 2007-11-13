#pragma once

#include "ProtoHapticDoc.h"

// Interface to convert file containing code into a PH document 
class CCodeToDocument
{
public:
	// Create a new converter
	CCodeToDocument(void);

	// Destroy the converter
	virtual ~CCodeToDocument(void);

	// Reads the code in the specified file and adds appropriate objects to the
	// existing specified document
	virtual void BuildDocument ( CString filename, CProtoHapticDoc* doc )= 0;

	// Write variable names to file
	virtual void SaveVariableDatabase ( CString filename )= 0;

	// Load variable names from file
	virtual void LoadVariableDatabase ( CString filename )= 0;

protected:
	// Amount by which to scale the model before output
	float modelScale;

};
