#include "stdafx.h"
#include "UsefullFunctions.h"

float positive(float f)
{
	if(f<0.0) return -f;
	else return f;
}

float distX(CShape* s, CShape* s1)
{
	float d= s->getLocationX()-s1->getLocationX();
	if (d<0)
		d= -d;
	return d;
}

float distY(CShape* s, CShape* s1)
{
	float d= s->getLocationY()-s1->getLocationY();
	if (d<0)
		d= -d;
	return d;
}

float distZ(CShape* s, CShape* s1)
{
	float d= s->getLocationZ()-s1->getLocationZ();
	if (d<0)
		d= -d;
	return d;
}