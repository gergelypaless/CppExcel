#include "utils.h"

#include <algorithm>
#include <sstream>

std::vector <std::string> SplitString(std::string s, const std::string& delimiter)
{
	size_t pos = 0;
	std::string token;
	std::vector<std::string> elements;
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		elements.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	elements.push_back(s);
	return elements;
}

std::vector <std::string> SplitString(std::string s, char delimiter)
{
	return SplitString(s, std::string(1, delimiter));
}

size_t StringLengthOf(size_t number)
{
	std::stringstream ss;
	ss << number;
	return ss.str().size();
}

bool IsNumber(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c) && c != '.'; }) == s.end();
}

double ConvertStringToDouble(const std::string& str)
{
	std::stringstream value(str);
	double number = 0;
	value >> number;
	return number;
}

size_t ConvertStringToUInt(const std::string& str)
{
	char* end;
	return std::strtol(str.c_str(), &end, 10);
}

size_t ConvertColLetterToNumber(char colLetter)
{
	return colLetter - 'A';
}

char ConvertNumberToColLetter(int8_t number)
{
	return 'A' + number;
}
