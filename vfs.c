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

#include "vfs.h"
#include "vfs_file.h"
#include <string.h>

const int vfs_default_iohooks = vfs_file;

const int vfs_all_iohooks = vfs_file;

struct vfs_list_node
{
  char *protocol;
  struct vfs_iohook *iohook;
  struct vfs_list_node *next;
};

struct vfs_module
{
  enum vfs_available_iohooks flag;
  struct vfs_list_node module;
};

struct vfs_module vfs_static_vfs_modules[] =
{
  { vfs_file,   { "file", &vfs_file_iohooks, NULL } },
  { vfs_none,   { NULL, NULL, NULL } }
};

struct vfs_list_node *g_IOHookList = NULL;
int g_IOHookListSize = 0;

int vfs_initialize_iohooks(int iohooks)
{
  int failed = 0;

  struct vfs_module *itr = vfs_static_vfs_modules;
  while (itr->flag != vfs_none)
  {
    if (!(itr->flag & iohooks && vfs_add_iohook(itr->module.protocol, itr->module.iohook) == 0))
      failed |= itr->flag;

    itr++;
  }

  return failed;
}

int vfs_add_iohook(const char *protocol, struct vfs_iohook *iohook)
{
  struct vfs_list_node *newNode = malloc(sizeof(struct vfs_list_node));
  newNode->protocol = malloc(sizeof(char) * strlen(protocol));
  strcpy(newNode->protocol, protocol);
  newNode->iohook = malloc(sizeof(struct vfs_iohook));
  *newNode->iohook = *iohook;

  newNode->next = NULL;

  if (g_IOHookList)
  {
    struct vfs_list_node *itr = g_IOHookList;
    while (itr->next)
      itr = itr->next;

    itr->next = newNode;
  }
  else
    g_IOHookList = newNode;

  return 0;
}

void vfs_handle_notifications(struct vfs_notification_callbacks *callbacks)
{
}

struct vfs_file_descriptor *vfs_open(const char *filepath, int flags)
{
  struct vfs_list_node *itr = g_IOHookList;
  while (itr)
  {
    if (strncmp(filepath, itr->protocol, strlen(itr->protocol)) == 0)
      return itr->iohook->open(filepath, flags);

    itr = itr->next;
  }

  return vfs_file_iohooks.open(filepath, flags);
}

size_t vfs_read(void *buffer, size_t size, size_t count, struct vfs_file_descriptor *fp)
{
  if (fp)
    return fp->iohook.read(buffer, size, count, fp);
  else
    return 0;
}

size_t vfs_write(const void *buffer, size_t size, size_t count, struct vfs_file_descriptor *fp)
{
  if (fp)
    return fp->iohook.write(buffer, size, count, fp);
  else
    return 0;
}

int vfs_seek(struct vfs_file_descriptor *fp, long int offset, int origin)
{
  if (fp)
    return fp->iohook.seek(fp, offset, origin);
  else
    return 0;
}

long int vfs_tell(struct vfs_file_descriptor *fp)
{
  if (fp)
    return fp->iohook.tell(fp);
  else
    return 0;
}

int vfs_flush(struct vfs_file_descriptor *fp)
{
  if (fp)
    return fp->iohook.flush(fp);
  else
    return 0;
}

int vfs_stat(const char *filepath, struct stat *buffer)
{
  struct vfs_list_node *itr = g_IOHookList;
  while (itr)
  {
    if (strncmp(filepath, itr->protocol, strlen(itr->protocol)) == 0)
      return itr->iohook->stat(filepath, buffer);

    itr = itr->next;
  }

  return vfs_file_iohooks.stat(filepath, buffer);
}

int vfs_close(struct vfs_file_descriptor *fp)
{
  if (fp)
    return fp->iohook.close(fp);
  else
    return 0;
}

struct vfs_directory_descriptor *vfs_opendir(const char *directorypath)
{
  struct vfs_list_node *itr = g_IOHookList;
  while (itr)
  {
    if (strncmp(directorypath, itr->protocol, strlen(itr->protocol)) == 0)
      return itr->iohook->opendir(directorypath);

    itr = itr->next;
  }

  return vfs_file_iohooks.opendir(directorypath);
}

struct dirent *vfs_readdir(struct vfs_directory_descriptor *dp)
{
  if (dp)
    return dp->iohook.readdir(dp);
  else
    return NULL;
}

void vfs_seekdir(struct vfs_directory_descriptor *dp, long loc)
{
  if (dp)
    return dp->iohook.seekdir(dp, loc);
}

void vfs_rewinddir(struct vfs_directory_descriptor *dp)
{
  if (dp)
    return dp->iohook.rewinddir(dp);
}

long vfs_telldir(struct vfs_directory_descriptor *dp)
{
  if (dp)
    return dp->iohook.telldir(dp);
  else
    return 0;
}

int vfs_closedir(struct vfs_directory_descriptor *dp)
{
  if (dp)
    return dp->iohook.closedir(dp);
  else
    return 0;
}
