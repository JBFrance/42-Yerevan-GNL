/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfrances <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 08:29:38 by jfrances          #+#    #+#             */
/*   Updated: 2023/02/24 10:29:11 by jfrances         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	check_free(char **ptr)
{
	if (*ptr != NULL)
	{
		free(*ptr);
		ptr = NULL;
	}
}

static char	*join_line(int nl_position, char **buffer)
{
	char	*res;
	char	*tmp;

	tmp = NULL;
	if (nl_position <= 0)
	{
		if (**buffer == '\0')
		{
			free(*buffer);
			*buffer = NULL;
			return (NULL);
		}
		res = *buffer;
		*buffer = NULL;
		return (res);
	}
	tmp = ft_substr(*buffer, nl_position, BUFFER_SIZE);
	res = *buffer;
	res[nl_position] = 0;
	*buffer = tmp;
	return (res);
}

static char	*read_line(int fd, char **stash, char *buffer)
{
	ssize_t	offset;
	char	*tmp;
	char	*nl;

	nl = ft_strchr(*stash, '\n');
	tmp = NULL;
	offset = 0;
	while (nl == NULL)
	{
		offset = read(fd, buffer, BUFFER_SIZE);
		if (offset <= 0)
			return (join_line(offset, stash));
		buffer[offset] = 0;
		tmp = ft_strjoin(*stash, buffer);
		check_free(stash);
		*stash = tmp;
		nl = ft_strchr(*stash, '\n');
	}
	return (join_line(nl - *stash + 1, stash));
}

char	*get_next_line(int fd)
{
	static char	*buffer[OPEN_MAX];
	char		*cache;
	char		*res;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	cache = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (cache == NULL)
		return (NULL);
	if (!buffer[fd])
		buffer[fd] = ft_strdup("");
	res = read_line(fd, &buffer[fd], cache);
	check_free(&cache);
	return (res);
}
