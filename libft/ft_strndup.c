/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:30:59 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/03 13:31:30 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strndup(const char *s, size_t n)
{
    size_t i;
    char *copy;

    copy = (char *)malloc(n + 1);
    if (!copy)
        return NULL;

    for (i = 0; i < n && s[i]; i++)
        copy[i] = s[i];

    copy[i] = '\0';
    return copy;
}
