///////////////////////////////////////////////////////////////////////////
// FILE: IgesRead.cpp
// DESCRIPTION: Interface for the IgesRead class.
// AUTHOR: Elias Cohenca - Technion Institute of Technology
// Class that defines the iges format for reading. This is composed
// of an array of entities, each linked to its original directory entry
// and parameter data. To make going through the trimmed surfaces faster
// their location is also stored in a different vector
///////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Iges.h"
#include "IgesRead.h"
//Include all the supported entities
#include "entopt.h"
#include "ent100.h"
#include "ent102.h"
#include "ent110.h"
#include "ent120.h"
#include "ent124.h"
#include "ent126.h"
#include "ent128.h"
#include "ent142.h"
#include "ent144.h"
#include "ent314.h"

IMPLEMENT_DYNAMIC(IgesRead, CObject) 

//Constructor
IgesRead::IgesRead(void) : p_delim(","), r_delim(";")
{
	first_line=TRUE;    //Check that we got through all the lines in the parameter data
	globFlag=TRUE;		//Check that we only analyze the global parameter once
	
	entity* p = new entopt(-1, dir(),param(),&ents,pNurb);  //Create helper entity for program options
	ents.insert(make_pair(0,p));
}
//Destructor
IgesRead::~IgesRead(void)
{
	for(entArray_it it=ents.begin();it!=ents.end();it++)
		delete it->second;
}
//Receives one line from the file reader and processes it
BOOL IgesRead::loadLine(const string& line)
{
	if((int)line.size()<73) return FALSE;
	if(line[72] == 'S')	//Start
	{
		start.append(line.begin(),line.end()-8);
		return TRUE;
	}
	if(line[72] == 'G')	//Global
	{
		string tmp(line.begin(),line.end()-8);
		tmpGlobal.append(tmp);
		return TRUE;
	}
	if(line[72] == 'D')	//Directory
	{
		if(globFlag==TRUE)
		{
			int holStrSz1=0;
			int	holStrSz2=0;
			string::size_type rPos=1;  // can't be less than one anyways
			string::size_type delim=0;

			string::size_type firstDelim=tmpGlobal.find_first_of(p_delim,0);
			string::size_type pPos=tmpGlobal.find_first_of("H",0);	//p_delim position
			if(firstDelim>pPos)  //check that there are values for p and r delim
			{
				holStrSz1=stringTo<int>(tmpGlobal.substr(0,pPos));
				p_delim=tmpGlobal.substr(pPos+1,holStrSz1);
				delim=tmpGlobal.find_first_of(p_delim,pPos + holStrSz1 + 1);
				rPos=tmpGlobal.find_first_of("H",pPos + holStrSz1 + 1);
				holStrSz2=stringTo<int>(tmpGlobal.substr(delim+1,rPos - delim - 1));
				r_delim=tmpGlobal.substr(rPos+1,holStrSz2);
			}

			int sz=0;
			string::size_type HPos=0;
			string::size_type delimPos=tmpGlobal.find_first_of(p_delim,rPos+holStrSz2);
			string::size_type nextDelimPos=0;
			
			while(delimPos<tmpGlobal.size()-1)
			{
				HPos=tmpGlobal.find_first_of("H",delimPos);
				nextDelimPos=tmpGlobal.find_first_of(p_delim,delimPos+1);
				if(HPos>nextDelimPos){
						global.push_back(tmpGlobal.substr(delimPos+1,nextDelimPos-delimPos-1));
						delimPos=nextDelimPos;
						continue;
				}

				sz=stringTo<int>(tmpGlobal.substr(delimPos+1,HPos-delimPos-1));
				
				global.push_back(tmpGlobal.substr(HPos+1,sz));
				delimPos=nextDelimPos;
			}
			globFlag=FALSE;		//if we are in 'D', we are past global,but check only once
		}

		if(stringTo<int>(line.substr(73,7))%2)
		{
		tmpDir.entityTypeNumber=stringTo<int>(line.substr(0,8));
		tmpDir.parameterData=stringTo<int>(line.substr(8,8));
		tmpDir.Structure=stringTo<int>(line.substr(16,8));
		tmpDir.lineFontPattern=stringTo<int>(line.substr(24,8));
		tmpDir.Level=stringTo<int>(line.substr(32,8));
		tmpDir.View=stringTo<int>(line.substr(40,8));
		tmpDir.transformationMatrix=stringTo<int>(line.substr(48,8));
		tmpDir.labelDisplay=stringTo<int>(line.substr(56,8));
		tmpDir.statusNumber=line.substr(64,8);
		tmpDir.blankStatus=stringTo<int>(tmpDir.statusNumber.substr(0,2));
		tmpDir.subordinateEntitySwitch=stringTo<int>(tmpDir.statusNumber.substr(2,2));
		tmpDir.entityUseFlag=stringTo<int>(tmpDir.statusNumber.substr(4,2));
		tmpDir.hierarchy=stringTo<int>(tmpDir.statusNumber.substr(6,2));
		tmpDir.sequenceNumber1=stringTo<int>(line.substr(73,7));
		}
		else {
		tmpDir.entityTypeNumberLine2=stringTo<int>(line.substr(0,8));
		tmpDir.lineWeightNumber=stringTo<int>(line.substr(8,8));
		tmpDir.colorNumber=stringTo<int>(line.substr(16,8));
		tmpDir.parameterLineCount=stringTo<int>(line.substr(24,8));
		tmpDir.formNumber=stringTo<int>(line.substr(32,8));
		tmpDir.Reserved1=stringTo<int>(line.substr(40,8));
		tmpDir.Reserved2=stringTo<int>(line.substr(48,8));
		tmpDir.entityLabel=line.substr(56,8);
		tmpDir.entitySubscriptNumber=stringTo<int>(line.substr(64,8));
		tmpDir.sequenceNumber2=stringTo<int>(line.substr(73,7));
		
		//After reading second line put it all in directory
		directory.push_back(tmpDir);
		tmpDir=zeroDir;
		}
		return TRUE;
	}

	if(line[72] == 'P')	//Parameter
	{
		vector<string> tokens;
		tokens.clear();
		Tokenize(line.substr(0,64),tokens, p_delim+r_delim+" ");
		vector<string>::iterator iter = tokens.begin();
		if(first_line==TRUE)
		{
			tmpParam.entityTypeNumber=stringTo<int>(*tokens.begin());
			first_line=FALSE;
			iter++;
			tmpParam.pToDir=stringTo<int>(line.substr(65,7));
			tmpParam.index=stringTo<int>(line.substr(73,7));
		}
		for( ; iter != tokens.end(); iter++ )   
		{
			tmpParam.data.push_back(stringTo<GLfloat>(*iter));			
		}
		
		if(!(line.find_first_of(r_delim)==line.npos))
		{
			parameters.push_back(tmpParam);
			first_line=TRUE;
			tmpParam.zero();
			vector<param>::iterator iter=parameters.end()-1;
			switch(iter->entityTypeNumber)
			{
				case 100:
				{
					entity* p = new ent100(100, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[100];
					break;
				}
				case 102:
				{
					entity* p = new ent102(102, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[102];
					break;
				}
				case 110:
				{
					entity* p = new ent110(110, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[110];
					break;
				}
				case 120:
				{
					entity* p = new ent120(120, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[120];
					break;
				}
				case 124:
				{
					entity* p = new ent124(124, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[124];
					break;
				}
				case 126:
				{
					entity* p = new ent126(126, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[126];
					break;
				}
				case 128:
				{
					entity* p = new ent128(128, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[128];
					break;
				}
				case 142:
				{
					entity* p = new ent142(142, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[142];
					break;
				}
				case 144:
				{
					entity* p = new ent144(144, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[144];
					trimmedSurf144.push_back(p);
					break;
				}
				case 314:
				{
					entity* p = new ent314(314, directory[(iter->pToDir-1)/2], *iter, &ents, pNurb);
					p->process();
					ents.insert(make_pair(directory[(iter->pToDir-1)/2].sequenceNumber1,p));
					++count[314];
					break;
				}
				default:
					++unsupported[iter->entityTypeNumber];
			}
		}
		return TRUE;
	}
	if(line[72] == 'T')	//Terminate
	{
		terminate.append(line.begin(),line.end()-8);
		return TRUE;
	}

	return FALSE;
}
//Tokenizes a string by finding a delimiter
void IgesRead::Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

//Reinitializes all data in the class. Used when opening a new file
//int met: method
//float tol: tesselation tolerance
void IgesRead::reInit(int met, float tol)
{
	start.clear();

	p_delim.clear();
	r_delim.clear();
	
	global.clear();
	directory.clear();
	parameters.clear();

	unsupported.clear();	
	
	for(entArray_it it=ents.begin();it!=ents.end();it++)
		delete it->second;
	ents.clear();
	
	entity* p = new entopt(-1, dir(),param(),&ents,pNurb);  //Create helper entity for program options
	ents.insert(make_pair(0,p));

	entArray_it entIt=ents.find(0);
	if(met==0)
	{
		BOOL m=TRUE;
		entIt->second->setData(&m,0);		
	}
	else if(met!=0)
	{
		BOOL m=FALSE;
		entIt->second->setData(&m,0);
	}
	entIt->second->setData(&tol,1);

	count.clear();   
	
	terminate.clear();

	trimmedSurf144.clear();   
	
	//Temporary vars for reading
	tmpGlobal.clear();
	tmpDir=zeroDir;   
	
	tmpParam.zero();   //Store temporary parameters before putting in the map
	first_line=TRUE;    //Check that we got through all the lines in the parameter data
	globFlag=TRUE; //Check that we only analyze the global parameter once
	p_delim=",";
	r_delim=";";
}