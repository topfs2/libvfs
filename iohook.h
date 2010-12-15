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

#include <stdlib.h>

struct vfs_file_descriptor;
struct vfs_directory_descriptor;
struct vfs_properties;
struct vfs_metadata;

typedef struct vfs_file_descriptor *      (*vfs_open_hook)      (const char *filepath, int flags);
typedef size_t                            (*vfs_read_hook)      (void *buffer, size_t size, size_t count, struct vfs_file_descriptor *fp);
typedef size_t                            (*vfs_write_hook)     (const void *buffer, size_t size, size_t count, struct vfs_file_descriptor *fp);
typedef int                               (*vfs_seek_hook)      (struct vfs_file_descriptor *fp, long int offset, int origin);
typedef long int                          (*vfs_tell_hook)      (struct vfs_file_descriptor *fp);
typedef struct vfs_properties *           (*vfs_stat_hook)      (const char *filepath);
typedef int                               (*vfs_flush_hook)     (struct vfs_file_descriptor *fp);
typedef int                               (*vfs_close_hook)     (struct vfs_file_descriptor *fp);

typedef struct vfs_directory_descriptor * (*vfs_opendir_hook)   (const char *directorypath);
typedef struct vfs_properties *           (*vfs_readdir_hook)   (struct vfs_directory_descriptor *dp);
typedef void                              (*vfs_seekdir_hook)   (struct vfs_directory_descriptor *dp, long loc);
typedef void                              (*vfs_rewinddir_hook) (struct vfs_directory_descriptor *dp);
typedef long                              (*vfs_telldir_hook)   (struct vfs_directory_descriptor *dp);
typedef int                               (*vfs_closedir_hook)  (struct vfs_directory_descriptor *dp);

struct vfs_iohook
{
  vfs_open_hook       open;
  vfs_read_hook       read;
  vfs_write_hook      write;
  vfs_seek_hook       seek;
  vfs_tell_hook       tell;
  vfs_stat_hook       stat;
  vfs_flush_hook      flush;
  vfs_close_hook      close;

  vfs_opendir_hook    opendir;
  vfs_readdir_hook    readdir;
  vfs_seekdir_hook    seekdir;
  vfs_rewinddir_hook  rewinddir;
  vfs_telldir_hook    telldir;
  vfs_closedir_hook   closedir;

  void *cls;
};

struct vfs_file_descriptor
{
  struct vfs_iohook iohook;

  void *cls;
};

struct vfs_directory_descriptor
{
  struct vfs_iohook iohook;

  void *cls;
};

struct vfs_metadata
{
  const char *key;
  const char *value;

  struct vfs_metadata *next;
};

struct vfs_properties
{
  const char *name;
  off_t size;
  int   hidden;

  struct vfs_metadata *extended_metadata;
  int more_metadata_exist;
};
