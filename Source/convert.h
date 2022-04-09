///////////////////////////////////////////////////////////////////////////
// FILE: convert.h
// DESCRIPTION: Template function to convert strings to any type
// Type is defined by calling the function with the type in < >
// Example:  stringTo<int>("125") converts the string "125" to an int
// with the same number
// Input: a string
// Output: the type selected for the template
///////////////////////////////////////////////////////////////////////////

#ifndef __CONVERT_H_
#define __CONVERT_H_

#pragma once

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

template <class T>
T stringTo(const std::string& s)
{
  T t;
  if(s=="        ") t=0; //An 8 space blank.Longest space expected in an Iges file
  else { 
		std::istringstream iss(s);
		iss >> t;
  }
  
  return t;
};


#endif