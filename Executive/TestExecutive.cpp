/////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Provides the functions to be executed by Executive class//
// ver 1.0																     //
// Author: Vishnu Karthik.R, CSE687 - Object Oriented Design, Spring 2018
/////////////////////////////////////////////////////////////////////
/*

* Package Operations:
* -------------------
* This package contains functions to be executed by Executive class

* Required Files:
* ---------------
* Query.h, Query.cpp
* StringUtilities.h, StringUtilities.cpp
* TestUtilities.h, TestUtilities.cpp
* PayLoad.h, PayLoad.cpp
* Persistence.h, Persistence.cpp
*
* Maintenance History:
* --------------------

* ver 1.0 : 16 Jan 2018
* - Initial Implementation

*/

#include "../Query/Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../DbCore/PayLoad.h"
#include "../Persistence/Persistence.h"
#include "TestExecutive.h"

using namespace NoSqlDb;

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.
template<typename T>
class DbProvider
{
public:
	DbCore<T>& db() { return db_; }
private:
	static DbCore<T> db_;
};

template<typename T>
DbCore<T> DbProvider<T>::db_;

///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool NoSqlDb::testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool NoSqlDb::testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------

bool NoSqlDb::testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");

	std::cout << "\n  Creating a db element with key \"DateTime\":";

	// create database to hold payload

	DbCore<pl> db;
	DbProvider<pl> dbp;
	dbp.db() = db;

	// create some demo elements and insert into db

	DbElement<pl> demoElem;
	pl payLoadInst("../DateTime", { "Utility", "DateTime" });
	demoElem.name("DateTime.cpp");
	demoElem.descrip("Represents clock time ");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(payLoadInst);

	if (demoElem.name() != "DateTime.cpp")
		return false;
	if (demoElem.descrip() != "Represents clock time ")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (payLoadInst != demoElem.payLoad())
		return false;

	db["DateTime"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}


//----< demo second part of requirement #3 >---------------------------

bool NoSqlDb::testR3b()
{
	DbProvider<pl> dbp;
	DbCore<pl> db = dbp.db();
	DbElement<pl> demoElem = db["DateTime"];
	Utilities::title("Demonstrating Requirement #3b - Creation of Elements");
	pl payLoadObj1("../DbCore", { "Core File","Db Core" });
	demoElem.name("DbCore.h");
	demoElem.descrip("Implements NoSql database");
	demoElem.payLoad(payLoadObj1);
	db["DbCore"] = demoElem;

	if (!db.contains("DbCore"))
		return false;

	pl payLoadObj2("../Executive", { "Test File", "Test Cases" });
	demoElem.name("Executive.cpp");
	demoElem.descrip("Used for writing test cases");
	demoElem.payLoad(payLoadObj2);
	db["Executive"] = demoElem;

	pl payLoadObj3("../Persistence", { "Save File","Load File" });
	demoElem.payLoad(payLoadObj3);
	demoElem.descrip("Used for saving data");
	demoElem.name("Persistence.h");
	db["Persistence"] = demoElem;

	pl payLoadObj4("../Query", { "Select", "Union" });
	demoElem.payLoad(payLoadObj4);
	demoElem.descrip("Used for querying database");
	demoElem.name("Query.cpp");
	db["Query"] = demoElem;

	pl payLoadObj5("../XmlDocument", { "Utility","xml" });
	demoElem.payLoad(payLoadObj5);
	demoElem.descrip("Xml manipulation");
	demoElem.name("XmlDocument.cpp");
	db["XmlDocument"] = demoElem;
	dbp.db() = db;
	putLine();
	std::cout << "\n  Added elements with keys: DateTime, StringUtilities, Query, Persistence, Executive, DbCore";
	putLine();
	return true;
}


bool NoSqlDb::testR3c()
{
	testR3b();
	DbProvider<pl> dbp;
	DbCore<pl> db = dbp.db();
	DbElement<pl> demoElem = db["DateTime"];
	Utilities::title("Demonstrating Requirement #3c - Creation of Child relationship to the created elements");
	if (db.size() != 6)
		return false;
	putLine();

	std::cout << "\n  creating child relationships of elements with parent";

	db["DateTime"].children().push_back("DbCore");

	db["DbCore"].children().push_back("Persistence");
	db["DbCore"].children().push_back("Query");

	db["XmlDocument"].children().push_back("Persistence");

	db["Query"].children().push_back("Executive");
	db["Persistence"].children().push_back("Executive");
	std::cout << "\n  Created child relationships for DateTime, DbCore, XmlDocument, Query, Persistence";
	putLine();
	Persistence<pl> p1(db);
	p1.save();
	std::cout << "\n  Inserted elements, saved content and the file generated";
	std::cout << "\n  XML Content saved in - " << std::experimental::filesystem::absolute("NoSqlDbFile.xml");
	putLine();
	std::cout << "\n  The elements are LOADED into the dbElement from the XML file in this step for the upcoming requirements";
	p1.loadXML();
	putLine();
	dbp.db() = db;
	return true;
}

