///////////////////////////////////////////////////////////////////////////
// FILE: ent314.h
// DESCRIPTION: COLOR DEFINITION ENTITY (TYPE 314) Class declaration
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// Defines a color for a surface. Unused in this program.
//////////////////////////////////////////////////////////////////////
#pragma once
#include "entity.h"

class ent314 :
	public entity
{
public:
	//Default constructor
	ent314(void);
	//Constructor
	ent314(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) {}
	//Process after reading data from file
	void process()
	{
		color.r=(GLfloat)(refParam.getData()[0]/100.0);
		color.g=(GLfloat)(refParam.getData()[1]/100.0);
		color.b=(GLfloat)(refParam.getData()[2]/100.0);
	}
	//Perform drawing related functions
	void draw()	{}
	//Destructor
	virtual ~ent314(void);

public:
	Vector3<float> color;	//RGB Color vector
};
