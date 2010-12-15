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

#include "vfs_file.h"
#include "vfs_helpers.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

const char *strip_protocol(const char *path)
{
  return strncmp("file://", path, 7) == 0 ? path + 7 : path;
}

struct vfs_file_descriptor *vfs_file_open(const char *filepath, int flags)
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

  FILE *file = fopen(stripped_filepath, mode);
  if (file)
  {
    struct vfs_file_descriptor *fp = malloc(sizeof(struct vfs_file_descriptor));
    fp->iohook = vfs_file_iohooks;
    fp->cls = (void *)file;

    if (flags & O_APPEND)
      vfs_file_seek(fp, 0, SEEK_END);

    return fp;
  }
  else
    return NULL;
}

size_t vfs_file_read(void *buffer, size_t size, size_t count, struct vfs_file_descriptor *fp)
{
  return fread(buffer, size, count, (FILE *)fp->cls);
}

size_t vfs_file_write(const void *buffer, size_t size, size_t count, struct vfs_file_descriptor *fp)
{
  return fwrite(buffer, size, count, (FILE *)fp->cls);
}

int vfs_file_seek(struct vfs_file_descriptor *fp, long int offset, int origin)
{
  return fseek((FILE *)fp->cls, offset, origin);
}

long int vfs_file_tell(struct vfs_file_descriptor *fp)
{
  return ftell((FILE *)fp->cls);
}

struct vfs_properties *vfs_file_stat(const char *filepath)
{
  // TODO name and hidden should be proper
  struct stat buffer;
  if (stat(strip_protocol(filepath), &buffer) == 0)
    return (struct vfs_properties *)vfs_new_properties(NULL, 0, 0, NULL);
  else
    return NULL;
}

int vfs_file_flush(struct vfs_file_descriptor *fp)
{
  return fflush((FILE *)fp->cls);
}

int vfs_file_close(struct vfs_file_descriptor *fp)
{
  int success = fclose((FILE *)fp->cls);
  free(fp);
  return success;
}

struct vfs_directory_descriptor *vfs_file_opendir(const char *directorypath)
{
  DIR *dir = opendir(strip_protocol(directorypath));
  if (dir)
  {
    struct vfs_directory_descriptor *dp = malloc(sizeof(struct vfs_directory_descriptor));
    dp->iohook = vfs_file_iohooks;
    dp->cls = (void *)dir;
    return dp;
  }
  else
    return NULL;
}

struct vfs_properties *vfs_file_readdir(struct vfs_directory_descriptor *dp)
{
  // TODO hidden should be proper
  struct dirent *d = readdir((DIR *)dp->cls);

  if (d)
    return (struct vfs_properties *)vfs_new_properties(d->d_name, 0, 0, NULL);
  else
    return NULL;
}

void vfs_file_seekdir(struct vfs_directory_descriptor *dp, long loc)
{
  seekdir((DIR *)dp->cls, loc);
}

void vfs_file_rewinddir(struct vfs_directory_descriptor *dp)
{
  rewinddir((DIR *)dp->cls);
}

long vfs_file_telldir(struct vfs_directory_descriptor *dp)
{
  return telldir((DIR *)dp->cls);
}

int vfs_file_closedir(struct vfs_directory_descriptor *dp)
{
  int success = closedir((DIR *)dp->cls);
  free(dp);
  return success;
}

const struct vfs_iohook vfs_file_iohooks =
{
  vfs_file_open,
  vfs_file_read,
  vfs_file_write,
  vfs_file_seek,
  vfs_file_tell,
  vfs_file_stat,
  vfs_file_flush,
  vfs_file_close,

  vfs_file_opendir,
  vfs_file_readdir,
  vfs_file_seekdir,
  vfs_file_rewinddir,
  vfs_file_telldir,
  vfs_file_closedir,

  NULL
};