//----< demo requirement #4 >------------------------------------------

bool NoSqlDb::testR4a()
{
	DbProvider<pl> dbp;
	DbCore<pl> db = dbp.db();
	DbElement<pl> db1;

	bool testFlag;
	bool retflag;
	bool retval = testR4c(db1, testFlag, db, retflag);
	if (!retflag) return retval;

	//*******************************Negative scenario - key already found************************************//

	db1.name("XMLElement.h");
	db1.descrip("Functions related to parsing elements");
	db1.dateTime(DateTime("Mon Jan 25 21:00:00 2018"));
	db1.children({ "child7","child8","child3" });
	db1.payLoad(PayLoad("../XMLElement", { "categ5","categ6" }));
	putLine();
	testFlag = db.insert("XMLElement", db1); //Function used to insert dbElement into dbCore
	if (!testFlag)
	{
		return testFlag;
	}

	std::cout << "\n  Tested negative scenario - Passed";
	putLine();
	//Calls Removal function
	testFlag = testR4b(db);
	if (!testFlag)
	{
		return testFlag;
	}
	dbp.db() = db;
	return true;
}

bool NoSqlDb::testR4c(NoSqlDb::DbElement<NoSqlDb::pl> &db1, bool &testFlag, NoSqlDb::DbCore<NoSqlDb::pl> &db, bool &retflag)
{
	retflag = true;

	//********************************ADDITION OF KEY/VALUE PAIRS************************************//

	//*******************************POSITIVE SCENARIO_INSERTION ************************************//
	Utilities::title("Demonstrating Requirement #4a - ADDITION OF KEY/VALUE PAIRS");

	std::cout << "\n  INSERTION";
	DbElement<pl> db2;
	db2.name("XMLParser.cpp");
	db2.descrip("XMl Parsing techniques");
	db2.dateTime(DateTime("Mon Jan 29 21:00:00 2018"));
	db2.children({ "child1","child2","child3" });
	db2.payLoad(PayLoad("../XMLParser", { "categ1","categ2" }));

	testFlag = db.insert("XMLParser", db2); //Function used to insert dbElement into dbCore
	if (!testFlag)
	{
		return testFlag;
	}

	db1.name("XmlElementParts.h");
	db1.descrip("Contains functions to split XML");
	db1.dateTime(DateTime("Mon Jan 27 21:00:00 2018"));
	db1.children({ "child5","child6","child3" });
	db1.payLoad(PayLoad("../XmlElementParts", { "categ1","categ6" }));

	testFlag = db.insert("XmlElementParts", db1); //Function used to insert dbElement into dbCore

	if (!testFlag)
	{
		return testFlag;
	}
	std::cout << "\n  Inserted \"XmlElementParts\" and \"XMLParser\" into DbElement";
	return true;
}


//----< demo requirement #4 >------------------------------------------

bool NoSqlDb::testR4b(DbCore<pl>& db)
{
	Utilities::title("Demonstrating Requirement #4b - REMOVAL OF KEY/VALUE PAIRS");
	bool testFlag;
	//********************************REMOVAL OF KEY/VALUE PAIRS************************************//

	//*******************************POSITIVE SCENARIO_REMOVAL************************************//
	std::cout << "\n  REMOVAL \n";

	testFlag = db.remove("XMLElement"); //Function used to remove dbElement with all its relationships removed
	if (!testFlag)
	{
		return testFlag;
	}
	std::cout << "\n  \"XMLElement\" - Element removed";
	putLine();
	return true;
}

//----< demo requirement #5 >------------------------------------------

bool NoSqlDb::testR5a()
{
	Utilities::title("Demonstrating Requirement #5a- ADD CHILD RELATIONSHIP");
	DbProvider<pl> dbp;
	DbCore<pl> db = dbp.db();
	//********************************ADD RELATIONSHIP************************************//
	std::cout << "\n  ADD RELATIONSHIP:";

	db["XMLParser"].addChild("XmlElementParts").addChild("Query").addChild("DateTime");
	putLine();

	std::cout << "\n  Added \"DateTime\" and \"Query\" child relationship to XMlParser";
	putLine();
	Utilities::title("Demonstrating Requirement #5b- REMOVE CHILD RELATIONSHIP");

	std::cout << "\n  REMOVE CHILD RELATIONSHIP";
	db["XMLParser"].removeChild("Query");

	db.deleteChildRelation("DateTime", "XMLParser");
	putLine();

	std::cout << "\n  Removed \"DateTime\" and \"Query\" child relationship from XMLParser";
	putLine();
	dbp.db() = db;
	return true;
	//********************************UPDATE 
}

