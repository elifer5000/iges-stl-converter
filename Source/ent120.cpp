///////////////////////////////////////////////////////////////////////////
// FILE: ent120.cpp
// DESCRIPTION: SURFACE OF REVOLUTION ENTITY (TYPE 120) Class definition
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// A surface of revolution is defined by an axis (a line entity), a
// start angle, an end angle, and a generatrix (a line or arc entity are
// supported) using Algorithm A8.1 from "The NURBS Book"
// (Piegl, Tiller - Springer, 2nd Ed.)
///////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ent120.h"

//Default constructor
ent120::ent120(void)
{
}
//Process after reading data from file
void ent120::process()
{
	pLine=refParam.getData()[0];
	gEnt=refParam.getData()[1];
	sAngle=refParam.getData()[2];
	tAngle=refParam.getData()[3];		

	entArray_it entGeneratrix=refEnts->find(gEnt);
	entArray_it entAxis=refEnts->find(pLine);

	//Get the axis of the surface
	axisS=*((Vector3<GLfloat>*)entAxis->second->returnData(0));		//start point
	axisT=*((Vector3<GLfloat>*)entAxis->second->returnData(1))-axisS;
	axisT.normalize();		//axis direction
	
	//Get generatrix control points, weights, and knot vector
	if(entGeneratrix->second->ID==110) //If generatrix is a line entity
	{
		numCtrlGen=2;
		order=2;
		genCtrlPoints=new Vector3<GLfloat>[numCtrlGen];
		genWeights=new GLfloat[numCtrlGen];
		genCtrlPoints[0]=*((Vector3<GLfloat>*)(entGeneratrix->second->returnData(0)));
		genWeights[0]=1.0;
		genCtrlPoints[1]=*((Vector3<GLfloat>*)(entGeneratrix->second->returnData(1)));
		genWeights[1]=1.0;
		genKnots=new GLfloat[4];
		genKnots[0]=genKnots[1]=0.;
		genKnots[2]=genKnots[3]=1.;
	}
	else if(entGeneratrix->second->ID==100) //If generatrix is a circular arc entity
	{
		numCtrlGen=*((int*)(entGeneratrix->second->returnData(-1)))+1;
		order=3;
		genCtrlPoints=new Vector3<GLfloat>[numCtrlGen];
		genWeights=new GLfloat[numCtrlGen];
		for(int i=0;i<numCtrlGen;i++)
		{
			genCtrlPoints[i]=*((Vector3<GLfloat>*)(entGeneratrix->second->returnData(i)));
			genWeights[i]=*((GLfloat*)(entGeneratrix->second->returnData(i+numCtrlGen)));
		}
		genKnots=new GLfloat[numCtrlGen+3];
		for(int i=0;i<numCtrlGen+3;i++)
		{
			genKnots[i]=*((GLfloat*)(entGeneratrix->second->returnData(i+2*numCtrlGen)));
		}
	}
	else
	{
		AfxMessageBox("Generatrix type not supported for revolution surface 120");
		return;
	}

	int narcs;		//number of arcs
	if(tAngle<sAngle) tAngle=DEG2RAD2(360.0) + sAngle;
	float theta=tAngle-sAngle;
	//Calculate how many arcs we have to divide in (1 for each quarter)
	if(theta<=DEG2RAD2(90.0))
	{
		narcs=1;
		sKnots=new GLfloat[6];
	}
	else if(theta<=DEG2RAD2(180.0))
	{
		narcs=2;
		sKnots=new GLfloat[8];
		sKnots[3]=sKnots[4]=0.5;
	}
	else if(theta<=DEG2RAD2(270.0))
	{
		narcs=3;
		sKnots=new GLfloat[10];
		sKnots[3]=sKnots[4]=GLfloat(1.0/3.0);
		sKnots[5]=sKnots[6]=GLfloat(2.0/3.0);
	}
	else
	{
		narcs=4;
		sKnots=new GLfloat[12];
		sKnots[3]=sKnots[4]=0.25;
		sKnots[5]=sKnots[6]=0.5;
		sKnots[7]=sKnots[8]=0.75;
	}
	float dtheta=theta/narcs;
	int j=3+2*(narcs-1);		//load end knots
	for(int i=0;i<3;j++,i++)
	{
		sKnots[i]=0.0;
		sKnots[j]=1.0;
	}

	int n=2*narcs;				//n+1 control points
	numCtrlS=n+1;
	ctrlPoints=new Vector4<GLfloat>[(n+1)*numCtrlGen];
	
	float wm=cos(dtheta/2.0);	//base angle
	float angle=0.0;			//compute sines and cosines once
	float* sines=new float[narcs+1];
	float* cosines=new float[narcs+1];
	sines[0]=cosines[0]=0.0;
	for(int i=1;i<=narcs;i++)
	{
		angle+=dtheta;
		cosines[i]=cos(angle);
		sines[i]=sin(angle);
	}
	//Calculate control points of the surface
	for(j=0;j<numCtrlGen;j++)	//Generatrix direction
	{
		Vector3<float> pointOnLine;
		PointToLine(genCtrlPoints[j],pointOnLine);
		Vector3<float> X=genCtrlPoints[j]-pointOnLine;
		float r=X.length();
		X.normalize();
		Vector3<float> Y=axisT.crossProduct(X);
		Vector3<float> P0=genCtrlPoints[j];
		ctrlPoints[0+(n+1)*j]=Vector4<float>(P0.x,P0.y,P0.z,genWeights[j]);
		Vector3<float> T0=Y;	
		int index=0;
		angle=0.0;
		for(int i=1;i<=narcs;i++)	//Revolution direction
		{
			Vector3<float> P2=pointOnLine+X*(r*cosines[i])+Y*(r*sines[i]);
			ctrlPoints[index+2+numCtrlS*j]=Vector4<float>(P2.x,P2.y,P2.z,genWeights[j]);
			Vector3<float> T2=X*(-1*sines[i])+Y*(cosines[i]);
			Vector3<float> P1;
			Intersect3DLines(P0,T0,P2,T2,P1);
			ctrlPoints[index+1+numCtrlS*j]=Vector4<float>(P1.x,P1.y,P1.z,genWeights[j]*wm);
			index+=2;
			if(i<narcs) { P0=P2; T0=T2; }
		}
	}
	//Apply weights
	for(int i=0;i<((n+1)*numCtrlGen);i++)
	{
		ctrlPoints[i].x=ctrlPoints[i].x*ctrlPoints[i].w;
		ctrlPoints[i].y=ctrlPoints[i].y*ctrlPoints[i].w;
		ctrlPoints[i].z=ctrlPoints[i].z*ctrlPoints[i].w;
	}
	
	delete []sines;
	delete []cosines;
}

