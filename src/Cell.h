#pragma once

#include "Alignment.h"

#include <string>
#include <ostream>
#include <fstream>

/*
 * this class represents a cell in the table
 */
class Cell
{
public:
	Cell() : content(""), alignment(Alignment::LEFT) { }
	explicit Cell(const std::string& content) : content(content), alignment(Alignment::LEFT) { }
	
	Cell(const Cell& other) = default;
	Cell& operator=(const Cell& other) = default;

	/* 
	 * clears the cell
	 */
	void Clear();

	/*
	 * returns the cell's content
	 */
	const std::string& GetContent() const;

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

private:
	std::string content;
	Alignment alignment;
};