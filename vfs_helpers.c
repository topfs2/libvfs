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

#include "vfs_helpers.h"
#include <string.h>

struct vfs_metadata *vfs_new_metadata(const char *key, const char *value)
{
  if (key && value)
  {
    char *key_copy   = (char *)malloc(sizeof(char) * strlen(key));
    char *value_copy = (char *)malloc(sizeof(char) * strlen(value));

    strcpy(key_copy, key);
    strcpy(value_copy, value);

    struct vfs_metadata tempdata = { key_copy, value_copy };

    struct vfs_metadata *metadata = (struct vfs_metadata *)malloc(sizeof(struct vfs_metadata));
    memcpy(metadata, &tempdata, sizeof(struct vfs_metadata));

    return metadata;
  }
  else
    return NULL;
}

struct vfs_metadata *vfs_free_metadata(struct vfs_metadata *metadata)
{
  if (metadata)
  {
    struct vfs_metadata *next = metadata->next;

    free((char *)metadata->key);
    free((char *)metadata->value);
    free(metadata);

    return next;
  }
  else
    return NULL;
}

struct vfs_properties *vfs_new_properties(const char *name, int size, int hidden, struct vfs_metadata *metadata)
{
  char *name_copy = NULL;
  if (name)
  {
    name_copy = (char *)malloc(sizeof(char) * strlen(name));
    strcpy(name_copy, name);
  }

  struct vfs_properties tempproperties = { name_copy, size, hidden, metadata };

  struct vfs_properties *properties = (struct vfs_properties *)malloc(sizeof(struct vfs_properties));
  memcpy(properties, &tempproperties, sizeof(struct vfs_properties));

  return properties;
}
