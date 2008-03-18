#include "stdafx.h"
#include "GhostCodeToDocument.h"
#include "VarValueDlg.h"
#include "FormelParser.h"

#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "Composite.h"
#include "Torus.h"
#include "Geometry.h"

CGhostCodeToDocument::CGhostCodeToDocument(void)
{
	// Max nominal surface property parameters to use for conversion

	// Stiffness
	maxStiffness= 0.5; // Defualt for Omni

	// Damping
	maxDamping= 0.5;

	// indicates an error in parsing the file
	error= false;

	// describes the error(s) encounterd, if error==true
	errorText= "Some problems were encountered and the code may not be imported correctly:\n\n";

	// currently processing this line number (for error reports)
	lineNumber= 0;

	// Has the user opened a variable database
	varDataOpened= false;

	// Default= no root separator
	m_rootSeparator= "";
}

CGhostCodeToDocument::~CGhostCodeToDocument(void)
{
}

// Set OH nominal max stiffness to use in conversion of units of k
void CGhostCodeToDocument::SetMaxStiffness ( float s )
{
	maxStiffness= s;
}

// Set OH nominal max damping to use in conversion of units of damping coeff.
void CGhostCodeToDocument::SetMaxDamping ( float d )
{
	maxDamping= d;
}

// Extracts the scale component from the transformation matrix
void CGhostCodeToDocument::GetScaleFromMatrix ( hduMatrix* matrix, float* scale )
{
	// Transform the origin and 3 points defining unit vectors along each axis, 
	// then measure their new length, that will be the scale in each axis
    hduVector3Dd origin, px, py, pz;
    matrix->multVecMatrix ( hduVector3Dd(0,0,0), origin );
    matrix->multVecMatrix ( hduVector3Dd(1,0,0), px );
    matrix->multVecMatrix ( hduVector3Dd(0,1,0), py );
    matrix->multVecMatrix ( hduVector3Dd(0,0,1), pz );

	// Measure the new lengths
    scale[0] = (float)(origin-px).magnitude();
    scale[1] = (float)(origin-py).magnitude();
    scale[2] = (float)(origin-pz).magnitude();
}

// Set root separator name (default= do not use separator)
void CGhostCodeToDocument::SetRootSeparatorName ( CString separator )
{
	m_rootSeparator= separator;
}

// Extracts the scale component from the transformation matrix
void CGhostCodeToDocument::GetTranslationFromMatrix ( hduMatrix* matrix, float* scale )
{
	// Return the translation components
	scale[0]= (*matrix)[3][0];	// x
	scale[1]= (*matrix)[3][1];	// y
	scale[2]= (*matrix)[3][2];	// z
}

// Extracts the rotation matrix component from the transformation matrix
void CGhostCodeToDocument::GetRotationFromMatrix ( hduMatrix* matrix, hduMatrix* rotation )
{
	matrix->getRotationMatrix ( *rotation );
}

// Read across physical lines until ';'
void CGhostCodeToDocument::GetLogicalLine ( CString* line, std::ifstream* inFile )
{
	// Max input line length
	static const int maxLine= 1000;

	CString l= "";
	*line= ""; //Empty the line

	do 
	{
		*line+= l;

		do
		{
			lineNumber++;	// Count in physical lines

			char buff[maxLine];
			inFile->getline ( buff, maxLine );
			l.Format ( "%s", buff );

			// Remove white space
			l.Replace ( " ", "" );
			l.Replace ( "}", "" );
			l.Replace ( "{", "" );
			l.Replace ( "	", "" );	// Tabs

			// Remove line comments
			int startLineComment= l.Find ( "//" );

			// If found line comment
			if ( startLineComment != -1 )
			{
				// Delete everything after startLineComment
				l.Delete ( startLineComment, l.GetLength() - startLineComment );
			}

			// Remove block comments here
			int startBlockComment= l.Find ( "/*" );
			int endBlockComment= l.Find ( "*/" );

			// If found block comment
			if ( startBlockComment != -1 )
			{
				// End of block comment not on this line
				if ( endBlockComment == -1 )
				{
					// Delete everything after startBlockComment
					l.Delete ( startBlockComment, l.GetLength() - startBlockComment );

					// Advance position until end of block comment is found
					while ( inFile->good () )
					{
						lineNumber++;	// Count in physical lines

						char buff[maxLine];
						inFile->getline ( buff, maxLine );

						CString str;
						str.Format ( "%s", buff );

						// Look for end block comment
						int endBlockComment= str.Find ( "*/" );

						// Remove everything before and including the end comment
						if ( endBlockComment != -1 )
						{
							str.Delete ( 0, endBlockComment + 2 /*delete end comment*/ );

							// Add remaining on to line
							l+= str;

							// Found end of block comment; Exit the loop
							break;
						}
					}
				}
				// End of block comment is on this line
				else
				{
					// Imagine....
					//   @ &
					// 0123456789...
					// a /**/ c

					// Delete everything after startBlockComment and before (including) endBlockComment
					l.Delete ( startBlockComment, endBlockComment - startBlockComment + 2 /* include end */ );
				}
			}
		
			// While not eof and not ( blank / pre-compiler / or-line comment )
		} while ( inFile->good () &&
			      ( l.GetLength() < 1 || l[0] == '#' ||
			        l.GetLength() < 2 || ( l[0] == '/' && l[1] == '/' ) ) );

		// While not eof / line doesnt end with ';'
	}
	while ( inFile->good () &&
		    ( l.GetLength()>0 && l [ l.GetLength()-1 ] != ';') );

	*line+= l;
}

// Get the radii of the torus
void CGhostCodeToDocument::GetTorusRadii ( float* radiusMajor, float* radiusMinor, CString* identifier, std::ifstream* inFile )
{
	// Go to start of file
	inFile->clear ( );
	inFile->seekg ( 0 );
	lineNumber= 0;	// Currently processing this line

	// Look at each line

	// Get first line
	CString line;
	GetLogicalLine ( &line, inFile );

	// Process first line
	GetTorusRadiiLine ( radiusMajor, radiusMinor, identifier, &line );

	// While more data
	while ( inFile->good () /* && !error */ )
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetTorusRadiiLine ( radiusMajor, radiusMinor, identifier, &line );
	}
}

