/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:01:41 by jalqam            #+#    #+#             */
/*   Updated: 2025/02/27 16:16:02 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value)
{
	t_token	*token;

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
	// char	*joined;
	//char	*final;
	int		i;

	tokens = NULL;
	i = -1;
	token_split = our_split(input);
	while (token_split[++i])
	{
		// if (token_split[i + 1] && token_split[i + 1][0] == '-')
		// {
		// 	joined = ft_strjoin(token_split[i], " ");
		// 	final = ft_strjoin(joined, token_split[i + 1]);
		// 	free(joined);
		// 	add_token(&tokens, final);
		// 	free(final);
		// 	i++;
		// }
		// else
			add_token(&tokens, token_split[i]);
	}
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
