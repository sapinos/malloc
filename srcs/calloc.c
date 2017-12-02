/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clanier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/02 17:35:22 by clanier           #+#    #+#             */
/*   Updated: 2017/12/02 20:12:10 by clanier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*ft_bzero(void *s, size_t n)
{
	while (n--)
		((unsigned char*)s)[n] = 0;
	return (s);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	size *= nmemb;
	if (!(ptr = ft_malloc(size)))
		return (NULL);
	ft_bzero(ptr, size);
	return (ptr);
}