void CGhostCodeToDocument::GetTorusRadiiLine ( float* radiusMajor, float* radiusMinor, CString* identifier, CString* line )
{
	CString operation;						// Name of operation on object
	std::vector<CString> parameters;		// list of parameters as strings

	// Get details of operation on identifier (and process if successfull)
	if ( GetOperation ( identifier, line, &operation, &parameters ) )
	{
		// error, wrong number of parameters for function
		bool parameterError= false;

		// The operation on the composite is add shape
		if ( operation == "setMajorRadius" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", radiusMajor );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), radiusMajor ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}
		else if ( operation == "setMinorRadius" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", radiusMinor );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), radiusMinor ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// Check for errors
	}
	// else, identifier not found
}

// Get the radius of the sphere
void CGhostCodeToDocument::GetSphereRadius ( float* radius, CString* identifier, std::ifstream* inFile )
{
	// Go to start of file
	inFile->clear ( );
	inFile->seekg ( 0 );
	lineNumber= 0;	// Currently processing this line

	// Look at each line

	// Get first line
	//char line[maxLine];
	//inFile->getline ( line, maxLine );
	CString line;
	GetLogicalLine ( &line, inFile );

	// Process first line
	GetSphereRadiusLine ( radius, identifier, &line );

	// While more data
	while ( inFile->good () /* && !error */ )
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetSphereRadiusLine ( radius, identifier, &line );

	}
}

void CGhostCodeToDocument::GetSphereRadiusLine ( float* radius, CString* identifier, CString* line )
{
	CString operation;						// Name of operation on object
	std::vector<CString> parameters;		// list of parameters as strings

	// Get details of operation on identifier (and process if successfull)
	if ( GetOperation ( identifier, line, &operation, &parameters ) )
	{
		// error, wrong number of parameters for function
		bool parameterError= false;

		// The operation on the composite is add shape
		if ( operation == "setRadius" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", radius );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), radius ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// Check for errors
	}
	// else, identifier not found
}


// Get the dimentions of the cube
void CGhostCodeToDocument::GetCubeDims ( float* x, float* y, float* z, CString* identifier, std::ifstream* inFile )
{
	// Go to start of file
	inFile->clear ( );
	inFile->seekg ( 0 );
	lineNumber= 0;	// Currently processing this line

	// Look at each line

	// Get first line
	//char line[maxLine];
	//inFile->getline ( line, maxLine );
	CString line;
	GetLogicalLine ( &line, inFile );

	// Process first line
	GetCubeDimsLine ( x, y, z, identifier, &line );

	// While more data
	while ( inFile->good () /* && !error */ )
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetCubeDimsLine ( x, y, z, identifier, &line );

	}
}

void CGhostCodeToDocument::GetCubeDimsLine ( float* x, float* y, float* z, CString* identifier, CString* line )
{
	CString operation;						// Name of operation on object
	std::vector<CString> parameters;		// list of parameters as strings

	// Get details of operation on identifier (and process if successfull)
	if ( GetOperation ( identifier, line, &operation, &parameters ) )
	{
		// error, wrong number of parameters for function
		bool parameterError= false;

		// The operation on the composite is add shape
		if ( operation == "setHeight" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", y );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), y ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// The operation on the composite is add shape
		if ( operation == "setLength" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", z );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), z ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// The operation on the composite is add shape
		if ( operation == "setWidth" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", x );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), x ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}


		// Check for errors
	}
	// else, identifier not found
}

// Get shape identifier name, given a line of code, the position at which the constructor was found
void CGhostCodeToDocument::GetShapeIdentifier ( CString* line, CString* identifier, int index )
{
		// Clear string identifier
		*identifier= "";

		// Copy string
		CString tmp= *line;

		// Remove white space
		tmp.Replace ( " ", "" );
		tmp.Replace ( "	", "" );

		// Find variable name
		for ( int i= index; i >= 0; i-- )
		{
			// Track back to assignment
			if ( tmp[i] == '=' )
			{
				// read identifier (backwards)
				for ( int j= i-1; j >= 0 && ( isalnum ( tmp[j] ) || tmp[j] == '_' ); j-- )
				{
					// Create string from char
					CString newChar ( "" );
					newChar.AppendChar ( tmp[j] );

					// Pre-pend new char to string
					*identifier= newChar + *identifier;
				}
			}
		}

		// if identifier not found, record error
		if ( identifier->GetLength () == 0 )
		{
			error= true;
			errorText.AppendFormat ( "Line %d: Error: Cannot find identifier for shape.\n", lineNumber );
			errorText.AppendFormat ( tmp );
		}
}

// Returns true if the line of code contains a shape being constructed
// If true type is the type of shape found and parameters is an array of constructor args
// We know what they will be based on the type of the shape
bool CGhostCodeToDocument::GetShapeConstruction ( CString* line, CString* identifier, GhostShapes* type, std::vector<CString>* params )
{
	// Create a tmp copy of line
	CString tmp= *line;

	// Remove all spaces
	tmp.Replace ( " ", "" );

	// Look for new gstSphere
	int foundSphere= tmp.Find ( "newgstSphere" );

	// Look for new gstCube
	int foundCube= tmp.Find ( "newgstCube" );

	// Look for new gstCylinder
	int foundCylinder= tmp.Find ( "newgstCylinder" );

	// Look for new separator
	int foundSeparator= tmp.Find ( "newgstSeparator" );

	// Look for new torus
	int foundTorus= tmp.Find ( "newgstTorus" );

	if ( foundSphere != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundSphere );
		
		// Save the type of shape
		*type= gstSphere;

		// Add shape to list
		return true;
	}

	if ( foundCube != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundCube );
		
		// Save the type of shape
		*type= gstCube;

		// Add shape to list
		return true;
	}

	if ( foundCylinder != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundCylinder );
		
		// Save the type of shape
		*type= gstCylinder;

		// Add shape to list
		return true;
	}

	if ( foundSeparator != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundSeparator );
		
		// Save the type of shape
		*type= gstSeparator;

		// Add shape to list
		return true;
	}

	if ( foundTorus != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundTorus );
		
		// Save the type of shape
		*type= gstTorus;

		// Add shape to list
		return true;
	}

	// no shapes found
	return false;
}

// Check that the string is a valid float literal
bool CGhostCodeToDocument::IsFloatLiteral ( CString* in )
{
	bool ret= true;

	for ( int i= 0; i < in->GetLength (); i++ )
	{
		if ( !isdigit((*in)[i]) && (*in)[i] != '.' )
		{
			// A minus sign is alowed as the first char
			if ( !(i==0 && (*in)[i] == '-' ) )
			{
				ret= false;
				break;
			}
		}
	}

	return ret;
}

// Get a substring a..b of in, leave in out
void CGhostCodeToDocument::GetSubstring ( CString* in, CString* out, int a, int b )
{
	*out= "";

	for ( int i= a; i <= b; i++ )
	{
		out->AppendChar ( (*in)[i] );
	}
}

