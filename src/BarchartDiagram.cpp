#include "BarchartDiagram.h"

std::string BarchartDiagram::CreateBar(size_t barIndex, double barHeight, Color color)
{
	auto barElementStringTokens = SplitString(R"delimiter(
				<rect class="bar" x="XXX" y="XXX" width="XXX" height="XXX" fill="XXX"/>)delimiter", "XXX");
	
	size_t width = barWidth;
	double height = barHeight;
	
	size_t x = barIndex * barWidth + barIndex * barSpacing;
	long y = -(long)barHeight;
	
	const char* fill = colorMap.at(color);
	
	std::string final =
			barElementStringTokens[0] + std::to_string(x) + barElementStringTokens[1] + std::to_string(y) +
			barElementStringTokens[2] + std::to_string(width) + barElementStringTokens[3] + std::to_string(height) +
			barElementStringTokens[4] + fill + barElementStringTokens[5];
	
	return final;
}

double BarchartDiagram::RemapBarValue(double barValue, double maxDisplayableValue)
{
	return barValue / maxDisplayableValue * diagramHeight;
}

std::string BarchartDiagram::CreateGroup(const Group& group, const std::vector<GroupMetaData>& groupMetaData, size_t groupIndex, size_t barCountInOneGroup, double maxDisplayableValue)
{
	auto groupElementStringTokens = SplitString(R"delimiter(
			<g transform="translate(XXX,-1)">XXX
				<text dy=".71em" y="9" x="XXX" style="text-anchor: middle;">XXX</text>
			</g>)delimiter", "XXX");
	
	size_t translateX = (groupIndex + 1) * offsetBetweenGroups + groupIndex * barWidth * barCountInOneGroup + groupIndex * barSpacing * barCountInOneGroup;
	double textX = (barCountInOneGroup * barWidth + (barCountInOneGroup - 1) * barSpacing) / 2.0;
	
	size_t tokenIter = 0;
	std::string final = groupElementStringTokens[tokenIter++];
	final += std::to_string(translateX) + groupElementStringTokens[tokenIter++];
	
	for (size_t i = 0; i < barCountInOneGroup; ++i)
	{
		final += CreateBar(i, RemapBarValue(group.values[i], maxDisplayableValue), groupMetaData[i].color);
	}
	
	final += groupElementStringTokens[tokenIter++] + std::to_string(textX);
	final += groupElementStringTokens[tokenIter++] + group.name;
	final += groupElementStringTokens[tokenIter++];
	return final;
}

std::string BarchartDiagram::CreateXAxis(const std::vector<Group>& groups, const std::vector<GroupMetaData>& groupMetaData, size_t barCountInOneGroup, double maxDisplayableValue)
{
	auto xAxisElementStringTokens = SplitString(R"delimiter(
			<g class="axis" transform="translate(0,XXX)">XXX
				<path class="domain" d="M0,6V0H900V6"/>
			</g>)delimiter", "XXX");
	
	std::string final = xAxisElementStringTokens[0] + std::to_string(diagramHeight) + xAxisElementStringTokens[1];
	for (size_t i = 0; i < groups.size(); i++)
	{
		final += CreateGroup(groups[i], groupMetaData, i, barCountInOneGroup, maxDisplayableValue);
	}
	final += xAxisElementStringTokens[2];
	return final;
}

std::string BarchartDiagram::CreateYAxisNumber(double numberToDisplay, size_t numberIndex, double spaceBetweenNumbers)
{
	auto numberElementStringTokens = SplitString(R"delimiter(
			<g transform="translate(0, XXX)">
			    <text dy=".32em" x="-9" y="0" style="text-anchor: end;">XXX</text>
			</g>)delimiter", "XXX");
	
	double translateY = diagramHeight - numberIndex * spaceBetweenNumbers;
	
	std::stringstream ss;
	ss << numberToDisplay;
	std::string final =
			numberElementStringTokens[0] + std::to_string(translateY) + numberElementStringTokens[1] + ss.str() + numberElementStringTokens[2];
	return final;
}

