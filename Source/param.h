// FILE: param.h
// DESCRIPTION: Class declaration for a parameter entry in an IGES file
// AUTHOR: Elias Cohenca - Technion Institute of Technology
//
// This class saves all the information read from a parameter entry in an IGES file
// It is used in the IgesRead class as part of the structure that reads the data.
// A directory entry will be referred as a DE
// A parameter entry will be referred as a PE
#pragma once

class param
{
public:
	param(void);		//Constructor
	~param(void);		//Destructor
	param( const param &rhs)	//Copy constructor
	{
		entityTypeNumber=rhs.entityTypeNumber;
		pToDir=rhs.pToDir;
		index=rhs.index;
		for(vector<GLfloat>::const_iterator iter=rhs.data.begin(); iter!=rhs.data.end();iter++)
			param::data.push_back(*iter);
	}
	//Assignment operator
	param& operator=( const param &rhs)
	{
		entityTypeNumber=rhs.entityTypeNumber;
		pToDir=rhs.pToDir;
		index=rhs.index;
		for(vector<GLfloat>::const_iterator iter=rhs.data.begin(); iter!=rhs.data.end();iter++)
			data.push_back(*iter);
		return *this;
	}
	//Comparison overloads
	// >,< are defined in case we want to order by entity type
	// == returns TRUE if parameter index is the same
	BOOL operator>( const param &rhs) const { return entityTypeNumber>rhs.entityTypeNumber; }
	BOOL operator<( const param &rhs) const { return entityTypeNumber<rhs.entityTypeNumber; }
	BOOL operator==( const param &rhs) const { return index==rhs.index; }
	
	//Erases all data in the parameter
	void zero() {entityTypeNumber=0; pToDir=0; index=0; data.erase(data.begin(),data.end()); }
	int getpToDir() { return pToDir;} //Returns index of DE
	vector<GLfloat> getData() { return data;} //Returns parameter data

public:
	int entityTypeNumber;  //Type of entity
	vector<GLfloat> data;  //Numerical parameter data
	int pToDir; //Numerical "pointer"(index) to DE
	int index;	//Position where this parameter starts in parameter list

};
