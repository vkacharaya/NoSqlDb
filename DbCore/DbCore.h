#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype							//
// ver 1.0																    //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018       //
// Author: Vishnu Karthik.R, CSE687 - Object Oriented Design, Spring 2018
/////////////////////////////////////////////////////////////////////
/*

* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  Those are left as
*   exercises for students.
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type.
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
*
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
* ver 1.0 : 10 Jan 2018
* - Added remove function

*/

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "../DateTime/DateTime.h"
#include "PayLoadUtils.h"

namespace NoSqlDb
{
	/////////////////////////////////////////////////////////////////////
	// DbElement class
	// - provides the value part of a NoSql key-value database

	template<typename T>
	class DbElement
	{
	public:
		using Key = std::string;
		using Children = std::vector<Key>;

		// methods to get and set DbElement fields

		std::string& name() { return name_; }
		std::string name() const { return name_; }
		DbElement<T>& name(const std::string& name) { name_ = name; return *this; }

		std::string& descrip() { return descrip_; }
		std::string descrip() const { return descrip_; }
		DbElement<T>& descrip(const std::string& name) { descrip_ = name; return *this; }

		DateTime& dateTime() { return dateTime_; }
		DateTime dateTime() const { return dateTime_; }
		DbElement<T>& dateTime(const DateTime& dateTime) { dateTime_ = dateTime; return *this; }

		Children& children() { return children_; }
		Children children() const { return children_; }
		DbElement<T>& children(const Children& children) { children_ = children; return *this; } //There are chances to have duplicate childrens - No Validation done

		DbElement<T>& addChild(const std::string& key);
		DbElement<T>& removeChild(const std::string& key);

		T& payLoad() { return payLoad_; }
		T payLoad() const { return payLoad_; }
		DbElement<T>& payLoad(const T& payLoad) { payLoad_ = payLoad; return *this; }

	private:
		std::string name_;
		std::string descrip_;
		DateTime dateTime_;
		Children children_;
		T payLoad_;
	};

	/////////////////////////////////////////////////////////////////////
	// DbCore class
	// - provides core NoSql db operations
	// - does not provide editing, querying, or persistance operations


	template <typename T>
	class DbCore
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;
		using Children = Keys;
		using DbStore = std::unordered_map<Key, DbElement<T>>;
		using iterator = typename DbStore::iterator;

		// methods to access database elements
		Keys keys();
		bool contains(const Key& key);
		size_t size();
		void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
		DbElement<T>& operator[](const Key& key);
		DbElement<T> operator[](const Key& key) const;
		typename iterator begin() { return dbStore_.begin(); }
		typename iterator end() { return dbStore_.end(); }

		//insertion and removal

		bool insert(const std::string& newKey, const DbElement<T>& newRecord);
		bool remove(const std::string& key, bool isKeyRemovalNeeded = true);
		bool deleteChildRelation(const std::string& childkey, const std::string& parentKey = "");

		// methods to get and set the private database hash-map storage

		DbStore& dbStore() { return dbStore_; }
		DbStore dbStore() const { return dbStore_; }
		void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

		//This function registers the search function whihc has to be executed'
		//during querying operation
		DbCore<T>& registerSearchFn(bool(T::* fpr1)(Conditions& reg)) {
			searchFunctions_.push_back(fpr1);
			return *this;
		}
		std::vector<bool(T:: *)(Conditions& reg)>& getSearchFn() { return searchFunctions_; }

