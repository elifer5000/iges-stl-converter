///////////////////////////////////////////////////////////////////////////
// FILE: entity.cpp
// DESCRIPTION: Parent Class definition for an entity entry in an IGES file
// AUTHOR: Elias Cohenca - Technion Institute of Technology
//
// This class serves as a starting point for all entity types for the program.
// Since all different entities derive from this class, we can store the
// different entities in one array (in the IgesRead class) and access them
// This also ensures that we can add new entities in the future, if needed.
///////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "entity.h"

//Default constructor
entity::entity(void)
{
}