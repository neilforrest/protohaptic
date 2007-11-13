#include "stdafx.h"
#include "AbstractModelCodeToDocument.h"

// Various shapes
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "Composite.h"
#include "Torus.h"
#include "FormelParser.h"

#include "VarValueDlg.h"

// Max input line length
static const int maxLine= 1000;

// Create a new converter
CAbstractModelCodeToDocument::CAbstractModelCodeToDocument(void)
{
	// indicates an error in parsing the file
	error= false;

	// describes the error(s) encounterd, if error==true
	errorText= "Some errors were encountered and the code cannot be imported:\n\n";

	// currently processing this line number (for error reports)
	lineNumber= 0;

	// Has the user opened a variable database
	varDataOpened= false;
}

// Destroy the converter
CAbstractModelCodeToDocument::~CAbstractModelCodeToDocument(void)
{

}

// Get shape identifier name, given a line of code, the position at which the constructor was found
void CAbstractModelCodeToDocument::GetShapeIdentifier ( CString* line, CString* identifier, int index )
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
bool CAbstractModelCodeToDocument::GetShapeConstruction ( CString* line, CString* identifier, AbstractModelShapes* type, std::vector<CString>* params )
{
	// Create a tmp copy of line
	CString tmp= *line;

	// if line contains a semi-colon, split it again
	//if ( tmp.Find ( ";" ) != -1 )
	//{
	//	tmp.F
	//}
		
	// Remove all spaces
	tmp.Replace ( " ", "" );

	// Look for CCube
	int foundCube= tmp.Find ( "CCube(" );

	// Look for CCylinder
	int foundCylinder= tmp.Find ( "CCylinder(" );

	// Look for CSphere
	int foundSphere= tmp.Find ( "CSphere(" );

	// Look for CComposite
	int foundComposite= tmp.Find ( "CComposite(" );

	// Look for CPolygon
	int foundPolygon= tmp.Find ( "CPolygon(" );

	// Look for CTorus
	int foundTorus= tmp.Find ( "CTorus(" );

	if ( foundCylinder != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundCylinder );
		
		// Save the type of shape
		*type= Cylinder;

		// Add shape to list
		return true;
	}

	if ( foundCube != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundCube );
		
		// Save the type of shape
		*type= Cube;

		// Add shape to list
		return true;
	}

	if ( foundSphere != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundSphere );
		
		// Save the type of shape
		*type= Sphere;

		// Add shape to list
		return true;
	}

	if ( foundComposite != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundComposite );
		
		// Save the type of shape
		*type= Composite;

		// Add shape to list
		return true;
	}

	if ( foundTorus != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundTorus );
		
		// Save the type of shape
		*type= Torus;

		// Add shape to list
		return true;
	}

	if ( foundPolygon != -1 )
	{
		// Get the name of the shape
		GetShapeIdentifier ( line, identifier, foundPolygon );
		
		// Save the type of shape
		*type= Polygon;

		// Save number of vetices parameter
		int i= foundPolygon + 9 /* chars in "CPolygon(" */;
		CString str= "";

		// Read first char
		char c= tmp[i];

		// Continue whilst still numeric
		while ( isdigit ( c ) && i < tmp.GetLength () )
		{
			str.AppendChar ( c );

			i++;
			c= tmp[i];
		}

		// Add to parameter list
		params->push_back ( str );

		// Add shape to list
		return true;
	}

	// no shapes found
	return false;
}

// Get a substring a..b of in, leave in out
void CAbstractModelCodeToDocument::GetSubstring ( CString* in, CString* out, int a, int b )
{
	*out= "";

	for ( int i= a; i <= b; i++ )
	{
		out->AppendChar ( (*in)[i] );
	}
}

// Check that the string is a valid float literal
bool CAbstractModelCodeToDocument::IsFloatLiteral ( CString* in )
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

