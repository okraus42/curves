/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:31:13 by okraus            #+#    #+#             */
/*   Updated: 2025/03/30 17:24:17 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

// utils
void	put_error(char const *const str);
void	put_str(char const *const str);
int		game(t_game *g);
void	*ok_bzero(void *ptr, size_t num);

#endif /* MAIN_H */
