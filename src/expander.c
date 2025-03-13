/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:43:00 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/13 16:32:48 by jalqam           ###   ########.fr       */
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

static char *get_env_value(t_env *env, char *key)
{
    t_env *current;

    current = env;
    while (current)
    {
        if (!ft_strcmp(current->key, key))
            return (current->value);
        current = current->next;
    }
    return (NULL);
}

static char *ft_strjoin_char(char *str, char c)
{
    char *result;
    int i;

    i = 0;
    result = malloc(ft_strlen(str) + 2);
    if (!result)
        return (NULL);
    while (str[i])
    {
        result[i] = str[i];
        i++;
    }
    result[i] = c;
    result[i + 1] = '\0';
    return (result);
}

char *handle_dollar_expander(t_token *token, t_env *env)
{
    char *result;
    char *str;
    int i;
    int start;
    char *var_name;
    char *var_value;

    result = ft_strdup("");
    str = token->value;
    i = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
        {
            i++;
            start = i;
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                i++;
            var_name = ft_substr(str, start, i - start);
            var_value = get_env_value(env, var_name);
            char *temp = result;
            result = ft_strjoin(result, var_value ? var_value : "");
            free(temp);
            free(var_name);
        }
        else
        {
            char *temp = result;
            result = ft_strjoin_char(result, str[i++]);
            free(temp);
        }
    }
    return (result);
}
