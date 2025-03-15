/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:13:05 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/08 16:58:53 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_key(char *var)
{
    char	*equal_sign;
    
    equal_sign = ft_strchr(var, '=');
    if (!equal_sign)
        return (ft_strdup(var));
    return (ft_substr(var, 0, equal_sign - var));
}

static int is_valid_export(char *arg)
{
    int i;

    i = 0;
    if (!ft_isalpha(arg[i]) && arg[i] != '_')
        return (0);
    i++;
    while (arg[i] && arg[i] != '=')
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_')
            return (0);
        i++;
    }
    return (1);
}
int	export_command(t_cmd *cmd, t_env **env)
{
    int		i;
    t_env	*new_node;
    t_env	*last;
    t_env	*existing;
    char	*k;

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
            ft_printf("minishell: export: `%s': not a valid identifier\n", cmd->args[i++]);
            continue;
        }
        k = get_key(cmd->args[i]);
        if ((existing = find_env_node(*env, k)))
        {
            update_env_value(existing, cmd->args[i]);
            free(k);
            i++;
            continue;
        }
        free(k);
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

static t_env *copy_env_list(t_env *env)
{
    t_env *copy;
    t_env *new;
    t_env *last;

    copy = NULL;
    last = NULL;
    while (env)
    {
        new = malloc(sizeof(t_env));
        if (!new)
            return (NULL);
        new->key = ft_strdup(env->key);
        new->value = env->value ? ft_strdup(env->value) : NULL;
        new->next = NULL;
        if (!new->key || (env->value && !new->value))
        {
            free_env_node(new);
            free_env_list(copy);
            return (NULL);
        }
        if (!copy)
            copy = new;
        else
            last->next = new;
        last = new;
        env = env->next;
    }
    return (copy);
}

int print_export_command(t_env *env)
{
    t_env *sorted;
    t_env *j;
    int swapped;

    sorted = copy_env_list(env);
    if (!sorted)
        return (1);
    swapped = 1;
    while (swapped)
    {
        swapped = 0;
        j = sorted;
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
    j = sorted;
    while (j)
    {
        if (j->value)
            ft_printf("declare -x %s=\"%s\"\n", j->key, j->value);
        else
            ft_printf("declare -x %s\n", j->key);
        j = j->next;
    }
    free_env_list(sorted);
    return (0);
}

void	append_node(t_env **env, t_env **last, t_env *new_node)
{
    if (!*env)
        *env = new_node;
    else
        (*last)->next = new_node;
    *last = new_node;
}
