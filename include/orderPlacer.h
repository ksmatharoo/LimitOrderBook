#ifndef _ORDER_PLACER_H
#define _ORDER_PLACER_H 

#include<string>
#include "order.h"
#include "orderParser.h"

namespace orderBook {

	void mainLoop(istream &input, LimitOrderBook &orderBook) {

		std::string line;
		ostream &out = orderBook.getOutputStream();
		try {
			while (std::getline(input, line)) {
				vector<string> tokens = ksm::split(line, ",");

				if (tokens.size() != 0) {
					if (tokens[0] == "exit" || tokens[0] == "quit")
						break;

					//based on the first token[0] create order
					sOrderPtr orderPtr = createOrderObject(tokens);

					if (orderPtr == nullptr)
						continue;

					OrderType orderType = orderPtr->m_orderType;

					if (orderType == e_Buy || orderType == e_Sell) {
						orderBook.addOrder(orderPtr);
					}
					else if (orderType == e_Cancel) {
						orderBook.cancelOrder(orderPtr);
					}
					else if (orderType == e_Edit) {
						orderBook.editOrder(orderPtr);

					}
					else if (orderType == e_Query || orderType == e_Level_ask || orderType == e_Level_bid) {
						orderBook.queryOrder(orderPtr);
					}
					else {
						out << "Invalid orderType:" << orderType << endl;
					}
				}
				else {
					out << "Invalid input" << endl;
				}
			}
		}
		catch(exception &e){
			cout << e.what() << endl;
		}
		catch (...) {
			cout << "oh !!!" << endl;
		}
		orderBook.print(out);
	}
}

#endif