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

typedef void (*vfs_file_added_hook)   (void *cls, const char *file);
typedef void (*vfs_file_removed_hook) (void *cls, const char *file);
typedef void (*vfs_file_updated_hook) (void *cls, const char *file);
typedef void (*vfs_file_moved_hook)   (void *cls, const char *from, const char *to);

struct vfs_notification_callbacks
{
  vfs_file_added_hook   file_added;
  vfs_file_removed_hook file_removed;
  vfs_file_updated_hook file_updated;
  vfs_file_moved_hook   file_moved;
};
