///////////////////////////////////////////////////////////////////////////
// FILE: entity.h
// DESCRIPTION: Parent Class declaration for an entity entry in an IGES file
// AUTHOR: Elias Cohenca - Technion Institute of Technology
//
// This class serves as a starting point for all entity types for the program.
// Since all different entities derive from this class, we can store the
// different entities in one array (in the IgesRead class) and access them
// This also ensures that we can add new entities in the future, if needed.
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "dir.h"
#include "param.h"

class entity
{
public:
	entity(void);				//Default Constructor
	virtual ~entity(void) { }	//Destructor
	//Constructor
	entity(int i, dir& d, param& p, map<int,entity*>* v, GLUnurbsObj* n)
		: ID(i), refDir(d), refParam(p)
	{
		refEnts=v;
		ppNurb=n;
	}
	//Copy constructor
	entity::entity( const entity &rhs) 
	{
		refDir=rhs.refDir;
		refParam=rhs.refParam;
	}
	//Assignment operator
	entity& operator=( const entity &rhs)
	{
		refDir=rhs.refDir;
		refParam=rhs.refParam;
		return *this;
	}
	int getIndex() { return refDir.sequenceNumber1; }
	//Comparison overloads
	// >,< are defined in case we want to order by entity type
	// == returns TRUE if sequenceNumber (the iges Index) is the same
	BOOL operator>( const entity &rhs) const { return refDir > rhs.refDir; }
	BOOL operator<( const entity &rhs) const { return refDir < rhs.refDir; }
	BOOL operator==( const entity &rhs) const { return refDir == rhs.refDir; }


	//Virtual Functions
	//Process:		Use to perform operations when reading the entity
	//Draw:			Use to perform drawing or trimming operations
	//endDraw:		Use to perform post-drawing operations (e.g. endSurface)
	//returnData:	Use to read data from an entity (this is useful since a derived
	//				entity can't know the members of another entity)
	//setData:		Use to set data in an entity. Same principle as returnData
	virtual void process() {AfxMessageBox("Error. Process not available for base type."); }
	virtual void draw() { AfxMessageBox("Error. Draw not available for base type."); }
	virtual void draw(int what) { AfxMessageBox("Error. Draw not available for base type."); }
	virtual void endDraw() { AfxMessageBox("Error. endDraw not available for base type."); }
	virtual void* returnData(int what) { return NULL; }
	virtual void setData(void* what, int psize) { }
	
		
public:
	int ID;					//Entity type
	dir refDir;				//Directory Entry info
	param refParam;			//Parameter data
	map<int,entity*>* refEnts;	//Pointer to the array of entities
	GLUnurbs *ppNurb;		//Pointer to nurb
	

};

typedef map<int,entity*> entArray;
typedef map<int,entity*>::iterator entArray_it;