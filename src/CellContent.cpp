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
		return std::make_unique<FunctionCellContent>(cellContentStr, table);
}

long double FunctionCellContent::Evaluate() const
{
	auto extractRange = [] (const std::string& f)
	{
		const auto& rangeStr = f.substr(5, 5);
		return Range::Create(rangeStr);
	};
	
	const auto& functionName = function.substr(1, 3);
	if (functionName == "SUM")
		return CalculateSumFunction(extractRange(function));
	else if (functionName == "AVG")
		return CalculateAvgFunction(extractRange(function));
	else if (functionName == "MIN")
		return CalculateMinFunction(extractRange(function));
	else if (functionName == "MAX")
		return CalculateMaxFunction(extractRange(function));
	else
		throw std::invalid_argument("unknown aggregate function");
}

long double FunctionCellContent::CalculateSumFunction(const Range& range) const
{
	long double sum = 0;
	table.Traverse(range, [&sum] (Cell& cell) {
		const auto& cellContent = cell.GetContent();
		if (IsNumber(cellContent))
			sum += ConvertStringToDouble(cellContent);
	});
	return sum;
}

long double FunctionCellContent::CalculateAvgFunction(const Range& range) const
{
	long double sum = 0;
	size_t count = 0;
	table.Traverse(range, [&sum, &count] (Cell& cell) {
		const auto& cellContent = cell.GetContent();
		if (IsNumber(cellContent))
		{
			sum += ConvertStringToDouble(cellContent);
			++count;
		}
	});
	return sum / count;
}

long double FunctionCellContent::CalculateMinFunction(const Range& range) const
{
	long double min = std::numeric_limits<long double>::max();
	table.Traverse(range, [&min] (Cell& cell) {
		const auto& cellContent = cell.GetContent();
		if (IsNumber(cellContent))
		{
			auto number = ConvertStringToDouble(cellContent);
			if (number < min)
				min = number;
		}
	});
	return min;
}

long double FunctionCellContent::CalculateMaxFunction(const Range& range) const
{
	long double max = 0;
	table.Traverse(range, [&max] (Cell& cell) {
		const auto& cellContent = cell.GetContent();
		if (IsNumber(cellContent))
		{
			auto number = ConvertStringToDouble(cellContent);
			if (number > max)
				max = number;
		}
	});
	return max;
}

std::ofstream& operator<<(std::ofstream& ofs, const CellContent& cellContent)
{
	cellContent.SaveToFile(ofs);
	return ofs;
}
