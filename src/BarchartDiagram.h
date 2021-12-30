#pragma once

#include "Color.h"

#include "utils.h"

#include <utility>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>


// helper structs to provide the diagram with data
struct Group
{
	std::string name;
	std::vector<double> values;
};

struct GroupMetaData
{
	std::string name;
	Color color;
};

struct BarchartDiagramData
{
	size_t barCountInOneGroup;
	std::vector<Group> groups;
	std::vector<GroupMetaData> groupMetaData;
	std::vector<double> yAxisNumbers;
};

// class to organize the static functions and variables
class BarchartDiagram
{
private:
	inline static constexpr size_t barWidth = 30; // px
	inline static constexpr size_t barSpacing = 2; // px
	inline static constexpr size_t offsetBetweenGroups = 5; // px
	inline static constexpr size_t diagramHeight = 450; // px
	
	inline static constexpr size_t bottomTextSpacing = 200; // px
	inline static constexpr size_t bottomRectSize = 20; // px
	
private:
	// helper function to remap one value to the diagram height
	static double RemapBarValue(double barValue, double maxDisplayableValue);
	
	// helper functions to create diagram
	static std::string CreateBar(size_t barIndex, double barHeight, Color color);
	static std::string CreateGroup(const Group& group, const std::vector<GroupMetaData>& groupMetaData, size_t groupIndex, size_t barCountInOneGroup, double maxDisplayableValue);
	static std::string CreateXAxis(const std::vector<Group>& groups, const std::vector<GroupMetaData>& groupMetaData, size_t barCountInOneGroup, double maxDisplayableValue);
	static std::string CreateYAxisNumber(double numberToDisplay, size_t numberIndex, double spaceBetweenNumbers);
	static std::string CreateYAxis(const std::vector<double>& numbers);
	static std::string CreateColoredInfo(const GroupMetaData& metaData, size_t index);
	static std::string CreateBottomSection(const std::vector<GroupMetaData>& groupMetaData);
	
public:
	static std::string CreateSvg(const BarchartDiagramData& barchartDiagramData);
	static std::string CreateHtml(const BarchartDiagramData& barchartDiagramData);
};
