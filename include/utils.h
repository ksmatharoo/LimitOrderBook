#ifndef _UTILS_H
#define _UTILS_H 

#include<vector>
#include<string>
using namespace std;

namespace ksm {
	vector<string> split(const string name, string delimiter) {
		size_t start = 0;
		vector<string> ret;
		size_t pos = name.find(delimiter);
		while (pos != string::npos) {
			ret.push_back(name.substr(start, (pos - start)));
			start = pos + 1;
			pos = name.find(delimiter, start);
		}
		ret.push_back(name.substr(start));

		return ret;

	}

}

#endif
