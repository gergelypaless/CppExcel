#pragma once

#include <unordered_map>

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


const std::unordered_map<const Color, const char*> colorMap = {
		std::make_pair(Color::RED, "red"),
		std::make_pair(Color::GREEN, "green"),
		std::make_pair(Color::BLUE, "blue"),
		std::make_pair(Color::YELLOW, "yellow"),
		std::make_pair(Color::ORANGE, "orange"),
		std::make_pair(Color::BROWN, "brown"),
		std::make_pair(Color::BLACK, "black"),
		std::make_pair(Color::WHITE, "white"),
		std::make_pair(Color::GREY, "grey"),
};
