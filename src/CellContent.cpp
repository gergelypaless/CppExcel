#include "CellContent.h"
#include "utils.h"
#include "Range.h"
#include "Table.h"

#include <limits>

std::unique_ptr<CellContent> CellContent::Create(const std::string& cellContentStr, Table& table)
{
	if (cellContentStr.empty())
		return std::make_unique<StringCellContent>("");
	else if (cellContentStr[0] != '=')
		return std::make_unique<StringCellContent>(cellContentStr);
	else
	{
		auto functionName = cellContentStr.substr(1, 3);
		if (functionName == "SUM")
			return std::make_unique<SumFunctionCellContent>(cellContentStr, table);
		else if (functionName == "AVG")
			return std::make_unique<AvgFunctionCellContent>(cellContentStr, table);
		else if (functionName == "MIN")
			return std::make_unique<MinFunctionCellContent>(cellContentStr, table);
		else if (functionName == "MAX")
			return std::make_unique<MaxFunctionCellContent>(cellContentStr, table);
		else
			throw std::invalid_argument("unknown aggregate function");
		
		
	}
}

Range FunctionCellContent::ExtractRange(const std::string& functionStr)
{
	const auto& rangeStr = functionStr.substr(5, 5);
	return Range::Create(rangeStr);
}

bool FunctionCellContent::Evaluate()
{
	if (IsEvaluated())
		return true;
	
	bool allEvaluated = true;
	// evaluate if all cells in the range are evaluated
	try
	{
		table.Traverse(range, [&allEvaluated](Cell& cell) {
			if (!cell.IsEvaluated())
				allEvaluated = false;
		});
	}
	catch (std::exception& ex)
	{
		Invalidate("#NAME?");
		return true;
	}
	
	if (allEvaluated)
	{
		auto value = EvaluateFunction();
		std::stringstream ss;
		ss << std::fixed << std::setprecision(3) << value;
		SetVisibleContent(ss.str());
	}
	return IsEvaluated();
}

std::ofstream& operator<<(std::ofstream& ofs, const CellContent& cellContent)
{
	cellContent.SaveToFile(ofs);
	return ofs;
}

long double SumFunctionCellContent::EvaluateFunction()
{
	// all cells in range must be evaluated beforehand!!!
	
	long double sum = 0;
	try
	{
		table.Traverse(range, [&sum] (Cell& cell) {
			const auto& cellContent = cell.GetContent();
			if (IsNumber(cellContent))
				sum += ConvertStringToDouble(cellContent);
		});
	}
	catch (std::exception& ex)
	{
		Invalidate("#NAME?");
		throw std::invalid_argument("part of the range is outside of the table");
	}
	return sum;
}

void SumFunctionCellContent::SaveToFile(std::ofstream& ofs) const
{
	ofs << "=SUM(";
	ofs << range << ")";
}

long double AvgFunctionCellContent::EvaluateFunction()
{
	long double sum = 0;
	size_t count = 0;
	try
	{
		table.Traverse(range, [&sum, &count] (Cell& cell) {
			const auto& cellContent = cell.GetContent();
			if (IsNumber(cellContent))
			{
				sum += ConvertStringToDouble(cellContent);
				++count;
			}
		});
	}
	catch (std::exception& ex)
	{
		Invalidate("#NAME?");
		throw std::invalid_argument("part of the range is outside of the table");
	}
	return sum / count;
}

void AvgFunctionCellContent::SaveToFile(std::ofstream& ofs) const
{
	ofs << "=AVG(";
	ofs << range << ")";
}

long double MinFunctionCellContent::EvaluateFunction()
{
	long double min = std::numeric_limits<long double>::max();
	try
	{
		table.Traverse(range, [&min] (Cell& cell) {
			const auto& cellContent = cell.GetContent();
			if (IsNumber(cellContent))
			{
				auto number = ConvertStringToDouble(cellContent);
				if (number < min)
					min = number;
			}
		});
	}
	catch (std::exception& ex)
	{
		Invalidate("#NAME?");
		throw std::invalid_argument("part of the range is outside of the table");
	}
	return min;
}

void MinFunctionCellContent::SaveToFile(std::ofstream& ofs) const
{
	ofs << "=MIN(";
	ofs << range << ")";
}

long double MaxFunctionCellContent::EvaluateFunction()
{
	long double max = 0;
	try
	{
		table.Traverse(range, [&max] (Cell& cell) {
			const auto& cellContent = cell.GetContent();
			if (IsNumber(cellContent))
			{
				auto number = ConvertStringToDouble(cellContent);
				if (number > max)
					max = number;
			}
		});
	}
	catch (std::exception& ex)
	{
		Invalidate("#NAME?");
		throw std::invalid_argument("part of the range is outside of the table");
	}
	
	return max;
}

void MaxFunctionCellContent::SaveToFile(std::ofstream& ofs) const
{
	ofs << "=MAX(";
	ofs << range << ")";
}