	private:
		bool deleteFromChildColl(const std::string& parentKey, const std::string& childDeleteKey);
		std::vector<bool(T:: *)(Conditions& reg)> searchFunctions_;
		DbStore dbStore_;
		bool doThrow_ = false;
	};

	/////////////////////////////////////////////////////////////////////
	// - Adding child to the DB elemnet using key
	template<typename T>
	DbElement<T>& DbElement<T>::addChild(const std::string& key)
	{
		children().push_back(key);
		return *this;
	}

	/////////////////////////////////////////////////////////////////////
	// - Removes child from the dbElement using key
	template<typename T>
	DbElement<T>& DbElement<T>::removeChild(const std::string& key)
	{
		Children& childRef = children();
		auto ir = std::find(childRef.begin(), childRef.end(), key);

		if (ir != childRef.end())
			childRef.erase(ir);

		return *this;
	}

	/////////////////////////////////////////////////////////////////////
	// - Adding element into the DB
	template<typename T>
	bool DbCore<T>::insert(const std::string& newKey, const DbElement<T>& newRecord)
	{
		try
		{
			if (contains(newKey))
			{
				throw std::exception("Item with the same key already found.");
			}
			DbStore& dbRecords = dbStore();
			DbElement<T>& demoElem = dbRecords[newKey];
			demoElem.name(newRecord.name());
			demoElem.descrip(newRecord.descrip());
			demoElem.dateTime(newRecord.dateTime());
			demoElem.children(newRecord.children());
			demoElem.payLoad(newRecord.payLoad());
			return true;
		}
		catch (std::exception& ex)
		{
			std::cout << "Could't insert the data. Please find the exception message: " << ex.what();
			return false;
		}
	}

	/////////////////////////////////////////////////////////////////////
	// - remove the element using the key and also removes the link using 
	// isKeyRemovalNeeded flag
	template<typename T>
	bool DbCore<T>::remove(const std::string& key, bool isKeyRemovalNeeded)
	{
		try
		{
			dbStore().erase(key); //C++ 11 feature
			if (isKeyRemovalNeeded)
			{
				deleteChildRelation(key, "");
			}
			return true;
		}
		catch (std::exception& ex)
		{
			std::cout << "Could not delete the element. Please find the detailed exception: " << ex.what();
			return false;
		}
	}

	/////////////////////////////////////////////////////////////////////
	// - Delete child relation using the keys 
	template<typename T>
	bool DbCore<T>::deleteChildRelation(const std::string& childkey, const std::string& parentKey)
	{
		try
		{
			if (parentKey != "")
			{
				deleteFromChildColl(parentKey, childkey);
			}
			else
			{
				for (auto dbs : dbStore())
				{
					deleteFromChildColl(dbs.first, childkey); //Can handle separately if it returns false
				}
			}
			return true;
		}
		catch (std::exception& ex)
		{
			std::cout << "Could not break the relationship. Please find the detailed exception: " << ex.what();
			return false;
		}
	}

	/////////////////////////////////////////////////////////////////////
	// - Delete from child collection using the keys passed
	template<typename T>
	bool DbCore<T>::deleteFromChildColl(const std::string& parentKey, const std::string& childDeleteKey)
	{
		try
		{
			Children& childColl = dbStore()[parentKey].children();

			std::vector<std::string>::iterator ir = std::find(childColl.begin(), childColl.end(), childDeleteKey);
			if (ir != childColl.end())
				childColl.erase(ir);

			return true;
		}
		catch (std::exception& ex)
		{
			throw std::exception(ex.what());
		}
	}

	/////////////////////////////////////////////////////////////////////
	// - check whether the value is present or not
	template<typename T>
	bool DbCore<T>::contains(const Key& key)
	{
		iterator iter = dbStore_.find(key);
		return iter != dbStore_.end();
	}

	/////////////////////////////////////////////////////////////////////
	// - Retrieve the keys of the element
	template<typename T>
	typename DbCore<T>::Keys DbCore<T>::keys()
	{
		DbCore<T>::Keys dbKeys;
		DbStore& dbs = dbStore();
		size_t size = dbs.size();
		dbKeys.reserve(size);
		for (auto item : dbs)
		{
			dbKeys.push_back(item.first);
		}
		return dbKeys;
	}

	/////////////////////////////////////////////////////////////////////
	// - Retrieves the size of the dbStore
	template<typename T>
	size_t DbCore<T>::size()
	{
		return dbStore_.size();
	}

	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes non-const db objects
	*  - In order to create a key-value pair in the database like this:
	*      db[newKey] = newDbElement
	*    we need to index with the new key and assign a default element.
	*    That will be replaced by newDbElement when we return from operator[]
	*  - However, if we want to index without creating new elements, we need
	*    to throw an exception if the key does not exist in the database.
	*  - The behavior we get is determined by doThrow_.  If false we create
	*    a new element, if true, we throw. Creating new elements is the default
	*    behavior.
	*/
	template<typename T>
	DbElement<T>& DbCore<T>::operator[](const Key& key)
	{
		if (!contains(key))
		{
			if (doThrow_)
				throw(std::exception("key does not exist in db"));
			else
				return (dbStore_[key] = DbElement<T>());
		}
		return dbStore_[key];
	}

	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes const db objects
	*/
	template<typename T>
	DbElement<T> DbCore<T>::operator[](const Key& key) const
	{
		if (!contains(key))
		{
			throw(std::exception("key does not exist in db"));
		}
		return dbStore_[key];
	}

	/////////////////////////////////////////////////////////////////////
	// display functions

	//----< display database key set >-----------------------------------

	template<typename T>
	void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
	{
		out << "\n  ";
		for (auto key : db.keys())
		{
			out << key << " ";
		}
	}
	//----< show record header items >-----------------------------------

	inline void showHeader(std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(30) << std::left << "DateTime";
		out << std::setw(20) << std::left << "Name";
		out << std::setw(35) << std::left << "Description";
		out << std::setw(20) << std::left << "Payload";
		out << "\n  ";
		out << std::setw(30) << std::left << "------------------------";
		out << std::setw(20) << std::left << "--------";
		out << std::setw(35) << std::left << "-----------------------";
		out << std::setw(20) << std::left << "-----------------------";
	}
	//----< display DbElements >-----------------------------------------

	template<typename T>
	void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(30) << std::left << std::string(el.dateTime());
		out << std::setw(20) << std::left << el.name();
		out << std::setw(35) << std::left << el.descrip();
		out << std::setw(20) << std::left << el.payLoad();
		typename DbElement<T>::Children children = el.children();
		if (children.size() > 0)
		{
			out << "\n    child keys: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
	}
	//----< display all records in database >----------------------------

	template<typename T>
	void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
	{
		showHeader(out);
		typename DbCore<T>::DbStore dbs = db.dbStore();
		for (auto item : dbs)
		{
			showElem(item.second, out);
		}
	}
}
