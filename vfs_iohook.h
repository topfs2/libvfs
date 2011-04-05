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

struct vfs_file_descriptor;
struct vfs_directory_descriptor;
struct vfs_watch_descriptor;
struct vfs_properties;
struct vfs_metadata;

typedef void *                            (*vfs_open_hook)          (const char *filepath, int flags);
typedef size_t                            (*vfs_read_hook)          (void *fp, void *buffer, size_t size, size_t count);
typedef size_t                            (*vfs_write_hook)         (void *fp, const void *buffer, size_t size, size_t count);
typedef int                               (*vfs_seek_hook)          (void *fp, long int offset, int origin);
typedef long int                          (*vfs_tell_hook)          (void *fp);
typedef struct vfs_properties *           (*vfs_stat_hook)          (const char *filepath);
typedef int                               (*vfs_flush_hook)         (void *fp);
typedef int                               (*vfs_close_hook)         (void *fp);

typedef void *                            (*vfs_opendir_hook)       (const char *directorypath);
typedef void                              (*vfs_readdir_hook)       (void *dp, void *cls, struct vfs_directory_callbacks callback);
typedef int                               (*vfs_closedir_hook)      (void *dp);

typedef void *                            (*vfs_add_watch_hook)     (const char *watchpath, struct vfs_notification_callbacks callbacks, void *cls);
typedef int                               (*vfs_remove_watch_hook)  (void *wp);

struct vfs_iohook
{
  // File hooks
  vfs_open_hook         open;
  vfs_read_hook         read;
  vfs_write_hook        write;
  vfs_seek_hook         seek;
  vfs_tell_hook         tell;
  vfs_stat_hook         stat;
  vfs_flush_hook        flush;
  vfs_close_hook        close;

  // Directory hooks
  vfs_opendir_hook      opendir;
  vfs_readdir_hook      readdir;
  vfs_closedir_hook     closedir;

  // Watchdog hooks (optional)
  vfs_add_watch_hook    add_watch;
  vfs_remove_watch_hook remove_watch;

  void *cls;
};
