#pragma once

#include "Range.h"

#include <string>
#include <memory>
#include <fstream>
#include <utility>
#include <sstream>
#include <iomanip>

// forward declaration
class Table;


class CellContent
{
public:
	static std::unique_ptr<CellContent> Create(const std::string& cellContentStr, Table& table);
	
public:
	CellContent() = default;
	virtual ~CellContent() = default;
	
	virtual std::string GetContentValue() const = 0;
	virtual void SaveToFile(std::ofstream& ofs) const = 0;
};


class StringCellContent : public CellContent
{
public:
	explicit StringCellContent(std::string contentStr) : content(std::move(contentStr)) {}
	
	std::string GetContentValue() const override
	{
		return content;
	}
	
	void SaveToFile(std::ofstream& ofs) const override
	{
		ofs << content;
	}
	
private:
	std::string content;
	
};

class FunctionCellContent : public CellContent
{
public:
	FunctionCellContent(std::string contentStr, Table& table) : function(std::move(contentStr)), table(table) {}
	
	std::string GetContentValue() const override
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(3) << Evaluate();
		return ss.str();
	}
	
	long double Evaluate() const;
	
	void SaveToFile(std::ofstream& ofs) const override
	{
		ofs << function;
	}
	
	long double CalculateSumFunction(const Range& range) const;
	long double CalculateAvgFunction(const Range& range) const;
	long double CalculateMinFunction(const Range& range) const;
	long double CalculateMaxFunction(const Range& range) const;
	
private:
	std::string function;
	Table& table;
};

std::ofstream& operator<<(std::ofstream& ofs, const CellContent& cellContent);

