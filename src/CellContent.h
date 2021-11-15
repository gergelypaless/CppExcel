#pragma once

#include "Range.h"

#include <string>
#include <memory>
#include <fstream>
#include <utility>
#include <sstream>
#include <iomanip>
#include <set>

// forward declaration
class Table;

/*
 * represents a cells content
 */
class CellContent
{
public:
	static std::unique_ptr<CellContent> Create(const std::string& cellContentStr, Table& table);
	
public:
	CellContent() = default;
	virtual ~CellContent() = default;
	
	const std::string& GetVisibleContent() const
	{
		if (!evaluated)
			throw std::invalid_argument("this cell is not evaluated, thus you cannot get the content from it. First call Evaluate()");
		return visibleContent;
	}
	
	/*
	 * saves a cell content to file
	 *
	 * @param ofs: output file stream
	 */
	virtual void SaveToFile(std::ofstream& ofs) const = 0;

	/*
	 * evaluates a cell content
	 */
	virtual bool Evaluate() = 0;
	
	/*
	 * resets the flag on a cell content
	 *
	 * @param: a set which contains all the cell content which are not evaluated
	 */
	virtual void ResetEvaluated(std::set<CellContent*>& functionSet) = 0;
	
	/*
	 * getter for the evaluated flag
	 */
	bool IsEvaluated() const
	{
		return evaluated;
	}

protected:
	/*
	 * sets the visible content of the cell content instance
	 *
	 * @param content: the new visible content
	 */
	bool SetVisibleContent(const std::string& content)
	{
		visibleContent = content;
		evaluated = true;
		return true;
	}

private:
	std::string visibleContent; // the text which you see in the table on the screen
protected:
	bool evaluated = false;
};

/*
 * represents a simple string cell content
 */
class StringCellContent : public CellContent
{
public:
	explicit StringCellContent(std::string contentStr) : content(std::move(contentStr)) {}
	
	void SaveToFile(std::ofstream& ofs) const override
	{
		ofs << content;
	}
	
	void ResetEvaluated(std::set<CellContent*>&) override
	{
		SetVisibleContent(content);
	}
	
	bool Evaluate() override
	{
		return SetVisibleContent(content);
	}
	
private:
	std::string content;
};

/*
 * represents a function cell content, which should be evaluated before printing
 */
class FunctionCellContent : public CellContent
{
public:
	FunctionCellContent(const std::string& functionStr, Table& table) : range(ExtractRange(functionStr)), table(table) {}
	
	virtual long double EvaluateFunction() = 0;
	
	void ResetEvaluated(std::set<CellContent*>& functionMap) override
	{
		evaluated = false;
		functionMap.insert(this);
	}
	
	bool Evaluate() override;
	
	/*
	 * function to set the content of this cell content if it is in a cycle
	 */
	void SetInCycle()
	{
		Invalidate("#CYCLE!");
	}
	
private:
	/*
	 * helper function to get the range from a string
	 *
	 * @param: string to extract the range from
	 */
	static Range ExtractRange(const std::string& functionStr);
	
protected:
	void Invalidate(const std::string& errorMessage)
	{
		SetVisibleContent(errorMessage);
	}
	
protected:
	Range range;
	Table& table;
};

/*
 * below: different kind of function cell content classes: SUM, AVG, MIN, MAX
 */
class SumFunctionCellContent : public FunctionCellContent
{
public:
	using FunctionCellContent::FunctionCellContent;
	long double EvaluateFunction() override;
	void SaveToFile(std::ofstream& ofs) const override;
};

class AvgFunctionCellContent : public FunctionCellContent
{
public:
	using FunctionCellContent::FunctionCellContent;
	long double EvaluateFunction() override;
	void SaveToFile(std::ofstream& ofs) const override;
};

class MinFunctionCellContent : public FunctionCellContent
{
public:
	using FunctionCellContent::FunctionCellContent;
	long double EvaluateFunction() override;
	void SaveToFile(std::ofstream& ofs) const override;
};

class MaxFunctionCellContent : public FunctionCellContent
{
public:
	using FunctionCellContent::FunctionCellContent;
	long double EvaluateFunction() override;
	void SaveToFile(std::ofstream& ofs) const override;
};

/*
 * operator helper function to print a cell content to file
 */
std::ofstream& operator<<(std::ofstream& ofs, const CellContent& cellContent);

