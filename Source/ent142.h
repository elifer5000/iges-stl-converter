///////////////////////////////////////////////////////////////////////////
// FILE: ent142.h
// DESCRIPTION: CURVE ON A PARAMETRIC SURFACE ENTITY (TYPE 142) Class declaration
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// A curve that's defined in the parametric space of a surface
///////////////////////////////////////////////////////////////////////////

#pragma once
#include "entity.h"

class ent142 :
	public entity
{
public:
	//Default constructor
	ent142(void);
	//Destructor
	virtual ~ent142(void);
	//Constructor
	ent142(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) {}
	//Process after reading data from file
	void process();
	//Perform drawing related functions
	void draw(int what);

public:
	int CRTN;	//Creation method of the curve
	int SPTR;	//Pointer to surface where curve lies
	int BPTR;	//Pointer to curve B in parametric space of surface S
	int CPTR;	//Pointer to curve C
	int PREF;	//PREF=1 B is preferred
				//PREF=2 C is preferred
				//PREF=3 B and C equally preferred
				//PREF=0 unspecified
	
};
