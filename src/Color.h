#pragma once

#include <unordered_map>
#include <map>

enum class Color
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	ORANGE,
	BROWN,
	BLACK,
	WHITE,
	GREY,
	END
};

struct ColorHelper
{
	static const std::map<const Color, const char*>& GetColorMap();

private:
	static const std::map<const Color, const char*> colorMap;
};


