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
#include "vfs_posix.h"
#include "vfs_properties.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

const char *strip_protocol(const char *path)
{
  return strncmp("file://", path, 7) == 0 ? path + 7 : path;
}

void *vfs_posix_open(const char *filepath, int flags);
size_t vfs_posix_read(void *fp, void *buffer, size_t size, size_t count);
size_t vfs_posix_write(void *fp, const void *buffer, size_t size, size_t count);
int vfs_posix_seek(void *fp, long int offset, int origin);
long int vfs_posix_tell(void *fp);
struct vfs_properties *vfs_posix_stat(const char *filepath);
int vfs_posix_flush(void *fp);
int vfs_posix_close(void *fp);
void *vfs_posix_opendir(const char *directorypath);
void vfs_posix_readdir(void *dp, void *cls, struct vfs_directory_callbacks callbacks);
int vfs_posix_closedir(void *dp);
void *vfs_posix_add_watch(const char *watchpath, struct vfs_notification_callbacks callbacks, void *cls);
int vfs_posix_remove_watch(void *cls);

void *vfs_posix_open(const char *filepath, int flags)
{
  const char *stripped_filepath = strip_protocol(filepath);
  struct stat buffer;
  if (flags & O_EXCL && flags & O_CREAT && stat(stripped_filepath, &buffer) == 0)
    return NULL;

  char mode[16];
  int i = 0;

  if (!(flags & O_WRONLY) || flags & O_RDWR)
    mode[i++] = 'r';
  if (flags & O_WRONLY || flags & O_RDWR)
    mode[i++] = 'w';

  mode[i++] = 'b';

  if (flags & O_APPEND)
    mode[i++] = 'a';
  if (flags & O_CREAT)
    mode[i++] = '+';

  mode[i] = '\0';

  FILE *fp = fopen(stripped_filepath, mode);
  if (fp)
  {
    if (flags & O_APPEND)
      vfs_posix_seek(fp, 0, SEEK_END);

    return fp;
  }
  else
    return NULL;
}

size_t vfs_posix_read(void *fp, void *buffer, size_t size, size_t count)
{
  return fread(buffer, size, count, (FILE *)fp);
}

size_t vfs_posix_write(void *fp, const void *buffer, size_t size, size_t count)
{
  return fwrite(buffer, size, count, (FILE *)fp);
}

int vfs_posix_seek(void *fp, long int offset, int origin)
{
  return fseek((FILE *)fp, offset, origin);
}

long int vfs_posix_tell(void *fp)
{
  return ftell((FILE *)fp);
}

struct vfs_properties *vfs_posix_stat(const char *filepath)
{
  // TODO name and hidden should be proper
  struct stat buffer;
  if (stat(strip_protocol(filepath), &buffer) == 0)
    return (struct vfs_properties *)vfs_properties_create(NULL, 0, 0, NULL);
  else
    return NULL;
}

int vfs_posix_flush(void *fp)
{
  return fflush((FILE *)fp);
}

int vfs_posix_close(void *fp)
{
  return fclose((FILE *)fp);
}

void *vfs_posix_opendir(const char *directorypath)
{
  DIR *dp = opendir(strip_protocol(directorypath));
  if (dp)
    return dp;

  return NULL;
}

void vfs_posix_readdir(void *dp, void *cls, struct vfs_directory_callbacks callbacks)
{
  struct dirent *dir;

  do
  {
    dir = readdir(dp);
    if (dir)
      callbacks.item(cls, vfs_properties_create(dir->d_name, 0, 0, NULL));

  } while (dir != NULL);

  callbacks.eof(cls);
}

int vfs_posix_closedir(void *dp)
{
  return closedir((DIR *)dp);
}

void *vfs_posix_add_watch(const char *watchpath, struct vfs_notification_callbacks callbacks, void *cls)
{
  return NULL;
}

int vfs_posix_remove_watch(void *cls)
{
  return 0;
}

const struct vfs_iohook vfs_posix_iohooks =
{
  vfs_posix_open,
  vfs_posix_read,
  vfs_posix_write,
  vfs_posix_seek,
  vfs_posix_tell,
  vfs_posix_stat,
  vfs_posix_flush,
  vfs_posix_close,

  vfs_posix_opendir,
  vfs_posix_readdir,
  vfs_posix_closedir,

  vfs_posix_add_watch,
  vfs_posix_remove_watch,

  NULL
};
