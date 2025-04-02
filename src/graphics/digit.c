#include "defines.h"


//use t_char
static void printDigit(t_game* g, uint32_t colour, uint8_t digit, uint32_t y, uint32_t x)
{
	for (uint32_t i = 0U; i < DIGIT_HEIGHT; i++)
	{
		for (uint32_t j = 0U; j < DIGIT_WIDTH; j++)
		{
			if (g->digit[digit][i][j])
				g->screen.data[(y + i) * WIN_WIDTH + x + j] = colour;
			else
			g->screen.data[(y + i) * WIN_WIDTH + x + j] = CLR_DARK_GRAY;
		}
	}
}

void printScore(t_game *g, int player)
{
	uint32_t	y;
	uint32_t	x;

	// print score

	//print wins
	//print deaths
}

static void drawHorizontalSegment(int digit[DIGIT_HEIGHT][DIGIT_WIDTH], int y)
{
	for (int i = 0; i < DIGIT_THICKNESS; i++)
	{
		for (int j = 2; j < DIGIT_WIDTH - 2; j++)
		{
			digit[y + i][j] = 1;
		}
	}
}

static void drawVerticalSegment(int digit[DIGIT_HEIGHT][DIGIT_WIDTH], int x,
								int yStart, int yEnd)
{
	for (int i = 0; i < DIGIT_THICKNESS; i++)
	{
		for (int j = yStart; j < yEnd; j++)
		{
			digit[j][x + i] = 1;
		}
	}
}

void generateDigits(t_game *g)
{
	uint8_t	num = 0;

	while (num < 10)
	{
		// Define which segments should be filled for each digit
		int segments[10][7] = {
			{1, 1, 1, 0, 1, 1, 1}, // 0
			{0, 1, 0, 0, 1, 0, 0}, // 1
			{1, 1, 0, 1, 0, 1, 1}, // 2
			{1, 1, 0, 1, 1, 0, 1}, // 3
			{0, 1, 1, 1, 1, 0, 0}, // 4
			{1, 0, 1, 1, 1, 0, 1}, // 5
			{1, 0, 1, 1, 1, 1, 1}, // 6
			{1, 1, 0, 0, 1, 0, 0}, // 7
			{1, 1, 1, 1, 1, 1, 1}, // 8
			{1, 1, 1, 1, 1, 0, 1}  // 9
		};

		int mid = DIGIT_HEIGHT / 2;
		int bottom = DIGIT_HEIGHT - DIGIT_THICKNESS - 2;

		if (segments[num][0])
			drawHorizontalSegment(g->digit[num], 2); // Top
		if (segments[num][1])
			drawVerticalSegment(g->digit[num], DIGIT_WIDTH - DIGIT_THICKNESS - 2, 2,
								mid); // Upper Right
		if (segments[num][2])
			drawVerticalSegment(g->digit[num], 2, 2, mid); // Upper Left
		if (segments[num][3])
			drawHorizontalSegment(g->digit[num], mid - DIGIT_THICKNESS / 2); // Middle
		if (segments[num][4])
			drawVerticalSegment(g->digit[num], DIGIT_WIDTH - DIGIT_THICKNESS - 2, mid,
								bottom); // Lower Right
		if (segments[num][5])
			drawVerticalSegment(g->digit[num], 2, mid, bottom); // Lower Left
		if (segments[num][6])
			drawHorizontalSegment(g->digit[num], bottom); // Bottom
		num++;
	}
}