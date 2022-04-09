///////////////////////////////////////////////////////////////////////////
// FILE: ent144.h
// DESCRIPTION: TRIMMED SURFACE ENTITY (TYPE 144) Class declaration
// Inherited members from entity parent class:
// DirEntry:					refDir
// Parameters:					refParam
// Pointer to entities array:	refEnts
// Entity type:					ID
// Pointer to NURB pointer:		ppNurb
//
// The trimmed surface is defined by the pointer to a NURBS surface (128)
// or a surface of revolution (120) which is itself converted to a NURBS
// surface. The trimming lines are also defined as NURBS curves.
// Using OpenGL's tesselator callback functions, in the IgesView class,
// we can retrieve the vertex information used for drawing the resulting
// surface and use it to save the triangle list as an STL file
//////////////////////////////////////////////////////////////////////
#pragma once
#include "entity.h"
	
class ent144 :
	public entity
{
public:
	//Default constructor
	ent144(void);
	//Constructor
	ent144(int i, dir& d, param& p, entArray* v, GLUnurbsObj* n) : entity(i,d,p,v,n) {}
	//Process after reading data from file
	void process();
	//Perform drawing related functions
	void draw();
	//Perform postdrawing operations
	void endDraw();
	//Destructor
	virtual ~ent144(void);

public:
	int PTS;   //"pointer"(index) to surface to be drawn and trimmed
	int N1;	   //if N1 is 0 the outer boundary is the boundary of the surface 
	int N2;    //number of inner boundaries
	int PTO;   //"pointer" to outer boundary
	vector<int> PTI;  //array of "pointers" to inner boundaries

	//GLuint DLnumber; //Display list
};

