#include "defines.h"


//use t_char
static void printDigit(t_game* g, t_char d)
{
	for (uint32_t i = 0U; i < DIGIT_HEIGHT; i++)
	{
		for (uint32_t j = 0U; j < DIGIT_WIDTH; j++)
		{
			if (g->digit[d.c][i][j])
				g->screen.data[(d.pos_y + i) * WIN_WIDTH + d.pos_x + j] = d.colour;
			else if (d.background)
				g->screen.data[(d.pos_y + i) * WIN_WIDTH + d.pos_x + j] = d.background;
		}
	}
}

void	printScore(t_game *g, int p)
{
	t_char		d;
	t_char		e;

	d.colour = g->player[p].colour;
	d.background = CLR_TRANSPARENT;
	d.pos_x = 64;
	d.pos_y = 128 * p + 128;
	e = d;
	e.colour = CLR_GRAY_1;
	e.c = 8;
	d.c = g->player[p].score / 100;
	printDigit(g, e);
	printDigit(g, d);
	d.pos_x += 32;
	e.pos_x += 32;
	d.c = g->player[p].score / 10 % 10;
	printDigit(g, e);
	printDigit(g, d);
	d.pos_x += 32;
	e.pos_x += 32;
	d.c = g->player[p].score % 10;
	printDigit(g, e);
	printDigit(g, d);
}

static void drawHorizontalSegment(uint8_t digit[DIGIT_HEIGHT][DIGIT_WIDTH], uint8_t y)
{
	for (uint8_t i = 0; i < DIGIT_THICKNESS; i++)
	{
		for (uint8_t j = 2; j < DIGIT_WIDTH - 2; j++)
		{
			digit[y + i][j] = 1;
		}
	}
}

static void drawVerticalSegment(uint8_t digit[DIGIT_HEIGHT][DIGIT_WIDTH], uint8_t x,
								uint8_t yStart, uint8_t yEnd)
{
	for (uint8_t i = 0; i < DIGIT_THICKNESS; i++)
	{
		for (uint8_t j = yStart; j < yEnd; j++)
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