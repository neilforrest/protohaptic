// ProtoHaptic
// ===========
// Author: Neil Forrest, 2006

// File: UsefullFunctions.h
// Classes: ----
// Purpose: A collection of useful functions

#include "stdafx.h"
#include "Shape.h"

/** Returns the distance along the x axis between the centres of two shapes */
float distX(CShape *s, CShape *s1);

/** Returns the distance along the y axis between the centres of two shapes */
float distY(CShape *s, CShape *s1);

/** Returns the distance along the z axis between the centres of two shapes */
float distZ(CShape *s, CShape *s1);

/** Returns the magnitude of f */
float positive(float f);
