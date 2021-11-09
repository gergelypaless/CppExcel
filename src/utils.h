#pragma once

#include <string>
#include <vector>

/*
 * splits a string at delimiter and returns a vector of strings of the tokens
 * 
 * @param s		the string to split
 * @param delimiter		the string or character to split at
 */
std::vector<std::string> SplitString(std::string s, const std::string& delimiter);
std::vector<std::string> SplitString(std::string s, char delimiter);

/*
 * return how many character a certain number has
 * 
 * @param number	the number to get the length about
 */
size_t StringLengthOf(size_t number);

/*
 * returns true if s is a number
 * 
 * @param s		string to determine is a number or not
 */
bool IsNumber(const std::string& s);

/*
 * converts a string to double
 * 
 * @param str	the string to convert
 */
double ConvertStringToDouble(const std::string& str);

/*
 * converts a string to unsigned int
 *
 * @param str	the string to convert
 */
size_t ConvertStringToUInt(const std::string& str);

/*
 * converts a column letter to a number (index)
 *
 * @param colLetter	the column letter to convert
 */
size_t ConvertColLetterToNumber(char colLetter);

/*
 * converts a number to a column letter
 * 
 * @param number	the number to convert
 */
char ConvertNumberToColLetter(int8_t number);