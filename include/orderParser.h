#ifndef _ORDER_PARSER_H
#define _ORDER_PARSER_H 

#include<iostream>
#include<chrono>
#include<map>
#include<list>
#include<unordered_map>
#include<memory>
#include "order.h"
#include "utils.h"
using namespace std;


namespace orderBook {


	sOrderPtr  createOrderObject(vector<string> &token) {
		if (token[0] == "order") {
			try {
				uint64_t id = stol(token[1]);
				OrderType type = token[2] == "sell" ? e_Sell : token[2] == "buy" ? e_Buy : e_Unknown;
				uint64_t qty = stol(token[3]);
				double price = stod(token[4]);
				sOrderPtr order(new sOrder(id, type, price, qty));
				return order;
			}
			catch (...) {
				return nullptr;
			}
		}
		else if (token[0] == "cancel") {
			uint64_t id = stol(token[1]);
			sOrderPtr order(new sOrder(id,e_Cancel));
			return order;

		}
		else if (token[0] == "amend") {
			uint64_t id = stol(token[1]);
			uint64_t qty = stol(token[2]);
			sOrderPtr order(new sOrder(id, e_Edit, 0, qty));
			return order;

		}

		else if (token[0] == "q") {
			if (token[1] == "order") {
				uint64_t id = stol(token[2]);
				sOrderPtr order(new sOrder(id, e_Query));
				return order;
			}
			else if (token[1] == "level") {
				if (token[2] == "ask") {
					uint64_t level = stol(token[3]);
					sOrderPtr order(new sOrder(level, (OrderType)(e_Query | e_Sell)));
					return order;
				}
				else if (token[2] == "bid") {
					uint64_t level = stol(token[3]);
					sOrderPtr order(new sOrder(level, (OrderType)(e_Query | e_Buy)));
					return order;
				}
				else {
					cout << "Invalid token[2]:"<< token[2] << endl;
					return nullptr;
				}
			}
			else {
				cout << "Invalid token[1]:" << token[2] << endl;
				return nullptr;
			}
		}
		else {
			cout << "Invalid token" << endl;
			return nullptr;
		
		}
	}
}

#endif