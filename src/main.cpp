#if 1
#include<iostream>
#include<fstream>
#include<string>
#include "order.h"
#include "orderParser.h"
#include "orderPlacer.h"

using namespace std;
using namespace orderBook;



int main(int argc, char **argv) {
	if (argc == 1) {
		std::cout << "Usage: OrderBookProcessor <filename>" << std::endl;
		std::cout << "No input file given." << std::endl;
	}

	const std::string filename(argc == 2 ? argv[1] : "");
	std::ifstream inFile;
	inFile.open(filename);
	
	LimitOrderBook orderBook(std::cout);

	if (/*inFile.is_open()*/false) {
		std::cout << "Reading order from file:" << filename<<endl;

		mainLoop(inFile, orderBook);
	}
	else {
		std::cout << "Running in interactive mode"<<endl;
		mainLoop(std::cin, orderBook);
	}

	return 0;
}

#endif