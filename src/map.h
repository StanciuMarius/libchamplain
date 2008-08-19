/*
 * Copyright (C) 2008 Pierre-Luc Beaudoin <pierre-luc@squidy.info>
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

#ifndef MAP_H
#define MAP_H

#include "champlain.h"
#include "champlainview.h"
#include "champlain_defines.h"
#include "zoomlevel.h"
#include "tile.h"

#include <glib.h>
#include <clutter/clutter.h>


struct _Map
{
  int zoom_levels;
  const gchar* name;
  ZoomLevel* current_level;
  int tile_size;
  
  Tile* (* get_tile) (Map* map, guint zoom_level, guint x, guint y);
  guint (* get_row_count) (Map* map, guint zoom_level);
  guint (* get_column_count) (Map* map, guint zoom_level);
  
  gdouble (* longitude_to_x) (Map* map, gdouble longitude, guint zoom_level);
  gdouble (* latitude_to_y) (Map* map, gdouble latitude, guint zoom_level);
  gdouble (* x_to_longitude) (Map* map, gdouble x, guint zoom_level);
  gdouble (* y_to_latitude) (Map* map, gdouble y, guint zoom_level);
  
};


CHAMPLAIN_API Map* champlain_map_new (ChamplainMapSource source);

void map_load_visible_tiles (Map* map, ChamplainRect viewport);

#endif
