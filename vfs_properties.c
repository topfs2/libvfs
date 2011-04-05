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

#include "vfs_properties.h"
#include <string.h>

struct vfs_metadata *vfs_metadata_create(const char *key, const char *value)
{
  if (key && value)
  {
    struct vfs_metadata *metadata = malloc(sizeof(struct vfs_metadata));

    metadata->key   = malloc(sizeof(char) * strlen(key));
    metadata->value = malloc(sizeof(char) * strlen(value));

    strcpy((char *)metadata->key, key);
    strcpy((char *)metadata->value, value);

    return metadata;
  }
  else
    return NULL;
}

void vfs_metadata_free(struct vfs_metadata *metadata)
{
  if (metadata)
  {
    free((char *)metadata->key);
    free((char *)metadata->value);
    free(metadata);
  }
}

struct vfs_properties *vfs_properties_create(const char *name, int size, int hidden, struct vfs_list *metadata_list)
{
  struct vfs_properties *properties = malloc(sizeof(struct vfs_properties));

  if (name)
  {
    properties->name = malloc(sizeof(char) * strlen(name));
    strcpy((char *)properties->name, name);
  }

  properties->size              = size;
  properties->hidden            = hidden;
  properties->metadata_list     = metadata_list;

  return properties;
}

void vfs_metadata_void_free(void *ptr)
{
  vfs_metadata_free((struct vfs_metadata *)ptr);
}

void vfs_properties_free(struct vfs_properties *properties)
{
  if (properties)
  {
    vfs_list_free(properties->metadata_list, vfs_metadata_void_free);
    free((char *)properties->name);
    free(properties);
  }
}