// From a line of code, get the operation name, if any, being applied to the specified object
// a give a list of its parameters. True if successfull.
bool CAbstractModelCodeToDocument::GetOperation ( CString* identifier,					// in
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
			errorText.AppendFormat ( "Line %d: Error: Cannot find end of parameter list.\n", lineNumber );
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
		int curPos= 0;

		// Get first token
		token= parametersString.Tokenize(",",curPos);

		while (token != "")
		{
			// Add cur token to list
			parameters->push_back ( token );

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

// Get the transform associated with the given shape identifier in the line of code specified
void CAbstractModelCodeToDocument::GetShapeTransformLine ( hduMatrix* transform, CString* identifier, CString* line )
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
		if ( operation == "Scale" )
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
				hduMatrix scaling= hduMatrix::createScale ( scale[0], scale[1], scale[2] );

				// Multiply this onto the shapes transformation matrix
				transform->multRight ( scaling );
			}
		}
		else if ( operation == "Rotate" )
		{
			// Rotation parameters
			float rotation[3];

			// Check number of parameters
			if ( parameters.size () == 3 )
			{

				// If parameter is a literal float		// Paramter *1
				if ( IsFloatLiteral ( &parameters.at(0) ) )
				{
					// convert it to float
					sscanf ( parameters.at (0), "%f", &rotation[0] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), &rotation[0] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// If parameter is a literal float		// Paramter *2
				if ( IsFloatLiteral ( &parameters.at(1) ) )
				{
					// convert it to float
					sscanf ( parameters.at (1), "%f", &rotation [1] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (1), &rotation[1] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// If parameter is a literal float		// Paramter *3
				if ( IsFloatLiteral ( &parameters.at(2) ) )
				{
					// convert it to float
					sscanf ( parameters.at (2), "%f", &rotation [2] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (2), &rotation[2] ) )
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
				hduMatrix rotationX= hduMatrix::createRotation ( 1, 0, 0, rotation[0] ); // x
				hduMatrix rotationY= hduMatrix::createRotation ( 0, 1, 0, rotation[1] ); // y
				hduMatrix rotationZ= hduMatrix::createRotation ( 0, 0, 1, rotation[2] ); // z

				// Combined rotation (in AM order x->y->z)
				hduMatrix rotationMatrix;
				rotationMatrix.multRight ( rotationX ); // x
				rotationMatrix.multRight ( rotationY ); // y
				rotationMatrix.multRight ( rotationZ ); // z

				// Multiply this onto the shapes transformation matrix
				transform->multRight ( rotationMatrix );
			}
		}
		else if ( operation == "Translate" )
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
				hduMatrix translation= hduMatrix::createTranslation ( translate[0], translate[1], translate[2] );

				// Multiply this onto the shapes transformation matrix
				transform->multRight ( translation );
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

// Read across physical lines until ';'
void CAbstractModelCodeToDocument::GetLogicalLine ( CString* line, std::ifstream* inFile )
{
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

// Get the transform associated with the given shape identifier in the inFile
void CAbstractModelCodeToDocument::GetShapeTransform ( hduMatrix* transform, CString* identifier, std::ifstream* inFile )
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
	GetShapeTransformLine ( transform, identifier, &line );

	// While more data
	while ( inFile->good () && !error)
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetShapeTransformLine ( transform, identifier, &line );

	}
}

// Extracts the scale component from the transformation matrix
void CAbstractModelCodeToDocument::GetScaleFromMatrix ( hduMatrix* matrix, float* scale )
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

// Extracts the scale component from the transformation matrix
void CAbstractModelCodeToDocument::GetTranslationFromMatrix ( hduMatrix* matrix, float* scale )
{
	// Return the translation components
	scale[0]= (*matrix)[3][0];	// x
	scale[1]= (*matrix)[3][1];	// y
	scale[2]= (*matrix)[3][2];	// z
}

// Extracts the rotation matrix component from the transformation matrix
void CAbstractModelCodeToDocument::GetRotationFromMatrix ( hduMatrix* matrix, hduMatrix* rotation )
{
	matrix->getRotationMatrix ( *rotation );
}

// Get the color associated with the given shape identifier in the inFile
void CAbstractModelCodeToDocument::GetShapeColor ( float* rgb, CString* identifier, std::ifstream* inFile )
{
	// Go to start of file
	inFile->clear ( );
	inFile->seekg ( 0 );
	lineNumber= 0;	// Currently processing this line

	// Set defualt color
	rgb[0]= 1.0f;
	rgb[1]= 1.0f;
	rgb[2]= 1.0f;

	// Look at each line

	// Get first line
	//char line[maxLine];
	//inFile->getline ( line, maxLine );
	CString line;
	GetLogicalLine ( &line, inFile );

	// Process first line
	GetShapeColorLine ( rgb, identifier, &line );

	// While more data
	while ( inFile->good () && !error)
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetShapeColorLine ( rgb, identifier, &line );
	}
}

// Get the color associated with the given shape identifier in the string
void CAbstractModelCodeToDocument::GetShapeColorLine ( float* rgb, CString* identifier, CString* line )
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

		// if found SetColor function
		if ( operation == "SetColor" )
		{
			// Check number of arguments
			if ( parameters.size () == 3 )
			{
				// First argument, Red component
				// .............................
				
				// Check parameter value
				if ( IsFloatLiteral ( &parameters.at ( 0 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 0 ), "%f", &rgb [0] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), &rgb[0] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// First argument, Green component
				// ...............................
				
				// Check parameter value
				if ( IsFloatLiteral ( &parameters.at ( 1 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 1 ), "%f", &rgb [1] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (1), &rgb[1] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// First argument, Blue component
				// ..............................
				
				// Check parameter value
				if ( IsFloatLiteral ( &parameters.at ( 2 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 2 ), "%f", &rgb [2] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (2), &rgb[2] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}
	}
}

// Get the haptic properties associated with the given shape identifier in the string
void CAbstractModelCodeToDocument::GetShapeHapticsLine ( float* stiffness,
														 float* staticFriction,
														 float* dynamicFriction,
														 float* damping,
														 CString* identifier, CString* line )
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

		// if found SetStiffness function
		if ( operation == "SetStiffness" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// Check parameter value
				if ( IsFloatLiteral ( &parameters.at ( 0 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 0 ), "%f", stiffness );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), stiffness ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// if found SetStaticFriction function
		if ( operation == "SetStaticFriction" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// Check parameter value
				if ( IsFloatLiteral ( &parameters.at ( 0 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 0 ), "%f", staticFriction );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), staticFriction ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// if found SetDynamicFriction function
		if ( operation == "SetDynamicFriction" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// Check parameter value
				if ( IsFloatLiteral ( &parameters.at ( 0 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 0 ), "%f", dynamicFriction );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), dynamicFriction ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}

		// if found SetDamping Friction function
		if ( operation == "SetDamping" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// Check parameter value
				if ( IsFloatLiteral ( &parameters.at ( 0 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 0 ), "%f", damping );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), damping ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}
	}
}

// Get the torus radius associated with the given shape identifier in the inFile
void CAbstractModelCodeToDocument::GetTorusRadius ( float* radius,
					  CString* identifier, std::ifstream* inFile )
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
	GetTorusRadiusLine ( radius, identifier, &line );

	// While more data
	while ( inFile->good () && !error)
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetTorusRadiusLine ( radius, identifier, &line );
	}
}

// Get the triangle vertices associated with the given shape identifier in the line
void CAbstractModelCodeToDocument::GetTorusRadiusLine ( float* radius,
					      CString* identifier, CString* line )
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

		// if found SetStiffness function
		if ( operation == "SetRadius" )
		{
			// Check number of arguments
			if ( parameters.size () == 1 )
			{
				// Check parameter value
				if ( IsFloatLiteral ( &parameters.at ( 0 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 0 ), "%f", radius );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), radius ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}
	}
}

// Get the triangle vertices associated with the given shape identifier in the inFile
void CAbstractModelCodeToDocument::GetTriangleVertices ( std::vector<float*>* verts,
					       CString* identifier, std::ifstream* inFile )
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
	GetTriangleVerticesLine ( verts, identifier, &line );

	// While more data
	while ( inFile->good () && !error)
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetTriangleVerticesLine ( verts, identifier, &line );
	}
}

