#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

class Console
{
public:
	enum class Color
	{
        WHITE = 0,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN
	};
    static void SetColor( Color color ) noexcept;
};

#endif