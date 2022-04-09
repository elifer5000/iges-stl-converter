///////////////////////////////////////////////////////////////////////////
// FILE: dir.cpp
// DESCRIPTION: Class definition for a directory entry in an IGES file
// AUTHOR: Elias Cohenca - Technion Institute of Technology
//
// This class saves all the information read from a directory entry in an IGES file
// It is used in the IgesRead class as part of the structure that reads the data.
// NOTE: Not all of this information is relevant to the program. The part that is
// is explained by the comments. For the other data, refer to the IGES documentation
// A directory entry will be referred as a DE
///////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "dir.h"

//Default constructor with initializers
dir::dir(void) : entityTypeNumber(0),parameterData(0), Structure(0),lineFontPattern(0),Level(0),
View(0),transformationMatrix(0),labelDisplay(0),statusNumber("0"),blankStatus(0),subordinateEntitySwitch(0),sequenceNumber1(0),
entityUseFlag(0),hierarchy(0),entityTypeNumberLine2(0),lineWeightNumber(0),colorNumber(0),parameterLineCount(0),formNumber(0),Reserved1(0),Reserved2(0),
entityLabel("0"),entitySubscriptNumber(0),sequenceNumber2(0)
{
}

//Destructor
dir::~dir(void)
{
}

//Copy constructor
dir::dir( const dir &rhs)
{
	entityTypeNumber=rhs.entityTypeNumber;
	parameterData=rhs.parameterData;
	Structure=rhs.Structure;
	lineFontPattern=rhs.lineFontPattern;
	Level=rhs.Level;
	View=rhs.View;
	transformationMatrix=rhs.transformationMatrix;
	labelDisplay=rhs.labelDisplay;
	statusNumber=rhs.statusNumber;
	blankStatus=rhs.blankStatus;
	subordinateEntitySwitch=rhs.subordinateEntitySwitch;
	entityUseFlag=rhs.entityUseFlag;
	hierarchy=rhs.hierarchy;
	sequenceNumber1=rhs.sequenceNumber1;
	entityTypeNumberLine2=rhs.entityTypeNumberLine2;
	lineWeightNumber=rhs.lineWeightNumber;
	colorNumber=rhs.colorNumber;
	parameterLineCount=rhs.parameterLineCount;
	formNumber=rhs.formNumber;
	Reserved1=rhs.Reserved1;
	Reserved2=rhs.Reserved2;
	entityLabel=rhs.entityLabel;
	entitySubscriptNumber=rhs.entitySubscriptNumber;
	sequenceNumber2=rhs.sequenceNumber2;
}
