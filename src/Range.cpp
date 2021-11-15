#include "Range.h"

void Range::CalculateTopLeftAndBottomRightCorners(double cell1Row, double cell1Col, double cell2Row, double cell2Col)
{
	double rectangleWidth = std::abs(cell1Row - cell2Row);
	double rectangleHeight = std::abs(cell1Col - cell2Col);

	std::pair<double, double> middlePoint = std::make_pair((cell1Row + cell2Row) / 2.0, (cell1Col + cell2Col) / 2.0);

	topLeft = std::make_pair(middlePoint.first - rectangleWidth / 2, middlePoint.second - rectangleHeight / 2);
	bottomRight = std::make_pair(std::round(middlePoint.first + rectangleWidth / 2), std::round(middlePoint.second + rectangleHeight / 2));
}

std::ofstream& operator<<(std::ofstream& ofs, const Range& range)
{
	range.SaveToFile(ofs);
	return ofs;
}