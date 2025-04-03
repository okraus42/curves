/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 17:16:34 by okraus            #+#    #+#             */
/*   Updated: 2025/04/03 18:23:45 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef GAME_H
# define GAME_H

#ifdef LIB
    #if LIB == 1
        #include "SDL.h"
    #else
        #include "mlx.h"
    #endif
#endif
#include "defines.h"
// utils
void	put_error(char const *const str);
void	put_str(char const *const str);
void	printScore(t_game *g, int p);


#endif /* GAME_H */
