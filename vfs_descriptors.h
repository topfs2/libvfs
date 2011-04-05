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

#include "vfs_iohook.h"
#include "vfs_lists.h"
#include <stdlib.h>
#include <pthread.h>

struct vfs_context_t
{
  struct vfs_list *iohook_list;
};

struct vfs_file_descriptor_t
{
  void *fp;
  struct vfs_iohook *iohook;
};

struct vfs_directory_descriptor_t
{
  void *dp;
  struct vfs_iohook *iohook;

  pthread_t readdir_thread;
};

struct vfs_watchdog_descriptor_t
{
  void *wp;
  struct vfs_iohook *iohook;
};
