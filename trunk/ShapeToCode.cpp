#include "stdafx.h"
#include "ShapeToCode.h"

CShapeToCode::CShapeToCode(void)
{
	modelScale= 1.0;
}

CShapeToCode::~CShapeToCode(void)
{
}

// Amount by which to scale the model before output
void CShapeToCode::SetScale ( float s )
{
	modelScale= s;
}