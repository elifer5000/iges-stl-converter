///////////////////////////////////////////////////////////////////////////
// FILE: ent120.h
// DESCRIPTION: SURFACE OF REVOLUTION ENTITY (TYPE 120) Class declaration
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
#pragma once
#include "entity.h"

class ent120 :
	public entity
{
public:
	//Default constructor
	ent120(void);
	//Constructor
	ent120(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) {}
	//Process after reading data from file
	void process();
	//Perform drawing related functions
	void draw();
	//Perform postdrawing operations
	void endDraw();
	
	//Find the projection of a point on a line, and put the result in vector O
	void PointToLine(Vector3<float>& pGen, Vector3<float>& O)
		{ O=axisT*((pGen-axisS).dotProduct(axisT)) + axisS; }
	//Check if 2 lines intersect, and save the intersection point in P1
	//Return:	1 if lines are parallel
	//			0 if lines intersect
	int Intersect3DLines(Vector3<float>& o1, Vector3<float>& d1, Vector3<float>& o2, Vector3<float>& d2, Vector3<float>& P1);
	
	//Returns data from this entity for other entities to use
	void* returnData(int what);
	//Destructor
	virtual ~ent120(void);
		
public:
	int pLine;		//Pointer to the DE of the axis of revolution
	int gEnt;		//Pointer to the DE of the generatrix entity
	float sAngle;	//Start angle (in radians)
	float tAngle;	//Terminate angle (in radians)
	Vector3<GLfloat> axisS;	//Start point of axis
	Vector3<GLfloat> axisT;	//Unit length vector in direction of axis
	
	GLfloat* genKnots;	//Generatrix knots
	Vector3<GLfloat>* genCtrlPoints;	//Generatrix control points
	GLfloat* genWeights;					//Generatrix weights
	int order;		//order of generatrix
	int numCtrlGen;	//number of control points in generatrix

	Vector4<GLfloat>* ctrlPoints;		//Surface control points
	int numCtrlS;	//number of ctrl points in calculated direction
	GLfloat* sKnots;	//Computed knots
	
	
	
};
