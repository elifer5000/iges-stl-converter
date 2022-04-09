///////////////////////////////////////////////////////////////////////////
// FILE: ent126.h
// DESCRIPTION: RATIONAL B-SPLINE CURVE ENTITY (TYPE 126) Class declaration
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// Defines a b-spline curve to be used as a trimming line for a trimmed
// surface.
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "entity.h"

class ent126 :
	public entity
{
public:
	//Default constructor
	ent126(void);
	//Constructor
	ent126(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) {}
	//Process after reading data from file
	void process();
	//Perform drawing related functions
	void draw(int what);
	//Returns data from this entity for other entities to use
	void* returnData(int what);
	//Destructor
	virtual ~ent126(void);

public:
	// K - Upper index of sum	M - Degree of basis
	// PROPS are informational only (see IGES spec)
	// A,N are for intermediate calculations only
	int K,M,A,N,PROP1,PROP2,PROP3,PROP4;
	float V0,V1;	//Starting and end value
	Vector4<GLfloat> *ctrlPoints;	//Control points
	Vector3<GLfloat> *paramCtrlPoints;  //Parametric (u,v) data is taken from x,y components
	
	//In revolution surfaces the parameters are in reversed order: u,v->v,u
	//This is only because the definition of a rev surf in this program
	//is different from the Iges definition
	Vector3<GLfloat> *revParamCtrlPoints;
										  	
	Vector3<GLfloat> normal;	//Normal to the curve (if curve is planar)
	GLfloat *Knots;				//Knot Vector
	BOOL doOnce;				//Checks that we do some corrections only once (look in code)
};
