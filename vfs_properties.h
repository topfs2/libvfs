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

#include "vfs_lists.h"

struct vfs_metadata
{
  const char *key;
  const char *value;
};

struct vfs_properties
{
  const char *name;
  off_t size;
  int   hidden;

  struct vfs_list *metadata_list;
  int more_metadata_exist;
};

struct vfs_metadata *             vfs_metadata_create     (const char *key, const char *value);
void                              vfs_metadata_free       (struct vfs_metadata *metadata);

struct vfs_properties *           vfs_properties_create   (const char *name, int size, int hidden, struct vfs_list *metadata_list);
void                              vfs_properties_free     (struct vfs_properties *properties);
