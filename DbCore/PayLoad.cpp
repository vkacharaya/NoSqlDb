////////////////////////////////////////////////////////////////////////////
// PayLoad.cpp - Provides an overall utilities package related to payload //
//                                                                        //
// ver 1.0																  //
// Author: Vishnu Karthik.R, CSE687 - Object Oriented Design, Spring 2018 //
////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains class to manipulate payload object which
* will be passed on to the DBElement object and also test stubs for Payload.

* Required Files:
* ---------------
* Condition.h, Condition.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 16 Jan 2018
* - Initial Implementation

*/

#include "PayLoad.h"

namespace NoSqlDb
{
	/////////////////////////////////////////////////////////////////////
	// - Operator overloaded for output stream
	std::ostream& operator<<(std::ostream& out, PayLoad payloadObj)
	{
		std::vector<std::string>& categories = payloadObj.getCategories();
		out << "The listed Categories are: ";
		for (size_t i = 0;i < categories.size();i++)
		{
			out << categories[i].c_str() << (i == categories.size() - 1 ? "" : ", ");
		}
		out << "|| FilePath: " << payloadObj.getFilePath();
		return out;
	}

	/////////////////////////////////////////////////////////////////////
	// Compares the pattern in the category
	bool PayLoad::checkPatternInCategory(Conditions& cond)
	{
		if (cond.getCategory()._Empty())
			return true;
		for (auto& elem : categories_)
		{
			if (regex_search(elem, cond.getCategory()))
				return true;
		}
		return false;
	}
	/////////////////////////////////////////////////////////////////////
	// - check the pattern in the file path
	bool PayLoad::checkFilePath(Conditions& cond)
	{
		if (cond.getFilePath()._Empty())
			return true;
		return regex_search(path_, cond.getFilePath());
	}

	/////////////////////////////////////////////////////////////////////
	// Inserts single category to the categor list
	void PayLoad::pushFileCategory(const std::string& category)
	{
		if (!containsCateg(category))
		{
			categories_.push_back(category);
		}
	}

	/////////////////////////////////////////////////////////////////////
	// - operator overloaded for comparing two PayLoad objects
	bool PayLoad::operator==(const PayLoad& rOperand)
	{
		if (path_ != rOperand.path_)
			return false;

		bool listMismatch = std::mismatch(categories_.begin(), categories_.end(), rOperand.categories_.begin()).first != categories_.end();
		if (listMismatch)
			return false;

		return true;
	}
	/////////////////////////////////////////////////////////////////////
	// - operator overloaded for comparing two PayLoad objects
	bool PayLoad::operator!=(const PayLoad& rOperand)
	{
		if (path_ == rOperand.path_)
		{
			bool listMismatch = std::mismatch(categories_.begin(), categories_.end(), rOperand.categories_.begin()).first != categories_.end();
			if (listMismatch)
				return true;
		}
		return false;
	}
	/////////////////////////////////////////////////////////////////////
	// - checks whether the categopry is present or not
	bool PayLoad::containsCateg(const std::string& category)
	{
		auto iter = std::find(categories_.begin(), categories_.end(), category);
		return iter != categories_.end();
	}

	#ifdef TEST_PAYLOAD

		int main()
		{
			std::cout << "\n  Demonstrate PayLoad";
			std::cout << "\n  ===================";
			std::cout << "\n";


			PayLoad payLoadObj1("TestPayLoad", { "Category1","Category2" });

			//Contains Category Check
			bool categValue = payLoadObj1.containsCateg("Category1");
			if (categValue)
				std::cout << "\n  " << "Yeah Found the value";
			else
				std::cout << "\n  " << "Not Found";

			//Pushes the category
			payLoadObj1.pushFileCategory("Category2");
			if (categValue)
				std::cout << "\n  " << "Yeah Found the value";
			else
				std::cout << "\n  " << "Not Found";

			//check File Path using regex
			Conditions cond1;
			cond1.setCategory("Category1");
			categValue = payLoadObj1.checkPatternInCategory(cond1);
			if (categValue)
				std::cout << "\n  " << "Yeah Found the value";
			else
				std::cout << "\n  " << "Not Found";

			//check File Path using regex
			Conditions cond1;
			cond1.setFilePath("TestPayLoad");
			categValue = payLoadObj1.checkFilePath(cond1);
			if (categValue)
				std::cout << "\n  " << "Yeah Found the value";
			else
				std::cout << "\n  " << "Not Found";

			std::cout << "\n\n";
			return 0;
		}

	#endif TEST_PAYLOAD

}