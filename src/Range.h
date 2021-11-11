#pragma once

#include "utils.h"

#include <vector>
#include <algorithm>
#include <cmath>

/*
 * this class represents a range in the table
 */
class Range
{
public:
	static Range Create(const std::string& rangeStr)
	{
		const auto& rangeTokens = SplitString(rangeStr, ":");
		size_t cell1Row = ConvertStringToUInt(rangeTokens[0].substr(1, rangeTokens[0].size() - 1)) - 1;
		size_t cell1Col = ConvertColLetterToNumber(rangeTokens[0][0]);
		size_t cell2Row = ConvertStringToUInt(rangeTokens[1].substr(1, rangeTokens[1].size() - 1)) - 1;
		size_t cell2Col = ConvertColLetterToNumber(rangeTokens[1][0]);
		return {cell1Row, cell1Col, cell2Row, cell2Col};
	}
	
public:
	Range(size_t cell1Row, size_t cell1Col, size_t cell2Row, size_t cell2Col)
	{
		CalculateTopLeftAndBottomRightCorners(cell1Row, cell1Col, cell2Row, cell2Col);
	}

	/*
	 * returns the top left corner of the range 
	 */
	auto GetTopLeftCorner() const
	{
		return topLeft;
	}

	/*
	 * returns the bottom right corner of the range
	 */
	auto GetBottomRightCorner() const
	{
		return bottomRight;
	}

private:
	/*
	 * calculates the top left and bottom right corners from cell coordinates
	 */
	void CalculateTopLeftAndBottomRightCorners(double cell1Row, double cell1Col, double cell2Row, double cell2Col);

private:
	std::pair<size_t, size_t> topLeft;
	std::pair<size_t, size_t> bottomRight;
};
