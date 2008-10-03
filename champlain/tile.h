/*
 * Copyright (C) 2008 Pierre-Luc Beaudoin <pierre-luc@pierlux.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef CHAMPLAIN_MAP_TILE_H
#define CHAMPLAIN_MAP_TILE_H

#include <glib.h>
#include <clutter/clutter.h>

typedef struct
{
  ClutterActor* actor;
  int x;
  int y;
  int size;
  int level;

  gboolean loading; // TRUE when a callback exist to load the tile, FALSE otherwise
  gboolean to_destroy; // TRUE when a tile struct should be deleted when loading is done, FALSE otherwise
} Tile;

void tile_free(Tile* tile);

#endif
