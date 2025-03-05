/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:43:00 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/05 17:10:25 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int expander_main(t_token *token)
{
    t_token *temp;
    char *expanded = NULL;

    temp = token;
    while (temp)
    {
        expanded = NULL;
        if (temp->value)
        {
            if (ft_strcmp(temp->value, "\"\"") == 0 || 
                ft_strcmp(temp->value, "''") == 0)
            {
                free(temp->value);
                temp->value = ft_strdup("");
                continue;
            }
            if (ft_strchr(temp->value, '\'') && ft_strchr(temp->value, '"'))
                expanded = handle_mixed_quotes(temp);
            else if (ft_strchr(temp->value, '\''))
                expanded = handle_onequote_expander(temp);
            else if (ft_strchr(temp->value, '"'))
                expanded = handle_twoquotes(temp);
            if (expanded)
            {
                free(temp->value);
                temp->value = expanded;
            }
        }
        temp = temp->next;
    }
    return (0);
}

char *handle_mixed_quotes(t_token *token)
{
    if (!token || !token->value)
        return (NULL);
        
    char *result = ft_strdup("");
    char *value = token->value;
    int i = 0;
    int inside_double = 0;
    int inside_single = 0;
    
    while (value[i])
    {
        if (value[i] == '"' && !inside_single)
        {
            inside_double = !inside_double;
            if (inside_single)
            {
                char *old_result = result;
                result = ft_strjoin(result, "\"");
                free(old_result);
            }
        }
        else if (value[i] == '\'' && !inside_double)
        {
            inside_single = !inside_single;
            if (inside_double)
            {
                char *old_result = result;
                result = ft_strjoin(result, "'");
                free(old_result);
            }
        }
        else
        {
            char *old_result = result;
            char new_char[2] = {value[i], '\0'};
            result = ft_strjoin(result, new_char);
            free(old_result);
        }
        i++;
    }
    
    return (result);
}

char *handle_twoquotes(t_token *token)
{
    if (!token || !token->value)
        return (NULL);
        
    char *result = ft_strdup("");
    char *value = token->value;
    int i = 0;
    
    while (value[i])
    {
        if (value[i] == '"')
        {
            i++;
            continue;
        }
        char *old_result = result;
        char new_char[2] = {value[i], '\0'};
        result = ft_strjoin(result, new_char);
        free(old_result);
        i++;
    }
    return (result);
}

char *handle_onequote_expander(t_token *token)
{
    if (!token || !token->value)
        return (NULL);
        
    char *result = ft_strdup("");
    char *value = token->value;
    int i = 0;
    int inside_quotes = 0;
    
    while (value[i])
    {
        if (value[i] == '\'')
        {
            inside_quotes = !inside_quotes;
            i++;
            continue;
        }
        
        char *old_result = result;
        char new_char[2] = {value[i], '\0'};
        result = ft_strjoin(result, new_char);
        free(old_result);
        
        i++;
    }
    return (result);
}
