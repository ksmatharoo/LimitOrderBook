#if 1

#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>

#include<iostream>
#include<fstream>
#include<string>

#include "orderPlacer.h"

using namespace orderBook;

//#define M_FILE
#if 1
BOOST_AUTO_TEST_CASE(Test_addOrder)
{
#ifdef M_FILE
	ofstream outFile;
	outFile.open("out.txt");
#else
	std::stringstream outputString;
#endif
	
		
#ifdef M_FILE
		LimitOrderBook orderBook(outFile);
#else
		LimitOrderBook orderBook(outputString);
#endif
		std::stringstream inputOrders;
		inputOrders << "order,0,sell,7,50.22\n";
		inputOrders << "order,1,sell,7,50.21\n";
		inputOrders << "order,4,buy,7,50.05\n";
		inputOrders << "order,3,buy,7,50.20\n";

		std::stringstream ExpectedOutput;
		ExpectedOutput << "   orderId orderType     price       qty\n";
		ExpectedOutput << "         3       buy     50.20         7\n";
		ExpectedOutput << "         4       buy     50.05         7\n";
		ExpectedOutput << "\n";
		ExpectedOutput << "   orderId orderType     price       qty\n";
		ExpectedOutput << "         1      sell     50.21         7\n";
		ExpectedOutput << "         0      sell     50.22         7\n";

		mainLoop(inputOrders, orderBook);

#ifdef M_FILE
		outFile.flush();
#endif
		
		BOOST_CHECK_EQUAL(outputString.str(), ExpectedOutput.str());

		BOOST_REQUIRE(outputString.str().length() == ExpectedOutput.str().length());

}
#endif
BOOST_AUTO_TEST_CASE(Test_cancelOrder)
{
	std::stringstream outputString;
	LimitOrderBook orderBook(outputString);

	std::stringstream inputOrders;
	inputOrders << "order,0,sell,7,50.20\n";
	inputOrders << "order,1,sell,7,50.05\n";
	inputOrders << "cancel,1\n";

	std::stringstream ExpectedOutput;
	ExpectedOutput << "   orderId orderType     price       qty\n";
	ExpectedOutput << "\n";
	ExpectedOutput << "   orderId orderType     price       qty\n";
	ExpectedOutput << "         0      sell     50.20         7\n";

	mainLoop(inputOrders, orderBook);


	BOOST_CHECK_EQUAL(outputString.str(), ExpectedOutput.str());

	BOOST_REQUIRE(outputString.str().length() == ExpectedOutput.str().length());

}

BOOST_AUTO_TEST_CASE(Test_AmendOrder)
{
	{
	std::stringstream outputString;
	LimitOrderBook orderBook(outputString);
	std::stringstream inputOrders;
	inputOrders << "order,0,sell,7,50.20\n";
	inputOrders << "order,1,sell,7,50.05\n";
	inputOrders << "order,2,sell,20,50.05\n";
	inputOrders << "amend,1,6\n";

	std::stringstream ExpectedOutput;
	ExpectedOutput << "   orderId orderType     price       qty\n";
	ExpectedOutput << "\n";
	ExpectedOutput << "   orderId orderType     price       qty\n";
	ExpectedOutput << "         1      sell     50.05         6\n";
	ExpectedOutput << "         2      sell     50.05        20\n";
	ExpectedOutput << "         0      sell     50.20         7\n";

	mainLoop(inputOrders, orderBook);
	BOOST_CHECK_EQUAL(outputString.str(), ExpectedOutput.str());
	BOOST_REQUIRE(outputString.str().length() == ExpectedOutput.str().length());
    }
	{
		std::stringstream outputString;
		LimitOrderBook orderBook(outputString);
		std::stringstream inputOrders;
		inputOrders << "order,0,sell,7,50.20\n";
		inputOrders << "order,1,sell,7,50.05\n";
		inputOrders << "order,2,sell,20,50.05\n";
		inputOrders << "amend,1,10\n";

		std::stringstream ExpectedOutput;
		ExpectedOutput << "   orderId orderType     price       qty\n";
		ExpectedOutput << "\n";
		ExpectedOutput << "   orderId orderType     price       qty\n";
		ExpectedOutput << "         2      sell     50.05        20\n";
		ExpectedOutput << "         1      sell     50.05        10\n";
		ExpectedOutput << "         0      sell     50.20         7\n";
		mainLoop(inputOrders, orderBook);

		BOOST_CHECK_EQUAL(outputString.str(), ExpectedOutput.str());

		BOOST_REQUIRE(outputString.str().length() == ExpectedOutput.str().length());

	}

}

BOOST_AUTO_TEST_CASE(Test_LevelQuery)
{
	{
		std::stringstream outputString;
		LimitOrderBook orderBook(outputString);
		std::stringstream inputOrders;
		inputOrders << "order,2,sell,20,50.05\n";
		inputOrders << "order,3,sell,7,50.20\n";
		inputOrders << "order,4,sell,7,50.05\n";
		inputOrders << "q,level,ask,1\n";

		std::stringstream ExpectedOutput;
		ExpectedOutput << "   orderId orderType     price       qty\n";
		ExpectedOutput << "         4      sell     50.05         7\n";
		ExpectedOutput << "   orderId orderType     price       qty\n";
		ExpectedOutput << "\n";
		ExpectedOutput << "   orderId orderType     price       qty\n";
		ExpectedOutput << "         2      sell     50.05        20\n";
		ExpectedOutput << "         4      sell     50.05         7\n";
		ExpectedOutput << "         3      sell     50.20         7\n";

		mainLoop(inputOrders, orderBook);
		BOOST_CHECK_EQUAL(outputString.str(), ExpectedOutput.str());
		BOOST_REQUIRE(outputString.str().length() == ExpectedOutput.str().length());
	}
	/*{
		std::stringstream outputString;
		LimitOrderBook orderBook(outputString);
		std::stringstream inputOrders;
		inputOrders << "order,0,sell,7,50.20\n";
		inputOrders << "order,1,sell,7,50.05\n";
		inputOrders << "order,2,sell,20,50.05\n";
		inputOrders << "amend,1,10\n";

		std::stringstream ExpectedOutput;
		ExpectedOutput << "   orderId orderType     price       qty\n";
		ExpectedOutput << "\n";
		ExpectedOutput << "   orderId orderType     price       qty\n";
		ExpectedOutput << "         2      sell     50.05        20\n";
		ExpectedOutput << "         1      sell     50.05        10\n";
		ExpectedOutput << "         0      sell     50.20         7\n";
		mainLoop(inputOrders, orderBook);

		BOOST_CHECK_EQUAL(outputString.str(), ExpectedOutput.str());

		BOOST_REQUIRE(outputString.str().length() == ExpectedOutput.str().length());

	}*/

}
#endif