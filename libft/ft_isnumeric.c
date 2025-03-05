/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumeric.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:04:08 by jalqam            #+#    #+#             */
/*   Updated: 2025/02/27 16:25:58 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_isnumeric(char *str)
{
	int i;

	i = 0;
	if(!str || !str[0])
		return(0);
	if(str[i] == '+' || str[i] == '-')
		i++;
	if(!str[i])
		return (0);
	while (str[i])
	{
		if(!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}