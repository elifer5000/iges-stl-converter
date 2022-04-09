///////////////////////////////////////////////////////////////////////////
// FILE: dir.h
// DESCRIPTION: Class declaration for a directory entry in an IGES file
// AUTHOR: Elias Cohenca - Technion Institute of Technology
//
// This class saves all the information read from a directory entry in an IGES file
// It is used in the IgesRead class as part of the structure that reads the data.
// NOTE: Not all of this information is relevant to the program. The part that is
// is explained by the comments. For the other data, refer to the IGES documentation
// A directory entry will be referred as a DE
///////////////////////////////////////////////////////////////////////////

#pragma once

class dir
{
public:
	dir(void);			//Constructor
	~dir(void);			//Destructor
	dir( const dir &rhs);	//Copy constructor
	
	//Comparison overloads
	// >,< are defined in case we want to order by entity type
	// == returns TRUE if sequenceNumber (the iges Index) is the same
	BOOL operator>( const dir &rhs) const { return entityTypeNumber>rhs.entityTypeNumber; }
	BOOL operator<( const dir &rhs) const { return entityTypeNumber<rhs.entityTypeNumber; }
	BOOL operator==( const dir &rhs) const { return sequenceNumber1==rhs.sequenceNumber1; }

public:
	int entityTypeNumber;		//The type of the entity (refer to IGES spec)
								//The supported types are the different entXXX files
	int parameterData;			//Index of where the parameter data is (in IGES file numeration)
	int Structure;
	int lineFontPattern;
	int Level;
	int View;
	int transformationMatrix;	//Index of DE of a transformation matrix associated with this DE
	int labelDisplay;
	string statusNumber;
	int blankStatus; 
	int subordinateEntitySwitch;
	int entityUseFlag;
	int hierarchy;	
	int	sequenceNumber1;		//Index of this DE
	int entityTypeNumberLine2;	//Same as entityTypeNumber
	int lineWeightNumber;		
	int colorNumber;			//If negative, points to a DE containing a color definition
								//(Unused in this program)
	int parameterLineCount;		//Number of lines in the parameter data (in IGES file)
	int formNumber;
	int Reserved1;
	int Reserved2;
	string entityLabel;
	int	entitySubscriptNumber;
	int sequenceNumber2;		//sequenceNumber1+1

public:
	//Assignment operator
	dir& operator=( const dir &rhs)
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

		return *this;
	}

};
