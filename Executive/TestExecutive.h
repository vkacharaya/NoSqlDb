#pragma once
/////////////////////////////////////////////////////////////////////
// TestExecutive.h - Provides the functions to be executed by Executive class//
// ver 1.0																     //
// Author: Vishnu Karthik.R, CSE687 - Object Oriented Design, Spring 2018
/////////////////////////////////////////////////////////////////////
/*

* Package Operations:
* -------------------
* This package contains functions to be executed by Executive clas

*
* Maintenance History:
* --------------------

* ver 1.0 : 16 Jan 2018
* - Initial Implementation

*/
namespace NoSqlDb
{
	using pl = PayLoad;

	//----< demo requirement #1 >------------------------------------------
	bool testR1();
	//----< demo requirement #2 >------------------------------------------
	bool testR2();
	//----< demo first part of requirement #3 >----------------------------
	bool testR3a();
	bool testR3b();
	//----< demo second part of requirement #3 >---------------------------
	bool testR3c();
	//----< demo requirement #4 >------------------------------------------
	bool testR4a();
	bool testR4c(DbElement<pl> &db1, bool &testFlag, DbCore<pl> &db, bool &retflag);
	bool testR4b(DbCore<pl>& db);
	//----< demo requirement #5 >------------------------------------------
	bool testR5a();
	bool testR5b();
	//----< demo requirement #6 >------------------------------------------
	bool testR6();
	bool testR6a();
	//----< demo requirement #7 >------------------------------------------
	bool testR7();
	//----< demo requirement #8 >------------------------------------------
	bool testR8();
	//----< demo requirement #9 >------------------------------------------
	bool testR9();
	//----< Registers test function into test executive >------------------------------------------
	void testExecutiveSet(TestExecutive &ex);
}

