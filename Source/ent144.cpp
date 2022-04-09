///////////////////////////////////////////////////////////////////////////
// FILE: ent144.cpp
// DESCRIPTION: TRIMMED SURFACE ENTITY (TYPE 144) Class definition
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
#include "StdAfx.h"
#include "ent144.h"

//Default constructor
ent144::ent144(void)
{
}

//Destructor
ent144::~ent144(void)
{
}

//Process after reading data from file
void ent144::process()
{
	PTS=(int)refParam.getData()[0];
	N1=(int)refParam.getData()[1];
	N2=(int)refParam.getData()[2];
	PTO=(int)refParam.getData()[3];
	for(int i=4;i<4+N2;i++)
		PTI.push_back((int)refParam.getData()[i]);
}

//Perform drawing related functions
void ent144::draw()
{
	//Find the surface in the array
	entArray_it surfIt=refEnts->find(PTS);
	
	if(surfIt->second->ID==128) //B-spline surface
	{
		//begin Surface
		surfIt->second->draw();
		
		gluBeginTrim(ppNurb);
		refEnts->find(PTO)->second->draw(0);	  //Outer boundary trim
		gluEndTrim(ppNurb);
		
		for(int i=0;i<N2;i++)
		{
			gluBeginTrim(ppNurb);
			refEnts->find(PTI[i])->second->draw(0);  //Inner boundaries trim
			gluEndTrim(ppNurb);
		}
		//FOR DEBUG
		//TRACE("DLNumber %d  Sequence %d  PTS %d  N1 %d N2 %d pnurbAddress %08x\n", DLnumber, refDir.sequenceNumber1, PTS, N1, N2, ppNurb);
		surfIt->second->endDraw(); //end surface
	}
	if(surfIt->second->ID==120)   //Surface of revolution
	{
		int gen=*(int*)(surfIt->second->returnData(0));
		entArray_it genIt=refEnts->find(gen);

		//begin Surface
		surfIt->second->draw();
		
		gluBeginTrim(ppNurb);
		if(genIt->second->ID==110) refEnts->find(PTO)->second->draw(1);	  //Outer boundary trim
		else if(genIt->second->ID==100) refEnts->find(PTO)->second->draw(2);
		gluEndTrim(ppNurb);

		for(int i=0;i<N2;i++)
		{
			gluBeginTrim(ppNurb);
			//Inner boundaries trim
			//Different cases:
			// ID=110	Generatrix is a line
			// ID=100	Generatrix is an arc
			if(genIt->second->ID==110) refEnts->find(PTI[i])->second->draw(1);
			else if(genIt->second->ID==100) refEnts->find(PTI[i])->second->draw(2);
			gluEndTrim(ppNurb);
		}
		//FOR DEBUG
		//TRACE("DLNumber %d  Sequence %d  PTS %d  N1 %d N2 %d pnurbAddress %08x\n", DLnumber, refDir.sequenceNumber1, PTS, N1, N2, ppNurb);
		surfIt->second->endDraw();	//end surface
	}
}

//Perform postdrawing operations
//Calls the data stored in the display list to render it
void ent144::endDraw()
{
	//glCallList(DLnumber);
}