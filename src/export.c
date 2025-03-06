/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:13:05 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/06 14:22:56 by jalqam           ###   ########.fr       */
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

void	append_node(t_env **env, t_env **last, t_env *new_node)
{
    if (!*env)
        *env = new_node;
    else
        (*last)->next = new_node;
    *last = new_node;
}
