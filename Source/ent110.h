///////////////////////////////////////////////////////////////////////////
// FILE: ent110.h
// DESCRIPTION: LINE ENTITY (TYPE 110) Class declaration
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// A line defined by a start and an end point.
// In this program used only as an axis for a surface of revolution
// or as it's generatrix
//////////////////////////////////////////////////////////////////////

#pragma once
#include "entity.h"

class ent110 :
	public entity
{
public:
	//Default constructor
	ent110(void);
	//Destructor
	virtual ~ent110(void);
	//Constructor
	ent110(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) {}
	//Process after reading data from file
	void process();
	//Perform drawing related functions (if necessary)
	void draw();
	//Returns data from this entity for other entities to use
	void* returnData(int what);
	
public:
	Vector4<GLfloat> P1;	//Line start point
	Vector4<GLfloat> P2;	//Line end point
	
};
