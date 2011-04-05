#pragma once
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

#include <stdlib.h>

struct vfs_list_node
{
  void *data;
  struct vfs_list_node *next;
};

struct vfs_list
{
  struct vfs_list_node *first;
  struct vfs_list_node *last;
  int size;
};

//#define FOREACH(itr, li) for (li = itr; itr ; li = itr = itr->next)
#define FOREACH(itr, li) for (li = (itr ? itr->data : NULL) ; itr ; li = ((itr = itr->next) ? itr->data : NULL) )

typedef void (*vfs_list_free_item) (void *item);

struct vfs_list *         vfs_list_create   ();
void                      vfs_list_free     (struct vfs_list *list, vfs_list_free_item free_item);

void                      vfs_list_append   (struct vfs_list *list, void *data);
struct vfs_list *         vfs_list_merge    (struct vfs_list *a, struct vfs_list *b);

struct vfs_list_node *    vfs_list_begin    (struct vfs_list *list);
