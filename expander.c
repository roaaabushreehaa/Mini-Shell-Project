/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:43:00 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/03 16:46:02 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expander_main(t_token *token)
{
	t_token	*temp;
	char	*expanded;

	temp = token;
	while (temp)
	{
		if (temp->value)
        {
            if (ft_strchr(temp->value, '\'') && ft_strchr(temp->value, '"'))
                expanded = handle_mixed_quotes(temp);
            else if (ft_strchr(temp->value, '\''))
                expanded = handle_onequote_expander(temp);
            else if (ft_strchr(temp->value, '"'))
                expanded = handel_twoquotes(temp);
                
			if (expanded)
			{
				free(temp->value);
				temp->value = expanded;
				printf("Expanded: %s\n", expanded);
			}
		}
		temp = temp->next;
	}
	return (0);
}

char	*handle_onequote_expander(t_token *token)
{
	char *result = ft_strdup("");
	char *value = token->value;
	char *temp = NULL;
	int i = 0;
	char char_str[2];
	int inside_quotes = 0;

	while (value[i])
	{
		if (value[i] == '\'')
		{
			inside_quotes = !inside_quotes;
		}
		else if (inside_quotes)
		{
			temp = result;
			char_str[0] = value[i];
			char_str[1] = '\0';
			result = ft_strjoin(result, char_str);
			free(temp);
		}
		else
		{
			temp = result;
			char_str[0] = value[i];
			char_str[1] = '\0';
			result = ft_strjoin(result, char_str);
			free(temp);
		}
		i++;
	}

	return (result);
}


char *handel_twoquotes(t_token *token)
{
    char *result = ft_strdup("");
    char *value = token->value;
    char *temp = NULL;
    int i = 0;
    char char_str[2];
    int inside_quotes = 0;

    while (value[i])
    {
        if (value[i] == '"')
        {
            inside_quotes = !inside_quotes;
        }
        else
        {
            temp = result;
            char_str[0] = value[i];
            char_str[1] = '\0';
            result = ft_strjoin(result, char_str);
            free(temp);
        }
        i++;
    }

    return (result);
}

char *handle_mixed_quotes(t_token *token)
{
    char *result = ft_strdup("");
    char *value = token->value;
    char *temp = NULL;
    int i = 0;
    char char_str[2];
    int inside_single = 0;
    int inside_double = 0;
    while (value[i])
    {
        if (value[i] == '\'' && !inside_double)
            inside_single = !inside_single;
        else if (value[i] == '"' && !inside_single)
            inside_double = !inside_double;
        else
        {
            temp = result;
            char_str[0] = value[i];
            char_str[1] = '\0';
            result = ft_strjoin(result, char_str);
            free(temp);
        }
        i++;
    }
    return (result);
}