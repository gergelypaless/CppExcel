#pragma once

#include "Table.h"

#include <vector>
#include <string>

class CommandProcessor
{
	static bool shouldExit;
	
public:
	static bool ShouldExit();
	
	/*
	 * commandToken:
	 *      index 0: command name - "edit"
	 *      index 1: cell identifier. ex. B5
	 *      index 2: new cell content
	 * 
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessEditCommand(const std::vector<std::string>& commandTokens, Table& table);
	
	/*
	 * commandToken:
	 *      index 0: command name - "add"
	 *      index 1: number, specifies how many rows or columns needs to be added
	 *      index 2: specifies if we need to add rows or columns
	 *
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessAddCommand(const std::vector<std::string>& commandTokens, Table& table);
	
	/*
	 * commandToken:
	 *      index 0: command name - "delete"
	 *      index 1: row number or column name
	 *
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessDeleteCommand(const std::vector<std::string>& commandTokens, Table& table);
	
	/*
	 * commandToken:
	 *      index 0: command name - "insert"
	 *      index 1: number, specifies how many rows or columns needs to be added
	 *      index 2: specifies if we need to add rows or columns
	 *      index 3: specifies whether we need to insert before or after the specified index. "before"/"after"
	 *      index 4: row number or column name
	 *
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessInsertCommand(const std::vector<std::string>& commandTokens, Table& table);
	
	/*
	 * commandToken:
	 *		index 0: command name 1 - "save"
	 *      index 1: filename
	 *		index 2: [optional] separator
	 *
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessSaveCommand(const std::vector<std::string>& commandTokens, Table& table);

	/*
	 * commandToken:
	 *      index 0: command name 1 - "sort"
	 *		index 1: command name 2 - "by"
	 *      index 2: row number or column name
	 *      index 3: [optional] ascending or descending order
	 *
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessSortCommand(const std::vector<std::string>& commandTokens, Table& table);

	/*
	 * commandToken:
	 *      index 0: command name - "swap"
	 *		index 1: cell 1
	 *      index 2: cell 2
	 * 
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessSwapCommand(const std::vector<std::string>& commandTokens, Table& table);

	/*
	 * commandToken:
	 *		aligning only one cell:
	 *			index 0: command name - "align"
	 *			index 1: cell
	 *			index 2: left or right
	 * 
	 *		aligning range of cells:
	 *			index 0: command name - "align"
	 *			index 1: range
	 *			index 2: left or right
	 * 
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessAlignCommand(const std::vector<std::string>& commandTokens, Table& table);

	/*
	 * commandToken:
	 *      index 0: command name 1 - "clear"
	 *		index 1: cell
	 *
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessClearCommand(const std::vector<std::string>& commandTokens, Table& table);
	
	/*
	 * commandToken:
	 *      index 0: command name 1 - "clear"
	 *		index 1: cell
	 *
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessBarchartCommand(const std::vector<std::string>& commandTokens, Table& table);

	/*
	 * calls a specific command function based on the first - name - argument
	 * 
	 * @param commandTokens		the components of the command
	 * @param table		the table to work with
	 */
	static void ProcessCommand(const std::vector<std::string>& commandTokens, Table& table);

};


