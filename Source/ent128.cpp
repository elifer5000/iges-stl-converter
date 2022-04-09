///////////////////////////////////////////////////////////////////////////
// FILE: ent128.cpp
// DESCRIPTION: RATIONAL B-SPLINE SURFACE ENTITY (TYPE 128) Class definition
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// Defines a b-spline surface to be used for a trimmed surface.
///////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ent128.h"

//Default constructor
ent128::ent128(void)
{
}
//Process after reading data from file
void ent128::process()
{
	K1 = (int)refParam.getData()[0];
	K2 = (int)refParam.getData()[1];
	M1 = (int)refParam.getData()[2];
	M2 = (int)refParam.getData()[3];
	PROP1 = (int)refParam.getData()[4];
	PROP2 = (int)refParam.getData()[5];
	PROP3 = (int)refParam.getData()[6];
	PROP4 = (int)refParam.getData()[7];
	PROP5 = (int)refParam.getData()[8];
	int N1=1+K1-M1;
	int N2=1+K2-M2;
	A=N1+2*M1;
	B=N2+2*M2;
	C=(1+K1)*(1+K2);	//Total number of control points
	
	ctrlPoints=new Vector4<GLfloat>[C];
		
	sKnots=new GLfloat[A+1];
	tKnots=new GLfloat[B+1];
	int cnt1=0;
	
	for(int i=0;i<=A;i++)
		sKnots[i]=refParam.getData()[9+i];
	for(int i=0;i<=B;i++)
		tKnots[i]=refParam.getData()[10+A+i];
	for(int i=0;i<C;i++)
	{
		ctrlPoints[i].w=refParam.getData()[11+A+B+i];
		ctrlPoints[i].x=refParam.getData()[11+A+B+C+cnt1++]*ctrlPoints[i].w;
		ctrlPoints[i].y=refParam.getData()[11+A+B+C+cnt1++]*ctrlPoints[i].w;
		ctrlPoints[i].z=refParam.getData()[11+A+B+C+cnt1++]*ctrlPoints[i].w;
	}
	U0=refParam.getData()[11+A+B+4*C];
	U1=refParam.getData()[12+A+B+4*C];
	V0=refParam.getData()[13+A+B+4*C];
	V1=refParam.getData()[14+A+B+4*C];
}

//Perform drawing related functions
void ent128::draw()
{
	//Get options and see if mixed mode is enabled
	//In this mode surfaces of degree 1 are tessellated in parametric error
	//to lower number of triangles needed
	entArray_it entIt=refEnts->find(0);
	if(*((BOOL*)(entIt->second->returnData(0))))
	{
		if(M1>1 || M2>1) gluNurbsProperty( ppNurb, GLU_SAMPLING_METHOD, GLU_OBJECT_PATH_LENGTH);
		else gluNurbsProperty( ppNurb, GLU_SAMPLING_METHOD, GLU_OBJECT_PARAMETRIC_ERROR  );
	}
	TRACE("Begin 128 Surface\n");
	gluBeginSurface(ppNurb);
	gluNurbsSurface(ppNurb, A+1,sKnots, B+1, tKnots,4,4*(K1+1),&ctrlPoints[0].x,M1+1,M2+1,GL_MAP2_VERTEX_4);
}
//Perform postdrawing operations
void ent128::endDraw()
{
	TRACE("End 128 Surface\n");
	gluEndSurface(ppNurb);
}
//Destructor
ent128::~ent128(void)
{
	delete[] ctrlPoints;
	delete[] tKnots;
	delete[] sKnots;
}
