/////////////////////////////////////////////////////////////////////
// Condition.cpp - Provides the functions helpful for retreiving the regex values//
// ver 1.0																     //
// Author: Vishnu Karthik.R, CSE687 - Object Oriented Design, Spring 2018
/////////////////////////////////////////////////////////////////////
/*

* Package Operations:
* -------------------
* This package contains functions used for 
* retreiving the regex values

* Required Files:
* ---------------
* Condition.h, Condition.cpp

* Maintenance History:
* --------------------

* ver 1.0 : 16 Jan 2018
* - Initial Implementation

*/

#include "Condition.h"

/////////////////////////////////////////////////////////////////////
// Sets the Date Interval pattern to the condition object
Conditions& Conditions::setDateInterval(DateTime& startDate, const DateTime& endDate)
{
	fromDate_ = startDate < endDate ? startDate : endDate;
	toDate_ = startDate > endDate ? startDate : endDate;
	return *this;
}

/////////////////////////////////////////////////////////////////////
// Sets the Name pattern to the condition object
Conditions& Conditions::setNameRegex(const std::string& regex)
{
	nameRegex_ = regex;
	return *this;
}

/////////////////////////////////////////////////////////////////////
// Sets the Description pattern to the condition object
Conditions& Conditions::setDescriptionRegex(const std::string& regex)
{
	descriptionRegex_ = regex;
	return *this;
}

/////////////////////////////////////////////////////////////////////
// Sets the Category pattern to the condition object
Conditions& Conditions::setCategory(const std::string& regex)
{
	categoryRegex_ = regex;
	return *this;
}

/////////////////////////////////////////////////////////////////////
// - Send matching keys for the regex pattern
Conditions& Conditions::setFilePath(const std::string& regex)
{
	filePathRegex_ = regex;
	return *this;
}