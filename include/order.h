#ifndef _ORDER_H
#define _ORDER_H 

#include<iostream>
#include<chrono>
#include<map>
#include<list>
#include<unordered_map>
#include <iomanip>
using namespace std;


namespace orderBook {

	enum OrderType {
		e_Unknown = 0,
		e_Buy = 1,
		e_Sell = 2,
		e_Edit = 4,
		e_Cancel = 8,
		e_Query = 16,
		e_Level_ask = (e_Query | e_Sell),
		e_Level_bid = (e_Query | e_Buy)
	};

	enum OrderStatus {
		e_NotMatching = 0,
		e_Fully = 1,
		e_Partially = 2,
		e_NotExecuted = 3,
	};

	string OrderTypeStr[] = { "unknown","buy","sell","edit","cancel","query" };


	struct sOrder
	{
		uint64_t m_orderId;
		OrderType m_orderType;
		double m_price;
		uint64_t m_qty;
		uint64_t m_createdTS;

		sOrder(uint64_t orderId, OrderType orderType = e_Unknown, double price = 0,
			uint64_t qty = 0) {
			this->m_orderId = orderId;
			this->m_orderType = orderType;
			this->m_price = price;
			this->m_qty = qty;
			this->m_createdTS =
				chrono::system_clock::now().time_since_epoch().count();
		}

		~sOrder() {
			//cout << "sOrder deleted with id :" << m_orderId << endl;
		}

		static void printHeader(ostream& out) {
			out << setw(10) << "orderId";
			out << setw(10) << "orderType";
			out << setw(10) << "price";
			out << setw(10) << "qty";
			//cout << setw(16) << "createdTS";
			out << endl;
		}

		void printSelf(ostream& out) {
			out << setw(10) << m_orderId;
			out << setw(10) << OrderTypeStr[m_orderType];
			out << setw(10) << std::fixed << setprecision(2) << m_price;
			out << setw(10) << m_qty;
			//cout << setw(16) << m_createdTS;
			out << endl;
		}
	};

	typedef shared_ptr<sOrder> sOrderPtr;
	typedef list<sOrderPtr> orderList;
	typedef list<sOrderPtr>::iterator orderListIter;


	class LimitOrderBook {

		//map <orderPrice,orderList>
		map<double, orderList, greater<double>>  m_buyMap;
		map<double, orderList, less<double>>  m_sellMap;

		//map of orderid vs orders 
		unordered_map<uint64_t, sOrderPtr> m_orders;
		ostream &out;
	public:
		ostream& getOutputStream() { return out; }

		LimitOrderBook(ostream &_out) :out(_out) {}

		void addOrder(sOrderPtr _order) {
			
			OrderStatus orderStatus = e_NotExecuted;
				while (orderStatus != e_NotMatching) {
					orderStatus = tryOrderExecute(_order);

					if (orderStatus == e_Fully)
						break;
				}

			if (orderStatus == e_Fully)
				return;

			auto iter = m_orders.find(_order->m_orderId);
			if (iter != m_orders.end()) {
				out << "order id already exist!!" << endl;
				return;
			}

			if (_order->m_orderType == e_Buy) {
				auto it = m_buyMap.find(_order->m_price);

				if (it != m_buyMap.end()) {
					orderList &list = it->second;
					list.push_back(_order);
					m_buyMap.insert(make_pair(_order->m_price, list));
				}
				else {
					orderList list{ _order };
					m_buyMap.insert(make_pair(_order->m_price, list));
				}
			}
			else {
				auto it = m_sellMap.find(_order->m_price);

				if (it != m_sellMap.end()) {
					orderList &list = it->second;
					list.push_back(_order);
					m_sellMap.insert(make_pair(_order->m_price, list));
				}
				else {
					orderList list{ _order };
					m_sellMap.insert(make_pair(_order->m_price, list));
				}
			}
			m_orders[_order->m_orderId] = _order;
		}

		void cancelOrder(sOrderPtr _order) {

			auto it = this->m_orders.find(_order->m_orderId);
			if (it != m_orders.end())
			{
				if (it->second->m_orderType == e_Buy) {
					auto buyIt = this->m_buyMap.find(it->second->m_price);
					if (buyIt != m_buyMap.end())
					{
						buyIt->second.remove(it->second);
						if (buyIt->second.size() == 0) {
							m_buyMap.erase(buyIt);
						}
					}
				}
				else {
					auto sellIt = this->m_sellMap.find(it->second->m_price);
					if (sellIt != m_sellMap.end())
					{
						sellIt->second.remove(it->second);
						if (sellIt->second.size() == 0) {
							m_sellMap.erase(sellIt);
						}
					}
				}
				m_orders.erase(it);
			}
			else {
				out << "Invalid Order" << endl;
			}
		}



