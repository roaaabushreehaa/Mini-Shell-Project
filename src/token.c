/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabu-shr <rabu-shr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:01:41 by jalqam            #+#    #+#             */
/*   Updated: 2025/03/13 14:03:06 by rabu-shr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(char *value)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, char *value)
{
	t_token	*new;
	t_token	*temp;

	new = new_token(value);
	if (!new)
		return ;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	char	**token_split;
	int		i;

	tokens = NULL;
	i = -1;
	token_split = our_split(input);
	while (token_split[++i])
	{
			add_token(&tokens, token_split[i]);
	}
	// tokens->count_pip = num_pip(tokens);
	// printf("number of pip:%d\n",tokens->count_pip);
	free_tokens_split(token_split);
	return (tokens);
}

void	print_tokens(t_token *head)
{
	t_token	*temp;

	temp = head;
	get_built_in_type(head);
	while (temp)
	{
		printf("[%s] ", temp->value);
		printf("type %u \n",temp->type);
		temp = temp->next;
	}
	printf("\n");
	//separate(head);
}

void	free_tokens(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}

void	free_tokens_split(char **token_split)
{
	int	i;

	i = 0;
	while (token_split[i])
		free(token_split[i++]);
	free(token_split);
}
