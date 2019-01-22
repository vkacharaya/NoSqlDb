#pragma once
/////////////////////////////////////////////////////////////////////////////////
// PayLoadUtils.h - Provides utilities to be used bu various packages across   //
// the project.																   //
// ver 1.0																       //
// Author: Vishnu Karthik.R, CSE687 - Object Oriented Design, Spring 2018      //
/////////////////////////////////////////////////////////////////////////////////
/*
/*

* Package Operations:
* -------------------
* This package contains functions to invoke the function member pointers
* , Lambda and functor.

* Required Files:
* ---------------
* PayLoad.h, PayLoad.cpp
* Condition.h, Condition.cpp
*
* Maintenance History:
* --------------------
*
* ver 1.0 : 16 Jan 2018
* - Initial Implementation

*/

#include "PayLoad.h"
#include "../Query/Condition.h"

namespace NoSqlDb
{
	//////////////////////////////////////////////////////////////////////////
	//Invokes member function pointers using type inference
	//in templates
	template<typename T, typename functionRef>
	inline bool execute(T payload, functionRef fnCallBk, Conditions& pattern)
	{
		return (payload.*fnCallBk)(pattern);
	}
}