//Perform drawing related functions
void ent120::draw()
{
	/*glPointSize(5.0);
	for(int i=0;i<(numCtrlS*numCtrlGen);i++)
	{
		glBegin(GL_POINTS);
		glVertex3f(ctrlPoints[i].x,ctrlPoints[i].y,ctrlPoints[i].z);
		glEnd();
	}
	
	for(int i=0;i<numCtrlGen;i++)
	{
		glBegin(GL_LINES);
		for(int j=0;j<numCtrlS-1;j++)
		{
			glVertex3f(ctrlPoints[j+i*numCtrlS].x,ctrlPoints[j+i*numCtrlS].y,ctrlPoints[j+i*numCtrlS].z);
			glVertex3f(ctrlPoints[j+1+i*numCtrlS].x,ctrlPoints[j+1+i*numCtrlS].y,ctrlPoints[j+1+i*numCtrlS].z);
		}
		glEnd();
	}*/
	
	entArray_it entIt=refEnts->find(0);	//For mixed mode
	if(*((BOOL*)(entIt->second->returnData(0))))
		gluNurbsProperty( ppNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
		
	TRACE("Begin 120 Surface\n");
	gluBeginSurface(ppNurb);
	gluNurbsSurface(ppNurb, numCtrlS+3,sKnots, numCtrlGen+order, genKnots,4,4*numCtrlS,&ctrlPoints[0].x,3,order,GL_MAP2_VERTEX_4);
	
}

//Perform postdrawing operations
void ent120::endDraw()
{
	TRACE("End 120 Surface\n");
	gluEndSurface(ppNurb);
}

//Check if 2 lines intersect, and save the intersection point in P1
	//Return:	1 if lines are parallel
	//			0 if lines intersect
int ent120::Intersect3DLines(Vector3<float>& o1, Vector3<float>& d1, Vector3<float>& o2, Vector3<float>& d2, Vector3<float>& P1) 
{
/*L1(t1) = o1 + d1*t1
  L2(t2) = o2 + d2*t2

  The solution is:
  t1 = Determinant{(o2-o1),d2,d1 X d2} / ||d1 X d2||^2
  and
  t2 = Determinant{(o2-o1),d1,d1 X d2} / ||d1 X d2||^2

  If the lines are parallel, the denominator ||d1 X d2||^2 is 0.
  If the lines do not intersect, t1 and t2 mark the points of closest approach on each line. */

	Vector3<float> d1Xd2=d1.crossProduct(d2);	//d1 X d2
	float denLen=d1Xd2.lengthSq();				//||d1 X d2||^2
	if( fabs(denLen)<=(EPSILON5) )
		return 1;

	float arr[9] = {(o2.x-o1.x),(o2.y-o1.y),(o2.z-o1.z),
					d2.x,d2.y,d2.z,
					d1Xd2.x,d1Xd2.y,d1Xd2.z};
	Matrix3<float> M=Matrix3<float>( arr );
	float t1=M.determinant()/denLen;

	P1=Vector3<float>(o1.x + d1.x*t1,o1.y + d1.y*t1,o1.z + d1.z*t1);

	return 0;
}
//Returns data from this entity for other entities to use
//Input: int what
//	0	return dir of generatrix
void* ent120::returnData(int what)
{
	switch(what)
	{
	case 0:
		return &gEnt;	//return dir of generatrix
		break;
	}
	return NULL;
}
//Destructor
ent120::~ent120(void)
{
	delete[] sKnots;
	delete[] ctrlPoints;
	delete[] genKnots;
	delete[] genCtrlPoints;
	delete[] genWeights;
}	
