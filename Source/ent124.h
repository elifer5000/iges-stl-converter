///////////////////////////////////////////////////////////////////////////
// FILE: ent124.h
// DESCRIPTION: TRANSFORMATION MATRIX ENTITY (TYPE 124) Class declaration
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// Defined by a rotation and a translation matrix
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "entity.h"

class ent124 :
	public entity
{
public:
	//Default constructor
	ent124(void);
	//Destructor
	virtual ~ent124(void);
	//Constructor
	ent124(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) {}
	//Process after reading data from file
	void process();
	//Perform drawing related functions
	void draw();
	//Returns data from this entity for other entities to use
	void* returnData(int what);
	
public:
	Matrix3<float> rotM;	//3x3 Rotation Matrix
	Vector3<float> transV;	//3x1 Translation Vector

};
