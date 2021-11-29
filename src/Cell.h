#pragma once

#include "Alignment.h"
#include "CellContent.h"

#include <string>
#include <ostream>
#include <fstream>
#include <memory>
#include <set>

// forward declaration
class Table;

/*
 * this class represents a cell in the table
 */
class Cell
{
public:
	explicit Cell(Table& table);
	Cell(const std::string& contentStr, Table& table);
	
	Cell(const Cell& other) = delete;
	Cell& operator=(const Cell& other) = delete;

	Cell(Cell&& other) noexcept;
	Cell& operator=(Cell&& other) noexcept;
	
	/* 
	 * clears the cell
	 */
	void Clear();

	/*
	 * returns the cell's content
	 */
	std::string GetContent() const;

	/*
	 * sets the cell's content
	 * 
	 * @param newContent	the new cell content
	 */
	void SetContent(const std::string& newContent);
	
	/*
	 * prints the cell to the ostream os
	 * 
	 * @param os	the ostream
	 * @param cellMaxLength		how big a cell's content length is
	 */
	void Print(std::ostream& os, size_t cellMaxLength) const;

	/*
	 * saves the cell to file
	 * 
	 * @param ofs	the ofstream output file
	 */
	void SaveToFile(std::ofstream& ofs) const;

	/*
	 * comparision operator<
	 * @param other		the other cell to compare to
	 */
	bool operator<(const Cell& other) const;

	/*
	 * sets the cell's alignment
	 * 
	 * @param alignment		the new alignment
	 */
	void SetAlignment(Alignment alignment)
	{
		this->alignment = alignment;
	}
	
	/*
	 * resets the evaluated flag on the cell content
	 *
	 * @param functionSet: a set which contains all the cell content which are not evaluated
	 */
	void ResetEvaluated(std::set<CellContent*>& functionSet)
	{
		content->ResetEvaluated(functionSet);
	}
	
	/*
	 * return true if the cell's content is already evaluated
	 */
	bool IsEvaluated() const
	{
		return content->IsEvaluated();
	}
	
private:
	std::unique_ptr<CellContent> content;
	Alignment alignment;
	Table& table;
};