// Get the triangle vertices associated with the given shape identifier in the line
void CAbstractModelCodeToDocument::GetTriangleVerticesLine ( std::vector<float*>* verts,
					           CString* identifier, CString* line )
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

		// if found SetStiffness function
		if ( operation == "AddVertex" )
		{
			// Check number of arguments
			if ( parameters.size () == 3 )
			{
				float *v= new float[3];

				// Check parameter value	Vertex A
				if ( IsFloatLiteral ( &parameters.at ( 0 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 0 ), "%f", &v[0] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (0), &v[0] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// Check parameter value	Vetex B
				if ( IsFloatLiteral ( &parameters.at ( 1 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 1 ), "%f", &v[1] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (1), &v[1] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// Check parameter value	Vertex C
				if ( IsFloatLiteral ( &parameters.at ( 2 ) ) )
				{
					// convert it to float
					sscanf ( parameters.at ( 2 ), "%f", &v[2] );
				}
				else
				{
					// Try to evaluate expression
					if ( !EvaluateFloatExpression ( &parameters.at (2), &v[2] ) )
					{
						// error, don't understand parameter
						parameterValueError= true;
					}
				}

				// If read ok
				if ( !parameterValueError )
				{
					// Add to vertex list
					verts->push_back ( v );
				}
			}
			else
			{
				// Error, wrong number of parameters for function
				parameterError= true;
			}
		}
	}
}

// Get the haptic properties associated with the given shape identifier in the inFile
void CAbstractModelCodeToDocument::GetShapeHaptics ( float* stiffness,
					   float* staticFriction,
					   float* dynamicFriction,
					   float* damping,
					   CString* identifier, std::ifstream* inFile )
{
	// Go to start of file
	inFile->clear ( );
	inFile->seekg ( 0 );
	lineNumber= 0;	// Currently processing this line

	// Set defaults
	*stiffness= 0.5f;
	*staticFriction= 0.0f;
	*dynamicFriction= 0.0f;
	*damping= 0.0f;

	// Look at each line

	// Get first line
	//char line[maxLine];
	//inFile->getline ( line, maxLine );
	CString line;
	GetLogicalLine ( &line, inFile );

	// Process first line
	GetShapeHapticsLine ( stiffness, staticFriction, dynamicFriction, damping, identifier, &line );

	// While more data
	while ( inFile->good () && !error)
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetShapeHapticsLine ( stiffness, staticFriction, dynamicFriction, damping, identifier, &line );
	}
}

// Returns true iff the specified shape identifier is ever added to the model in file inFile
bool CAbstractModelCodeToDocument::IsAddedToModel ( CString* identifier, std::ifstream* inFile )
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
	if ( IsAddedToModelLine ( identifier, &line ) )
	{
		return true;
	}

	// While more data
	while ( inFile->good () && !error)
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		if ( IsAddedToModelLine ( identifier, &line ) )
		{
			return true;
		}
	}

	// Not added to model anywhere in file
	return false;
}

