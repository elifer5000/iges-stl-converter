///////////////////////////////////////////////////////////////////////////
// FILE: ent142.cpp
// DESCRIPTION:  CURVE ON A PARAMETRIC SURFACE ENTITY (TYPE 142) Class definition
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// A curve that's defined in the parametric space of a surface
///////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ent142.h"

//Default constructor
ent142::ent142(void)
{
}
//Destructor
ent142::~ent142(void)
{
}
//Process after reading data from file
void ent142::process()
{
	CRTN=(int)refParam.getData()[0];
	SPTR=(int)refParam.getData()[1];
	BPTR=(int)refParam.getData()[2];
	CPTR=(int)refParam.getData()[3];
	PREF=(int)refParam.getData()[4];
}
//Perform drawing related functions
//Input: int what
//	0	surface being trimmed is a b-spline surface
//	1	surface being trimmed is a surface of revolution with line generatrix
//	2	surface begin trimmed is a surface of revolution with arc generatrix
void ent142::draw(int what)
{
	if(PREF==0 || PREF==1 || PREF==3)
		(refEnts->find(BPTR))->second->draw(what);

	if(PREF==2)
		AfxMessageBox("Type 142, PREF=2 is unsupported for trimming");
}