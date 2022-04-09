///////////////////////////////////////////////////////////////////////////
// FILE: ent126.cpp
// DESCRIPTION: RATIONAL B-SPLINE CURVE ENTITY (TYPE 126) Class definition
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
#include "StdAfx.h"
#include "ent126.h"

//Default constructor
ent126::ent126(void)
{
}
//Process after reading data from file
void ent126::process()
{
	K = (int)refParam.getData()[0];
	M = (int)refParam.getData()[1];
	PROP1 = (int)refParam.getData()[2];
	PROP2 = (int)refParam.getData()[3];
	PROP3 = (int)refParam.getData()[4];
	PROP4 = (int)refParam.getData()[5];
	N=1+K-M;	//Intermediate variables for calcs
	A=N+2*M;

	ctrlPoints=new Vector4<GLfloat>[K+1];
	paramCtrlPoints=new Vector3<GLfloat>[K+1];
	revParamCtrlPoints=new Vector3<GLfloat>[K+1];
	Knots=new GLfloat[A+1];
	int cnt1=0;
		
	for(int i=0;i<=A;i++)
		Knots[i]=refParam.getData()[6+i];
	for(int j=0;j<=K;j++)
	{
		ctrlPoints[j].w=refParam.getData()[7+A+j];
		ctrlPoints[j].x=refParam.getData()[8+A+K+cnt1++]*ctrlPoints[j].w;
		ctrlPoints[j].y=refParam.getData()[8+A+K+cnt1++]*ctrlPoints[j].w;
		ctrlPoints[j].z=refParam.getData()[8+A+K+cnt1++]*ctrlPoints[j].w;
		paramCtrlPoints[j].z=ctrlPoints[j].w;
		paramCtrlPoints[j].x=ctrlPoints[j].x*paramCtrlPoints[j].z;
		paramCtrlPoints[j].y=ctrlPoints[j].y*paramCtrlPoints[j].z;
		//In revolution surfaces the parameters are in reversed order: u,v->v,u
		//This is only because the definition of a rev surf in this program
		//is different from the Iges definition
		revParamCtrlPoints[j].z=ctrlPoints[j].w;
		revParamCtrlPoints[j].x=ctrlPoints[j].y*revParamCtrlPoints[j].z;
		revParamCtrlPoints[j].y=ctrlPoints[j].x*revParamCtrlPoints[j].z;
	}
	V0=refParam.getData()[11+A+4*K];
	V1=refParam.getData()[12+A+4*K];
	normal.x=refParam.getData()[13+A+4*K];
	normal.y=refParam.getData()[14+A+4*K];
	normal.z=refParam.getData()[15+A+4*K];
	doOnce=FALSE;

}
//Perform drawing related functions
//Input: int what
//	0	surface being trimmed is a b-spline surface
//	1	surface being trimmed is a surface of revolution with line generatrix
//	2	surface begin trimmed is a surface of revolution with arc generatrix
void ent126::draw(int what)
{
	if(!doOnce)
	for(int i=0;i<K+1;i++)
	{
		if(what==1)
			revParamCtrlPoints[i].x/=(float)(2.0*M_PI);
		if(what==2)
		{
			revParamCtrlPoints[i].x/=(float)(2.0*M_PI);
			revParamCtrlPoints[i].y/=(float)(M_PI);
		}
		doOnce=TRUE;
	}
	if(what==1 || what==2)	gluNurbsCurve (ppNurb, A+1, Knots, 3, &revParamCtrlPoints[0].x, M+1, GLU_MAP1_TRIM_3);
	if(what==0) gluNurbsCurve (ppNurb, A+1, Knots, 3, &paramCtrlPoints[0].x, M+1, GLU_MAP1_TRIM_3);
}
//Returns data from this entity for other entities to use
//Input: int what
//	0	return first parameter control point
//	1	return last parameter control point
//	2	return first reversed parameter control point (for surface of revolution)
//	3	return last reversed parameter control point (for surface of revolution)
void* ent126::returnData(int what)
{
	switch(what)
	{
	case 0:
		return &paramCtrlPoints[0];
		break;
	case 1:
		return &paramCtrlPoints[K];
		break;
	case 2:
		return &revParamCtrlPoints[0];
		break;
	case 3:
		return &revParamCtrlPoints[K];
		break;

	}
	return NULL;
}
//Destructor
ent126::~ent126(void)
{
	delete[] ctrlPoints;
	delete[] paramCtrlPoints;
	delete[] revParamCtrlPoints;
	delete[] Knots;
}
