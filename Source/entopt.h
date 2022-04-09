// OPTIONS ENTITY (DOES NOT BELONG TO IGES STANDARD)
// This is a helper class to keep a set of options that other entities can access
// in order to update themselves
//
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//////////////////////////////////////////////////////////////////////
#pragma once
#include "entity.h"

class entopt :
	public entity
{
public:
	//Default Constructor
	entopt(void);
	//Constructor
	entopt(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n)
	{
		mixedMethod=TRUE;
		trimTol=0.001f;
	}
	//Destructor
	virtual ~entopt(void);
	//Returns data from this entity for other entities to use
	//Input: int what
	//	0	return mixedMethod
	//	1	return the trimming tolerance
	void* returnData(int what)
	{
		switch(what)
		{
		case 0:
			return &mixedMethod;
			break;
		case 1:
			return &trimTol;
			break;
		}

		return NULL;

	}
	//Allows other entities to set data in this entity
	//void* what		The data itself
	//int option		What we want to change
	//	0		change mixedMethod
	//	1		change the trimming tolerance
	void setData(void* what, int option)
	{
		if( option == 0 )
			mixedMethod=*((BOOL*)what);
		if( option == 1 )
			trimTol=*((float*)what);
	}


public:
	BOOL mixedMethod;   //True if we want to use parametric error for lower degree surfaces
						//and path length for higher degree surfaces
	float trimTol;		//Trimming tolerance. To fix trimlines that don't join well
};
