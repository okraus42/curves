/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:31:09 by okraus            #+#    #+#             */
/*   Updated: 2025/03/30 14:44:42 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "main.h"
#include <unistd.h> //for debug
#include <stdlib.h> //for malloc and free and srand()
#include <time.h>

int main(int argc, char *argv[])
{
	t_game	*g;
	int		retVal;

	retVal = 0;
	if (argc == 1 && argv[0][0])
	{
		srand(time(0));
		g = malloc(sizeof(t_game));
		if (!g)
			return (1);
		ok_bzero(g, sizeof(t_game));
		game(g);
		free(g);
    }
	else
		retVal = 1;
	return (retVal);
}
