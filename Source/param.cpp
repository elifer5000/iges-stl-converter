// FILE: param.cpp
// DESCRIPTION: Class definition for a parameter entry in an IGES file
// AUTHOR: Elias Cohenca - Technion Institute of Technology
//
// This class saves all the information read from a parameter entry in an IGES file
// It is used in the IgesRead class as part of the structure that reads the data.
// A directory entry will be referred as a DE
// A parameter entry will be referred as a PE
#include "StdAfx.h"
#include "param.h"

//Default constructor with initializers
param::param(void) : pToDir(0), index(0), entityTypeNumber(0)
{
}

//Destructor
param::~param(void)
{
}