std::string BarchartDiagram::CreateYAxis(const std::vector<double>& numbers)
{
	auto yAxisElementStringTokens = SplitString(R"delimiter(
			<g class="axis">XXX
				<path class="domain" d="M-6,0H0V450H-6"/>
                <text transform="rotate(-90)" y="6" dy=".71em" style="text-anchor: end;">Value</text>
			</g>)delimiter", "XXX");
	
	double spaceBetweenNumbers = diagramHeight / (double)(numbers.size() - 1);
	
	std::string final = yAxisElementStringTokens[0];
	for (size_t i = 0; i < numbers.size(); ++i)
	{
		final += CreateYAxisNumber(numbers[i], i, spaceBetweenNumbers);
	}
	final += yAxisElementStringTokens[1];
	return final;
}

std::string BarchartDiagram::CreateColoredInfo(const GroupMetaData& metaData, size_t index)
{
	auto coloredElementStringTokens = SplitString(R"delimiter(
			<g transform="translate(XXX, 0)">
                <rect width="XXX" height="XXX" fill="XXX" stroke="black"/>
                <text x="30" y="15">XXX</text>
            </g>)delimiter", "XXX");
	
	size_t translateX = index * bottomTextSpacing;
	size_t rectSize = bottomRectSize;
	const char* fill = colorMap.at(metaData.color);
	const std::string& content = metaData.name;
	
	std::string final = coloredElementStringTokens[0] + std::to_string(translateX) + coloredElementStringTokens[1];
	final += std::to_string(rectSize) + coloredElementStringTokens[2] + std::to_string(rectSize) + coloredElementStringTokens[3];
	final += fill + coloredElementStringTokens[4] + content + coloredElementStringTokens[5];
	return final;
}

std::string BarchartDiagram::CreateBottomSection(const std::vector<GroupMetaData>& groupMetaData)
{
	auto bottomElementStringTokens = SplitString(R"delimiter(
			<g transform="translate(40,510)">XXX
			</g>)delimiter", "XXX");
	
	std::string final = bottomElementStringTokens[0];
	for (size_t i = 0; i < groupMetaData.size(); ++i)
	{
		final += CreateColoredInfo(groupMetaData[i], i);
	}
	final += bottomElementStringTokens[1];
	return final;
}

std::string BarchartDiagram::CreateSvg(const BarchartDiagramData& barchartDiagramData)
{
	auto svgElementStringTokens = SplitString(R"delimiter(
			<svg width="960" height="600" id="svg">XXX
			</svg>)delimiter", "XXX");
	
	auto diagramElementStringTokens = SplitString(R"delimiter(
			<g transform="translate(40,20)">XXX
			</g>)delimiter", "XXX");
	
	std::string final = svgElementStringTokens[0] + diagramElementStringTokens[0];
	final += CreateXAxis(barchartDiagramData.groups, barchartDiagramData.groupMetaData, barchartDiagramData.barCountInOneGroup, barchartDiagramData.yAxisNumbers.back());
	final += CreateYAxis(barchartDiagramData.yAxisNumbers);
	final += diagramElementStringTokens[1] + CreateBottomSection(barchartDiagramData.groupMetaData);
	final += svgElementStringTokens[1];
	return final;
}

std::string BarchartDiagram::CreateHtml(const BarchartDiagramData& barchartDiagramData)
{
	auto htmlStringTokens = SplitString(R"delimiter(
			<!DOCTYPE html>
			<html lang="en">
			<head>
			    <meta charset="UTF-8">
			    <meta http-equiv="X-UA-Compatible" content="IE=edge">
			    <meta name="viewport" content="width=device-width, initial-scale=1.0">
			    <title>Document</title>
			
			    <style>
			        * {
			            margin: 0px;
			        }
			
			        #svg {
			            display: block;
			            margin: auto;
			            margin-top: 100px;
			        }
			
			        .bar:hover {
			            fill: darkgray;
			        }
			
			        .axis {
			            font: 10px sans-serif;
			        }
			
			        .axis path,
			        .axis line {
			            fill: none;
			            stroke: #000;
			            shape-rendering: crispEdges;
			        }
			    </style>
			</head>
			<body>
			XXX
			</body>
			</html>)delimiter", "XXX");
	
	std::string final = htmlStringTokens[0];
	final += CreateSvg(barchartDiagramData) + htmlStringTokens[1];
	return final;
}
