/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excutable.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:18:13 by rabu-shr          #+#    #+#             */
/*   Updated: 2025/03/05 18:14:42 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int itrate(t_token * tokens)
// {
// 	t_token *temp;
// 	int fd;
// 	temp=tokens;

// 	while (temp)
// 	{
// 		if (temp->type == REDIRECT_OUT && temp->next)
// 		{
// 			fd = open(temp->next->value,O_WRONLY | O_CREAT | O_TRUNC);
// 			if(fd != -1)
// 				temp->out_files=temp->next->value;
// 		}
// 			else if (temp->type == REDIRECT_IN && temp->next)
// 			{
// 				fd = open(temp->next->value, O_RDONLY);
// 				if(fd!=-1)
// 				temp->in_files=temp->next->value;
// 				else
// 					return (1);
// 			}
// 		temp=temp->next;
// 	}
// 	print_files_name(tokens);
// 	return (0);
// }

// void print_files_name(t_token *token)
// {
// 	t_token *temp;
// 	temp =token ;
// 	while (temp)
// 	{
// 		printf("%s",temp->in_files);
// 		printf("%s",temp->out_files);
// 		temp = temp->next;
// 	}

// }