// Returns true iff the specified shape identifier is ever added to the model in string line
bool CAbstractModelCodeToDocument::IsAddedToModelLine ( CString* identifier, CString* line )
{
	// Copy string
	CString tmp= *line;

	// Remove white space
	tmp.Replace ( " ", "" );
	tmp.Replace ( "	", "" );

	return ( tmp.Find ( "AddModelItem(" + *identifier + ")" ) != -1 );
}

// Gets a list of sub-shape identifiers for the specified composite shape identifier, in file inFile
void CAbstractModelCodeToDocument::GetMemberShapes ( CString* compositeIdent, 
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
	while ( inFile->good () && !error)
	{
		// Get next line
		GetLogicalLine ( &line, inFile );

		// Process line 
		GetMemberShapesLine ( compositeIdent, memberIdents, &line );
	}
}

// Gets a list of sub-shape identifiers for the specified composite shape identifier, in string line
void CAbstractModelCodeToDocument::GetMemberShapesLine ( CString* compositeIdent, 
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
		if ( operation == "AddShape" )
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
bool CAbstractModelCodeToDocument::FindShapeIndex ( CString* identifier, int* index )
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

// Look for first variable in expression, returning index and true if found
bool CAbstractModelCodeToDocument::FindVariable ( CString* expression, CString* variable, int* index )
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
bool CAbstractModelCodeToDocument::EvaluateFloatExpression ( CString* expression, float* answer )
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

// Look-up or prompt user for value of variable
bool CAbstractModelCodeToDocument::GetVariableValue ( CString* identifier, CString* value )
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

// Reads the code in the specified file and adds appropriate objects to the
// existing specified document
void CAbstractModelCodeToDocument::BuildDocument ( CString filename, CProtoHapticDoc* doc )
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
	AbstractModelShapes type;
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
	while ( !inFile.eof () && !error)
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
		MessageBox ( 0, errorText, "Error Importing File", MB_OK );
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

		// For each line search for operations on "name"
		GetShapeTransform ( &transform, &name, &inFile );

		// Save the shapes transform
		transforms.push_back ( transform );

		// if its a composite its not populated yet
		populated.push_back ( false );
	}

	// Check for errors
	if ( error )
	{
		MessageBox ( 0, errorText, "Error Importing File", MB_OK );
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

		// List of vertices (for polygon)
		std::vector<float*> vertices;

		// Create new PH shape
		switch ( types.at ( i ) )
		{
		case Cube:
			shape= new CCube ( 1, 1, 1 );
			break;
		case Sphere:
			shape= new CSphere ( 1, 1, 1 );
			break;
		case Cylinder:
			shape= new CCylinder ( 1, 1, 1 );
			break;
		case Torus:
			shape= new CTorus ( );

			// Get radius of torus
			float radius;
			GetTorusRadius ( &radius, &name, &inFile );

			// Set radius of torus
			((CTorus*)shape)->setRadius ( radius );

			break;
		case Polygon:
			// Get number of verts
			int verts;
			sscanf ( parameters.at ( i ).at ( 0 ), "%d", &verts );

			// Get triangle vertices
			GetTriangleVertices ( &vertices, &name, &inFile );

			// Validate number of verts
			if ( vertices.size() == 3 )
			{
				// Represent as a single triangle
				shape= new CTriangle ( );

				// Set triangle vertices
				((CTriangle*)shape)->SetVertexA ( vertices.at ( 0 )[0],
												  vertices.at ( 0 )[1],
												  vertices.at ( 0 )[2] );
				((CTriangle*)shape)->SetVertexB ( vertices.at ( 1 )[0],
												  vertices.at ( 1 )[1],
												  vertices.at ( 1 )[2] );
				((CTriangle*)shape)->SetVertexC ( vertices.at ( 2 )[0],
												  vertices.at ( 2 )[1],
												  vertices.at ( 2 )[2] );
			}
			else if ( vertices.size () == 4 )
			{
				// Represent as composite of two triangles
				shape= new CComposite ( );

				// Triangle 1
				CTriangle* tri1= new CTriangle ();
				
				// Set triangle vertices
				tri1->SetVertexA ( vertices.at ( 0 )[0],
								   vertices.at ( 0 )[1],
								   vertices.at ( 0 )[2] );
				tri1->SetVertexB ( vertices.at ( 1 )[0],
								   vertices.at ( 1 )[1],
								   vertices.at ( 1 )[2] );
				tri1->SetVertexC ( vertices.at ( 2 )[0],
								   vertices.at ( 2 )[1],
								   vertices.at ( 2 )[2] );

				// Triangle 2
				CTriangle* tri2= new CTriangle ();

				// Set triangle vertices
				tri2->SetVertexA ( vertices.at ( 0 )[0],
								   vertices.at ( 0 )[1],
								   vertices.at ( 0 )[2] );
				tri2->SetVertexB ( vertices.at ( 2 )[0],
								   vertices.at ( 2 )[1],
								   vertices.at ( 2 )[2] );
				tri2->SetVertexC ( vertices.at ( 3 )[0],
								   vertices.at ( 3 )[1],
								   vertices.at ( 3 )[2] );

				// Compose Triangles
				((CComposite*)shape)->addShape ( tri1 );
				((CComposite*)shape)->addShape ( tri2 );
			}
			else
			{
				// Error, wrong number of vertices
				error= true;
				errorText.AppendFormat ( "Error: Unsupported number of polygon vertices\n" );
			}

			break;
		case Composite:
			shape= new CComposite ( );

			// Record that the composite has yet to be populated
			populated.at ( i )= false;

			break;
		}

		shape->SetName ( name );

		// If not a composite
		//if (shape->getType () != SHAPE_COMPOSITE )
		//{
			// Apply the shapes transforms
			// ...........................

			// Set scaling
			float scale[3];

			// Extract scale component
			GetScaleFromMatrix ( &transforms.at ( i ), scale );

			//char s[200];
			//sprintf ( s, "%f, %f, %f", scale[0], scale[1], scale[2] );
			//MessageBox ( 0, s, "", MB_OK );

			// Set scale of PH shape

			// PH spheres are half the size of AM spheres
			if ( types.at ( i ) == Sphere )
			{
				// Double the scaling factor for spheres
				shape->setSize ( scale[0]*2.0f, scale[1]*2.0f, scale[2]*2.0f );
			}
			else
			{
				// Not a sphere, use normal scaling factor
				shape->setSize ( scale[0], scale[1], scale[2] );
			}

			// Set translation
			float translate[3];

			// Extract translation component from matrix
			GetTranslationFromMatrix ( &transforms.at ( i ), translate );

			// Set the position of the PH shape
			shape->setLocation ( translate[0], translate[1], translate[2] );

			// Set rotation
			hduMatrix rotation;
			
			// Extract the rotation component
			GetRotationFromMatrix ( &transforms.at ( i ), &rotation );

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

			// Set visual properties of shape
			// ..............................

			// Get color of AM shape
			float rgb[3];
			GetShapeColor ( rgb, &identifiers.at ( i ), &inFile );

			// Set color of PH shape
			shape->setColour ( rgb[0], rgb[1], rgb[2] );

			// Set haptic properties of shape
			// ..............................

			// Get haptic properties of AM shape
			float stiffness;
			float staticFriction;
			float dynamicFriction;
			float damping;

			GetShapeHaptics ( &stiffness, &staticFriction, &dynamicFriction, &damping,
							  &identifiers.at ( i ), &inFile );

			// Set haptic properties of PH shape
			shape->setStiffness ( stiffness );
			shape->setStaticFriction ( staticFriction );
			shape->setDynamicFriction ( dynamicFriction );
			shape->setDampening ( damping );
		//}	// if not a composite

		// If shape should be added to the model
		if ( IsAddedToModel ( &identifiers.at ( i ), &inFile ) && shape->getType() != SHAPE_COMPOSITE )
		{
			// Add the shape to the model
			doc->AddShape ( shape );
			doc->SetModifiedFlag ( ); // Doc has changed
		}

		// Above
		// Save a ptr to the shape for later
		shapes.push_back ( shape );
		
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
			if ( IsAddedToModel ( &identifiers.at ( i ), &inFile ) )
			{
				// Add the shape to the model
				doc->AddShape ( shape );
				doc->SetModifiedFlag ( ); // Doc has changed
			}
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

// Recursivly populate the specified composite, so that only complete composites are added to a composite
void CAbstractModelCodeToDocument::PopulateComposite ( int compIndex, std::ifstream* inFile )
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
					MessageBox(0,"Error","",MB_OK);
				}

				//MessageBox ( 0, "Adding to " + identifiers.at ( i ) + " subshape " + members.at ( j ), "", MB_OK );

				// Calculate the combined transformation of individual shape and composite
				// Then apply this to the PH member shape

			}
			else
			{
				// Error, cant find shape in list
				error= true;
				errorText.AppendFormat ( "Internal Error: Lookup of composite shape %s failed\n", members.at ( j ) );
			}
		}

		// Record that the shape is now fully populated
		populated.at ( compIndex )= true;
	}
}


// Write variable names to file
void CAbstractModelCodeToDocument::SaveVariableDatabase ( CString filename )
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
void CAbstractModelCodeToDocument::LoadVariableDatabase ( CString filename )
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

	// Current name value pair
	char name[maxLine];
	char value[maxLine];

	// Read first line
	char line [maxLine];
	inFile.getline ( line, maxLine );

	// While more data
	while ( !inFile.eof () && !error)
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