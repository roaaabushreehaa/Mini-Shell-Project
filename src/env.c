/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:27:24 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/06 14:49:08 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_env_node(t_env *node)
{
    free(node->key);
    free(node->value);
    free(node);
}

t_env *create_env_node(char *var)
{
    t_env *node;
    char *equal_char;

    equal_char = ft_strchr(var, '=');
    if (!equal_char)
        return (NULL);
    node = ft_calloc(1, sizeof(t_env));
    if (!node)
        return (NULL);
    node->key = ft_calloc((equal_char - var) + 1, sizeof(char));
    if (!node->key)
    {
        free(node);
        return (NULL);
    }
    ft_strlcpy(node->key, var, equal_char - var + 1);
    node->value = ft_strdup(equal_char + 1);
    if (!node->value)
    {
        free(node->key);
        free(node);
        return (NULL);
    }
    node->next = NULL;
    return (node);
}

// void	free_env_list(t_env *env)
// {
//     t_env	*temp;

//     while (env)
//     {
//         temp = env;
//         env = env->next;
//         free_env_node(temp);
//     }
// }

t_env	*init_envp(char **env_list)
{
    t_env	*env;
    t_env	*last;
    t_env	*temp;
    int		i;

    i = 0;
    env = NULL;
    last = NULL;
    while (env_list[i])
    {
        temp = create_env_node(env_list[i]);
        if (!temp)
        {
            free_env_node(env);
            return (NULL);
        }
        append_node(&env, &last, temp);
        i++;
    }
    return (env);
}


t_env *find_env_node(t_env *env, char *key)
{
    if (!env || !key)
        return (NULL);
    while (env)
    {
        if (!ft_strcmp(env->key, key))
            return (env);
        env = env->next;
    }
    return (NULL);
}

void	update_env_value(t_env *node, char *var)
{
    char	*equal_sign;
    char	*new_value;

    equal_sign = ft_strchr(var, '=');
    if (!equal_sign)
        return ;
    new_value = ft_strdup(equal_sign + 1);
    free(node->value);
    node->value = new_value;
}
