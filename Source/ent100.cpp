///////////////////////////////////////////////////////////////////////////
// FILE: ent100.cpp
// DESCRIPTION: CIRCULAR ARC ENTITY (TYPE 100) Class definition
// Inherited members from entity parent class:
// Directory entry:				refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// A circular arc is defined in the XY plane, with ZT being the
// translation in the Z direction. Usually a transformation matrix
// is associated with this entity to rotate it to any plane.
// The arc is defined by its center, start and end points. The process
// function in this class transforms this arc into a NURBS curve, using
// algorithm A7.1 from "The NURBS Book" (Piegl, Tiller - Springer, 2nd Ed.)
// This curve can then be retrieved by entity 120 (revolution surface)
// if the arc is defined as its generatrix, using the returnData function
///////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ent100.h"

//Default constructor
ent100::ent100(void)
{
}

//Process after reading data from file
//Description: Reads data from parameter list, and converts the arc to
//a NURBS curve
void ent100::process()
{
	Vector3<float> X(1.0,0.0,0.0);		//Unit vector in X direction
	Vector3<float> Y(0.0,1.0,0.0);		//Unit vector in Y direction

	ZT=refParam.getData()[0];			//Z displacement
	v1.x=refParam.getData()[1];			//Arc center
	v1.y=refParam.getData()[2];
	v2.x=refParam.getData()[3];			//Start point
	v2.y=refParam.getData()[4];
	v3.x=refParam.getData()[5];			//End point
	v3.y=refParam.getData()[6];

	radius=sqrt((v2.x-v1.x)*(v2.x-v1.x)+(v2.y-v1.y)*(v2.y-v1.y));
	sAngle=atan2((v2.y-v1.y),(v2.x-v1.x));	//Start angle in radians
	tAngle=atan2((v3.y-v1.y),(v3.x-v1.x));	//End angle in radians
	
	int narcs;		//number of arcs
	if(tAngle<sAngle) tAngle=DEG2RAD2(360.0) + sAngle;
	float theta=tAngle-sAngle;
	
	//Calculate how many arcs we have to divide in (1 for each quarter)
	if(theta<=DEG2RAD2(90.0)) narcs=1;
	else if(theta<=DEG2RAD2(180.0)) narcs=2;
	else if(theta<=DEG2RAD2(270.0)) narcs=3;
	else narcs=4;

	float dtheta=theta/narcs;	
	n=2*narcs;		//n+1 control points
	
	ctrlPoints=new Vector3<GLfloat>[n+1];
	weights=new GLfloat[n+1];
	float w1=cos(dtheta/2.0);
	//Initialize start values
	Vector3<float> P0=v1 + X*(radius*cos(sAngle)) + Y*(radius*sin(sAngle));
	Vector3<float> T0=X*(-1*sin(sAngle)) + Y*cos(sAngle);
	ctrlPoints[0]=P0;
	weights[0]=1.0;
	
	Vector3<float> P1,P2,T2;
	int index=0;
	float angle=sAngle;
	//Find control points for each arc	
	for(int i=1;i<=narcs;i++)
	{
		angle+=dtheta;
		P2=v1 + X*(radius*cos(angle)) + Y*(radius*sin(angle));
		ctrlPoints[index+2]=P2;
		weights[index+2]=1.0;
		T2=X*(-1*sin(angle)) + Y*cos(angle);
		Intersect3DLines(P0,T0,P2,T2,P1);
		ctrlPoints[index+1]=P1;
		weights[index+1]=w1;
		index+=2;
		if(i<narcs)
			{ P0=P2; T0=T2; }
	}
	//Define knot vector
	int j=2*narcs+1;
	Knots=new GLfloat[j+3];
	for(int i=0;i<3;i++)
	{
		Knots[i]=0.0;
		Knots[i+j]=1.0;
	}
	switch(narcs)
	{
	case 1: break;
	case 2: Knots[3]=Knots[4]=0.5;
			break;
	case 3: Knots[3]=Knots[4]=(GLfloat)(1.0/3.0);
			Knots[5]=Knots[6]=(GLfloat)(2.0/3.0);
			break;
	case 4: Knots[3]=Knots[4]=0.25;
			Knots[5]=Knots[6]=0.5;
			Knots[7]=Knots[8]=0.75;
			break;
	}
	

	Matrix3<float> rot;
	Vector3<float> trans;
	//If there's a transformation matrix, get it
	if(refDir.transformationMatrix>0)
	{
		entArray_it entIt=refEnts->find(refDir.transformationMatrix);
		rot=*((Matrix4<float>*)(entIt->second->returnData(0)));
		trans=*((Vector3<float>*)(entIt->second->returnData(1)));
	}
	//Apply transformation to control points
	for(int i=0;i<n+1;i++)
	{
		ctrlPoints[i].z+=ZT;
		Vector3<GLfloat> tmp=rot*ctrlPoints[i];
		ctrlPoints[i]=tmp+trans;
		/*ctrlPoints[i].x*=weights[i];		//We don't need to multiply the weights
		ctrlPoints[i].y*=weights[i];		//because we'll do it when we calculate
		ctrlPoints[i].z*=weights[i];*/		//the points of a surface of revolution
											//(we would need though if we were to
											// draw just this arc)
	}
			
}

//Check if 2 lines intersect, and save the intersection point in P1
//Return:	1 if lines are parallel
//			0 if lines intersect
int ent100::Intersect3DLines(Vector3<float>& o1, Vector3<float>& d1, Vector3<float>& o2, Vector3<float>& d2, Vector3<float>& P1) 
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

//Perform drawing related functions (if necessary)
void ent100::draw()
{	
	/// Draw control points and profile. For testing only ///
	/*glPointSize(5.0);
	for(int i=0;i<(n+1);i++)
	{
		glBegin(GL_POINTS);
		glVertex4fv(ctrlPoints[i]);
		glEnd();
	}
	glBegin(GL_LINES);
	for(int i=0;i<n;i++)
	{
		glVertex4fv(ctrlPoints[i]);
		glVertex4fv(ctrlPoints[i+1]);
	}
	glEnd();
	
	gluBeginCurve(ppNurb);
	gluNurbsCurve(ppNurb,n+4,Knots,4,&ctrlPoints[0].x,3,GL_MAP1_VERTEX_4);
	gluEndCurve(ppNurb);*/
}
//Returns data from this entity for other entities to use
//Input: int what
//	-1 returns the number of control points-1
//	0 to n returns the n+1 control points
//	n+1 to 2n+1 returns the n+1 weights
//  2n+2 and higher returns the n+4 knots
void* ent100::returnData(int what)
{
	if(what==-1)
		return &n;  //number of control points-1
	else if(what>=0 && what<=n)	
		return &ctrlPoints[what];	//control points
	else if(what>=n+1 && what<=2*n+1)
		return &weights[what-n-1];		//weights
	else if(what>=2*n+2)
		return &Knots[what-2*n-2];	//Knot vector (offset of 2(n+1)
	else return NULL;				//		ctrl points to get to the knots data)
	
}

//Destructor
ent100::~ent100(void)
{
	delete[] Knots;
	delete[] ctrlPoints;
	delete[] weights;
}