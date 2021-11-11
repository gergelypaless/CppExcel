#include "CellContent.h"

std::unique_ptr<CellContent> CellContent::Create(const std::string& cellContentStr)
{
	if (cellContentStr.empty())
		return std::make_unique<StringCellContent>("");
	else if (cellContentStr[0] != '=')
		return std::make_unique<StringCellContent>(cellContentStr);
	else
		return std::make_unique<FunctionCellContent>(cellContentStr);
}

std::string FunctionCellContent::Evaluate() const
{
	// TODO: implement
	return "";
}

std::ofstream& operator<<(std::ofstream& ofs, const CellContent& cellContent)
{
	cellContent.SaveToFile(ofs);
	return ofs;
}
