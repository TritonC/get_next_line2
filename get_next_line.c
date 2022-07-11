/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 12:49:38 by manuel            #+#    #+#             */
/*   Updated: 2022/07/11 18:54:19 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_join_and_free(char *buffer, char *sub_bufer)
{
	char	*tmp;

	tmp = ft_strjoin(buffer, sub_bufer);
	free(buffer);
	return (tmp);
}
/*
this function checks if there is a line to copy, and stores it in a 
dynamic array and returns the new line with a line feed and a null.
*/

char	*check_line(char *str)
{
	int		count;
	char	*line;
	int		count2;

	if (!str)
		return (NULL);
	count = 0;
	count2 = 0;
	while (str[count] && str[count] != '\n')
		count++;
	line = ft_calloc(count + 2, sizeof(char));
	while (count2 <= count)
	{
		line[count2] = str[count2];
		count2++;
	}
	return (line);
}

/*
this function, is to store the remaining content of the file 
(remaining content == to content after the line to be displayed)
we create a dynamic array with the size of the BUFFER_SIZE + 1, 
and there we store what we want to be copied from the line, we join to save 
that is the function passed by parameter to be returned, and line is 
released just at the end of the function
*/
char	*read_and_save(int fd, char *buff)
{
	char	*sub_buff;
	ssize_t	bytes_count;

	sub_buff = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!sub_buff)
		return (NULL);
	bytes_count = 1;
	while (bytes_count > 0)
	{
		bytes_count = read(fd, sub_buff, BUFFER_SIZE);
		if (bytes_count <= 0)
		{
			free (sub_buff);
			free (buff);
			return (NULL);
		}
		buff = ft_join_and_free(buff, sub_buff);
		if (ft_strchr(buff, '\n') || bytes_count < BUFFER_SIZE)
			break ;
	}
	free (sub_buff);
	return (buff);
}

/*
this function is used to store the rest of the file, if it exists, so it 
assigns a ft_strlen - i, where i is the first line to copy, and the rest 
I want to store for my static variable to continue reading in case the 
function is called with a loop.
*/
char	*rest_of_file(char *buffer)
{
	int		i;
	int		j;
	char	*sub_buffer;

	i = 0;
	if (!buffer)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free (buffer);
		return (NULL);
	}
	sub_buffer = ft_calloc(ft_strlen(buffer) - i + 1, sizeof(char));
	i++;
	j = 0;
	while (buffer[i])
		sub_buffer[j++] = buffer [i++];
	free (buffer);
	return (sub_buffer);
}

/*
the function get_next_line makes first a call to read_and_save, which r
eturns the number of characters defined by BUFFER_SIZE in a char * type 
variable, after checking if such information exists in the variable, it is 
passed by the function check_line, which copies the content of the previous 
variable in a new one with dynamic memory and it is returned, so that it is our 
final return value in the function. and finally a call is made to rest_of_file,
this stores the rest of the file descriptor, in a static variable of type 
char * so that the function can continue to be called in loop!
*/
char	*get_next_line(int fd)
{
	char		*line_print;
	static char	*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	buffer = read_and_save (fd, buffer);
	if (!buffer)
		return (NULL);
	line_print = check_line(buffer);
	buffer = rest_of_file(buffer);
	return (line_print);
}

int	main()
{
	int		fd;
	int		i;

	i = -1;
	fd = open("gnlTester/files/42_no_nl", O_RDONLY);
	while (++i < 1)
	{
		printf("%s", get_next_line(fd));
	}
	return (0);
}
