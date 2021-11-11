#pragma once

#include "Alignment.h"
#include "CellContent.h"

#include <string>
#include <ostream>
#include <fstream>
#include <memory>

/*
 * this class represents a cell in the table
 */
class Cell
{
public:
	Cell() : content(CellContent::Create("")), alignment(Alignment::LEFT) { }
	explicit Cell(const std::string& content) : content(CellContent::Create(content)), alignment(Alignment::LEFT) { }
	
	Cell(const Cell& other) = delete;
	Cell& operator=(const Cell& other) = delete;

	Cell(Cell&& other) noexcept : content(std::move(other.content)), alignment(other.alignment) { }
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

private:
	std::unique_ptr<CellContent> content;
	Alignment alignment;
};