# LimitOrderBook

This is sample limit order book written in c++ 11 and test cases are using BOOST framework.

How to build

define a environmnet variable for boost named "BOOST_ROOT_DIR"
to comppile test case.

$git clone <url>
$cd <dir>
$mkdir build
$cd build 
$cmake ...

this will generate 2 binaries one LOB and LOB_TEST

Run Binary
//LOB
$LOB <order.txt>   //{order.txt file given in repo}
this will read order from file and process it and display output.

$LOB    // without file name it will run in interactive mode and ask for user input in specific format check order.txt file

//LOB_TEST
$LOB_TEST  // this will run the unit test cases for the order book
