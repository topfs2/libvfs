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
#include "vfs_directory_hooks.h"
#include "vfs_watchdog_hooks.h"
#include <fcntl.h>

enum vfs_available_iohooks
{
  vfs_none  = 0x00,
  vfs_file  = 0x01
};

extern const int vfs_default_iohooks;
extern const int vfs_all_iohooks;

// The opaque structs
typedef struct vfs_context_t *              vfs_context;
typedef struct vfs_file_descriptor_t *      vfs_file_descriptor;
typedef struct vfs_directory_descriptor_t * vfs_directory_descriptor;
typedef struct vfs_watchdog_descriptor_t *  vfs_watchdog_descriptor;

typedef struct vfs_readdir_control_t *      vfs_readdir_control;

struct vfs_iohook;

// Context methods
vfs_context                       vfs_initialize      (int iohooks);
int                               vfs_add_iohook      (vfs_context ctx, const char *protocol, struct vfs_iohook *hook);

// File methods
vfs_file_descriptor               vfs_open            (vfs_context ctx, const char *filepath, int flags);
size_t                            vfs_read            (vfs_file_descriptor fp, void *buffer, size_t size, size_t count);
size_t                            vfs_write           (vfs_file_descriptor fp, const void *buffer, size_t size, size_t count);
int                               vfs_seek            (vfs_file_descriptor fp, long int offset, int origin);
long int                          vfs_tell            (vfs_file_descriptor fp);
struct vfs_properties *           vfs_stat            (vfs_context ctx, const char *filepath);
int                               vfs_flush           (vfs_file_descriptor fp);
int                               vfs_close           (vfs_file_descriptor fp);

// Directory methods
vfs_directory_descriptor          vfs_opendir         (vfs_context ctx, const char *directorypath);
void                              vfs_readdir_sync    (vfs_directory_descriptor dp, struct vfs_directory_callbacks callbacks, void *cls);
int                               vfs_closedir        (vfs_directory_descriptor dp);

// Watchdog methods
vfs_watchdog_descriptor           vfs_add_watch       (vfs_context ctx, const char *watchpath, struct vfs_notification_callbacks callbacks, void *cls);
int                               vfs_remove_watch    (vfs_watchdog_descriptor wp);
