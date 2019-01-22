#pragma once
/////////////////////////////////////////////////////////////////////
// Persistence.h - Provides the functions helpful for saving & readng the data in xml//
// ver 1.0																     //
// Author: Vishnu Karthik.R, CSE687 - Object Oriented Design, Spring 2018
/////////////////////////////////////////////////////////////////////
/*

* Package Operations:
* -------------------
* This package contains functions helpful for saving & readng the data in xml

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* XmlDocument.h, XmlDocument.cpp
* XmlElement.h, XmlElement.cpp
* Persistence.h, Persistence.cpp
*
* Maintenance History:
* --------------------

* ver 1.0 : 16 Jan 2018
* - Initial Implementation

*/

#include "../DbCore/DbCore.h"
#include "../DbCore/PayLoadXMLTemplate.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"

using namespace XmlProcessing;

namespace NoSqlDb
{
	/////////////////////////////////////////////////////////////////////
	// Persistence class
	// - provides save and load operations
	// - does not provide editing, querying, or persistance operations

	template<typename T>
	class Persistence
	{
	public:
		using sPtr = std::shared_ptr<AbstractXmlElement>;

		Persistence(DbCore<T>& db) :_db(db) {}

		void save(const char* file = "../DbCore/NoSqlDbFile.xml");

		void loadXML(bool overRideReq = true, const char* file = "../DbCore/NoSqlDbFile.xml");
	private:
		DbCore<T>& _db;
	};

	/////////////////////////////////////////////////////////////////////
	// Save value to the XML with the default value 
	// passed on to the function being "../DbCore/NoSqlDbFile.xml"
	/////////////////////////////////////////////////////////////////////
	template<typename T>
	void Persistence<T>::save(const char* file)
	{
		FILE *databaseFile = nullptr;
		try
		{
			sPtr rootElemtag_Db = makeTaggedElement("db");
			sPtr rootElem_Db = makeDocElement(rootElemtag_Db);
			XmlDocument xDoc(rootElem_Db);
			for (auto elem : _db)
			{
				sPtr docElem_dbRec = makeTaggedElement("dbRecord");
				rootElemtag_Db->addChild(docElem_dbRec);
				sPtr docElem_Key = makeTaggedElement("key", elem.first);
				docElem_dbRec->addChild(docElem_Key);
				sPtr docElem_Value = makeTaggedElement("value");
				docElem_dbRec->addChild(docElem_Value);
				sPtr docElem_name = makeTaggedElement("name", elem.second.name());
				docElem_Value->addChild(docElem_name);
				sPtr docElem_desc = makeTaggedElement("description", elem.second.descrip());
				docElem_Value->addChild(docElem_desc);
				sPtr docElem_date = makeTaggedElement("datetime", static_cast<std::string>(elem.second.dateTime()));
				docElem_Value->addChild(docElem_date);
				sPtr docElem_child = makeTaggedElement("childCollection");
				docElem_Value->addChild(docElem_child);

				for (auto child : elem.second.children())
				{
					sPtr docElem_childval = makeTaggedElement("child", child);
					docElem_child->addChild(docElem_childval);
				}
				PayLoadXMLTemplate tp;
				tp.createPayLoadXML(docElem_dbRec, elem.second.payLoad());
			}
			fopen_s(&databaseFile, file, "w+");
			fputs(xDoc.toString().c_str(), databaseFile);
			fclose(databaseFile);
		}
		catch (std::exception)
		{
			fclose(databaseFile);
		}
	}

	/////////////////////////////////////////////////////////////////////
	// Loads value from XML to the DbElement Object
	// The override flag provides the user the convenience to override 
	// or not
	/////////////////////////////////////////////////////////////////////
	template<typename T>
	void Persistence<T>::loadXML(bool overRideReq, const char* xmlFile)
	{
		XmlDocument doc(xmlFile, XmlDocument::file);
		std::vector<sPtr> records = doc.descendents("dbRecord").select();
		for (auto tmpRec : records)
		{
			DbElement<T> elem;
			std::string key;
			std::vector<sPtr> tmpChild = tmpRec->children();
			for (auto tmpElem : tmpChild)
			{
				if (tmpElem->tag() == "key")
				{
					key = tmpElem->children()[0]->value();
				}
				else
				{
					std::vector<sPtr> pValueChildren = tmpElem->children();
					for (auto pValueChild : pValueChildren)
					{
						if (pValueChild->tag() == "name")
							elem.name(pValueChild->children()[0]->value());

						if (pValueChild->tag() == "description")
							elem.descrip(pValueChild->children()[0]->value());

						if (pValueChild->tag() == "datetime")
							elem.dateTime(pValueChild->children()[0]->value());

						if (pValueChild->tag() == "childCollection")
						{
							std::vector<sPtr> childrenCol = pValueChild->children();
							for (auto pValue : childrenCol)
							{
								elem.addChild(pValue->children()[0]->value());
							}
						}
						PayLoadXMLTemplate tp;
						tp.RetrivePayLoadfromXML(pValueChild, elem.payLoad());
					}
				}
				if (elem.dateTime() != _db.dbStore()[key].dateTime() && !overRideReq)
					continue;

				_db.dbStore()[key] = elem;
			}
		}
	}
}