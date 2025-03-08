/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:43:00 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/08 14:21:22 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
            // else if(ft_strchr(temp->value, '$'))
            //     expanded = handle_dollar_expander(temp);
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

// char *handle_mixed_quotes(t_token *token)
// {
//     if (!token || !token->value)
//         return (NULL);
        
//     char *result;
//     char *value;
//     int i;
//     int inside_double;
//     int inside_single;
//     char *old_result;
//     char new_char[2];
//     i = 0;
//     inside_double = 0;
//     result = ft_strdup("");
//     value = token->value;
//     inside_single = 0;
//     while (value[i])
//     {
//         if (value[i] == '"' && !inside_single)
//         {
//             inside_double = !inside_double;
//             if (inside_single)
//             {
//                 old_result = result;
//                 result = ft_strjoin(result, "\"");
//                 free(old_result);
//             }
//         }
//         else if (value[i] == '\'' && !inside_double)
//         {
//             inside_single = !inside_single;
//             if (inside_double)
//             {
//                 char *old_result = result;
//                 result = ft_strjoin(result, "'");
//                 free(old_result);
//             }
//         }
//         else
//         {
//             old_result = result;
//             new_char[0] = value[i];
//             new_char[1] = '\0';
//             result = ft_strjoin(result, new_char);
//             free(old_result);
//         }
//         i++;
//     }
    
//     return (result);
// }

char *handle_mixed_quotes(t_token *token)
{
    if (!token || !token->value)
        return (NULL);
        
    char *result;
    char *value;
    int i;
    int inside_double;
    int inside_single;
    
    i = 0;
    inside_double = 0;
    inside_single = 0;
    result = ft_strdup("");
    value = token->value;
    
    while (value[i])
    {
        if (value[i] == '"' && !inside_single)
        {
            inside_double = !inside_double;
            i++;
            continue;
        }
        else if (value[i] == '\'' && !inside_double)
        {
            inside_single = !inside_single;
            i++;
            continue;
        }
        
        char *old_result = result;
        char new_char[2] = {value[i], '\0'};
        result = ft_strjoin(result, new_char);
        free(old_result);
        i++;
    }
    return result;
}

char *handle_twoquotes(t_token *token)
{
    char *str;
    char *result;
    int i;
    int j;

    str = ft_strdup(token->value);
    result = malloc(ft_strlen(str) + 1);
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '"' && (i == 0 || str[i - 1] != '\\'))
        {
            i++;
            while (str[i] && str[i] != '"')
                result[j++] = str[i++];
            if (str[i])
                i++;
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    free(str);
    return (result);
}

char *handle_onequote_expander(t_token *token)
{
    if (!token || !token->value)
        return (NULL);
    
    char *old_result;
    char new_char[2];
    char *result;
    char *value;
    int i;

    int inside_quotes;
    result = ft_strdup("");
    value = token->value;
    inside_quotes = 0;
    i = 0;
    while (value[i])
    {
        if (value[i] == '\'')
        {
            inside_quotes = !inside_quotes;
            i++;
            continue;
        }
        old_result = result;
        new_char[0] = value[i];
        new_char[1] = '\0';
        result = ft_strjoin(result, new_char);
        free(old_result);
        
        i++;
    }
    return (result);
}

// char *handle_dollar_expander(t_token *token, t_env *env)
// {
//     char *result = ft_strdup("");
//     char *str = token->value;
//     int i = 0;
//     int len;
//             ft_strlcpy(env_value, str + start, len + 1);
//             result = ft_strjoin(result,);
//         }
//     }
//     return result;
// }