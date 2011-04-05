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

#include "vfs.h"
#include <stdlib.h>

struct vfs_properties *properties;
struct vfs_metadata *metadata;

typedef int  (*vfs_directory_callback_item)         (void *cls, struct vfs_properties *properties);
typedef int  (*vfs_directory_callback_item_update)  (void *cls, const char *file, struct vfs_metadata *extended_metadata);
typedef void (*vfs_directory_callback_eof)          (void *cls);

struct vfs_directory_callbacks
{
  vfs_directory_callback_item         item;
  vfs_directory_callback_item_update  item_update;
  vfs_directory_callback_eof          eof;
};
