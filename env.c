/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:27:24 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/05 17:06:19 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_env_list(t_env *env)
{
    t_env	*temp;

    while (env)
    {
        temp = env;
        env = env->next;
        free_env_node(temp);
    }
}

static void	append_node(t_env **env, t_env **last, t_env *new_node)
{
    if (!*env)
        *env = new_node;
    else
        (*last)->next = new_node;
    *last = new_node;
}

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
            free_env_list(env);
            return (NULL);
        }
        append_node(&env, &last, temp);
        i++;
    }
    return (env);
}

int is_valid_export(char *arg)
{
    int i;

    i = 0;
    while (arg[i] && arg[i] != '=')
        i++;
    if (i > 0 && arg[i] == '=')
    {
        if (i > 0 && (arg[i - 1] == ' ' || arg[i + 1] == ' '))
            return (0);
        if (ft_isdigit(arg[0]))
            return (0);
        return (1);
    }
    return (0);
}

int	export_command(t_cmd *cmd, t_env **env)
{
    int		i;
    t_env	*new_node;
    t_env	*last;

    i = 1;
    if (!cmd->args[i])
        return (print_export_command(*env));
    last = *env;
    while (last && last->next)
        last = last->next;
    while (cmd->args[i])
    {
        if (!is_valid_export(cmd->args[i]))
        {
            ft_printf("minishell: export: `%s': not a valid identifier\n", cmd->args[i]);
            i++;
            continue;
        }
        new_node = create_env_node(cmd->args[i++]);
        if (!new_node)
            return (1);
        append_node(env, &last, new_node);
    }
    return (0);
}

static void swap_nodes(t_env *a, t_env *b)
{
    char *temp_key;
    char *temp_value;

    temp_key = a->key;
    temp_value = a->value;
    a->key = b->key;
    a->value = b->value;
    b->key = temp_key;
    b->value = temp_value;
}

int	print_export_command(t_env *env)
{
    t_env	*j;
    int		swapped;

    swapped = 1;
    while (swapped)
    {
        swapped = 0;
        j = env;
        while (j->next)
        {
            if (ft_strcmp(j->key, j->next->key) > 0)
            {
                swap_nodes(j, j->next);
                swapped = 1;
            }
            j = j->next;
        }
    }
    while (env)
    {
        ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
        env = env->next;
    }
    return (0);
}
