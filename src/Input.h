#pragma once

#include "TableContainer.h"

#include <istream>
#include <ostream>
#include <memory>


class Input
{
public:
	/*
	 * reads input from an istream is, writes information to the user to the ostream os
	 * 
	 * @param is	the istream to read input from
	 * @param os	the ostream to write information to
	 * @param table		the table to work with
	 */
	static void Read(TableContainer& tableContainer, std::istream& is, std::ostream& os);

	/*
	 * reads a file and parses the content to a table
	 * 
	 * @param filename		the file'S name
	 * @param sep	column separator
	 */
	static void ReadFile(TableContainer& tableContainer, const std::string& filename, char sep = ';');

};
