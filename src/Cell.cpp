#include "Cell.h"

#include "utils.h"

Cell::Cell(Table& table) : content(CellContent::Create("", table)), alignment(Alignment::LEFT), table(table)
{

}

Cell::Cell(const std::string& contentStr, Table& table) : content(CellContent::Create(contentStr, table)), alignment(Alignment::LEFT), table(table)
{

}

Cell::Cell(Cell&& other) noexcept: content(std::move(other.content)), alignment(other.alignment), table(other.table) { }

Cell& Cell::operator=(Cell&& other) noexcept
{
	std::swap(content, other.content);
	std::swap(alignment, other.alignment);
	return *this;
}

void Cell::Clear()
{
	SetContent("");
}

std::string Cell::GetContent() const
{
	return content->GetVisibleContent();
}

void Cell::SetContent(const std::string& newContent)
{
	content = CellContent::Create(newContent, table);
}

void Cell::Print(std::ostream& os, size_t cellMaxLength) const
{
	const auto& myContent = content->GetVisibleContent();
	if (alignment == Alignment::LEFT)
		os << myContent << std::string(cellMaxLength - myContent.size(), ' ');
	else if (alignment == Alignment::RIGHT)
		os << std::string(cellMaxLength - myContent.size(), ' ') << myContent;
}

void Cell:: SaveToFile(std::ofstream& ofs) const
{
	ofs << *content;
}

bool Cell::operator<(const Cell& other) const
{
	const auto& myContent = content->GetVisibleContent();
	const auto& otherContent = other.content->GetVisibleContent();
	if (IsNumber(myContent) && IsNumber(otherContent))
	{
		return ConvertStringToDouble(myContent) < ConvertStringToDouble(otherContent);
	}
	else if (IsNumber(myContent) && !IsNumber(otherContent))
	{
		return false;
	}
	else if (!IsNumber(myContent) && IsNumber(otherContent))
	{
		return true;
	}
	return myContent < otherContent;
}
