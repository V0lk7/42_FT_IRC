#include <iostream>
#include <string>
#include <vector>


int	csplit(std::vector<std::string>& tab, std::string& str, const std::string delimiter) {
	size_t startPos = 0;
	size_t endPos = str.find(delimiter);

	while (endPos != std::string::npos) {

		std::string token = str.substr(startPos, endPos - startPos);
		tab.push_back(token);

		startPos = endPos + delimiter.length();
		endPos = str.find(delimiter, startPos);
	}

	std::string lastToken = str.substr(startPos);
	tab.push_back(lastToken);

	return 0;
}