// From a line of code, get the operation name, if any, being applied to the specified object
// a give a list of its parameters. True if successfull.
bool CGhostCodeToDocument::GetOperation ( CString* identifier,					// in
												  CString* line,						// in
												  CString* operation,					// out
												  std::vector<CString>* parameters )	// out
{
	// Make a copy of the line
	CString tmp= *line;

	// Remove white space
	tmp.Replace ( " ", "" );
	tmp.Replace ( "	", "" ); // Tab

	// Look for the identifier appearing in the line
	int foundIdent= tmp.Find ( *identifier );

	// Make sure identifier is not a substring of an identifier
	
	// Check that char before foundIdent is not alpha, digit or underscore
	if ( ( foundIdent-1 >= 0 ) && ( isalnum ( tmp[foundIdent-1] ) || tmp[foundIdent-1] == '_' ) )
	{
		// We didn't really find the ident, just a substring
		return false;
	}

	// If found identifier
	if ( foundIdent != -1 )
	{
		// Nothing after ident. or after ident
		if ( foundIdent + identifier->GetLength () + 1 >= tmp.GetLength () )
		{
			// Failed to find operation
			return false;
		}

		// From last char of ident to first of operation
		int offset= 0;

		// Look for "." or "->" after the identifier
		if ( tmp [ foundIdent + identifier->GetLength () ] == '.' )
		{
			offset= 1;
		}

		else if ( tmp [ foundIdent + identifier->GetLength () ]     == '-'  &&
				  tmp [ foundIdent + identifier->GetLength () + 1 ] == '>' )
		{
			offset= 2;
		}

		else
		{
			// There's something after the ident but not . or ->
			// Failed
			return false;
		}

		// Found a . or ->

		// Find index of start of parameters
		int startParameters= tmp.Find ( "(", foundIdent + identifier->GetLength () + offset );

		// Find index of end of parameters
		int endParameters= -1;

		// Number of nested braces
		int bracesCount= 1;

		// Look from after first brace to eol
		for ( int i= startParameters+1; i < tmp.GetLength (); i++ )
		{
			if ( tmp[i] == '(' )	// open bracket
			{
				bracesCount++;
			}
			if ( tmp[i] == ')' )	// close bracket
			{
				bracesCount--;
			}

			if ( bracesCount == 0 )	// Reached end of parameters
			{
				endParameters= i;
				break;
			}
		}

		// If didn't find end of parameter list
		if ( endParameters == -1 )
		{
			error= true;
			errorText.AppendFormat ( "Line %d: Error: Cannot find end of parameter list.\n%s\n", lineNumber, tmp );
		}

		// Get the name of the operation
		GetSubstring ( &tmp, operation, 
					   foundIdent + identifier->GetLength () + offset,	// From
					   startParameters-1 );


		// Get the parameter list
		CString parametersString;

		GetSubstring ( &tmp, &parametersString, 
			           foundIdent + identifier->GetLength () + offset + operation->GetLength () + 1,	// From
					   endParameters - 1 );

		// Tokenize the parameter list
		CString token;
		CString group;
		bool grouping= false;
		int curPos= 0;

		// Get first token
		token= parametersString.Tokenize(",",curPos);

		int runningBraceCount= 0;

		while (token != "")
		{
			int braceCount= 0;
			for ( int k= 0; k < token.GetLength (); k++ )
			{
				if ( token.GetBuffer()[k] == '(' )
					braceCount++;

				if ( token.GetBuffer()[k] == ')' )
					braceCount--;
			}

			if ( braceCount > 0 )
			{
				grouping= true;
				//MessageBox ( 0, "StartGroup", "", MB_OK );
			}

			if ( grouping )
			{
				runningBraceCount+= braceCount;

				group.Append ( token );
				group.Append ( "," ); // Restore removed comma
				//MessageBox ( 0, token, "AddToGroup", MB_OK );

				if ( runningBraceCount == 0 )
				{
					parameters->push_back ( group );
					group= "";
					grouping= false;
					//MessageBox ( 0, "StopGroup", "", MB_OK );
				}
			}
			else
			{
				// Add cur token to list
				parameters->push_back ( token );
			}

			// Get next token
			token= parametersString.Tokenize(",",curPos);
		};

		// Success
		return true;
	}
	else
	{
		// Didn't find identifier in line
		return false;
	}
}

// Get the shapes haptic properties
void CGhostCodeToDocument::GetShapeHaptics ( float* kSpring, float* dynF, float* statF, float* damp, 
	                   CString* identifier, std::ifstream* inFile )
{
	// Go to start of file
	inFile->clear ( );
	inFile->seekg ( 0 );
	lineNumber= 0;	// Currently processing this line

	// Look at each line

	// Get first line
	//char line[maxLine];
	//inFile->getline ( line, maxLine );
	CString line;
	GetLogicalLine ( &line, inFile );

	// Process first line
	GetShapeHapticsLine ( kSpring, dynF, statF, damp, identifier, &line );

	// While more data
	while ( inFile->good () /* && !error */ )
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetShapeHapticsLine ( kSpring, dynF, statF, damp, identifier, &line );

	}
}

