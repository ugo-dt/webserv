/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mkdir_p.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 18:55:26 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/10/29 19:04:49 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"
#include <cstdlib>
#include <sys/stat.h>

/* Make a directory; already existing dir okay */
static int maybe_mkdir(const char* path, mode_t mode)
{
    struct stat st;
    errno = 0;

    /* Try to make the directory */
    if (mkdir(path, mode) == 0)
        return (EXIT_SUCCESS);

    /* If it fails for any reason but EEXIST, fail */
    if (errno != EEXIST)
        return (EXIT_FAILURE);

    /* Check if the existing path is a directory */
    if (stat(path, &st) != 0)
        return (EXIT_FAILURE);

    /* If not, fail with ENOTDIR */
    if (!S_ISDIR(st.st_mode)) {
        errno = ENOTDIR;
        return (EXIT_FAILURE);
    }
    errno = 0;
    return (EXIT_SUCCESS);
}

// https://gist.github.com/JonathonReinhart/8c0d90191c38af2dcadb102c4e202950
int mkdir_p(const char *path)
{
    /* Adapted from http://stackoverflow.com/a/2336245/119527 */
    char *_path = NULL;
    char *p; 
    int result = -1;
    mode_t mode = 0777;

    errno = 0;

    /* Copy string so it's mutable */
    _path = strdup(path);
    if (_path == NULL)
	{
		free(_path);
		return (EXIT_FAILURE);
	}

    /* Iterate the string */
    for (p = _path + 1; *p; p++) {
        if (*p == '/') {
            /* Temporarily truncate */
            *p = '\0';

            if (maybe_mkdir(_path, mode) != 0)
			{
				free(_path);
				return (EXIT_FAILURE);
			}

            *p = '/';
        }
    }
    if (maybe_mkdir(_path, mode) != 0)
	{
		free(_path);
		return (EXIT_FAILURE);
	}
    result = 0;
    free(_path);
    return result;
}
