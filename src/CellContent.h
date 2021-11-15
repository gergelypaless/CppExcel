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
	
	virtual void SaveToFile(std::ofstream& ofs) const = 0;

	virtual bool Evaluate() = 0;
	
	virtual void ResetEvaluated(std::set<CellContent*>&) = 0;
	
	bool IsEvaluated() const
	{
		return evaluated;
	}

protected:
	bool SetVisibleContent(const std::string& content)
	{
		visibleContent = content;
		evaluated = true;
		return true;
	}

private:
	std::string visibleContent;
protected:
	bool evaluated = false;
};


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

class FunctionCellContent : public CellContent
{
public:
	FunctionCellContent(const std::string& functionStr, Table& table) : range(ExtractRange(functionStr)), valid(true), table(table) {}
	
	virtual long double EvaluateFunction() = 0;
	
	void ResetEvaluated(std::set<CellContent*>& functionMap) override
	{
		evaluated = false;
		functionMap.insert(this);
		valid = false;
	}
	
	bool Evaluate() override;
	
	void SetInCycle()
	{
		valid = false;
		SetVisibleContent("#CYCLE!");
	}
	
private:
	static Range ExtractRange(const std::string& functionStr);
	
protected:
	void Invalidate(const std::string& errorMessage)
	{
		valid = false;
		SetVisibleContent(errorMessage);
	}
	
protected:
	Range range;
	mutable bool valid;
	Table& table;
};

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

std::ofstream& operator<<(std::ofstream& ofs, const CellContent& cellContent);