//----< demo requirement #5 >------------------------------------------

bool NoSqlDb::testR5b()
{
	Utilities::title("Demonstrating Requirement #5c- EDITING VALUE");
	DbProvider<pl> dbp;
	DbCore<pl> db = dbp.db();

	db["XMLParser"].name("Edited Name").descrip("Edited Description").dateTime(DateTime().now())
		.payLoad(PayLoad("Edited FP", { "Edited PL1", "Edited PL2" }));
	putLine();
	std::cout << "\n  Edited XMLParser element with edited name, edited description and edited values";
	putLine();


	Utilities::title("Demonstrating Requirement #5d- Replacing instances of Payload and also the entire value of the Element");
	putLine();
	//Entire DbElement
	DbElement<pl> elem;
	elem.name("Replaced Name").descrip("Replaced Description").dateTime(DateTime().now()).children({ "Query","DbCore" })
		.payLoad(PayLoad("Replaced FP", { "Replaced PL1", "Replaced PL2" }));
	db["XMLParser"] = elem;

	std::cout << "\n  Replaced the instance of the value element with new instance ";
	//Payload instance
	db["XMLParser"].payLoad(PayLoad("Overwritten FP1", { "Overwritten PL1", "Overwritten PL2" }));

	std::cout << "\n  Replaced with new payload instance with filepath=\"Overwritten FP1\" and category=\"Overwritten PL1,Overwritten PL2\" to XMLParser element";
	putLine();
	dbp.db() = db;
	return true;
}
//----< demo requirement #6 >------------------------------------------

bool NoSqlDb::testR6()
{
	DbProvider<pl> dbp;
	DbCore<pl> db = dbp.db();
	Query<pl> queryObj1(db);

	Utilities::title("Demonstrating Requirement #6a- Query to retrieve the value");
	DbElement<pl> record = queryObj1.valueOf("XMLParser");
	putLine();
	std::cout << "  Value retrieved for XMLParser element for XMLParser";
	putLine();
	Utilities::title("Demonstrating Requirement #6b- Query to retrieve the child collection");
	auto childCollection = queryObj1.childrenOf("XMLParser");
	putLine();
	std::cout << "  Child Collection retrieved using key for XMLParser";
	putLine();

	Utilities::title("Demonstrating Requirement #6c- Query to retrieve the keys based on regex");
	std::vector<std::string> filteredKeys = queryObj1.matchingKeys("XMLParser");
	putLine();
	std::cout << "  Passed the pattern \"XMLParser\" to the matchingKeys function. The matched keys are: ";
	for (auto elem : filteredKeys)
	{
		std::cout << elem << " ";
	}
	putLine();

	Utilities::title("Demonstrating Requirement #6d- Query to filter the dbElements based on metadata matching the regex");
	putLine();
	testR6a();
	return true;

}

bool NoSqlDb::testR6a()
{
	DbProvider<pl> dbp;
	DbCore<pl> db = dbp.db();
	Query<pl> queryObj2(db);
	Conditions cond1;
	cond1.setNameRegex("Replaced Name").setDescriptionRegex("Replaced Description");
	queryObj2.filterOn(cond1);
	std::cout << "  The filtered keys are: ";
	for (auto elem : queryObj2.getQueriedKeys())
	{
		std::cout << elem << " ";
	}
	putLine();

	Utilities::title("Demonstrating Requirement #6e- Query to filter the dbElements based on datetime");
	putLine();
	Query<pl> queryObj3(db);
	Conditions cond4;
	DateTime fromDate("Mon Jan 27 21:00:00 2018"), toDate("Mon Jan 29 21:00:00 2019");
	cond4.setDateInterval(fromDate, toDate);
	queryObj3.filterOn(cond4);
	std::cout << "  Passed the pattern \"Mon Jan 27 21:00 : 00 2018\" and \"Mon Jan 29 21:00 : 00 2019\" to the matchingKeys function. ";
	putLine();
	std::cout << "  The filtered keys are : ";
	for (auto elem : queryObj3.getQueriedKeys())
	{
		std::cout << elem << " ";
	}
	putLine();
	return true;
}

//----< demo requirement #7 >------------------------------------------


