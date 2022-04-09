///////////////////////////////////////////////////////////////////////////
// FILE: ent124.cpp
// DESCRIPTION: TRANSFORMATION MATRIX ENTITY (TYPE 124) Class definition
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// Defined by a rotation and a translation matrix
///////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ent124.h"

//Default constructor
ent124::ent124(void)
{
}

//Destructor
ent124::~ent124(void)
{
}

//Process after reading data from file
void ent124::process()
{
	rotM.at(0,0)=refParam.getData()[0];
	rotM.at(1,0)=refParam.getData()[1];
	rotM.at(2,0)=refParam.getData()[2];
	transV.x=refParam.getData()[3];
	rotM.at(0,1)=refParam.getData()[4];
	rotM.at(1,1)=refParam.getData()[5];
	rotM.at(2,1)=refParam.getData()[6];
	transV.y=refParam.getData()[7];
	rotM.at(0,2)=refParam.getData()[8];
	rotM.at(1,2)=refParam.getData()[9];
	rotM.at(2,2)=refParam.getData()[10];
	transV.z=refParam.getData()[11];
}		

//Perform drawing related functions
void ent124::draw()
{
}
//Returns data from this entity for other entities to use
//Input: int what
//	0	return rotation matrix
//	1	return translation vector
void* ent124::returnData(int what)
{
	switch(what)
	{
	case 0:
		return &rotM;	//return rotation matrix
		break;
	case 1:
		return &transV;	//return translation vector
		break;
	}
	return NULL;
}