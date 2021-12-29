#pragma once

#include "Color.h"

#include <vector>
#include <string>


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
	std::vector<Group> XAxisNames;
	std::vector<GroupMetaData> groupMetaData;
};

class BarchartDiagram
{
public:
	BarchartDiagram(const BarchartDiagramData& barchartDiagramData)
	{
		
	}

private:


};
