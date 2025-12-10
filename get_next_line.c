/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalju-be <jalju-be@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 02:32:01 by jalju-be          #+#    #+#             */
/*   Updated: 2025/10/19 18:10:06 by jalju-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*gnl_free(char **str)
{
	if (*str)
		free(*str);
	*str = NULL;
	return (NULL);
}

char	*gnl_extract_line(const char *text)
{
	int		i;
	int		size;
	char	*line;

	if (!text || !*text)
		return (NULL);
	size = lentonew(text);
	line = malloc(size + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (i < size)
	{
		line[i] = text[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

void	gnl_read(int fd, char **save, char *buffer)
{
	char	*tmp;
	int		bytes;

	if (!*save)
		*save = ft_strdup("");
	bytes = 1;
	while (bytes > 0 && *save)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
		{
			gnl_free(save);
			return ;
		}
		if (bytes == 0)
			return ;
		buffer[bytes] = '\0';
		tmp = *save;
		*save = ft_strjoin(tmp, buffer);
		free(tmp);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
}

void	gnl_update_rem(char **save)
{
	char	*next;
	char	*new_part;

	next = ft_strchr(*save, '\n');
	if (!next)
	{
		gnl_free(save);
		return ;
	}
	new_part = ft_strdup(next + 1);
	if (!new_part)
	{
		gnl_free(save);
		return ;
	}
	free(*save);
	*save = new_part;
}

char	*get_next_line(int fd)
{
	static char	*remain;
	char		*buf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (gnl_free(&remain));
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	gnl_read(fd, &remain, buf);
	free(buf);
	if (!remain || !*remain)
		return (gnl_free(&remain));
	line = gnl_extract_line(remain);
	if (!line)
		return (gnl_free(&remain));
	gnl_update_rem(&remain);
	return (line);
}
