#pragma once
///////////////////////////////////////////////////////////////////////
// Query.h - provides querying features from db object               //
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
*   DbCore.h, DbCore.cpp
*   XmlDocument.h, XmlDocument.cpp
*   XmlElement.h, XmlElement.cpp
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
#include "../DbCore/DbCore.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include <vector>
#include <regex>
#include <memory>
#include <iostream>
#include <functional>
#include "Condition.h"
#include <filesystem>

using namespace XmlProcessing;

namespace NoSqlDb
{
	template<typename T>
	class Query
	{
	public:
		Query<T>(DbCore<T>& db) : _db(db) {}

		DbCore<T>& getDBCopy() { return _db; };

		DbElement<T> valueOf(const std::string& key);

		std::vector<DbElement<T>> childrenOf(const std::string &key);

		std::vector<std::string> matchingKeys(const std::string& keyPattern);

		Query<T>& filterOn(Conditions& cond);

		Query<T>& unionOn(Conditions& cond);

		std::vector<std::string>& getQueriedKeys() { return queriedKeys_; };

		Query<T>& updateDbObj();

	private:
		std::vector<std::string> queriedKeys_;
		DbCore<T>& _db;
		bool shouldPushKeys(const DbElement<T>& dbElem, Conditions& cond);
	};

	/////////////////////////////////////////////////////////////////////
	// Filters based on the condition passed and returns the vector of keys
	template<typename T>
	Query<T>& Query<T>::filterOn(Conditions& cond)
	{
		DbElement<T> dbElem;
		if (queriedKeys_.empty())
		{
			for (auto elem : _db.dbStore())
			{
				if (shouldPushKeys(elem.second, cond))
					queriedKeys_.push_back(elem.first);
			}
		}
		else
		{
			for (size_t i = 0; i < queriedKeys_.size(); i++)
			{
				dbElem = _db.dbStore()[queriedKeys_[i]];
				if (shouldPushKeys(dbElem, cond))
					queriedKeys_.erase(queriedKeys_.begin() + i);
			}
		}
		return *this;
	}

	/////////////////////////////////////////////////////////////////////
	// This function checks whether the key should be pushed or not 
	// depending on the function pointer execution and also other checks
	// on name, description and datetime
	template<typename T>
	bool Query<T>::shouldPushKeys(const DbElement<T>& dbElem, Conditions& cond)
	{
		if (cond.getName()._Empty() || regex_search(dbElem.name(), cond.getName()))
			if (cond.getDescription()._Empty() || regex_search(dbElem.descrip(), cond.getDescription()))
				if (dbElem.dateTime() >= cond.getFromDate() && dbElem.dateTime() <= cond.getToDate())
				{
					bool tValue = true;
					//This line invokes the registered function which was done during intialization of DBCore
					for (auto fnCallBk : _db.getSearchFn())
					{
						tValue = NoSqlDb::execute(dbElem.payLoad(), fnCallBk, cond);
						if (!tValue)
						{
							break;
						}
					}
					return tValue;
				}
		return false;
	}

	/////////////////////////////////////////////////////////////////////
	// Joins based on the condition passed and returns the vector of keys
	template<typename T>
	Query<T>& Query<T>::unionOn(Conditions& cond)
	{
		for (auto elem : _db.dbStore())
		{
			if (shouldPushKeys(elem.second, cond))
				if (find(queriedKeys_.begin(), queriedKeys_.end(), elem.first) == queriedKeys_.end())
					queriedKeys_.push_back(elem.first);
		}
		return *this;
	}

	/////////////////////////////////////////////////////////////////////
	// Returns the value of the key
	template<typename T>
	DbElement<T> Query<T>::valueOf(const std::string& key)
	{
		if (_db.contains(key))
			return _db.dbStore().at(key); //Returns reference
		return DbElement<T>();
	}

	/////////////////////////////////////////////////////////////////////
	// Returns Children of the key
	template<typename T>
	std::vector<DbElement<T>> Query<T>::childrenOf(const std::string& key)
	{
		std::vector<DbElement<T>> child;
		DbElement<T> elem = valueOf(key);
		child.reserve(elem.children().size());
		for (auto childKey : elem.children())
		{
			if (_db.contains(childKey))
				child.push_back(valueOf(childKey));
		}
		return child;
	}

	/////////////////////////////////////////////////////////////////////
	// - Send matching keys for the regex pattern
	template<typename T>
	std::vector<std::string> Query<T>::matchingKeys(const std::string& keyPattern)
	{
		std::vector<std::string> matchedKeys;
		for (auto elem : _db.dbStore())
		{
			if (regex_search(elem.first, std::regex(keyPattern)))
				matchedKeys.push_back(elem.first);
		}
		return matchedKeys;
	}

	/////////////////////////////////////////////////////////////////////
	// - Updates Db Object if needed
	template<typename T>
	Query<T>& Query<T>::updateDbObj()
	{
		std::vector<std::string> keys;
		for (auto elem : _db)
		{
			if (!(find(queriedKeys_.begin(), queriedKeys_.end(), elem.first) != queriedKeys_.end()))
				keys.push_back(elem.first);
		}
		for_each(keys.begin(), keys.end(), [this](const std::string& key) {_db.remove(key, false);});
		return *this;
	}
}