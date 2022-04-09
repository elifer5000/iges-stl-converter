///////////////////////////////////////////////////////////////////////////
// FILE: IgesRead.h
// DESCRIPTION: Interface for the IgesRead class.
// AUTHOR: Elias Cohenca - Technion Institute of Technology
// Class that defines the iges format for reading. This is composed
// of an array of entities, each linked to its original directory entry
// and parameter data. To make going through the trimmed surfaces faster
// their location is also stored in a different vector
///////////////////////////////////////////////////////////////////////////

#pragma once

#include "dir.h"
#include "param.h"
#include "entity.h"

typedef map<int,entity*> entArray;
typedef map<int,entity*>::iterator entArray_it;

class IgesRead : public CObject
{
public:
	DECLARE_DYNAMIC(IgesRead)
	IgesRead(void);		//Constructor
	~IgesRead(void);	//Destructor	
	BOOL loadLine(const string& line);  //load from an iges file
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
	void reInit(int met, float tol);	//Reinitialize values with method and tolerance
	
public:
	string start;	//First line in Iges file. Information only

	string p_delim;	//Parameter delimeter. Default is ,
	string r_delim;	//Record delimiter. Default is ;
	
	vector<string> global;		//Global data
	vector<dir> directory;		//Vector of DEs
	vector<param> parameters;	//Vector of parameters

	map<int,int> unsupported;	//list unsupported types found in the file
	
	//The map of entities map<int,entity*>. First int is location as used
	//in the original file (what's defined as the "pointer" in IGES)
	entArray ents;				
	
	//Count of the entities read
	map<int,int> count;   //<entity type, count>
	
	string terminate;	//Last line in Iges file. Information only

	vector<entity*> trimmedSurf144;	//list of the trimmed surfaces (type 144)
	GLUnurbs *pNurb;				//pointer to nurb object	

	//Temporary vars for reading
	string tmpGlobal;
	dir tmpDir;   //Stores 2 lines from the directory entry
	dir zeroDir;  //To reinit tmpDir
	param tmpParam;   //Store temporary parameters before putting in the map
	BOOL first_line;    //Check that we got through all the lines in the parameter data
	BOOL globFlag; //Check that we only analyze the global parameter once


};
