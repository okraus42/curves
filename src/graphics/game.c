/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 17:03:00 by okraus            #+#    #+#             */
/*   Updated: 2025/04/04 19:05:43 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "defines.h"
#include <X11/Xlib.h> // Needed for X11 functions
#include <math.h>	  // atan2(), cos(), sin(), sqrt()
#include <stdio.h>	  //for debug and sprintf
#include <stdlib.h>	  //for free and rand()

#include "mlx.h"
#include <stdlib.h>

// draw score board

// calculate scores (hitting yourself  -1, others hitting you +1) +1 for every death of others

// show score total ||

void add_point(t_game* g, uint8_t p, uint32_t colour)
{
	uint8_t i;

	i = 0;
	while (i < g->players)
	{
		if (colour == g->player[i].colour && i != p)
			g->player[i].score += 1;
		++i;
	}
}

void draw_player(t_game* g, uint8_t p)
{
	// printf("players %i | %i\n", g->players, p);
	int dx, dy;
	int sensory_pixel_x, sensory_pixel_y;
	// g->player[p].old_front_pixel_x = g->player[p].front_pixel_x;
	// g->player[p].old_front_pixel_y = g->player[p].front_pixel_y;
	// g->player[p].front_pixel_x = (g->player[p].x + g->player[p].dx) / g->player[p].radius;
	// g->player[p].front_pixel_y = (g->player[p].y + g->player[p].dy) / g->player[p].radius;
	sensory_pixel_x =
		(g->player[p].x +
		 (((72 - g->player[p].radius) / 16) + 2) * g->player[p].dx) /
		MOVE_SPEED;
	sensory_pixel_y =
		(g->player[p].y +
		 (((72 - g->player[p].radius) / 16) + 2) * g->player[p].dy) /
		MOVE_SPEED;
	// printf("OLD FRONT_PIXEL %i %i\n", g->player[p].old_front_pixel_x, g->player[p].old_front_pixel_y);
	// printf("SENSORY FRONT_PIXEL %i %i\n", sensory_pixel_x, sensory_pixel_y);
	if (sensory_pixel_x < 0 || sensory_pixel_x >= WIN_WIDTH ||
		sensory_pixel_y < 0 | sensory_pixel_y >= WIN_HEIGHT)
	{
		g->player[p].alive = false;
		g->player[p].deaths += 1;
		g->player[p].score += g->players - g->alive;
		g->alive -= 1;
		// printf("%s Player %i died.%s\n", g->player[p].ansi, p + 1, ANSI_RESET);
	}
	else if (g->screen.data[sensory_pixel_y * WIN_WIDTH + sensory_pixel_x])
	{
		g->player[p].alive = false;
		g->player[p].deaths += 1;
		g->player[p].score += g->players - g->alive;
		g->alive -= 1;
		add_point(
			g, p,
			g->screen.data[sensory_pixel_y * WIN_WIDTH + sensory_pixel_x]);
		// printf("%s Player %i died.%s\n", g->player[p].ansi, p + 1, ANSI_RESET);
		// printf("Player %lu died.\n", g->frame);
		// printf("Player %x died.\n", g->screen.data[g->player[p].front_pixel_y * WIN_WIDTH + g->player[p].front_pixel_x]);
	}
	// else
	// {
	// 	printf("Player %x lives.\n", g->screen.data[g->player[p].old_front_pixel_y * WIN_WIDTH + g->player[p].old_front_pixel_x]);
	// }
	for (dx = -CIRCLE_SIZE; dx <= CIRCLE_SIZE; dx++)
	{
		for (dy = -CIRCLE_SIZE; dy <= CIRCLE_SIZE; dy++)
		{
			int pixel_x = g->player[p].x / MOVE_SPEED + dx;
			int pixel_y = g->player[p].y / MOVE_SPEED + dy;
			// if (dx == 0 && dy == 0)
			// printf("PIXEL %i %i\n", pixel_x, pixel_y);
			// Check if the pixel is inside the circle and within screen boundaries
			if (dx * dx + dy * dy <= CIRCLE_SIZE * CIRCLE_SIZE &&
				pixel_x >= 0 && pixel_x < WIN_WIDTH && pixel_y >= 0 &&
				pixel_y < WIN_HEIGHT)
			{
				g->screen.data[pixel_y * WIN_WIDTH + pixel_x] =
					g->player[p].colour;
			}
		}
	}
}

void draw_players(t_game* g)
{
	int i;

	i = 0;
	while (i < g->players)
	{
		if (g->player[i].alive)
			draw_player(g, i);
		++i;
	}
}

void draw_char(t_game* g, t_char c)
{
	uint32_t y;
	uint32_t x;

	for (y = 0U; y < 64U; y++)
	{
		for (x = 0U; x < 64U; x++)
		{
			// mlx_pixel_put(g->mlx, g->win, i, j, color);
			if (x + c.pos_x >= g->frame / SLOW_DOWN % WIN_WIDTH &&
				x + c.pos_x < g->frame / SLOW_DOWN % WIN_WIDTH + 4)
			{
				// printf("text1\n");
				if (c.c < 128U && g->font[c.c].data[y * 64U + x] && c.colour)
				{
					// printf("text1\n");
					g->screen.data[(y + c.pos_y) * WIN_WIDTH + (x + c.pos_x)] =
						CLR_GOLD;
				}
				else if (c.c < 128U && c.c >= 32U &&
						 !g->font[c.c].data[y * 64U + x] && c.background)
					g->screen.data[(y + c.pos_y) * WIN_WIDTH + (x + c.pos_x)] =
						CLR_SILVER;
				else
				{
					//empty for now
				}
			}
			else
			{
				// printf("text2\n");
				if (c.c < 128U && g->font[c.c].data[y * 64U + x] && c.colour)
					g->screen.data[(y + c.pos_y) * WIN_WIDTH + (x + c.pos_x)] =
						c.colour;
				else if (c.c < 128U && c.c >= 32U &&
						 !g->font[c.c].data[y * 64U + x] && c.background)
					g->screen.data[(y + c.pos_y) * WIN_WIDTH + (x + c.pos_x)] =
						c.background;
				else
				{
					//empty for now
				}
			}
		}
	}
}

void draw_text(t_game* g, t_text text)
{
	t_char c;
	c.pos_x = text.pos_x;
	c.pos_y = text.pos_y;
	c.colour = text.colour;
	c.background = text.background;
	while (*(text.s))
	{
		c.c = *(text.s);
		draw_char(g, c);
		c.pos_x += 64;
		(text.s)++;
	}
}

// Function to copy the screen
void copy_screen(t_game* g)
{
	//possibly optimise with uint64_t
	uint32_t  x;
	uint32_t  y;
	uint32_t* data;
	int		  bpp, size_line, endian;

	y = 0;
	data = (uint32_t*)mlx_get_data_addr(g->img, &bpp, &size_line, &endian);
	(void)bpp;
	(void)size_line;
	(void)endian;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			data[y * WIN_WIDTH + x] = g->screen.data[y * WIN_WIDTH + x];
			x++;
		}
		y++;
	}
}

// Function to clear the screen
void clear_screen(t_game* g)
{
	//possibly optimise with uint64_t
	uint32_t x;
	uint32_t y;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			g->screen.data[y * WIN_WIDTH + x] = CLR_TRANSPARENT;
			// g->screen.data[y * WIN_WIDTH + x] = CLR_GRAY_5;
			x++;
		}
		y++;
	}
	// mlx_clear_window(g->mlx, g->win);
}

// Close window function

int close_window(void* param)
{
	t_game* g = (t_game*)param;
	mlx_loop_end(g->mlx); // Explicitly end the mlx_loop
	return 0;			  // Return 0 to indicate the handler has finished
}

// Key press event handler (set key state)
int key_press(int keycode, void* param)
{
	t_game* g = (t_game*)param;

	// if (keycode == KEY_ESC)
	// {
	// 	close_window(g); // Close the window
	// 	return (0);
	// }
	// else
	// {
	// 	printf("key %i\n", keycode);
	// }

	if (keycode >= 0 && keycode < 65536)
		g->keys[keycode] = true; // Mark key as pressed

	return (0);
}

// Key release event handler (unset key state)
int key_release(int keycode, void* param)
{
	t_game* g = (t_game*)param;

	if (keycode >= 0 && keycode < 65536)
		g->keys[keycode] = false; // Mark key as released

	return (0);
}

#define ASCII_PATH "assets/img/ascii.xpm"

//copy image

//read ascii
int read_ascii(t_game* g)
{
	uint32_t  y;
	uint32_t  x;
	uint32_t  c;
	uint32_t* data;

	data = (uint32_t*)mlx_get_data_addr(g->image.img, &g->image.bpp,
										&g->image.size_line, &g->image.endian);
	if (g->image.width != 640 || g->image.height != 640)
	{
		put_error("Failed to parse ascii.\n");
		return (1);
	}
	// put_str("Reading ascii.\n");
	y = 0;
	while (y < 640U)
	{
		x = 0;
		while (x < 640U)
		{
			c = 10U * (y / 64U) + (x / 64U) + 32U;

			if (c > ' ' && c <= '~' && data[y * 640U + x] != 0U)
				g->font[c].data[y % 64 * 64 + x % 64] = 0xFFFFFFFFU;
			x++;
		}
		y++;
	}
	// put_str("Read ascii.\n");
	return (0);
}

//init players 2-6
// count victories

// player colour and controls

//end when first player gets to 10 points

//crop iamge function

// typedef struct s_player
// {
// 	int64_t	x;
// 	int64_t	y;
// 	int64_t	dx;
// 	int64_t	dy;
// 	char	name[32];
// 	//enum coalition
// 	uint32_t	colour;
// 	uint16_t	key_left;
// 	uint16_t	key_right;
// 	uint16_t	score;
// 	uint8_t		deaths;
// 	uint8_t		wins;
// 	bool		on;
// 	// int32_t	front_pixel_x;
// 	// int32_t	front_pixel_y;
// 	// int32_t	old_front_pixel_x;
// 	// int32_t	old_front_pixel_y;
// 	float	radius;
// 	float	theta;
// 	float	d_theta;
// 	bool	alive;
// } t_player;

void init_player_1(t_game* g)
{
	g->player[0].alive = true;
	g->player[0].theta = (rand() % 1024) / 128.0 - 512.0;
	g->player[0].radius = 16 + (rand() % 32);
	// g->player[0].radius = 16;
	// Fixed distance on the circle corresponds to an angular step
	g->player[0].d_theta = 1.0 / (float)g->player[0].radius;
	g->player[0].on = true;
	sprintf(g->player[0].name, "PLAYER 1");
	g->player[0].key_left = KEY_Z;
	g->player[0].key_right = KEY_X;
	g->player[0].colour = CLR_DARK_BLUE;
	g->player[0].ansi = ANSI_COLOUR_BLUE_MEDIUM;
	// printf("%s Player 1 Left: %-10s | Right: %-10s %s\n", g->player[0].ansi, "KEY_Z", "KEY_C", ANSI_RESET);
	//position on hexagon
	g->player[0].x = WIN_WIDTH / 2.0 - (WIN_HEIGHT / 4.0) / cos(MY_PI / 6.0);
	g->player[0].x += (rand() % (WIN_WIDTH / 8)) - (WIN_WIDTH / 16);
	g->player[0].y = WIN_HEIGHT / 2.0; // Mid left
	g->player[0].y += (rand() % (WIN_HEIGHT / 8)) - (WIN_HEIGHT / 16);
	g->player[0].x *= MOVE_SPEED;
	g->player[0].y *= MOVE_SPEED;
}

void init_player_2(t_game* g)
{
	g->player[1].alive = true;
	g->player[1].theta = (rand() % 1024) / 128.0 - 512.0;
	g->player[1].radius = 16 + (rand() % 32);
	// g->player[1].radius = 64;
	// Fixed distance on the circle corresponds to an angular step
	g->player[1].d_theta = 1.0 / (float)g->player[1].radius;
	g->player[1].on = true;
	sprintf(g->player[1].name, "PLAYER 2");
	g->player[1].key_left = KEY_LEFT;
	g->player[1].key_right = KEY_DOWN;
	g->player[1].colour = CLR_DARK_GREEN;
	g->player[1].ansi = ANSI_COLOUR_GREEN_DARK;
	// printf("%s Player 2 Left: %-10s | Right: %-10s %s\n", g->player[1].ansi, "KEY_LEFT", "KEY_RIGHT", ANSI_RESET);
	g->player[1].x = WIN_WIDTH / 2.0 + (WIN_HEIGHT / 4.0) / cos(MY_PI / 6.0);
	g->player[1].x += (rand() % (WIN_WIDTH / 8)) - (WIN_WIDTH / 16);
	g->player[1].y = WIN_HEIGHT / 2.0; // Mid right
	g->player[1].y += (rand() % (WIN_HEIGHT / 8)) - (WIN_HEIGHT / 16);
	g->player[1].x *= MOVE_SPEED;
	g->player[1].y *= MOVE_SPEED;
}

void init_player_3(t_game* g)
{
	g->player[2].alive = true;
	g->player[2].theta = (rand() % 1024) / 128.0 - 512.0;
	g->player[2].radius = 16 + (rand() % 32);
	// Fixed distance on the circle corresponds to an angular step
	g->player[2].d_theta = 1.0 / (float)g->player[2].radius;
	g->player[2].on = true;
	sprintf(g->player[2].name, "PLAYER 3");
	g->player[2].key_left = KEY_TILDE;
	g->player[2].key_right = KEY_1;
	g->player[2].colour = CLR_DARK_RED;
	g->player[2].ansi = ANSI_COLOUR_RED_MEDIUM;
	// printf("%s Player 3 Left: %-10s | Right: %-10s %s\n", g->player[2].ansi, "KEY_TILDE", "KEY_2", ANSI_RESET);
	g->player[2].x =
		WIN_WIDTH / 2.0 - (WIN_HEIGHT / 4.0) / cos(MY_PI / 6.0) / 2.0;
	g->player[2].x += (rand() % (WIN_WIDTH / 8)) - (WIN_WIDTH / 16);
	g->player[2].y = WIN_HEIGHT / 2.0 - WIN_HEIGHT / 4.0; // Top left
	g->player[2].y += (rand() % (WIN_HEIGHT / 8)) - (WIN_HEIGHT / 16);
	g->player[2].x *= MOVE_SPEED;
	g->player[2].y *= MOVE_SPEED;
}

void init_player_4(t_game* g)
{
	g->player[3].alive = true;
	g->player[3].theta = (rand() % 1024) / 128.0 - 512.0;
	g->player[3].radius = 16 + (rand() % 32);
	// Fixed distance on the circle corresponds to an angular step
	g->player[3].d_theta = 1.0 / (float)g->player[3].radius;
	g->player[3].on = true;
	sprintf(g->player[3].name, "PLAYER 4");
	g->player[3].key_left = KEY_NUM_0;
	g->player[3].key_right = KEY_NUM_DOT;
	g->player[3].colour = CLR_DARK_CYAN;
	g->player[3].ansi = ANSI_COLOUR_CYAN_DARK;
	// printf("%s Player 4 Left: %-10s | Right: %-10s %s\n", g->player[3].ansi, "KEY_NUM_0", "KEY_NUM_DOT", ANSI_RESET);
	g->player[3].x =
		WIN_WIDTH / 2.0 + (WIN_HEIGHT / 4.0) / cos(MY_PI / 6.0) / 2.0;
	g->player[3].x += (rand() % (WIN_WIDTH / 8)) - (WIN_WIDTH / 16);
	g->player[3].y = WIN_HEIGHT / 2.0 + WIN_HEIGHT / 4.0; // Bottom right
	g->player[3].y += (rand() % (WIN_HEIGHT / 8)) - (WIN_HEIGHT / 16);
	g->player[3].x *= MOVE_SPEED;
	g->player[3].y *= MOVE_SPEED;
}

void init_player_5(t_game* g)
{
	g->player[4].alive = true;
	g->player[4].theta = (rand() % 1024) / 128.0 - 512.0;
	g->player[4].radius = 16 + (rand() % 32);
	// Fixed distance on the circle corresponds to an angular step
	g->player[4].d_theta = 1.0 / (float)g->player[4].radius;
	g->player[4].on = true;
	sprintf(g->player[4].name, "PLAYER 5");
	g->player[4].key_left = KEY_N;
	g->player[4].key_right = KEY_M;
	g->player[4].colour = CLR_DARK_MAGENTA;
	g->player[4].ansi = ANSI_COLOUR_MAGENTA_DARK;
	// printf("%s Player 5 Left: %-10s | Right: %-10s %s\n", g->player[4].ansi, "KEY_B", "KEY_M", ANSI_RESET);
	g->player[4].x =
		WIN_WIDTH / 2.0 - (WIN_HEIGHT / 4.0) / cos(MY_PI / 6.0) / 2.0;
	g->player[4].x += (rand() % (WIN_WIDTH / 8)) - (WIN_WIDTH / 16);
	g->player[4].y = WIN_HEIGHT / 2.0 + WIN_HEIGHT / 4.0; // Bottom left
	g->player[4].y += (rand() % (WIN_HEIGHT / 8)) - (WIN_HEIGHT / 16);
	g->player[4].x *= MOVE_SPEED;
	g->player[4].y *= MOVE_SPEED;
}

void init_player_6(t_game* g)
{
	g->player[5].alive = true;
	g->player[5].theta = (rand() % 1024) / 128.0 - 512.0;
	g->player[5].radius = 16 + (rand() % 32);
	// Fixed distance on the circle corresponds to an angular step
	g->player[5].d_theta = 1.0 / (float)g->player[5].radius;
	g->player[5].on = true;
	sprintf(g->player[5].name, "PLAYER 6");
	g->player[5].key_left = KEY_NUM_MULTIPLY;
	g->player[5].key_right = KEY_NUM_MINUS;
	g->player[5].colour = CLR_DARK_YELLOW;
	g->player[5].ansi = ANSI_COLOUR_YELLOW_DARK;
	// printf("%s Player 6 Left: %-10s | Right: %-10s %s\n", g->player[5].ansi, "KEY_NUM_*", "KEY_NUM_MINUS", ANSI_RESET);
	g->player[5].x =
		WIN_WIDTH / 2.0 + (WIN_HEIGHT / 4.0) / cos(MY_PI / 6.0) / 2.0;
	g->player[5].x += (rand() % (WIN_WIDTH / 8)) - (WIN_WIDTH / 16);
	g->player[5].y = WIN_HEIGHT / 2.0 - WIN_HEIGHT / 4.0; // Top right
	g->player[5].y += (rand() % (WIN_HEIGHT / 8)) - (WIN_HEIGHT / 16);
	g->player[5].x *= MOVE_SPEED;
	g->player[5].y *= MOVE_SPEED;
}

void init_players(t_game* g)
{
	init_player_1(g);
	init_player_2(g);
	init_player_3(g);
	init_player_4(g);
	init_player_5(g);
	init_player_6(g);
	g->alive = g->players;
}

void init_game(t_game* g)
{
	init_players(g);
	g->player[0].wins = 0;
	g->player[1].wins = 0;
	g->player[2].wins = 0;
	g->player[3].wins = 0;
	g->player[4].wins = 0;
	g->player[5].wins = 0;
	g->player[0].deaths = 0;
	g->player[1].deaths = 0;
	g->player[2].deaths = 0;
	g->player[3].deaths = 0;
	g->player[4].deaths = 0;
	g->player[5].deaths = 0;
	g->player[0].score = 0;
	g->player[1].score = 0;
	g->player[2].score = 0;
	g->player[3].score = 0;
	g->player[4].score = 0;
	g->player[5].score = 0;
	g->rounds = 0;
}

// Update function for continuous movement
int menu_loop(t_game* g)
{
	t_text text;
	t_text info[2];
	t_text players[6];
	text.pos_x = 224;
	text.pos_y = 32;
	text.colour = CLR_DARK_TURQUOISE;
	text.background = CLR_TRANSPARENT;
	text.s = "| . .. MENU .. . |";

	info[0].pos_x = 32;
	info[0].pos_y = 64 + 64 + 8;
	info[0].colour = CLR_DARK_TURQUOISE;
	info[0].background = CLR_TRANSPARENT;
	info[0].s = "PLAYER   |  LEFT | RIGHT";

	players[0].pos_x = 32;
	players[0].pos_y = 64 + 8 + (64 + 8) * 2;
	players[0].colour = CLR_DARK_BLUE;
	players[0].background = CLR_TRANSPARENT;
	players[0].s = "PLAYER 1 |     Z |     X";

	players[1].pos_x = 32;
	players[1].pos_y = 64 + 8 + (64 + 8) * 3;
	players[1].colour = CLR_DARK_GREEN;
	players[1].background = CLR_TRANSPARENT;
	players[1].s = "PLAYER 2 |  LEFT |  DOWN";

	players[2].pos_x = 32;
	players[2].pos_y = 64 + 8 + (64 + 8) * 4;
	if (g->players > 2)
		players[2].colour = CLR_DARK_RED;
	else
		players[2].colour = CLR_DARK_GRAY;
	players[2].background = CLR_TRANSPARENT;
	players[2].s = "PLAYER 3 |     ~ |     1";

	players[3].pos_x = 32;
	players[3].pos_y = 64 + 8 + (64 + 8) * 5;
	if (g->players > 3)
		players[3].colour = CLR_DARK_CYAN;
	else
		players[3].colour = CLR_DARK_GRAY;
	players[3].background = CLR_TRANSPARENT;
	players[3].s = "PLAYER 4 | NUM 0 | NUM .";

	players[4].pos_x = 32;
	players[4].pos_y = 64 + 8 + (64 + 8) * 6;
	if (g->players > 4)
		players[4].colour = CLR_DARK_MAGENTA;
	else
		players[4].colour = CLR_DARK_GRAY;
	players[4].background = CLR_TRANSPARENT;
	players[4].s = "PLAYER 5 |     N |     M";

	players[5].pos_x = 32;
	players[5].pos_y = 64 + 8 + (64 + 8) * 7;
	if (g->players > 5)
		players[5].colour = CLR_DARK_YELLOW;
	else
		players[5].colour = CLR_DARK_GRAY;
	players[5].background = CLR_TRANSPARENT;
	players[5].s = "PLAYER 6 | NUM * | NUM -";

	info[1].pos_x = 32;
	info[1].pos_y = WIN_HEIGHT - 64 - 32;
	info[1].colour = CLR_DARK_TURQUOISE;
	info[1].background = CLR_TRANSPARENT;
	info[1].s = "ENTER (play)  ESC (quit)";

	if (g->keys[KEY_UP])
	{
		g->keys[KEY_UP] = false;
		if (g->players > 2)
			g->players -= 1;
	}
	else if (g->keys[KEY_DOWN])
	{
		g->keys[KEY_DOWN] = false;
		if (g->players < 6)
			g->players += 1;
	}
	g->frame++;
	if ((g->frame % SLOW_DOWN))
		return (0);
	draw_text(g, text);
	draw_text(g, info[0]);
	draw_text(g, info[1]);
	draw_text(g, players[0]);
	draw_text(g, players[1]);
	draw_text(g, players[2]);
	draw_text(g, players[3]);
	draw_text(g, players[4]);
	draw_text(g, players[5]);
	copy_screen(g);
	// mlx_put_image_to_window(g->mlx, g->win, g->image.img, 100, 100);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	if (g->keys[KEY_ENTER])
	{
		g->alive = g->players;
		// printf("players %i\n", g->players);
		init_game(g);
		clear_screen(g);
		g->mode = 1;
	}
	if (g->keys[KEY_ESC])
	{
		close_window(g);
	}

	return (0);
}

bool	check_score(t_game *g)
{
	bool	retVal = false;
	for (uint8_t i = 0U; i < g->players; i++)
	{
		if (g->player[i].score >= WINNING_SCORE)
			retVal = true;
	}
	return retVal;
}

// Update function for continuous movement
int game_loop(t_game* g)
{
	t_text text;
	int	   p = 0;
	text.pos_x = 224;
	text.pos_y = 32;
	text.colour = CLR_DARK_TURQUOISE;
	text.background = CLR_TRANSPARENT;
	text.s = "| . ..CURVES.. . |";
	g->frame++;
	if ((g->frame % SLOW_DOWN))
		return (0);
	while (p < g->players)
	{
		if (g->player[p].alive)
		{
			// Rotate based on the fixed distance MOVE_SPEED
			if (g->keys[g->player[p].key_left])
			{
				g->player[p].theta -=
					g->player[p]
						.d_theta; // Move right along the circle by the fixed arc length
			}
			if (g->keys[g->player[p].key_right])
			{
				g->player[p].theta +=
					g->player[p]
						.d_theta; // Move right along the circle by the fixed arc length
			}
			// Compute new dx, dy based on the updated theta while maintaining MOVE_SPEED
			g->player[p].dx =
				(int)(g->player[p].radius *
					  cos(g->player[p].theta)); // dx along the circle
			g->player[p].dy =
				(int)(g->player[p].radius *
					  sin(g->player[p].theta)); // dy along the circle
			g->player[p].x += g->player[p].dx;
			g->player[p].y += g->player[p].dy;
			// printf("%i f %f %f %f\n", p, g->player[1].radius, cos(g->player[1].theta), g->player[1].radius * cos(g->player[1].theta));
			// printf("dx %li\n", g->player[1].dx);
			// printf("x %li\n", g->player[1].x);
			// printf("dy %li\n", g->player[1].dy);
			// printf("y %li\n", g->player[1].y);
		}
		p++;
	}
	// printf("B: %f %li %li %li %li\n", g->player[0].theta, g->player[0].x, g->player[0].y, g->player[0].dx, g->player[0].dy);

	// if (g->keys[KEY_UP] && g->y > 0)
	// {
	// 	g->y -= MOVE_SPEED;
	// 	moved = true;
	// }
	// if (g->keys[KEY_DOWN] && g->y < WIN_HEIGHT - CIRCLE_SIZE)
	// {
	// 	g->y += MOVE_SPEED;
	// 	moved = true;
	// }
	// printf("hi\n");
	// clear_screen(g);
	draw_players(g);
	for (uint8_t i = 0; i < g->players; i++)
		printScore(g, i);

	draw_text(g, text);
	copy_screen(g);
	// mlx_put_image_to_window(g->mlx, g->win, g->image.img, 100, 100);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	if (g->alive <= 1)
	{
		if (g->alive == 1)
		{
			if (g->player[0].alive)
			{
				g->player[0].wins += 1;
				g->player[0].score += g->players;
				// printf("%s%s Player 1 WON! %s\n", g->player[0].ansi,
				// 	   ANSI_REVERSE, ANSI_RESET);
			}
			else if (g->player[1].alive)
			{
				g->player[1].wins += 1;
				g->player[1].score += g->players;
				// printf("%s%s Player 2 WON! %s\n", g->player[1].ansi,
				// 	   ANSI_REVERSE, ANSI_RESET);
			}
			else if (g->player[2].alive)
			{
				g->player[2].wins += 1;
				g->player[2].score += g->players;
				// printf("%s%s Player 3 WON! %s\n", g->player[2].ansi,
				// 	   ANSI_REVERSE, ANSI_RESET);
			}
			else if (g->player[3].alive)
			{
				g->player[3].wins += 1;
				g->player[3].score += g->players;
				// printf("%s%s Player 4 WON! %s\n", g->player[3].ansi,
				// 	   ANSI_REVERSE, ANSI_RESET);
			}
			else if (g->player[4].alive)
			{
				g->player[4].wins += 1;
				g->player[4].score += g->players;
				// printf("%s%s Player 5 WON! %s\n", g->player[4].ansi,
				// 	   ANSI_REVERSE, ANSI_RESET);
			}
			else if (g->player[5].alive)
			{
				g->player[5].wins += 1;
				g->player[5].score += g->players;
				// printf("%s%s Player 6 WON! %s\n", g->player[5].ansi,
				// 	   ANSI_REVERSE, ANSI_RESET);
			}
			else
				printf("%s Nobody won %s\n", ANSI_BG_GRAY, ANSI_RESET);
			g->player[0].alive = false;
			g->player[1].alive = false;
			g->player[2].alive = false;
			g->player[3].alive = false;
			g->player[4].alive = false;
			g->player[5].alive = false;
			g->alive = 0;
			// int i = 0;
			// while (i < g->players)
			// {
			// 	printf("%s%s Player %i: | SCORE: %3i | WINS %2i | DEATHS: %2i "
			// 		   "%s\n",
			// 		   g->player[i].ansi, ANSI_REVERSE, i, g->player[i].score,
			// 		   g->player[i].wins, g->player[i].deaths, ANSI_RESET);
			// 	i++;
			// }
		}
		if (!check_score(g) && g->keys[KEY_SPACE])
		{
			g->rounds += 1;
			clear_screen(g);
			init_players(g);
			g->keys[KEY_SPACE] = false;
		}
		else if (check_score(g) && g->keys[KEY_SPACE])
		{
			int i = 0;
			while (i < g->players)
			{
				printf("%s%s Player %i: | SCORE: %3i | WINS %2i | DEATHS: %2i "
					   "%s\n",
					   g->player[i].ansi, ANSI_REVERSE, i, g->player[i].score,
					   g->player[i].wins, g->player[i].deaths, ANSI_RESET);
				i++;
			}
			clear_screen(g);
			g->mode = 0;
		}
	}
	if (g->keys[KEY_ESC])
	{
		clear_screen(g);
		g->mode = 0;
		g->keys[KEY_ESC] = false;
	}
	return (0);
}

//pause loop after game
//pause loop before game

int main_loop(void* param)
{
	t_game* g = (t_game*)param;
	int		retVal;

	retVal = 0;
	if (g->mode == 0)
		retVal = menu_loop(g);
	else if (g->mode == 1)
		retVal = game_loop(g);
	// else if (g->mode == 2)
	// 	retVal = aftergame_loop(g);
	// else if (g->mode == 3)
	// 	retVal = pregame_loop(g);
	// else if (g->mode == 4)
	// 	retVal = endgame_loop(g);
	return (retVal);
}

//welcome screen

// loop games

// score round
// score game

// next round space

//team play

// Main game loop
int game(t_game* g)
{
	g->players = 2;
	g->mlx = mlx_init();
	if (!g->mlx)
	{
		put_error("MiniLibX initialization failed.\n");
		return 1;
	}

	g->win = mlx_new_window(g->mlx, WIN_WIDTH, WIN_HEIGHT, "Move the Square");
	if (!g->win)
	{
		put_error("Failed to create MiniLibX window.\n");
		return 1;
	}

	g->image.img = mlx_xpm_file_to_image(g->mlx, ASCII_PATH, &g->image.width,
										 &g->image.height);
	if (!g->image.img)
	{
		put_error("Failed to load sprite sheet\n");
		return (1);
	}
	if (read_ascii(g))
	{
		return (1);
	}
	g->img = mlx_new_image(g->mlx, WIN_WIDTH, WIN_HEIGHT);
	// draw_players(g);
	mlx_hook(g->win, 2, 1L << 0, key_press, g);	  // Handle key press
	mlx_hook(g->win, 3, 1L << 1, key_release, g); // Handle key release
	mlx_hook(g->win, 17, 0, close_window, g);	  // Handle window close event
	// mlx_expose_hook(g->win, update_game, g);
	mlx_loop_hook(g->mlx, main_loop, g); // Continuous movement check

	// Enter the main event loop
	mlx_loop(g->mlx); // This will run until mlx_loop_end() is called
	if (g->mlx && g->image.img && g->img) // Ensure pointers are valid
	{
		mlx_destroy_image(g->mlx, g->image.img);
		g->image.img = NULL; // Mark the image as destroyed
		mlx_destroy_image(g->mlx, g->img);
		g->img = NULL; // Mark the image as destroyed
	}
	if (g->mlx && g->win) // Ensure pointers are valid
	{
		mlx_destroy_window(g->mlx, g->win);
		g->win = NULL; // Mark the window as destroyed
	}
	mlx_destroy_display(
		g->mlx); // Clean up any resources related to the display
	free(g->mlx);
	g->mlx = NULL;
	return 0; // This will never be reached unless the loop ends
}
