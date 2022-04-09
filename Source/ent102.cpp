///////////////////////////////////////////////////////////////////////////
// FILE: ent102.cpp
// DESCRIPTION: COMPOSITE CURVE ENTITY (TYPE 102) Class definition
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
#include "StdAfx.h"
#include "ent102.h"

//Default constructor
ent102::ent102(void)
{
}
//Destructor
ent102::~ent102(void)
{
}
//Process after reading data from file
void ent102::process()
{
	N=(int)refParam.getData()[0];
	for(int i=1;i<N+1;i++)
		pToDE.push_back((int)refParam.getData()[i]);
	doOnce=FALSE;
}
//Perform drawing related functions (if necessary)
//First checks that the trimming lines are closed. If not, it joins them,
//and in case that the space is too big it adds a line between them (max 1)
//Input: int what
//	0	surface being trimmed is a b-spline surface
//	1	surface being trimmed is a surface of revolution with line generatrix
//	2	surface begin trimmed is a surface of revolution with arc generatrix
void ent102::draw(int what)
{ 
	
	/////////////////////////////////////////////////////////////
	//Check that the trimming lines are continous and fix if not
	TRACE("Begin check\n");
	entArray_it entIt=refEnts->find(0);
	float tol=*((float*)(entIt->second->returnData(1)));	//get trimming line's endpoint
															//separation from options
	BOOL fix=FALSE;		//True if adding a line is needed
	Vector3<GLfloat>* v1_end;
	Vector3<GLfloat>* v2_beg;
	Vector3<GLfloat> p[2];	//begin and end point of added line
	GLfloat Knots[4]= {0.,0.,1.,1.};	//for b-spline line to be added if needed
	vector<int>::iterator missingLine=pToDE.end();	//marks the place to add the line
	
	//Go through all points and compare endpoints to see if they are connected
	for(vector<int>::iterator it= pToDE.begin(); it!=(pToDE.end()-1); it++)
	{
		entArray_it entIt1=refEnts->find(*it);
		entArray_it entIt2=refEnts->find(*(it+1));
		if(what==0)	//b-spline surface
		{
			v1_end=((Vector3<GLfloat>*)entIt1->second->returnData(1));
			v2_beg=((Vector3<GLfloat>*)entIt2->second->returnData(0));
		}
		if(what==1 || what==2)	//surface of revolution
		{
			v1_end=((Vector3<GLfloat>*)entIt1->second->returnData(3));
			v2_beg=((Vector3<GLfloat>*)entIt2->second->returnData(2));
		}
		
		//if endpoint of 1 is different from beginpoint of 2
		if(*v1_end!=*v2_beg)
		{
			TRACE("Before V1 %f %f %f V2 %f %f %f \n", ((Vector3<GLfloat>*)entIt1->second->returnData(3))->x, ((Vector3<GLfloat>*)entIt1->second->returnData(3))->y,((Vector3<GLfloat>*)entIt1->second->returnData(3))->z, ((Vector3<GLfloat>*)entIt2->second->returnData(2))->x,((Vector3<GLfloat>*)entIt2->second->returnData(2))->y,((Vector3<GLfloat>*)entIt2->second->returnData(2))->z);
			//and it's within the distance tolerance
			if(fabs(v1_end->x - v2_beg->x)<tol && fabs(v1_end->y - v2_beg->y)<tol)
				*v2_beg=*v1_end;	//then join them
			else	//else add a line between them
			{
				p[0]=*v1_end;
				p[1]=*v2_beg;
				//surface of revolution with line generatrix
				//doOnce has to be false, else we'll do the division
				//by PI and 2PI more than once
				if(what==1 && doOnce==FALSE)
				{
					p[0].x/=(float)(2.0*M_PI);
					p[1].x/=(float)(2.0*M_PI);
				}
				if(what==2 && doOnce==FALSE)
				//surface of revolution with arc generatrix
				{
					p[0].x/=(float)(2.0*M_PI);
					p[0].y/=(float)(M_PI);
					p[1].x/=(float)(2.0*M_PI);
					p[1].y/=(float)(M_PI);	
				}
				missingLine=it;	//place where the line is to be added
				fix=TRUE;		//do the fix
			}
		}
		TRACE("V1 %f %f %f V2 %f %f %f \n", ((Vector3<GLfloat>*)entIt1->second->returnData(3))->x, ((Vector3<GLfloat>*)entIt1->second->returnData(3))->y,((Vector3<GLfloat>*)entIt1->second->returnData(3))->z, ((Vector3<GLfloat>*)entIt2->second->returnData(2))->x,((Vector3<GLfloat>*)entIt2->second->returnData(2))->y,((Vector3<GLfloat>*)entIt2->second->returnData(2))->z);
	}
	////////////////////////////////////////////
	//Check if there's a missing line at the end
	entArray_it entIt1;
	entArray_it entIt2;
		
	entIt1=refEnts->find(*(pToDE.end()-1));
	entIt2=refEnts->find(*pToDE.begin());
	if(what==0)
	{
		v1_end=((Vector3<GLfloat>*)entIt1->second->returnData(1));
		v2_beg=((Vector3<GLfloat>*)entIt2->second->returnData(0));
	}
	if(what==1 || what==2)
	{
		v1_end=((Vector3<GLfloat>*)entIt1->second->returnData(3));
		v2_beg=((Vector3<GLfloat>*)entIt2->second->returnData(2));
	}
	
	//if endpoint of 1 is different from beginpoint of 2
	if(*v1_end!=*v2_beg)
	{
		TRACE("Before V1 %f %f %f V2 %f %f %f \n", v1_end->x,v1_end->y,v1_end->z,v2_beg->x,v2_beg->y,v2_beg->z);
		//If end and beginning points are really close together join them
		if(fabs(v1_end->x - v2_beg->x)<tol && fabs(v1_end->y - v2_beg->y)<tol)
			*v2_beg=*v1_end;
		else	//else add a line between them
		{
			p[0]=*v1_end;
			p[1]=*v2_beg;

			if(what==1 && doOnce==FALSE)
			{
				p[0].x/=(float)(2.0*M_PI);
				p[1].x/=(float)(2.0*M_PI);
			}
			if(what==2 && doOnce==FALSE)
			{
				p[0].x/=(float)(2.0*M_PI);
				p[0].y/=(float)(M_PI);
				p[1].x/=(float)(2.0*M_PI);
				p[1].y/=(float)(M_PI);	
			}
			
			fix=TRUE;
		}
	}
	TRACE("V1 %f %f %f V2 %f %f %f \n", ((Vector3<GLfloat>*)entIt1->second->returnData(1))->x, ((Vector3<GLfloat>*)entIt1->second->returnData(1))->y,((Vector3<GLfloat>*)entIt1->second->returnData(1))->z, ((Vector3<GLfloat>*)entIt2->second->returnData(0))->x,((Vector3<GLfloat>*)entIt2->second->returnData(0))->y,((Vector3<GLfloat>*)entIt2->second->returnData(0))->z);
	TRACE("End check\n");
	/////////////////////////////////////////////

	//Go through all entities belonging to the composite line and draw them
	for(vector<int>::iterator it= pToDE.begin(); it!=pToDE.end(); it++)
	{
		entArray_it entIt=refEnts->find(*it);
		entIt->second->draw(what);
		//Add a missing line if needed
		if(missingLine==it && fix==TRUE)
		{
			gluNurbsCurve(ppNurb,4,Knots,3,&p[0].x,2,GLU_MAP1_TRIM_3);
			fix=FALSE;
		}
	}
	
	//If the missing line to add is at the end
	if(fix)
	{
		gluNurbsCurve(ppNurb,4,Knots,3,&p[0].x,2,GLU_MAP1_TRIM_3);
	}
	doOnce=TRUE;

}
//Returns data from this entity for other entities to use
//Input: int what
//	0	return number of entities in line
//	1	return vector of entities in line
void* ent102::returnData(int what)
{
	switch(what)
	{
	case 0:
		return &N;			//number of entities in line
	case 1:
		return &pToDE;		//vector of entities in line
		break;
	}
	return NULL;
}