		void queryOrder(sOrderPtr currentOrder) {
			OrderType ot = currentOrder->m_orderType;
			if (ot == e_Query) {
				auto it = this->m_orders.find(currentOrder->m_orderId);
				if (it != m_orders.end()) {
					it->second->printHeader(out);
					it->second->printSelf(out);
				}
				else {
					out << "No order found with ID:" <<
						currentOrder->m_orderId << endl;
				}
			}
			else if (ot == e_Level_ask) {
				int level = 0;
				bool found = false;
				uint64_t requiredLevel = currentOrder->m_orderId;
				for (auto it : m_sellMap) {
					for (auto order : it.second) {
						if (level == requiredLevel) {
							order->printHeader(out);
							order->printSelf(out);
							found = true;
							break;
						}
						level++;
					}
					if (found)
						break;
				}
			}
			else if (ot == e_Level_bid) {
				int level = 0;
				uint64_t requiredLevel = currentOrder->m_orderId;
				bool found = false;
				for (auto it : m_buyMap) {
					for (auto order : it.second) {
						if (level == requiredLevel) {
							order->printHeader(out);
							order->printSelf(out);
							found = true;
							break;
						}
						level++;
					}
					if (found)
						break;
				}
			}
		}

		void editOrder(sOrderPtr currentOrder) {
			auto it = m_orders.find(currentOrder->m_orderId);
			if (it != m_orders.end())
			{
				//update the qualtity if curent order is decreasing
				if (currentOrder->m_qty < it->second->m_qty) {
					it->second->m_qty = currentOrder->m_qty;
				}
				else {
					//change current order type to buy and sets its price
					currentOrder->m_orderType = it->second->m_orderType;
					currentOrder->m_price = it->second->m_price;

					//cancel existing order and place new order with new quantity
					cancelOrder(it->second);

					addOrder(currentOrder);
				}
			}
		}

		void print(ostream& out) {
			//system("cls");
			sOrder::printHeader(out);
			for (auto it : m_buyMap) {
				for (auto order : it.second) {
					order->printSelf(out);
				}
			}
			out << endl;
			sOrder::printHeader(out);
			for (auto it : m_sellMap) {
				for (auto order : it.second) {
					order->printSelf(out);
				}
			}
		}

		OrderStatus tryOrderExecute(sOrderPtr currentOrder) {
			// Confirm levels are active.  For buy orders, we take highest buy level
			// and assume the trade price was due to lowest sell order.
			if (currentOrder->m_orderType == e_Buy && this->m_sellMap.empty()) {
				return e_NotMatching;
			}
			else if (currentOrder->m_orderType == e_Sell && this->m_buyMap.empty()) {
				return e_NotMatching;
			}

			if (currentOrder->m_orderType == e_Buy) {
				auto sellit = m_sellMap.begin();
				if (sellit->first > currentOrder->m_price) {
					return e_NotMatching;
				}
				else { //execute order

					orderList& sellList = sellit->second;
					while (!sellList.empty() && currentOrder->m_qty > 0) {

						sOrderPtr sellOrderPtr = sellList.front();
						if (sellOrderPtr->m_qty >= currentOrder->m_qty) {
							sellOrderPtr->m_qty -= currentOrder->m_qty;

							out << "order "<< currentOrder->m_orderId<< " executed " << currentOrder->m_qty
							<< "@"<< sellOrderPtr->m_price<<endl;

							if (sellOrderPtr->m_qty == 0) {
								sellList.pop_front();

								auto it = m_orders.find(sellOrderPtr->m_orderId);
								if (it != m_orders.end()) {
									m_orders.erase(it);
								}
							}

							return e_Fully;
						}
						else {
							currentOrder->m_qty -= sellOrderPtr->m_qty;
							out << "order " << currentOrder->m_orderId << " executed " << sellOrderPtr->m_qty
								<< "@" << sellOrderPtr->m_price << endl;
							//sell order exhausted delete it 

							auto it = m_orders.find(sellOrderPtr->m_orderId);
							if (it != m_orders.end()) {
								m_orders.erase(it);
							}

							sellList.pop_front();
						}
					}
					//delete the empty map left with price only key ,no value list
					m_sellMap.erase(sellit);

					return e_Partially;
				}
			}
			else if (currentOrder->m_orderType == e_Sell) {
				auto buyit = m_buyMap.begin();
				if (buyit->first < currentOrder->m_price) {
					return e_NotMatching;
				}
				else { //execute order

					orderList& buyList = buyit->second;
					while (!buyList.empty() && currentOrder->m_qty > 0) {

						sOrderPtr buyOrderPtr = buyList.front();
						if (buyOrderPtr->m_qty >= currentOrder->m_qty) {
							buyOrderPtr->m_qty -= currentOrder->m_qty;
							out << "order " << currentOrder->m_orderId << " executed " << currentOrder->m_qty
								<< "@" << currentOrder->m_price << endl;

							if (buyOrderPtr->m_qty == 0) {

								auto it = m_orders.find(buyOrderPtr->m_orderId);
								if (it != m_orders.end()) {
									m_orders.erase(it);
								}

								buyList.pop_front();
							}

							return e_Fully;
						}
						else {
							currentOrder->m_qty -= buyOrderPtr->m_qty;
							//sell order exhausted delete it 
							out << "order " << currentOrder->m_orderId << " executed " << buyOrderPtr->m_qty
								<< "@" << currentOrder->m_price << endl;

							auto it = m_orders.find(buyOrderPtr->m_orderId);
							if (it != m_orders.end()) {
								m_orders.erase(it);
							}

							buyList.pop_front();
						}
					}
					//delete the empty map left with price only key ,no value list
					m_buyMap.erase(buyit);

					return e_Partially;
				}
			}
			else {
				out << "Invalid Current order type" << currentOrder->m_orderType << endl;
				return e_NotExecuted;
			
			}
		}
	};
}

#endif