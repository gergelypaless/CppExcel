#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <utility>


class CellContent
{
public:
	static std::unique_ptr<CellContent> Create(const std::string& cellContentStr);
	
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
	explicit FunctionCellContent(std::string contentStr) : function(std::move(contentStr)) {}
	
	std::string GetContentValue() const override
	{
		return Evaluate();
	}
	
	std::string Evaluate() const;
	
	void SaveToFile(std::ofstream& ofs) const override
	{
		ofs << function;
	}
	
private:
	std::string function;
	
};

std::ofstream& operator<<(std::ofstream& ofs, const CellContent& cellContent);