bool NoSqlDb::testR7()
{
	DbProvider<pl> dbp;
	DbCore<pl> db = dbp.db();
	Query<pl> queryObj1(db);
	Utilities::title("Demonstrating Requirement #7a- AND Operation");
	putLine();
	Conditions cond1, cond2;
	DateTime toDt("Mon Jan 29 21:00:00 2017");
	cond1.setDateInterval(toDt);
	cond2.setNameRegex("Replaced Name");
	queryObj1.filterOn(cond1); //function invoke
	std::cout << "  Ran two conditions on Date(Mon Jan 29 21:00:00 2017) and Name(Replaced Name) using filterOn condition.\n";
	std::cout << "  The filtered keys are: ";
	for (auto elem : queryObj1.getQueriedKeys())
		std::cout << elem << " ";
	putLine();
	Utilities::title("Demonstrating Requirement #7b- OR Operation");
	putLine();
	Query<pl> queryObj2(db);
	Conditions cond3, cond4;
	cond3.setNameRegex("Replaced Name");
	cond4.setNameRegex("Query");
	queryObj2.unionOn(cond3).unionOn(cond4);
	std::cout << "  Ran two separate conditions on Name(Replaced Name) and Name(Query) using unionOn condition.";
	putLine();
	std::cout << "  The filtered keys are: ";
	for (auto elem : queryObj2.getQueriedKeys())
		std::cout << elem << " ";
	putLine();

	Utilities::title("Demonstrating Requirement #7c- Search Category");
	Query<pl> queryObj3(db);
	putLine();
	//Registers the callbacks for searching //This can be done for different types  //making the search functionality generic 
	//in the query class
	db.registerSearchFn(&pl::checkFilePath).registerSearchFn(&pl::checkPatternInCategory);
	std::cout << "  Registered checkFilePath and checkPatternInCategory function for templated types in the DBcore to perform search";
	Conditions cond5;
	putLine();
	cond5.setCategory("Overwritten PL1");
	queryObj3.unionOn(cond5); //function invoke
	std::cout << "  The filtered keys are: ";
	for (auto elem : queryObj3.getQueriedKeys())
		std::cout << elem << " ";
	putLine();
	return true;
}

//----< demo requirement #8 >------------------------------------------

bool NoSqlDb::testR8()
{
	DbProvider<pl> dbp;
	DbCore<pl> db = dbp.db();
	Query<pl> queryObj1(db);

	Utilities::title("Demonstrating Requirement #8a- SAVE Operation");
	putLine();
	Persistence<pl> test(db);
	test.save();

	std::cout << "  Files Saved successfully";
	putLine();

	std::cout << "  \"" << std::experimental::filesystem::absolute("NoSqlDbFile.xml") << "\" " << "- xml contents in this path";
	putLine();
	Utilities::title("Demonstrating Requirement #8b- LOAD Operation");
	test.loadXML();
	putLine();
	std::cout << "  Files Loaded succesfully";
	putLine();

	std::cout << "  ***********************The final DB values saved in the XML after the test case execution***********************";
	putLine();
	showDb(db);
	putLine();
	putLine();
	std::cout << "  *******************XML File Path in the windows file system********************";
	putLine();

	std::cout << std::experimental::filesystem::absolute("NoSqlDbFile.xml");
	putLine();
	return true;
}

//----< demo requirement #9 >------------------------------------------
bool NoSqlDb::testR9()
{
	Utilities::title("Demonstrating Requirement #9 - Payload with the said attributes created--Visual examination needed");
	putLine();
	return true;
}

//----< Registers test function into test executive >------------------------------------------
void NoSqlDb::testExecutiveSet(TestExecutive &ex)
{
	TestExecutive::TestStr ts5b{ testR5b, "Editing of key values" };
	TestExecutive::TestStr ts6{ testR6, "Queries" };
	TestExecutive::TestStr ts7{ testR7, "Filtering and Joining Queries" };
	TestExecutive::TestStr ts8{ testR8, "Persistence" };
	TestExecutive::TestStr ts9{ testR9, "Payload class" };
	//// register test structures with TestExecutive instance, ex

	ex.registerTest(ts5b);
	ex.registerTest(ts6);
	ex.registerTest(ts7);
	ex.registerTest(ts8);
	ex.registerTest(ts9);
	if (ex.doTests()) 
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";
	int c;
	std::cin >> c;
	Utilities::putline(2);
}

int main()
{
	Utilities::Title("Testing NoSqlDb - He said, she said database");
	Utilities::putline();
	TestExecutive ex;
	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<std::string>" };
	TestExecutive::TestStr ts3c{ testR3c, "Implemented generic key/value in-memory database" };
	TestExecutive::TestStr ts4{ testR4a, "Addition & deletion of key value pairs" };
	TestExecutive::TestStr ts5a{ testR5a, "Addition & Deletion of relationships" };
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3c);
	ex.registerTest(ts4);
	ex.registerTest(ts5a);
	// define test structures with test function and message
	testExecutiveSet(ex);
	return 0;
}


