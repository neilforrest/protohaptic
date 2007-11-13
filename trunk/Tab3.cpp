// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: Tab1.cpp
// Classes: CTab1
// Purpose: A tab in the shape properties dialog

#include "stdafx.h"
#include "ProtoHaptic.h"
#include "Tab3.h"
#include <HDU/hduMatrix.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTab3 dialog


CTab3::CTab3(CWnd* pParent /*=NULL*/)
	: CDialog(CTab3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTab3)
	//}}AFX_DATA_INIT

	m_doc= NULL;
	m_shape= NULL;

	// Shape properties at last call to ShapeChanged
	// Initialise to defaults
	lastPos[0]= 0;
	lastPos[1]= 0;
	lastPos[2]= 0;

	lastSize[0]= 1;
	lastSize[1]= 1;
	lastSize[2]= 1;

	for ( int i= 0; i < 16; i++ )
	{
		lastRot[i]= -1;
	}

	shapeSet= false;
}

// Returns true if the shapes properties have changed since last call
bool CTab3::ShapeChanged ( )
{
	// Current shape is not set
	if ( m_shape == NULL )
	{
		// True if last shape was set
		bool tmp= shapeSet;

		// Update shapeSet
		shapeSet= false;

		// Return old shapeSet
		return tmp;
	}
	// m_shape is != NULL

	// Compare to last values
	bool same= true;

	same&= m_shape->getLocationX () == lastPos[0];
	same&= m_shape->getLocationY () == lastPos[1];
	same&= m_shape->getLocationZ () == lastPos[2];

	same&= m_shape->getSizeX () == lastSize[0];
	same&= m_shape->getSizeY () == lastSize[1];
	same&= m_shape->getSizeZ () == lastSize[2];

	for ( int i= 0; i < 16; i++ )
	{
		same&= m_shape->getRotation ()[i] == lastRot[i];
	}

	same&= shapeSet;

	// Save current values
	lastPos[0]= m_shape->getLocationX ();
	lastPos[1]= m_shape->getLocationY ();
	lastPos[2]= m_shape->getLocationZ ();

	lastSize[0]= m_shape->getSizeX ();
	lastSize[1]= m_shape->getSizeY ();
	lastSize[2]= m_shape->getSizeZ ();

	for ( int i= 0; i < 16; i++ )
	{
		lastRot[i]= m_shape->getRotation () [i];
	}

	shapeSet= true;

	// Return answer
	return !same;
}

void CTab3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTab3)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_POSX, m_posX);
	DDX_Control(pDX, IDC_POSY, m_posY);
	DDX_Control(pDX, IDC_POSZ, m_posZ);
	DDX_Control(pDX, IDC_SIZEX, m_sizeX);
	DDX_Control(pDX, IDC_SIZEY, m_sizeY);
	DDX_Control(pDX, IDC_SIZEZ, m_sizeZ);
	DDX_Control(pDX, IDC_ROTX, m_rotX);
	DDX_Control(pDX, IDC_ROTY, m_rotY);
	DDX_Control(pDX, IDC_ROTZ, m_rotZ);
	DDX_Control(pDX, IDC_BUTTON1, m_applyPos);
	DDX_Control(pDX, IDC_BUTTON3, m_applySize);
	DDX_Control(pDX, IDC_BUTTON4, m_applyRot);
}

// Convert string to float
bool CTab3::StringToFloat ( char* str, float* flt )
{
	// Count the number of digits before the d.p.
	int prefixCount= 0;

	// Length of whole string
	int strLength= strlen ( str );

	// Initialise answer to 0
	(*flt)= 0;

	bool reverseSign= false;
	int readFrom= 0;

	// Check for minus sign
	if ( strLength >= 1 && str[0] == '-' )
	{
		reverseSign= true;
		readFrom= 1;
	}

	// For each char in str
	for ( int i= readFrom; i < strLength; i++ )
	{
		// If char is not the d.p.
		if ( str[i] != '.' )
		{
			// Icr count
			prefixCount++;
		}
		else
		{
			// char is d.p. finnished counting
			break;
		}
	}

	// Encountered dp. yet?
	bool decimal= false;

	// Current power of 10
	int power= prefixCount-1;

	// For each char in str
	for ( int i= readFrom; i < strLength; i++ )
	{
		// If char is valid
		if ( isdigit ( str[i] ) || // Char is a number, OR
			((!decimal)&&(str[i]=='.')) )// Decimal not encountered AND char is decimal
		{
			// If char is decimal, record this
			if ( str[i] == '.' )
			{
				decimal= true;
			}
			else	// char is a digit
			{
				// Int value of char
				int digit= str[i] - '0';

				// Add to the answer
				(*flt)+= (float) ( ((float)digit) * pow ( (double)10, power ) );

				// decriment the power of 10
				power--;
			}
		}
		else // invalid char
		{
			return false;
		}
	}

	// reverse sign
	if ( reverseSign )
	{
		(*flt)= 0 - (*flt);
	}

	// Success
	return true;
}

