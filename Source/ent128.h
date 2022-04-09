///////////////////////////////////////////////////////////////////////////
// FILE: ent128.h
// DESCRIPTION: RATIONAL B-SPLINE SURFACE ENTITY (TYPE 128) Class declaration
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// Defines a b-spline surface to be used for a trimmed surface.
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "entity.h"

class ent128 :
	public entity
{
public:
	//Default constructor
	ent128(void);
	//Constructor
	ent128(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) {}
	//Process after reading data from file
	void process();
	//Perform drawing related functions
	void draw();
	//Perform postdrawing operations
	void endDraw();
	//Destructor
	virtual ~ent128(void);

public:
	//K1 - Upper index of first sum		K2 - Upper index of second sum
	//M1 - Degree of first basis set	M2 - Degree of second basis set
	//PROPS - informational only (see IGES spec)
	//A,B are for intermediate calculations only
	//C is total number of control points in surface
	int A,B,C,K1,K2,M1,M2,PROP1,PROP2,PROP3,PROP4,PROP5;
	float U0,U1,V0,V1;				//Starting and end values
	Vector4<GLfloat> *ctrlPoints;	//Control points
	GLfloat *sKnots;				//First knot sequence
	GLfloat *tKnots;				//Second knot sequence
	
	
};
