#pragma once
/*
 *      Copyright (C) 2010 Tobias Arrskog
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

#include "iohook.h"
#include <fcntl.h>

typedef void (*vfs_file_added_hook)   (const char *file);
typedef void (*vfs_file_removed_hook) (const char *file);
typedef void (*vfs_file_updated_hook) (const char *file);
typedef void (*vfs_file_moved_hook)   (const char *from, const char *to);

struct vfs_notification_callbacks
{
  vfs_file_added_hook   file_added;
  vfs_file_removed_hook file_removed;
  vfs_file_updated_hook file_updated;
  vfs_file_moved_hook   file_moved;
};

enum vfs_available_iohooks
{
  vfs_none  = 0x00,
  vfs_file  = 0x01
};

extern const int vfs_default_iohooks;
extern const int vfs_all_iohooks;

int vfs_initialize_iohooks(int iohooks);
int vfs_add_iohook(const char *protocol, struct vfs_iohook *hook);
void vfs_handle_notifications(struct vfs_notification_callbacks *callbacks);

void                              vfs_free_properties(struct vfs_properties *properties);

struct vfs_file_descriptor *      vfs_open      (const char *filepath, int flags);
size_t                            vfs_read      (void *buffer, size_t size, size_t count, struct vfs_file_descriptor *fp);
size_t                            vfs_write     (const void *buffer, size_t size, size_t count, struct vfs_file_descriptor *fp);
int                               vfs_seek      (struct vfs_file_descriptor *fp, long int offset, int origin);
long int                          vfs_tell      (struct vfs_file_descriptor *fp);
struct vfs_properties *           vfs_stat      (const char *filepath);
int                               vfs_flush     (struct vfs_file_descriptor *fp);
int                               vfs_close     (struct vfs_file_descriptor *fp);

struct vfs_directory_descriptor * vfs_opendir   (const char *directorypath);
struct vfs_properties *           vfs_readdir   (struct vfs_directory_descriptor *dp);
void                              vfs_seekdir   (struct vfs_directory_descriptor *dp, long loc);
void                              vfs_rewinddir (struct vfs_directory_descriptor *dp);
long                              vfs_telldir   (struct vfs_directory_descriptor *dp);
int                               vfs_closedir  (struct vfs_directory_descriptor *dp);