BEGIN_MESSAGE_MAP(CTab3, CDialog)
	//{{AFX_MSG_MAP(CTab3)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTab3::OnBnClickedApplyPosition)
	ON_BN_CLICKED(IDC_BUTTON3, &CTab3::OnBnClickedSizeApply)
	ON_BN_CLICKED(IDC_BUTTON4, &CTab3::OnBnClickedRotationApply)
END_MESSAGE_MAP()

#define PI 3.14159265

// Get Euler angles from rotation matrix
void CTab3::GetEulerFromMatrix ( float* matrix, float *x, float *y, float *z )
{
    // Assuming the angles are in radians.
	if (matrix[4] > 0.998) { // singularity at north pole
		*y = atan2(matrix[2],matrix[10]);
		*x = PI/2.0;
		*z = 0;
		return;
	}
	if (matrix[4] < -0.998) { // singularity at south pole
		*y= atan2(matrix[2],matrix[10]);
		*x = -PI/2.0;
		*z = 0;
		return;
	}
	*y = atan2(-matrix[8],matrix[0]);
	*z = atan2(-matrix[6],matrix[5]);
	*x = asin(matrix[4]);
}

#define RAD_2_DEG ((float)360.0f/(2.0f*((float)PI)))

// Update edit boxes from shape properties
void CTab3::UpdateEditBoxes ()
{
	// If shape is set
	if ( m_shape != NULL )
	{
		// Enable edit boxes
		m_posX.EnableWindow ( TRUE );
		m_posY.EnableWindow ( TRUE );
		m_posZ.EnableWindow ( TRUE );
		m_sizeX.EnableWindow ( TRUE );
		m_sizeY.EnableWindow ( TRUE );
		m_sizeZ.EnableWindow ( TRUE );
		m_rotX.EnableWindow ( TRUE );
		m_rotY.EnableWindow ( TRUE );
		m_rotZ.EnableWindow ( TRUE );

		m_applyPos.EnableWindow ( TRUE );
		m_applySize.EnableWindow ( TRUE );
		m_applyRot.EnableWindow ( TRUE );

		CString str;

		str.Format ( "%.4f", m_shape->getLocationX () );
		m_posX.SetWindowText ( str );

		str.Format ( "%.4f", m_shape->getLocationY () );
		m_posY.SetWindowText ( str );

		str.Format ( "%.4f", m_shape->getLocationZ () );
		m_posZ.SetWindowText ( str );

		str.Format ( "%.4f", m_shape->getSizeX () );
		m_sizeX.SetWindowText ( str );

		str.Format ( "%.4f", m_shape->getSizeY () );
		m_sizeY.SetWindowText ( str );

		str.Format ( "%.4f", m_shape->getSizeZ () );
		m_sizeZ.SetWindowText ( str );

		// Convert array into angles

		// Rotation
		CString rotateStringX, rotateStringY, rotateStringZ;
		float rotate[3];		// rotation about x, y, z

		// shapes rotation matrix
		float* rotationArray= m_shape->getRotation ();

		GetEulerFromMatrix ( rotationArray, &rotate[0], &rotate[1], &rotate[2] );

		rotateStringY.Format ( "%.4f", -rotate[1]*RAD_2_DEG );
		rotateStringZ.Format ( "%.4f", -rotate[0]*RAD_2_DEG );
		rotateStringX.Format ( "%.4f", -rotate[2]*RAD_2_DEG );

		m_rotX.SetWindowText ( rotateStringX );
		m_rotY.SetWindowText ( rotateStringY );
		m_rotZ.SetWindowText ( rotateStringZ );
	}
	else
	{
		// Clear and disable edit boxes
		m_posX.SetWindowText ( "" );
		m_posY.SetWindowText ( "" );
		m_posZ.SetWindowText ( "" );
		m_sizeX.SetWindowText ( "" );
		m_sizeY.SetWindowText ( "" );
		m_sizeZ.SetWindowText ( "" );
		m_rotX.SetWindowText ( "" );
		m_rotY.SetWindowText ( "" );
		m_rotZ.SetWindowText ( "" );

		m_posX.EnableWindow ( FALSE );
		m_posY.EnableWindow ( FALSE );
		m_posZ.EnableWindow ( FALSE );
		m_sizeX.EnableWindow ( FALSE );
		m_sizeY.EnableWindow ( FALSE );
		m_sizeZ.EnableWindow ( FALSE );
		m_rotX.EnableWindow ( FALSE );
		m_rotY.EnableWindow ( FALSE );
		m_rotZ.EnableWindow ( FALSE );

		m_applyPos.EnableWindow ( FALSE );
		m_applySize.EnableWindow ( FALSE );
		m_applyRot.EnableWindow ( FALSE );
	}
}

