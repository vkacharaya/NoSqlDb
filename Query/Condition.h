#pragma once
///////////////////////////////////////////////////////////////////////
// Condition.h - provides querying features from db object           //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Vishnu Karthik Ravindran							 //
//              vravindr@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes:
* //- TestExecutor    Executes single test in the context of a try-catch block
* //- TestExecutive   Uses TestExecutor to run a sequence of tests
*
* Required Files:
* ---------------
*   Query.h, Query.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 26 Jan 2018
* - first release
*
* Notes:
* ------
* - Designed to provide all functionality in header file.
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/
#include <regex>
#include "../DateTime/DateTime.h"

class Conditions
{
public:
	Conditions() :fromDate_("Mon Jan 01 00:00:00 1990") {
	}
	std::regex& getName() { return nameRegex_; }
	std::regex& getDescription() { return descriptionRegex_; }
	DateTime& getFromDate() { return fromDate_; }
	DateTime& getToDate() { return toDate_; }
	std::regex& getCategory() { return categoryRegex_; }
	std::regex& getFilePath() { return filePathRegex_; }

	Conditions& setNameRegex(const std::string& regex);
	Conditions& setDescriptionRegex(const std::string& regex);
	Conditions& setDateInterval(DateTime& startDate, const DateTime& endDate = DateTime());
	Conditions& setCategory(const std::string& regex);
	Conditions& setFilePath(const std::string& regex);

private:
	std::regex nameRegex_;
	std::regex descriptionRegex_;
	std::regex categoryRegex_;
	std::regex filePathRegex_;
	DateTime fromDate_;
	DateTime toDate_;
};
