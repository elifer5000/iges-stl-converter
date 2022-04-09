///////////////////////////////////////////////////////////////////////////
// FILE: ent100.h
// DESCRIPTION: CIRCULAR ARC ENTITY (TYPE 100) Class declaration
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

#pragma once
#include "entity.h"

class ent100 :
	public entity
{
public:
	//Default constructor
	ent100(void);
	//Constructor
	ent100(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) {}
	//Process after reading data from file
	void process();
	//Check if 2 lines intersect, and save the intersection point in P1
	//Return:	1 if lines are parallel
	//			0 if lines intersect
	int Intersect3DLines(Vector3<float>& o1, Vector3<float>& d1, Vector3<float>& o2, Vector3<float>& d2, Vector3<float>& P1);
	//Perform drawing related functions (if necessary)
	void draw();
	//Returns data from this entity for other entities to use
	void* returnData(int what);
	//Destructor
	virtual ~ent100(void);

public:
	float ZT; //Parallel displacement of arc from XT,YT plane
	Vector3<float> v1;	//Arc Center
	Vector3<float> v2;	//Arc Start Point
	Vector3<float> v3;	//Arc End Point
	float radius;		//Arc radius
	float sAngle;		//Start angle
	float tAngle;		//End angle
	int n;				//n+1 control points
	GLfloat *Knots;					//Nurbs Knot vector
	Vector3<GLfloat> *ctrlPoints;	//Nurbs curve control points
	GLfloat* weights;				//Nurbs curve weights

};
