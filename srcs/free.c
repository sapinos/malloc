/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clanier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/01 16:28:45 by clanier           #+#    #+#             */
/*   Updated: 2017/12/04 17:53:05 by clanier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

void	free_malloc(t_malloc *mem)
{
	size_t	stmp;

	stmp = mem->size;
	mem->is_free = true;
	if (mem->next && mem->next->is_free)
	{
		mem->size += mem->next->size + sizeof(t_malloc);
		if (mem->next->next)
			mem->next->next->prev = mem;
		mem->next = mem->next->next;
	}
	if (mem->prev && mem->prev->is_free)
	{
		mem->prev->size += mem->size + sizeof(t_malloc);
		mem->prev->next = mem->next;
		if (mem->next)
			mem->next->prev = mem->prev;
	}
	if (g_line.env & ENV_SCRIBBLE)
		ft_memset((void*)((size_t)mem + sizeof(t_malloc)), 0x55, stmp);
}

void	free_page(void **page, char page_type)
{
	t_page		*ptmp;
	t_malloc	*mtmp;

	ptmp = (t_page*)*page;
	if (!ptmp->prev && !ptmp->next)
		return ;
	ft_putstr("What\nhave\nI\ndone\n...\n\n\nrun.");
	mtmp = ptmp->first;
	while (mtmp)
	{
		if (!mtmp->is_free)
			return ;
		mtmp = mtmp->next;
	}
	if (ptmp->prev)
		ptmp->prev->next = ptmp->next;
	else if (page_type == 't')
		g_line.tiny = ptmp->next;
	else if (page_type == 's')
		g_line.small = ptmp->next;
	if (ptmp->next)
		ptmp->next->prev = ptmp->prev;
	g_line.limit += ptmp->size + sizeof(t_page);
	munmap((void*)ptmp, ptmp->size + sizeof(t_page));
	ptmp = NULL;
}

void	*find_pointer(void *ptr, void **page)
{
	t_page		*ptmp;
	t_malloc	*mtmp;

	g_line.old_size = 0;
	ptmp = (t_page*)*page;
	while (ptmp)
	{
		if (ptr > (void*)ptmp && ptr < ptmp->end)
		{
			mtmp = ptmp->first;
			while (mtmp && ptr != (void*)((size_t)mtmp + sizeof(t_malloc)))
				mtmp = mtmp->next;
			if (mtmp && ptr == (void*)((size_t)mtmp + sizeof(t_malloc)))
				return (mtmp);
			else
				return (NULL);
		}
		ptmp = ptmp->next;
	}
	return (NULL);
}

void	free_memory(t_malloc *mem, void **page, char page_type)
{
	g_line.old_size = mem->size;
	free_malloc(mem);
	free_page(page, page_type);
}

void	ft_free(void *ptr)
{
	t_malloc	*mtmp;

	if (!ptr)
		return ;
	if ((mtmp = find_pointer(ptr, &g_line.tiny)))
		return (free_memory(mtmp, &g_line.tiny, 't'));
	if ((mtmp = find_pointer(ptr, &g_line.small)))
		return (free_memory(mtmp, &g_line.small, 's'));
	mtmp = g_line.large;
	while (mtmp && ptr != (void*)((size_t)mtmp + sizeof(t_malloc)))
		mtmp = mtmp->next;
	if (mtmp && ptr == (void*)((size_t)mtmp + sizeof(t_malloc)))
	{
		if (mtmp->prev)
			mtmp->prev->next = mtmp->next;
		else
			g_line.large = mtmp->next;
		if (mtmp->next)
			mtmp->next->prev = mtmp->prev;
		g_line.limit += mtmp->size + sizeof(t_malloc);
		g_line.old_size = mtmp->size;
		munmap(mtmp, mtmp->size + sizeof(t_malloc));
	}
}
