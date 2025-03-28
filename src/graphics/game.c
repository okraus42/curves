/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 17:03:00 by okraus            #+#    #+#             */
/*   Updated: 2025/03/28 17:42:22 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "defines.h"
#include <stdio.h>	//for debug
#include <stdlib.h> //for free
#include <X11/Xlib.h>  // Needed for X11 functions
#include <math.h> // atan2(), cos(), sin(), sqrt()

# include "mlx.h"
# include <stdlib.h>

// Function to draw a blue square

void draw_player(t_game* g, uint8_t p)
{
	int dx, dy;
	int sensory_pixel_x, sensory_pixel_y;
	// g->player[p].old_front_pixel_x = g->player[p].front_pixel_x;
	// g->player[p].old_front_pixel_y = g->player[p].front_pixel_y;
	// g->player[p].front_pixel_x = (g->player[p].x + g->player[p].dx) / g->player[p].radius;
	// g->player[p].front_pixel_y = (g->player[p].y + g->player[p].dy) / g->player[p].radius;
	sensory_pixel_x = (g->player[p].x + 3 * g->player[p].dx) / g->player[p].radius;
	sensory_pixel_y = (g->player[p].y + 3 * g->player[p].dy) / g->player[p].radius;
	// printf("OLD FRONT_PIXEL %i %i\n", g->player[p].old_front_pixel_x, g->player[p].old_front_pixel_y);
	// printf("SENSORY FRONT_PIXEL %i %i\n", sensory_pixel_x, sensory_pixel_y);
	if (sensory_pixel_x < 0 || sensory_pixel_x >= WIN_WIDTH
		|| sensory_pixel_y < 0 | sensory_pixel_y >= WIN_HEIGHT)
	{
		g->player[p].alive = false;
		printf("Player %i died.\n", p);
	}
	else if (g->screen.data[sensory_pixel_y * WIN_WIDTH + sensory_pixel_x])
	{
		g->player[p].alive = false;
		printf("Player %i died.\n", p);
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
            int pixel_x = g->player[p].x / g->player[p].radius + dx;
            int pixel_y = g->player[p].y / g->player[p].radius + dy;
			// if (dx == 0 && dy == 0)
            	// printf("PIXEL %i %i\n", pixel_x, pixel_y);
            // Check if the pixel is inside the circle and within screen boundaries
            if (dx * dx + dy * dy <= CIRCLE_SIZE * CIRCLE_SIZE &&
                pixel_x >= 0 && pixel_x < WIN_WIDTH &&
                pixel_y >= 0 && pixel_y < WIN_HEIGHT)
            {
				// if (pixel_x == g->player[p].front_pixel_x && pixel_y == g->player[p].front_pixel_y)
				// 	g->screen.data[pixel_y * WIN_WIDTH + pixel_x] = CLR_LIGHT_BLUE;
				// else
				if (p == 0)
					g->screen.data[pixel_y * WIN_WIDTH + pixel_x] = CLR_BLUE;
				else if (p == 1)
					g->screen.data[pixel_y * WIN_WIDTH + pixel_x] = CLR_DARK_GREEN;
            }
        }
    }
}


void draw_players(t_game* g)
{
	int	i;

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
			if (x + c.pos_x >= g->frame / SLOW_DOWN % WIN_WIDTH && x + c.pos_x < g->frame / SLOW_DOWN % WIN_WIDTH + 4)
			{
				// printf("text1\n");
				if (c.c < 128U && g->font[c.c].data[y * 64U + x] && c.colour)
				{
					// printf("text1\n");
					g->screen.data[(y + c.pos_y) * WIN_WIDTH + (x + c.pos_x)] = CLR_GOLD;
				}
				else if (c.c < 128U && c.c >= 32U && !g->font[c.c].data[y * 64U + x] && c.background)
					g->screen.data[(y + c.pos_y) * WIN_WIDTH + (x + c.pos_x)] = CLR_SILVER;
				else
				{
					//empty for now
				}
			}
			else
			{
				// printf("text2\n");
				if (c.c < 128U && g->font[c.c].data[y * 64U + x] && c.colour)
					g->screen.data[(y + c.pos_y) * WIN_WIDTH + (x + c.pos_x)] = c.colour;
				else if (c.c < 128U && c.c >= 32U && !g->font[c.c].data[y * 64U + x] && c.background)
					g->screen.data[(y + c.pos_y) * WIN_WIDTH + (x + c.pos_x)] = c.background;
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
	t_char	c;
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
	uint32_t	x;
	uint32_t	y;
	uint32_t	*data;
	int bpp, size_line, endian;

	y = 0;
	data = (uint32_t *)mlx_get_data_addr(g->img, &bpp, &size_line, &endian);
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
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			g->screen.data[y * WIN_WIDTH + x] = CLR_BLACK;
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

	


	if (keycode == KEY_ESC)
	{
		close_window(g); // Close the window
		return (0);
	}
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

# define ASCII_PATH "assets/img/ascii.xpm"

// Update function for continuous movement
int update_game(void* param)
{
	t_game* g = (t_game*)param;
	t_text	text;
	text.pos_x = 224;
	text.pos_y = 32;
	text.colour = CLR_DARK_MAROON;
	text.background = CLR_DARK_TEAL;
	text.s = "   ...CURVES...   ";
	g->frame++;
	if ((g->frame % SLOW_DOWN))
		return (0);
	if (g->player[0].alive)
	{
		// Rotate based on the fixed distance MOVE_SPEED
		if (g->keys[KEY_LEFT])
		{
			g->player[0].theta -= g->player[0].d_theta;  // Move right along the circle by the fixed arc length
		}
		if (g->keys[KEY_RIGHT])
		{
			g->player[0].theta += g->player[0].d_theta;  // Move right along the circle by the fixed arc length
		}
		// Compute new dx, dy based on the updated theta while maintaining MOVE_SPEED
		g->player[0].dx = (int)(g->player[0].radius * cos(g->player[0].theta));  // dx along the circle
		g->player[0].dy = (int)(g->player[0].radius * sin(g->player[0].theta));  // dy along the circle
		g->player[0].x += g->player[0].dx;
		g->player[0].y += g->player[0].dy;
	}

	if (g->player[1].alive)
	{
		// Rotate based on the fixed distance MOVE_SPEED
		if (g->keys[KEY_A])
		{
			g->player[1].theta -= g->player[1].d_theta;  // Move right along the circle by the fixed arc length
		}
		if (g->keys[KEY_D])
		{
			g->player[1].theta += g->player[1].d_theta;  // Move right along the circle by the fixed arc length
		}
		// Compute new dx, dy based on the updated theta while maintaining MOVE_SPEED
		g->player[1].dx = (int)(g->player[1].radius * cos(g->player[1].theta));  // dx along the circle
		g->player[1].dy = (int)(g->player[1].radius * sin(g->player[1].theta));  // dy along the circle
		g->player[1].x += g->player[1].dx;
		g->player[1].y += g->player[1].dy;
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
	draw_text(g, text);
	copy_screen(g);
	// mlx_put_image_to_window(g->mlx, g->win, g->image.img, 100, 100);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	if (!g->player[0].alive && !g->player[1].alive)
		close_window(g);
	return (0);
}

//copy image


//read ascii
int	read_ascii(t_game *g)
{
	uint32_t	y;
	uint32_t	x;
	uint32_t	c;
	uint32_t	*data;

	data = (uint32_t *)mlx_get_data_addr(g->image.img, &g->image.bpp, &g->image.size_line, &g->image.endian);
	if (g->image.width != 640 || g->image.height != 640)
	{
		put_error("Failed to parse ascii.\n");
		return (1);
	}
	put_str("Reading ascii.\n");
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
	put_str("Read ascii.\n");
	return (0);
}



//crop iamge function

// Main game loop
int game(t_game* g)
{
	g->players = 2;
	g->player[0].alive = true;
	g->player[0].dx = g->player[0].radius * 141 / 200;
	g->player[0].dy = g->player[0].radius * 141 / 200;
	g->player[1].alive = true;
	g->player[1].dx = -g->player[1].radius * 141 / 200;
	g->player[1].dy = -g->player[1].radius * 141 / 200;
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
	// Compute the current angle (theta) from dx and dy
	// printf("A: %f %li %li %li %li\n", g->player[0].theta, g->player[0].x, g->player[0].y, g->player[0].dx, g->player[0].dy);
	g->player[0].theta = atan2(g->player[0].dy, g->player[0].dx);
	g->player[0].theta = 0.42f;

	//hardcoded radius for now
	g->player[0].radius = 32;

	// Compute the angular change (delta_theta) corresponding to the MOVE_SPEED
	g->player[0].d_theta = MOVE_SPEED / (float)g->player[0].radius; // Fixed distance on the circle corresponds to an angular step
	

	// Create an array to store mlx and win pointers to pass in key press handler....
	g->player[0].x = (WIN_WIDTH / 3 - CIRCLE_SIZE / 2) * g->player[0].radius;
	g->player[0].y = (WIN_HEIGHT / 2 - CIRCLE_SIZE / 2) * g->player[0].radius ;

	g->player[1].theta = atan2(g->player[1].dy, g->player[1].dx);
	g->player[1].theta = -2.42f;

	//hardcoded radius for now
	g->player[1].radius = 32;

	// Compute the angular change (delta_theta) corresponding to the MOVE_SPEED
	g->player[1].d_theta = MOVE_SPEED / (float)g->player[1].radius; // Fixed distance on the circle corresponds to an angular step
	

	// Create an array to store mlx and win pointers to pass in key press handler....
	g->player[1].x = (WIN_WIDTH / 3 * 2 - CIRCLE_SIZE / 2) * g->player[1].radius;
	g->player[1].y = (WIN_HEIGHT /  2 - CIRCLE_SIZE / 2) * g->player[1].radius ;

	g->image.img =
		mlx_xpm_file_to_image(g->mlx, ASCII_PATH, &g->image.width, &g->image.height);
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
	mlx_expose_hook(g->win, update_game, g);
	mlx_loop_hook(g->mlx, update_game, g);		  // Continuous movement check

	// Enter the main event loop
	mlx_loop(g->mlx);		  // This will run until mlx_loop_end() is called
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