void CTab3::setShape(CShape *s)
{
	m_shape= s;

	// If shape is different
	if ( ShapeChanged () )
	{
		// Update edit boxes from shape properties
		UpdateEditBoxes ( );
	}
}	

/** Set the document in which the specified shape resides */
void CTab3::SetDocument ( CProtoHapticDoc* doc )
{
	m_doc= doc;
}

void CTab3::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_shape!=NULL)
	{
		
	} 
	else
	{
		
	}
	// Do not call CDialog::OnPaint() for painting messages
}

void CTab3::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTab3::OnBnClickedApplyPosition()
{
	// TODO: Add your control notification handler code here
	if ( m_shape == NULL )
	{
		return;
	}

	// Get strings from edits
	char strX[256];
	m_posX.GetWindowText ( strX, 256 );

	char strY[256];
	m_posY.GetWindowText ( strY, 256 );

	char strZ[256];
	m_posZ.GetWindowText ( strZ, 256 );

	// COnvert to float
	float answerX, answerY, answerZ;

	// If successfull
	if ( StringToFloat ( strX, &answerX ) &&
		 StringToFloat ( strY, &answerY ) && 
		 StringToFloat ( strZ, &answerZ ) )
	{
		// Save document for undo
		if ( m_doc != NULL )
		{
			m_doc->saveState ( );
		}

		// Set the shape
		m_shape->setLocation ( answerX, answerY, answerZ );
	}
	else
	{
		MessageBox ( "Please enter a number for the x, y and z fields." );
	}
}

void CTab3::OnBnClickedSizeApply()
{
	// TODO: Add your control notification handler code here
	if ( m_shape == NULL )
	{
		return;
	}

	// Get strings from edits
	char strX[256];
	m_sizeX.GetWindowText ( strX, 256 );

	char strY[256];
	m_sizeY.GetWindowText ( strY, 256 );

	char strZ[256];
	m_sizeZ.GetWindowText ( strZ, 256 );

	// COnvert to float
	float answerX, answerY, answerZ;

	// If successfull
	if ( StringToFloat ( strX, &answerX ) &&
		 StringToFloat ( strY, &answerY ) && 
		 StringToFloat ( strZ, &answerZ ) )
	{
		// Save document for undo
		if ( m_doc != NULL )
		{
			m_doc->saveState ( );
		}

		// Set the shape
		m_shape->setSize ( answerX, answerY, answerZ );
	}
	else
	{
		MessageBox ( "Please enter a number for the x, y and z fields." );
	}
}

void CTab3::OnBnClickedRotationApply()
{
	// TODO: Add your control notification handler code here
	if ( m_shape == NULL )
	{
		return;
	}

	// Get strings from edits
	char strX[256];
	m_rotX.GetWindowText ( strX, 256 );

	char strY[256];
	m_rotY.GetWindowText ( strY, 256 );

	char strZ[256];
	m_rotZ.GetWindowText ( strZ, 256 );

	// COnvert to float
	float answerX, answerY, answerZ;

	// If successfull
	if ( StringToFloat ( strX, &answerX ) &&
		 StringToFloat ( strY, &answerY ) && 
		 StringToFloat ( strZ, &answerZ ) )
	{
		// Create a rotation matrix
		hduMatrix m;

		// Apply the rotation matrix
		m.multRight ( hduMatrix::createRotation ( 0, 1, 0, answerY/RAD_2_DEG ) );
		m.multRight ( hduMatrix::createRotation ( 0, 0, 1, answerZ/RAD_2_DEG ) );
		m.multRight ( hduMatrix::createRotation ( 1, 0, 0, answerX/RAD_2_DEG ) );

		// Convert to 16 element array
		float rotation[16]=
		{
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		};

		// Save document for undo
		if ( m_doc != NULL )
		{
			m_doc->saveState ( );
		}

		// Set rotation
		m_shape->setRotation ( rotation );
	}
	else
	{
		MessageBox ( "Please enter a number for the x, y and z fields." );
	}
}

BOOL CTab3::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateEditBoxes ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
