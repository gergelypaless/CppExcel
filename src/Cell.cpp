#include "Cell.h"

#include "utils.h"

void Cell::Clear()
{
	SetContent("");
}

const std::string& Cell::GetContent() const
{
	return content;
}

void Cell::SetContent(const std::string& newContent)
{
	content = newContent;
}

void Cell::Print(std::ostream& os, size_t cellMaxLength) const
{
	if (alignment == Alignment::LEFT)
		os << content << std::string(cellMaxLength - content.size(), ' ');
	else if (alignment == Alignment::RIGHT)
		os << std::string(cellMaxLength - content.size(), ' ') << content;
}

void Cell:: SaveToFile(std::ofstream& ofs) const
{
	ofs << content;
}

bool Cell::operator<(const Cell& other) const
{
	if (IsNumber(content) && IsNumber(other.content))
	{
		return ConvertStringToDouble(content) < ConvertStringToDouble(other.content);
	}
	else if (IsNumber(content) && !IsNumber(other.content))
	{
		return false;
	}
	else if (!IsNumber(content) && IsNumber(other.content))
	{
		return true;
	}
	return content < other.content;
}
