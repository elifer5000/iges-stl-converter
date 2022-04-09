///////////////////////////////////////////////////////////////////////////
// FILE: ent102.h
// DESCRIPTION: COMPOSITE CURVE ENTITY (TYPE 102) Class declaration
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// A composite curve defined by N curves whose index is given.
// For trimmed surfaces (which is what's supported in this program)
// the composite curve (the trimming line) must be closed, so checks
// are done in the draw function to correct unclosed lines
//////////////////////////////////////////////////////////////////////
#pragma once
#include "entity.h"

class ent102 :
	public entity
{
public:
	//Default constructor
	ent102(void);
	//Destructor
	virtual ~ent102(void);
	//Constructor
	ent102(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) { }
	//Process after reading data from file
	void process();
	//Perform drawing related functions (if necessary)
	void draw(int what);
	//Returns data from this entity for other entities to use
	void* returnData(int what);
public:
	int N;				//Number of entities in curve
	vector<int> pToDE;	//Index of the DE of the curves
	BOOL doOnce;		//Checks that we do some corrections only once (look in code)
};