void CGhostCodeToDocument::GetShapeHapticsLine ( float* kSpring, float* dynF, float* statF, float* damp, 
	                       CString* identifier, CString* line )
{
	CString operation;						// Name of operation on object
	std::vector<CString> parameters;		// list of parameters as strings

	// Get details of operation on identifier (and process if successfull)
	if ( GetOperation ( identifier, line, &operation, &parameters ) )
	{
		// error, wrong number of parameters for function
		bool parameterError= false;

		// The operation on the composite is add shape
		if ( operation == "setSurfaceKspring" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", kSpring );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), kSpring ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// The operation on the composite is add shape
		if ( operation == "setSurfaceKdamping" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", damp );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), damp ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// The operation on the composite is add shape
		if ( operation == "setSurfaceFdynamic" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", dynF );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), dynF ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// The operation on the composite is add shape
		if ( operation == "setSurfaceFstatic" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", statF );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), statF ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// Check for errors
	}
	// else, identifier not found
}

// Get the dimentions of the cylinder
void CGhostCodeToDocument::GetCylinderDims ( float* radius, float* height, CString* identifier, std::ifstream* inFile )
{
	// Go to start of file
	inFile->clear ( );
	inFile->seekg ( 0 );
	lineNumber= 0;	// Currently processing this line

	// Look at each line

	// Get first line
	//char line[maxLine];
	//inFile->getline ( line, maxLine );
	CString line;
	GetLogicalLine ( &line, inFile );

	// Process first line
	GetCylinderDimsLine ( radius, height, identifier, &line );

	// While more data
	while ( inFile->good () /* && !error */ )
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetCylinderDimsLine ( radius, height, identifier, &line );

	}
}

void CGhostCodeToDocument::GetCylinderDimsLine ( float* radius, float* height, CString* identifier, CString* line )
{
	CString operation;						// Name of operation on object
	std::vector<CString> parameters;		// list of parameters as strings

	// Get details of operation on identifier (and process if successfull)
	if ( GetOperation ( identifier, line, &operation, &parameters ) )
	{
		// error, wrong number of parameters for function
		bool parameterError= false;

		// The operation on the composite is add shape
		if ( operation == "setRadius" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", radius );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), radius ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// The operation on the composite is add shape
		if ( operation == "setHeight" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", height );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), height ) )
					{
						// error, don't understand parameter
						parameterError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// Check for errors
	}
	// else, identifier not found
}

// Get vector from gstVector constructor string eg. "gstVector ( 0, 0, 1 )"
bool CGhostCodeToDocument::GetGstVector ( float* vect, CString* gstVect )
{
	std::vector<CString> parameters;
	CString operation;

	// Make a copy of the line
	CString tmp= *gstVect;

	// Remove white space
	tmp.Replace ( " ", "" );
	tmp.Replace ( "	", "" ); // Tab

	// Find index of start of parameters
	int startParameters= tmp.Find ( "(" );

	// Find index of end of parameters
	int endParameters= -1;

	// Number of nested braces
	int bracesCount= 1;

	// Look from after first brace to eol
	for ( int i= startParameters+1; i < tmp.GetLength (); i++ )
	{
		if ( tmp[i] == '(' )	// open bracket
		{
			bracesCount++;
		}
		if ( tmp[i] == ')' )	// close bracket
		{
			bracesCount--;
		}

		if ( bracesCount == 0 )	// Reached end of parameters
		{
			endParameters= i;
			break;
		}
	}

	// If didn't find end of parameter list
	if ( endParameters == -1 )
	{
		error= true;
		errorText.AppendFormat ( "Line %d: Error: Cannot find end of parameter list.\n", lineNumber );
	}

	// Get the name of the operation
	GetSubstring ( &tmp, &operation, 
				   0,	// From
				   startParameters-1 );


	// Get the parameter list
	CString parametersString;

	GetSubstring ( &tmp, &parametersString, 
		           operation.GetLength () + 1,	// From
				   endParameters - 1 );

	// Tokenize the parameter list
	CString token;
	int curPos= 0;

	// Get first token
	token= parametersString.Tokenize(",",curPos);

	while (token != "")
	{
		// Add cur token to list
		parameters.push_back ( token );

		// Get next token
		token= parametersString.Tokenize(",",curPos);
	};

	if ( operation != CString ( "gstVector" ) )
	{
		error= true;
		errorText= "First argument to rotate operation must be \"gstVector ( x, y, z )\"";

		return false;
	}

	if ( parameters.size () != 3 )
	{
		error= true;
		errorText= "gstVector: Wrong number of arguments.";

		return false;
	}

	for ( int i= 0; i < 3; i ++ )
	{
		// If parameter is a literal float		// Paramter *3
		if ( IsFloatLiteral ( &parameters.at(i) ) )
		{
			// convert it to float
			sscanf ( parameters.at (i), "%f", &vect[i] );
		}
		else
		{
			// Try to evaluate expression
			if ( !EvaluateFloatExpression ( &parameters.at (i), &vect[i] ) )
			{
				// error, don't understand parameter
				error= true;
				errorText= "gstVector: Error, don't understand parameter.";

				return false;
			}
		}
	}
}

// Look-up or prompt user for value of variable
bool CGhostCodeToDocument::GetVariableValue ( CString* identifier, CString* value )
{
	// Search for variable name in table
	int index= -1;

	for ( int i= 0; i < varNames.size (); i++ )
	{
		// If found
		if ( varNames.at ( i ) == *identifier )
		{
			// Return stored value
			*value= varValues.at ( i );

			return true;
		}
	}
	
	// if not found, get input from user
	CVarValueDlg dlg;

	// Set parameters for dialog
	dlg.SetParameters ( identifier, lineNumber, value, this, varDataOpened );;
	
	// Show dialog
	INT_PTR ret= dlg.DoModal ();

	// Check dialog return value
	if ( ret != IDOK )
	{
		// Did not obtain answer from user
		return false;
	}

	// If user opened a database, search again for this identifier
	if ( dlg.IsOpenedDatabase () )
	{
		GetVariableValue ( identifier, value );
	}
	else
	{
		// Save name value pair for future look up
		varNames.push_back ( *identifier );
		varValues.push_back ( *value );
	}

	// Success
	return true;
}

// Look for first variable in expression, returning index and true if found
bool CGhostCodeToDocument::FindVariable ( CString* expression, CString* variable, int* index )
{
	// Tokenize to get values/vars
	CString token;
	int curPos= 0;
	std::vector<CString> values;

	// Get first token
	token= expression->Tokenize ( "+-/*() ", curPos);

	while (token != "")
	{
		// Add cur token to list
		values.push_back ( token );

		// Get next token
		token= expression->Tokenize ( "+-/*() ", curPos);
	};

	
	// For each value found
	for ( int i= 0; i < values.size (); i++ )
	{
		// If not literal
		if ( !IsFloatLiteral ( &values.at ( i ) ) )
		{
			// Get index of first occur
			*index= expression->Find ( values.at ( i ), 0 );

			// Save var name
			*variable= values.at ( i );

			return true;
		}
	}

	return false;
}

// Evaluate the expression as a float, true if success
bool CGhostCodeToDocument::EvaluateFloatExpression ( CString* expression, float* answer )
{
	// Create copy of expression
	CString tmp= *expression;

	// Remove white space
	tmp.Replace ( " ", "" );
	tmp.Replace ( "	", "" );

	// Replace all non literal values 

	// index of first char of non-literal
	int index;
	CString variableName;
	
	// Look for first variable
	bool foundVar= FindVariable ( &tmp, &variableName, &index );

	// Continue whilest variable was found
	while ( foundVar )
	{
		// Replace variable with literal
		CString lit;
		
		// If cant get variable value
		if ( !GetVariableValue ( &variableName, &lit ) )
		{
			return false;
		}

		//MessageBox ( 0, "_" + tmp + "_", "Before", MB_OK );

		// Remove variable name
		tmp.Delete ( index, variableName.GetLength() );

		// Insert variable value
		tmp.Insert ( index, lit );

		//MessageBox ( 0, "_" + tmp + "_", "After", MB_OK );

		// Look for next variable
		foundVar= FindVariable ( &tmp, &variableName, &index );
	}

	// Evaluate using function evaluator

	// Formular parser object
	CFormulaParser  form;

	int errorPosition;		// index of error
	CString strErrortext;	// Description of error

	// Do calculation
	double retValue = form.Calculation ( tmp /* expression string */, 
										 0 /* value of x */,
										 errorPosition, strErrortext );

	// Check return status
	if(strErrortext.IsEmpty())
	{
		*answer= retValue;

		// Success
		return true;
	}
	else
	{
		// Extra error info
		MessageBox ( 0, strErrortext, "Parameter Parse Error", MB_OK );

		// Failed to evaluate function
		return false;
	}
}

// Get the transform associated with the given shape identifier in the line of code specified
void CGhostCodeToDocument::GetShapeTransformLine ( hduMatrix* scaleM, hduMatrix* rotateM, hduMatrix* translateM, CString* identifier, CString* line )
{
	CString operation;						// Name of operation on object
	std::vector<CString> parameters;		// list of parameters as strings

	// Get details of operation on identifier (and process if successfull)
	if ( GetOperation ( identifier, line, &operation, &parameters ) )
	{
		// error, wrong number of parameters for function
		bool parameterError= false;

		// error, don't understand parameter
		bool parameterValueError= false;

		// Compare the name of the operation to the one that we expect to see
		if ( operation == "setScale" )
		{
			// Scale parameters
			float scale[3];
			
			// Check number of parameters
			if ( parameters.size () == 3 )
			{
				// If parameter is a literal float		// Paramter *1
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", &scale[0] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), &scale[0] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// If parameter is a literal float		// Paramter *2
				if ( IsFloatLiteral ( &parameters.at(1) ) )
				{
					// convert it to float
					sscanf ( parameters.at (1), "%f", &scale [1] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (1), &scale[1] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(2) ) )
				{
					// convert it to float
					sscanf ( parameters.at (2), "%f", &scale [2] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (2), &scale[2] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}
			}
			else
			{
				parameterError= true;
			}

			if ( !parameterError && !parameterValueError )
			{
				// If no errors occured, construct the scaling matrix
				*scaleM= hduMatrix::createScale ( scale[0], scale[1], scale[2] );
			}
		}
		else if ( operation == "setRotate" )
		{
			// Rotation parameters
			float rotateVector[3];
			float rotateRadians;

			// Check number of parameters
			if ( parameters.size () == 2 )
			{

				// If parameter is a literal float		// Paramter *1= gstVector
				if ( !GetGstVector ( rotateVector, &parameters.at(0) ) )
				{
					// error, don't understand parameter
					parameterValueError= true;
				}

				// If parameter is a literal float		// Paramter *2= radians
				if ( IsFloatLiteral ( &parameters.at(1) ) )
				{
					// convert it to float
					sscanf ( parameters.at (1), "%f", &rotateRadians );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (1), &rotateRadians ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

			}
			else
			{
				parameterError= true;
			}

			if ( !parameterError && !parameterValueError )
			{
				// If no errors occured, construct the rotation matrix
				hduVector3Dd normVector ( rotateVector[0], rotateVector[1], rotateVector[2] );
				normVector.normalize ();

				(*rotateM)= hduMatrix::createRotation ( normVector[0], normVector[1], normVector[2], rotateRadians );
			}
		}
		else if ( operation == "setTranslate" )
		{
			// Translation parameters
			float translate[3];

			// Check number of parameters
			if ( parameters.size () == 3 )
			{

				// If parameter is a literal float		// Paramter *1
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", &translate[0] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), &translate[0] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// If parameter is a literal float		// Paramter *2
				if ( IsFloatLiteral ( &parameters.at(1) ) )
				{
					// convert it to float
					sscanf ( parameters.at (1), "%f", &translate [1] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (1), &translate[1] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(2) ) )
				{
					// convert it to float
					sscanf ( parameters.at (2), "%f", &translate [2] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (2), &translate[2] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}
			}
			else
			{
				parameterError= true;
			}

			if ( !parameterError && !parameterValueError )
			{
				// If no errors occured, construct the translation matrix
				*translateM= hduMatrix::createTranslation ( translate[0], translate[1], translate[2] );
			}
		}

		// Check for errors
		if ( parameterError ) 
		{
			error= true;
			errorText.AppendFormat ( "Line %d: Error: Wrong number of parameters.\n", lineNumber );
			errorText.AppendFormat ( *line );
		}

		if ( parameterValueError ) 
		{
			error= true;
			errorText.AppendFormat ( "Line %d: Error: Cannot understand parameter value.\n", lineNumber );
			errorText.AppendFormat ( *line );
		}
	}
}

// Get the transform associated with the given shape identifier in the inFile
void CGhostCodeToDocument::GetShapeTransform ( hduMatrix* scale, hduMatrix* rotate, hduMatrix* translate, CString* identifier, std::ifstream* inFile )
{
	// Go to start of file
	inFile->clear ( );
	inFile->seekg ( 0 );
	lineNumber= 0;	// Currently processing this line

	// Look at each line

	// Get first line
	//char line[maxLine];
	//inFile->getline ( line, maxLine );
	CString line;
	GetLogicalLine ( &line, inFile );

	// Process first line
	GetShapeTransformLine ( scale, rotate, translate, identifier, &line );

	// While more data
	while ( inFile->good () /* && !error */ )
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetShapeTransformLine ( scale, rotate, translate, identifier, &line );

	}
}

void CGhostCodeToDocument::ApplyTransform ( hduMatrix* transform, int i, std::ifstream* inFile )
{
	if ( shapes.at ( i )->getType () == SHAPE_COMPOSITE )
	{
		// Get members of shape
		std::vector<CString> memberIdents;
		GetMemberShapes ( &identifiers.at ( i ), 
						  &memberIdents,
						  inFile );

		// For each member shape
		for ( int j= 0; j < memberIdents.size (); j++ )
		{
			int index= -1;
			if ( FindShapeIndex ( &memberIdents.at ( j ), &index ) )
			{
				// Recurse into subshape
				hduMatrix combinedTransform= *transform;
				combinedTransform.multLeft ( (transforms.at ( index )) );

				//char s[256];

				//for ( int i= 0; i < 4; i++ )
				//{
				//	for ( int j= 0; j < 4; j++ )
				//	{
				//		sprintf ( s, "%f, ", (*transform)[i][j] );
				//		OutputDebugString ( s );
				//	}
				//}
				//OutputDebugString ( "\n" );
				//MessageBox ( 0, "Here is this objects transform...", "", MB_OK );


				//for ( int i= 0; i < 4; i++ )
				//{
				//	for ( int j= 0; j < 4; j++ )
				//	{
				//		sprintf ( s, "%f, ", ((transforms.at ( index )))[i][j] );
				//		OutputDebugString ( s );
				//	}
				//}
				//OutputDebugString ( "\n" );
				//MessageBox ( 0, "Here is the childs transform...", "", MB_OK );

				//for ( int i= 0; i < 4; i++ )
				//{
				//	for ( int j= 0; j < 4; j++ )
				//	{
				//		sprintf ( s, "%f, ", (combinedTransform)[i][j] );
				//		OutputDebugString ( s );
				//	}
				//}
				//OutputDebugString ( "\n" );
				//MessageBox ( 0, "Here is the final transform...", "", MB_OK );

				ApplyTransform ( &combinedTransform, index, inFile );
			}
			else
			{
				errorText.AppendFormat ( "Warning: Positioning shape: Cannot find shape \"%s\".", memberIdents.at ( j ) );
				error= true;
			}
		}

	}
	else
	{
		TransformAtomicPHShape ( transform, i, inFile );
	}
}

// Apply the specified transformation to the specified atomic PH shape
void CGhostCodeToDocument::TransformAtomicPHShape ( hduMatrix* transform, int i, std::ifstream* inFile )
{
	CShape* shape= shapes.at ( i );

			// Apply the shapes transforms
			// ...........................

			// Set scaling
			float scale[3]= { 1.0, 1.0, 1.0 };
			GetScaleFromMatrix ( transform, scale );

			// Default absolute dimentions of shapes
			float radius= 1;
			float x= 1, y= 1, z= 1;
			float radiusMajor= 1, radiusMinor= 1; // Torus only

			// Set scale of PH shape
			switch ( types.at ( i ) )
			{
			case gstCube:
				// Account for box lengths
				GetCubeDims ( &x, &y, &z, &identifiers.at(i), inFile );

				shape->setSize ( scale[0]*x, scale[1]*y, scale[2]*z );

				break;
			case gstSphere:

				// Account for sphere radius
				GetSphereRadius ( &radius, &identifiers.at(i), inFile );

				shape->setSize ( scale[0]*radius*2.0f, scale[1]*radius*2.0f, scale[2]*radius*2.0f );
				break;
			case gstCylinder:

				// Account for cylinder radius
				GetCylinderDims ( &radius, &y, &identifiers.at(i), inFile );

				shape->setSize ( scale[0]*radius*2.0f, scale[2]*radius*2.0f, scale[1]*y );
				break;
			case gstTorus:

				// Account for radii of the torus
				GetTorusRadii ( &radiusMajor, &radiusMinor, &identifiers.at(i), inFile );

				shape->setSize ( scale[0], scale[2], scale[1] );
				((CTorus*)shape)->setRadiusMajor ( radiusMajor );
				((CTorus*)shape)->setRadiusMinor ( radiusMinor );

				break;
			}

			// Set rotation
			hduMatrix rotation;

			GetRotationFromMatrix( transform, &rotation );

			// In the case of a cylinder do a 90 deg rotation about x first
			if ( types.at ( i ) == gstCylinder )
			{
				rotation.multLeft ( hduMatrix::createRotation ( 1, 0, 0, M_PI*0.5 ) );
			}
			// Ghost torus is flat on the floor, PH / Glut torus is flat in x y plane
			// So rotate PH torus 90, about x- to put it on the floor, then apply ghost transformations
			if ( types.at ( i ) == gstTorus )
			{
				rotation.multLeft ( hduMatrix::createRotation ( 1, 0, 0, M_PI*0.5 ) );
			}


			// Convert to float [16] array
			float rotationArray[16]=
			{
				(float)rotation[0][0], (float)rotation[0][1], (float)rotation[0][2], (float)rotation[0][3],
				(float)rotation[1][0], (float)rotation[1][1], (float)rotation[1][2], (float)rotation[1][3],
				(float)rotation[2][0], (float)rotation[2][1], (float)rotation[2][2], (float)rotation[2][3],
				(float)rotation[3][0], (float)rotation[3][1], (float)rotation[3][2], (float)rotation[3][3]
			};

			// Set the rotation
			shape->setRotation ( rotationArray );

			// Set translation
			float translate[3];

			// Extract translation component from matrix
			GetTranslationFromMatrix ( transform, translate );

			// Set the position of the PH shape
			shape->setLocation ( translate[0],
								 translate[1],
								 translate[2] );

}

// Reads the code in the specified file and adds appropriate objects to the
// existing specified document
void CGhostCodeToDocument::BuildDocument ( CString filename, CProtoHapticDoc* doc )
{
	// Try to open file for read
	std::ifstream inFile;
	inFile.open ( filename, std::ios_base::in );

	// If failed to open file
	if ( !inFile.is_open () )
	{
		// Display error
		MessageBox ( 0, "Failed to open code file.", "Error", MB_OK );

		return;
	}

	// Get a list of all abstract model shapes in the file
	lineNumber= 1;	// Currently processing this line

	// Get first line
	CString line;
	GetLogicalLine ( &line, &inFile );

	// Process first line
	GhostShapes type;
	std::vector<CString> params;
	CString ident;

	// Look for shapes
	if ( GetShapeConstruction ( &line, &ident, &type, &params ) )
	{
		// Add to list of shapes
		identifiers.push_back ( ident );
		types.push_back ( type );
		parameters.push_back ( params );
	}

	// While more data
	while ( !inFile.eof () /* && !error */ )
	{
		// Get next line
		GetLogicalLine ( &line, &inFile );

		// Process line 

		// Look for shapes
		if ( GetShapeConstruction ( &line, &ident, &type, &params ) )
		{
			// Add to list of shapes
			identifiers.push_back ( ident );
			types.push_back ( type );
			parameters.push_back ( params );
		}
	}

	// Check for errors
	if ( error )
	{
		MessageBox ( 0, errorText, "Error Importing File: Get objects.", MB_OK );
		inFile.close ();	// close file now and exit
		return;
	}

	// For each shape in the list
	for ( unsigned int i= 0; i < identifiers.size(); i++ )
	{
		// Search for all operations on it and build a transformation matrix

		// The identifier we're looking for
		CString name= identifiers.at ( i );

		// Matrix to accumulate the transform
		hduMatrix transform;

		// Individual operations
		hduMatrix scale, rotate, translate;

		// For each line search for operations on "name"
		GetShapeTransform ( &scale, &rotate, &translate, &name, &inFile );

		transform.multRight ( scale );
		transform.multRight ( rotate );
		transform.multRight ( translate );

		// Save the shapes transform
		transforms.push_back ( transform );

		// if its a composite its not populated yet
		populated.push_back ( false );
	}

	// Check for errors
	if ( error )
	{
		MessageBox ( 0, errorText, "Error Importing File: Get Transforms.", MB_OK );
		inFile.close ();	// close file now and exit
		return;
	}

	// Add shapes to the document
	for ( unsigned int i= 0; i < identifiers.size(); i++ )
	{
		// The identifier we're looking for
		CString name= identifiers.at ( i );

		// Ptr to the new shape
		CShape* shape= NULL;

		//// List of vertices (for polygon)
		//std::vector<float*> vertices;

		// Create new PH shape
		switch ( types.at ( i ) )
		{
		case gstCube:
			shape= new CCube ( 1, 1, 1 );
			break;
		case gstSphere:
			shape= new CSphere ( 1, 1, 1 );
			break;
		case gstCylinder:
			shape= new CCylinder ( 1, 1, 1 );
			break;
		case gstTorus:
			shape= new CTorus ( );
			break;
		case gstSeparator:
			shape= new CComposite ( );

			// Record that the composite has yet to be populated
			populated.at ( i )= false;

			break;
		}

		shape->SetName ( name );

		// Set haptic properties of shape
		// ..............................

		// Get haptic properties of AM shape
		float stiffness= 0.6;
		float staticFriction= 0.1;
		float dynamicFriction= 0.5;
		float damping= 0.0;

		GetShapeHaptics ( &stiffness, &dynamicFriction, &staticFriction, &damping,
						   &identifiers.at ( i ), &inFile );

		// Set haptic properties of PH shape
		float ohStiffness= stiffness / maxStiffness;
		float ohDamping= damping / maxDamping;

		// Cap between 0..1
		if ( ohStiffness > 1 ) ohStiffness= 1.0;
		if ( ohDamping > 1 ) ohDamping= 1.0;
		if ( ohStiffness < -1 ) ohStiffness= -1.0;
		if ( ohDamping < -1 ) ohDamping= -1.0;
		
		shape->setStiffness ( ohStiffness );
		shape->setDampening ( ohDamping );

		shape->setStaticFriction ( 0 );//staticFriction );
		shape->setDynamicFriction ( 0 );//dynamicFriction );

		// If shape should be added to the model (not using root sep) (comps added later)
		if ( m_rootSeparator == "" && shape->getType() != SHAPE_COMPOSITE )
		{
			// Add the shape to the model
			doc->AddShape ( shape );
			doc->SetModifiedFlag ( ); // Doc has changed
		}

		// Above
		// Save a ptr to the shape for later
		shapes.push_back ( shape );
	}

	// Position shapes (start with root object and work down
	int rootIndex;
	if ( FindShapeIndex ( &m_rootSeparator, &rootIndex ) )
	{
		//PositionShape ( rootIndex, &inFile );
		ApplyTransform ( &(transforms.at ( rootIndex )), rootIndex, &inFile );
	}
	else
	{
		errorText+= "Cannot find root separator.";
		error= true;
	}

	// Add member shapes to composites
	for ( unsigned int i= 0; i < shapes.size(); i++ )
	{
		// The shape we're looking for
		CShape* shape= shapes.at ( i );

		// If the shape is a composite
		if ( shape->getType ( ) == SHAPE_COMPOSITE )
		{
			// Recursivly populate the specified composite
			// so that only complete composites are added to a composite
			PopulateComposite ( i, &inFile );

			// Add to model
			if ( m_rootSeparator == "" )	// If not using a root separator
			{
				// Add the shape to the model
				doc->AddShape ( shape );
				doc->SetModifiedFlag ( ); // Doc has changed
			}
		}
	}

	// If using a root separator
	if ( m_rootSeparator != "" )
	{
		// Look up root separator index
		int rootIndex;
		if ( FindShapeIndex ( &m_rootSeparator, &rootIndex ) )
		{
			// Add the root separator to the document
			doc->AddShape ( shapes.at ( rootIndex ) );
			doc->SetModifiedFlag ( ); // Doc has changed
		}
		else
		{
			errorText.AppendFormat ( "Error: The root separator \"%s\" was not found.", m_rootSeparator );
			error= true;
		}
	}

	// Check for errors
	if ( error )
	{
		MessageBox ( 0, errorText, "Error Importing File", MB_OK );
		inFile.close ();	// close file now and exit
		return;
	}

	// Close the code file
	inFile.close ();
}

// Position this shape by applying the correct transform
void CGhostCodeToDocument::PositionShape ( int i, std::ifstream* inFile )
{
	if ( shapes.at ( i )->getType () != SHAPE_COMPOSITE )
	{
		// Transform this shapes
		TransformShape ( &transforms.at ( i ), i, inFile );
	}
	else
	{
		// Get members of shape
		std::vector<CString> memberIdents;
		GetMemberShapes ( &identifiers.at ( i ), 
						  &memberIdents,
						  inFile );

		// For each member shape
		for ( int j= 0; j < memberIdents.size (); j++ )
		{
			int index= -1;
			if ( FindShapeIndex ( &memberIdents.at ( j ), &index ) )
			{
				// Recurse into subshape
				PositionShape ( index, inFile );
			}
			else
			{
				errorText.AppendFormat ( "Warning: Positioning shape: Cannot find shape \"%s\".", memberIdents.at ( j ) );
				error= true;
			}
		}

		TransformShape ( &transforms.at ( i ), i, inFile );
	}
}

// Gets a list of sub-shape identifiers for the specified composite shape identifier, in file inFile
void CGhostCodeToDocument::GetMemberShapes ( CString* compositeIdent, 
											 std::vector<CString>* memberIdents,
											 std::ifstream* inFile )
{
	// Go to start of file
	inFile->clear ( );
	inFile->seekg ( 0 );
	lineNumber= 0;	// Currently processing this line

	// Look at each line

	// Get first line
	//char line[maxLine];
	//inFile->getline ( line, maxLine );
	CString line;
	GetLogicalLine ( &line, inFile );

	// Process first line
	GetMemberShapesLine ( compositeIdent, memberIdents, &line );

	// While more data
	while ( inFile->good () /* && !error */ )
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetMemberShapesLine ( compositeIdent, memberIdents, &line );
	}
}

// Gets a list of sub-shape identifiers for the specified composite shape identifier, in string line
void CGhostCodeToDocument::GetMemberShapesLine ( CString* compositeIdent, 
														 std::vector<CString>* memberIdents,
														 CString* line )
{
	CString operation;						// Name of operation on object
	std::vector<CString> parameters;		// list of parameters as strings

	// Get details of operation on identifier (and process if successfull)
	if ( GetOperation ( compositeIdent, line, &operation, &parameters ) )
	{
		// error, wrong number of parameters for function
		bool parameterError= false;

		// The operation on the composite is add shape
		if ( operation == "addChild" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// Add parameter to list of member shapes
				memberIdents->push_back ( parameters.at ( 0 ) );
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// Check for errors
	}
	// else, identifier not found
}

// Look-up the shape index from the identifier. True iff success.
bool CGhostCodeToDocument::FindShapeIndex ( CString* identifier, int* index )
{
	*index= -1; // Not found

	for ( unsigned int i= 0; i < identifiers.size (); i++ )
	{
		if ( identifiers.at ( i ) == *identifier )
		{
			//found it
			*index= i;
			return true;
		}
	}

	// Not found it
	return false;
}

// Transform the specified  PH shape by the specified transform
void CGhostCodeToDocument::TransformShape ( hduMatrix* transform, int i, std::ifstream* inFile  )
{
	CShape* shape= shapes.at ( i );

	if ( shape->getType () != SHAPE_COMPOSITE )
	{

		// Apply the shapes transforms
		// ...........................
		// Warning: No support for scaling of separators

		// Set scaling
		float scale[3]= { 1, 1, 1};

		// Extract scale component
		GetScaleFromMatrix ( transform, scale );

		// Cylinder primitives at different rotations
		if ( shape->getType () != SHAPE_CYLINDER )
		{
			shape->setSize ( scale[0]*shape->getSizeX(), 
							 scale[1]*shape->getSizeY(),
							 scale[2]*shape->getSizeZ() );
		}
		else
		{
			shape->setSize ( scale[0]*shape->getSizeX(), 
							 scale[2]*shape->getSizeY(),
							 scale[1]*shape->getSizeZ() );
		}

		// Set rotation
		hduMatrix rotation;
		
		// Extract the rotation component
		GetRotationFromMatrix ( transform, &rotation );

		hduMatrix curRot= getHduMatrix ( shapes.at ( i )->getRotation () );
		curRot.multRight ( rotation );

		// Convert to float [16] array
		float rotationArray[16]=
		{
			(float)curRot[0][0], (float)curRot[0][1], (float)curRot[0][2], (float)curRot[0][3],
			(float)curRot[1][0], (float)curRot[1][1], (float)curRot[1][2], (float)curRot[1][3],
			(float)curRot[2][0], (float)curRot[2][1], (float)curRot[2][2], (float)curRot[2][3],
			(float)curRot[3][0], (float)curRot[3][1], (float)curRot[3][2], (float)curRot[3][3]
		};

		// Set the rotation
		shape->setRotation ( rotationArray );

		// Set translation
		float translate[3];

		// Extract translation component from matrix
		GetTranslationFromMatrix ( transform, translate );

		hduVector3Dd rotatedPos;
		rotation.multVecMatrix ( hduVector3Dd ( shape->getLocationX(), shape->getLocationY(), shape->getLocationZ() ),
								 rotatedPos );

		// Set the position of the PH shape
		shape->setLocation ( translate[0]+shape->getLocationX(),
							 translate[1]+shape->getLocationY(),
							 translate[2]+shape->getLocationZ() );
	}
	else
	{
		CComposite* c= ((CComposite*)shape);

		// Get members of shape
		std::vector<CString> memberIdents;
		GetMemberShapes ( &identifiers.at ( i ), 
						  &memberIdents,
						  inFile );

		// For each member shape
		for ( int j= 0; j < memberIdents.size (); j++ )
		{
			int memberIndex= -1;
			if ( FindShapeIndex ( &memberIdents.at ( j ), &memberIndex ) )
			{
				TransformShape ( transform, memberIndex, inFile );
			}
			else
			{
				errorText.AppendFormat ( "Warning: Could not transform \"%s\". Declaration not found.\n", memberIdents.at ( j ) );
				error= true;
			}
		}
	}
}

// Recursivly populate the specified composite, so that only complete composites are added to a composite
void CGhostCodeToDocument::PopulateComposite ( int compIndex, std::ifstream* inFile )
{
	// if not already populated this composite
	if ( !populated.at ( compIndex ) )
	{
		// Get a ptr to the composite
		CShape* shape= shapes.at ( compIndex );

		// Look for member shape identifiers
		std::vector<CString> members;	// list of members idents

		// Gets a list of sub-shape identifiers for the specified composite shape identifier, in file inFile
		GetMemberShapes ( &identifiers.at ( compIndex ), 
						  &members,
						  inFile );

		//CString s;
		//s.Format ( "%d members of %s", members.size(), identifiers.at ( compIndex ) );
		//MessageBox ( 0, s, s, MB_OK );

		// For each member in the list
		for ( unsigned int j= 0; j < members.size (); j++ )
		{
			int index;

			// Look-up the shape ptr from the identifier
			if ( FindShapeIndex ( &members.at ( j ), &index ) )
			{
				// if the subshape is a composite itself
				if ( shapes.at ( index )->getType () == SHAPE_COMPOSITE )
				{
					// Recursivley populate the sub-composite before adding to the composite
					PopulateComposite ( index, inFile );
				}

				// Add the member to the composite
				((CComposite*)shape)->addShape ( shapes.at ( index ) );

				if ( shapes.at ( index )->getType () == SHAPE_COMPOSITE &&
					 ((CComposite*)shapes.at ( index ))->getShapeCount () == 0 )
				{
					errorText.AppendFormat ( "Warning: Trying to add empty composite (%s) to composite %s: Skipping adding of empty composite.", identifiers.at ( index ), identifiers.at ( compIndex ) );
					error= true;
				}
			}
			else
			{
				// Error, cant find shape in list
				error= true;
				errorText.AppendFormat ( "Warning: Lookup of member \"%s\" of composite \"%s\" failed: Item not added.\n", members.at ( j ), identifiers.at ( compIndex ) );
			}
		}

		// Record that the shape is now fully populated
		populated.at ( compIndex )= true;
	}
}

// Write variable names to file
void CGhostCodeToDocument::SaveVariableDatabase ( CString filename )
{
	std::ofstream outFile;

	// Open file for output
	outFile.open ( filename );

	// Validate
	if ( !outFile.is_open() )
	{
		MessageBox ( 0, "Error: Cannot open file for output:" + filename, "Error", MB_OK );
		return;
	}

	// For each entry in table
	for ( unsigned int i= 0; i < varNames.size () && i < varValues.size (); i++ )
	{
		// Output "name, value" pair
		outFile << varNames.at ( i );
		outFile << " ";
		outFile << varValues.at ( i );
		outFile << "\n";
	}

	// Close output file
	outFile.close ( );
}

// Load variable names from file
void CGhostCodeToDocument::LoadVariableDatabase ( CString filename )
{
	std::ifstream inFile;

	// Open file for input
	inFile.open ( filename );

	// Validate
	if ( !inFile.is_open() )
	{
		MessageBox ( 0, "Error: Cannot open file for reading database:" + filename, "Error", MB_OK );
		return;
	}

	// Clear current database
	varNames.clear ();
	varValues.clear ();

	static const int maxLine= 1000;

	// Current name value pair
	char name[maxLine];
	char value[maxLine];

	// Read first line
	char line [maxLine];
	inFile.getline ( line, maxLine );

	// While more data
	while ( !inFile.eof () /* && !error */ )
	{
		// Process line
		sscanf ( line, "%s %s", &name, &value );

		// store
		varNames.push_back ( CString ( name ) );
		varValues.push_back ( CString ( value ) );

		// Read next line
		inFile.getline ( line, maxLine );
	}

	// Close output file
	inFile.close ( );

	// Has the user opened a variable database
	varDataOpened= true;
}