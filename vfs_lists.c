/*
 *      Copyright (C) 2010-2011 Tobias Arrskog
 *      https://github.com/topfs2/libvfs
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "vfs_lists.h"

struct vfs_list *vfs_list_create()
{
  struct vfs_list *list = malloc(sizeof(struct vfs_list));

  list->first = NULL;
  list->last  = NULL;
  list->size  = 0;

  return list;
}

void vfs_list_free_node(struct vfs_list_node *node, vfs_list_free_item free_item)
{
  if (node)
  {
    vfs_list_free_node(node->next, free_item);

    if (free_item && node->data)
      free_item(node->data);

    free(node);
  }
}

void vfs_list_free(struct vfs_list *list, vfs_list_free_item free_item)
{
  if (list)
  {
    vfs_list_free_node(list->first, free_item);
    free(list);
  }
}

void vfs_list_append(struct vfs_list *list, void *data)
{
  if (list)
  {
    struct vfs_list_node *item = malloc(sizeof(struct vfs_list_node));
    item->data = data;

    if (list->last)
    {
      list->last->next = item;
      list->last = item;
    }
    else // Should only happen if first is also NULL
      list->last = list->first = item;

    list->size++;
  }
}

struct vfs_list *vfs_list_merge(struct vfs_list *a, struct vfs_list *b)
{
  if (a)
  {
    if (b)
    {
      if (a->last)
      {
        a->last->next = b->first;
        a->last = b->last;
      }
      else
      {
        a->first = b->first;
        a->last  = b->last;
      }

      a->size += b->size;
    }

    return a;
  }
  else
    return b;
}

struct vfs_list_node *vfs_list_begin(struct vfs_list *list)
{
  if (list)
    return list->first;
  else
    return NULL;
}
