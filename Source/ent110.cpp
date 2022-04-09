///////////////////////////////////////////////////////////////////////////
// FILE: ent110.cpp
// DESCRIPTION: LINE ENTITY (TYPE 110) Class definition
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

#include "StdAfx.h"
#include "ent110.h"

//Default constructor
ent110::ent110(void)
{
}
//Destructor
ent110::~ent110(void)
{
}
//Process after reading data from file
void ent110::process()
{
	P1=Vector4<GLfloat>(refParam.getData()[0],refParam.getData()[1],refParam.getData()[2],1.0);
	P2=Vector4<GLfloat>(refParam.getData()[3],refParam.getData()[4],refParam.getData()[5],1.0);
	
	//If there's a transformation matrix associated with this entity
	//load it and multiply by it
	if(refDir.transformationMatrix>0)
	{
		Matrix3<float> rot;
		Vector3<float> trans;

		entArray_it entIt=refEnts->find(refDir.transformationMatrix);
		rot=*((Matrix4<float>*)(entIt->second->returnData(0)));
		trans=*((Vector3<float>*)(entIt->second->returnData(1)));
	
		Matrix4<float> M;
		M.setRotation(rot);
		M.setTranslation(trans);
		Vector4<GLfloat> tmp=M*P1;
		P1=tmp;	
		tmp=M*P2;
		P2=tmp;
	}
}		
//Perform drawing related functions (if necessary)
void ent110::draw()
{
	glBegin(GL_LINES);
	glVertex3fv(P1);
	glVertex3fv(P2);
	glEnd();
}
//Returns data from this entity for other entities to use
//Input: int what
//	0	return first point
//	1	return second point
void* ent110::returnData(int what)
{
	switch(what)
	{
	case 0:
		return &P1;
		break;
	case 1:
		return &P2;
		break;
	}
	return NULL;
}