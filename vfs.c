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
#include "vfs_descriptors.h"
#include "vfs_posix.h"
#include <string.h>

const int vfs_default_iohooks = vfs_file;
const int vfs_all_iohooks = vfs_file;

struct vfs_module
{
  const char *protocol;
  struct vfs_iohook *iohook;
};

struct vfs_module_node
{
  enum vfs_available_iohooks flag;
  struct vfs_module module;
};

struct vfs_module_node vfs_static_vfs_modules[] =
{
  { vfs_file, { "file", &vfs_posix_iohooks } },
  { vfs_none, { NULL,   NULL } }
};

/* Helper methods, should be moved later */
struct vfs_module *vfs_create_module(const char *protocol, struct vfs_iohook *iohook)
{
  struct vfs_module *module = malloc(sizeof(struct vfs_module));

  int len = strlen(protocol) + 1;

  module->protocol = malloc(sizeof(char) * len);
  strncpy((char *)module->protocol, protocol, len);
  module->iohook = iohook;

  return module;
}

struct vfs_iohook *vfs_find_iohooks(vfs_context ctx, const char *filepath)
{
  if (ctx)
  {
    struct vfs_module *li;
    struct vfs_list_node *itr = vfs_list_begin(ctx->iohook_list);
    FOREACH(itr, li)
    {
      if (strncmp(filepath, li->protocol, strlen(li->protocol)) == 0)
        return li->iohook;
    }
  }

  return NULL;
}

/* Normal methods which belongs in vfs.c */

vfs_context vfs_initialize(int iohooks)
{
  int failed = 0;
  vfs_context ctx = malloc(sizeof(struct vfs_context_t));
  ctx->iohook_list = vfs_list_create();

  struct vfs_module_node *itr = vfs_static_vfs_modules;
  while (itr->flag != vfs_none)
  {
    if (!(itr->flag & iohooks && vfs_add_iohook(ctx, itr->module.protocol, itr->module.iohook) == 0))
      failed |= itr->flag;

    itr++;
  }

  return ctx;
}

int vfs_add_iohook(vfs_context ctx, const char *protocol, struct vfs_iohook *iohook)
{
  if (ctx)
  {
    struct vfs_module *module = vfs_create_module(protocol, iohook);
    vfs_list_append(ctx->iohook_list, module);

    return 0;
  }

  return 1;
}

vfs_file_descriptor vfs_open(vfs_context ctx, const char *filepath, int flags)
{
  struct vfs_iohook *iohook = vfs_find_iohooks(ctx, filepath);

  if (iohook && iohook->open)
  {
    vfs_file_descriptor fp = malloc(sizeof(struct vfs_file_descriptor_t));

    fp->fp      = iohook->open(filepath, flags);
    fp->iohook  = iohook;

    if (fp->fp)
      return fp;
    else
      free(fp);
  }

  return NULL;
}

size_t vfs_read(vfs_file_descriptor fp, void *buffer, size_t size, size_t count)
{
  if (fp)
  {
    struct vfs_iohook *iohook = fp->iohook;

    if (iohook && iohook->read)
      return iohook->read(fp->fp, buffer, size, count);
  }
  return 0;
}

size_t vfs_write(vfs_file_descriptor fp, const void *buffer, size_t size, size_t count)
{
  if (fp)
  {
    struct vfs_iohook *iohook = fp->iohook;

    if (iohook && iohook->write)
      return iohook->write(fp->fp, buffer, size, count);
  }

  return 0;
}

int vfs_seek(vfs_file_descriptor fp, long int offset, int origin)
{
  if (fp)
  {
    struct vfs_iohook *iohook = fp->iohook;

    if (iohook && iohook->seek)
      return iohook->seek(fp->fp, offset, origin);
  }

  return 0;
}

long int vfs_tell(vfs_file_descriptor fp)
{
  if (fp)
  {
    struct vfs_iohook *iohook = fp->iohook;

    if (iohook && iohook->tell)
      return iohook->tell(fp->fp);
  }

  return 0;
}

struct vfs_properties *vfs_stat(vfs_context ctx, const char *filepath)
{
  struct vfs_iohook *iohook = vfs_find_iohooks(ctx, filepath);

  if (iohook && iohook->stat)
    return iohook->stat(filepath);

  return NULL;
}

int vfs_flush(vfs_file_descriptor fp)
{
  if (fp)
  {
    struct vfs_iohook *iohook = fp->iohook;

    if (iohook && iohook->flush)
      return iohook->flush(fp->fp);
  }

  return 0;
}

int vfs_close(vfs_file_descriptor fp)
{
  if (fp)
  {
    struct vfs_iohook *iohook = fp->iohook;

    if (iohook && iohook->close)
    {
      int result = iohook->close(fp->fp);
      free(fp);
      return result;
    }
  }

  return 0;
}

vfs_directory_descriptor vfs_opendir(vfs_context ctx, const char *directorypath)
{
  struct vfs_iohook *iohook = vfs_find_iohooks(ctx, directorypath);

  if (iohook && iohook->opendir)
  {
    vfs_directory_descriptor dp = malloc(sizeof(struct vfs_directory_descriptor_t));

    dp->dp          = iohook->opendir(directorypath);
    dp->iohook      = iohook;

    if (dp->dp)
      return dp;
    else
      free(dp);
  }

  return NULL;
}

void vfs_readdir_sync(vfs_directory_descriptor dp, struct vfs_directory_callbacks callbacks, void *cls)
{
  if (dp)
  {
    struct vfs_iohook *iohook = dp->iohook;

    if (iohook && iohook->readdir)
      iohook->readdir(dp->dp, cls, callbacks);
  }
}

void vfs_readdir_async(vfs_directory_descriptor dp, struct vfs_directory_callbacks callbacks, void *cls)
{
  if (dp)
  {
    struct vfs_iohook *iohook = dp->iohook;

    if (iohook && iohook->readdir)
      iohook->readdir(dp->dp, cls, callbacks);
  }
}

int vfs_closedir(vfs_directory_descriptor dp)
{
  if (dp)
  {
    struct vfs_iohook *iohook = dp->iohook;

    if (iohook && iohook->closedir)
    {
      int result = iohook->closedir(dp->dp);
      free(dp);
      return result;
    }
  }

  return 0;
}

vfs_watchdog_descriptor vfs_add_watch(vfs_context ctx, const char *watchpath, struct vfs_notification_callbacks callbacks, void *cls)
{
  return NULL;
}

int vfs_remove_watch(vfs_watchdog_descriptor wp)
{
  return 0;
}
