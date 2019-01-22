#include "Query.h"

using namespace NoSqlDb;

#ifdef TEST_QUERY

int main()
{
	std::cout << "\n  Demonstrate Query";
	std::cout << "\n  ===================";
	std::cout << "\n";

	DbCore<std::string> db;
	DbElement<std::string> demoElem = db["DateTime"];

	std::string payLoadObj1("Test");
	demoElem.name("DbCore.h");
	demoElem.descrip("Implements NoSql database");
	demoElem.payLoad(payLoadObj1);
	db["DbCore"] = demoElem;

	Query<std::string> queryObj2(db);
	Conditions cond3, cond4;
	cond3.setNameRegex("Replaced Name");
	cond4.setNameRegex("Query");
	queryObj2.unionOn(cond3).unionOn(cond4);
	std::cout << "The filtered keys are: ";
	for (auto elem : queryObj2.getQueriedKeys())
		std::cout << elem << " ";

	Query<std::string> queryObj2(db);
	Conditions cond3, cond4;
	cond3.setNameRegex("Replaced Name");
	cond4.setNameRegex("Query");
	queryObj2.unionOn(cond3).unionOn(cond4);
	std::cout << "The filtered keys are: ";
	for (auto elem : queryObj2.getQueriedKeys())
		std::cout << elem << " ";

	Query<std::string> queryObj3(db);
	Conditions cond4;
	DateTime fromDate("Mon Jan 27 21:00:00 2018"), toDate("Mon Jan 29 21:00:00 2019");
	cond4.setDateInterval(fromDate, toDate);
	queryObj3.filterOn(cond4);

	Query<std::string> queryObj2(db);
	Conditions cond1;
	cond1.setNameRegex("Replaced Name").setDescriptionRegex("Replaced Description");
	queryObj2.filterOn(cond1);
	std::cout << "The filtered keys are: ";
	for (auto elem : queryObj2.getQueriedKeys())
	{
		std::cout << elem << " ";
	}

	return 0;
}

#endif TEST_QUERY

