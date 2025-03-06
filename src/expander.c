/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:43:00 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/06 15:17:20 by rabu-shr         ###   ########.fr       */
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
void init_quote_state(t_quote_state *state, char *token_value)
{
    state->result = ft_strdup("");
    state->value = token_value;
    state->i = 0;
    state->inside_double = 0;
    state->inside_single = 0;
}

void process_quote_char(t_quote_state *state, char quote_char)
{
    char *old_result = state->result;
    
    if (quote_char == '"' && !(state->inside_single))
    {
        state->inside_double = !(state->inside_double);
        if (state->inside_single)
        {
            state->result = ft_strjoin(state->result, "\"");
            free(old_result);
        }
    }
    else if (quote_char == '\'' && !(state->inside_double))
    {
        state->inside_single = !(state->inside_single);
        if (state->inside_double)
        {
            state->result = ft_strjoin(state->result, "'");
            free(old_result);
        }
    }
}

static void add_char_to_result(t_quote_state *state)
{
    char *temp = state->result;
    char curr[2] = {state->value[state->i], '\0'};
    state->result = ft_strjoin(state->result, curr);
    free(temp);
}

char *handle_mixed_quotes(t_token *token)
{
    if (!token || !token->value)
        return (NULL);
    t_quote_state state;
    init_quote_state(&state, token->value);
    int is_first_quote = 1;
    
    while (state.value[state.i])
    {
        if (state.value[state.i] == '"')
        {
            if (!state.inside_single)
            {
                state.inside_double = !state.inside_double;
                if (is_first_quote || (state.value[state.i + 1] == 'c' && !state.inside_double))
                {
                    is_first_quote = 0;
                    state.i++;
                    continue;
                }
            }
            add_char_to_result(&state);
        }
        else if (state.value[state.i] == '\'')
        {
            if (!state.inside_double)
            {
                state.inside_single = !state.inside_single;
                add_char_to_result(&state);
            }
            else
                add_char_to_result(&state);
        }
        else
            add_char_to_result(&state);
        state.i++;
    }
    return state.result;
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
//     int start;
//     char *env_value;
        
//     while (str[i])
//     {
//         if (str[i] == '$' && str[i + 1])
//         {
//             i++;
//             start = i;
//             while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
//                         i++;
//             len = i - start;
//             env_value=malloc(len + 1);
//             ft_strlcpy(env_value, str + start, len + 1);
//             result = ft_strjoin(result,);
//         }
//     }
//     return result;
// }