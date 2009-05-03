/*
 * Copyright (C) 2009 Pierre-Luc Beaudoin <pierre-luc@pierlux.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"

#include "champlain-map-source-factory.h"

#define DEBUG_FLAG CHAMPLAIN_DEBUG_NETWORK
#include "champlain-debug.h"

#include "champlain.h"
#include "champlain-cache.h"
#include "champlain-defines.h"
#include "champlain-enum-types.h"
#include "champlain-map-source.h"
#include "champlain-marshal.h"
#include "champlain-private.h"
#include "champlain-zoom-level.h"

#include <glib.h>
#include <string.h>

enum
{
  /* normal signals */
  LAST_SIGNAL
};

enum
{
  PROP_0,
};

/* static guint champlain_map_source_factory_signals[LAST_SIGNAL] = { 0, }; */

G_DEFINE_TYPE (ChamplainMapSourceFactory, champlain_map_source_factory, G_TYPE_OBJECT);

#define GET_PRIVATE(obj)    (G_TYPE_INSTANCE_GET_PRIVATE((obj), CHAMPLAIN_TYPE_MAP_SOURCE_FACTORY, ChamplainMapSourceFactoryPrivate))

struct _ChamplainMapSourceFactoryPrivate
{
  GSList *registered_sources;
};

static ChamplainMapSource * champlain_map_source_new_osm_mapnik (void);
static ChamplainMapSource * champlain_map_source_new_osm_cyclemap (void);
static ChamplainMapSource * champlain_map_source_new_osm_osmarender (void);
static ChamplainMapSource * champlain_map_source_new_oam (void);
static ChamplainMapSource * champlain_map_source_new_mff_relief (void);


static void
champlain_map_source_factory_get_property (GObject *object,
    guint prop_id,
    GValue *value,
    GParamSpec *pspec)
{
  //ChamplainMapSourceFactory *map_source_factory = CHAMPLAIN_MAP_SOURCE_FACTORY(object);
  //ChamplainMapSourceFactoryPrivate *priv = map_source_factory->priv;

  switch(prop_id)
    {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void
champlain_map_source_factory_set_property (GObject *object,
    guint prop_id,
    const GValue *value,
    GParamSpec *pspec)
{
  //ChamplainMapSourceFactory *map_source_factory = CHAMPLAIN_MAP_SOURCE_FACTORY(object);
  //ChamplainMapSourceFactoryPrivate *priv = map_source_factory->priv;

  switch(prop_id)
    {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void
champlain_map_source_factory_finalize (GObject *object)
{
  ChamplainMapSourceFactory *factory = CHAMPLAIN_MAP_SOURCE_FACTORY (object);

  g_slist_free (factory->priv->registered_sources);

  G_OBJECT_CLASS (champlain_map_source_factory_parent_class)->finalize (object);
}

static void
champlain_map_source_factory_class_init (ChamplainMapSourceFactoryClass *klass)
{
  g_type_class_add_private (klass, sizeof (ChamplainMapSourceFactoryPrivate));

  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = champlain_map_source_factory_finalize;
  object_class->get_property = champlain_map_source_factory_get_property;
  object_class->set_property = champlain_map_source_factory_set_property;
}

typedef struct {
  gchar *name;
  ChamplainMapSourceConstructor constructor;
} MapSourceConstructor;

static void
champlain_map_source_factory_init (ChamplainMapSourceFactory *factory)
{
  ChamplainMapSourceFactoryPrivate *priv = GET_PRIVATE (factory);

  factory->priv = priv;

  priv->registered_sources = NULL;

  champlain_map_source_factory_register (factory, CHAMPLAIN_MAP_SOURCE_OSM_MAPNIK,
      champlain_map_source_new_osm_mapnik);
  champlain_map_source_factory_register (factory, CHAMPLAIN_MAP_SOURCE_OSM_CYCLEMAP,
      champlain_map_source_new_osm_cyclemap);
  champlain_map_source_factory_register (factory, CHAMPLAIN_MAP_SOURCE_OSM_OSMARENDER,
      champlain_map_source_new_osm_osmarender);
  champlain_map_source_factory_register (factory, CHAMPLAIN_MAP_SOURCE_OAM,
      champlain_map_source_new_oam);
  champlain_map_source_factory_register (factory, CHAMPLAIN_MAP_SOURCE_MFF_RELIEF,
      champlain_map_source_new_mff_relief);
}

ChamplainMapSourceFactory *
champlain_map_source_factory_get_default (void)
{
  static ChamplainMapSourceFactory *instance = NULL;

  if (G_UNLIKELY (instance == NULL))
    {
      instance = g_object_new (CHAMPLAIN_TYPE_MAP_SOURCE_FACTORY, NULL);
      return instance;
    }

  return g_object_ref (instance);
}

gchar **
champlain_map_source_factory_get_list (ChamplainMapSourceFactory *factory)
{

  gint count;
  gchar **ret;
  GSList *item;

  count = g_slist_length (factory->priv->registered_sources);
  ret = g_new0(char*, count + 1);
  item = factory->priv->registered_sources;

  count = 0;
  while (item != NULL)
    {
      MapSourceConstructor *cons = (MapSourceConstructor*) item->data;
      ret[count] = g_strdup (cons->name);
      item = g_slist_next (item);
      count++;
    }
  ret[count] = NULL;

  return ret;
}

ChamplainMapSource *
champlain_map_source_factory_create (ChamplainMapSourceFactory *factory,
    const gchar *name)
{
  GSList *item;

  item = factory->priv->registered_sources;

  while (item != NULL)
    {
      MapSourceConstructor *cons = (MapSourceConstructor*) item->data;
      if (strcmp (cons->name, name) == 0)
        return cons->constructor ();
      item = g_slist_next (item);
    }
  return NULL;
}

gboolean
champlain_map_source_factory_register (ChamplainMapSourceFactory *factory,
    const gchar *name,
    ChamplainMapSourceConstructor callback)
{
  MapSourceConstructor *cons;

  /* FIXME: check for existing factory with that name? */
  cons = g_slice_new0(MapSourceConstructor);
  cons->name = g_strdup (name);
  cons->constructor = callback;
  factory->priv->registered_sources = g_slist_append (factory->priv->registered_sources, cons);
  return TRUE;
}

static ChamplainMapSource *
champlain_map_source_new_osm_cyclemap (void)
{
  return CHAMPLAIN_MAP_SOURCE (champlain_network_map_source_new_full (CHAMPLAIN_MAP_SOURCE_OSM_CYCLEMAP,
      "(CC) BY 2.0 OpenStreetMap contributors",
      "http://creativecommons.org/licenses/by/2.0/", 0, 18, 256,
      CHAMPLAIN_MAP_PROJECTION_MERCATOR,
      "http://andy.sandbox.cloudmade.com/tiles/cycle/#Z#/#X#/#Y#.png"));
}

static ChamplainMapSource *
champlain_map_source_new_osm_osmarender (void)
{
  return CHAMPLAIN_MAP_SOURCE (champlain_network_map_source_new_full (CHAMPLAIN_MAP_SOURCE_OSM_OSMARENDER,
      "(CC) BY 2.0 OpenStreetMap contributors",
      "http://creativecommons.org/licenses/by/2.0/", 0, 18, 256,
      CHAMPLAIN_MAP_PROJECTION_MERCATOR,
      "http://tah.openstreetmap.org/Tiles/tile/#Z#/#X#/#Y#.png"));
}

static ChamplainMapSource *
champlain_map_source_new_osm_mapnik (void)
{
  return CHAMPLAIN_MAP_SOURCE (champlain_network_map_source_new_full (CHAMPLAIN_MAP_SOURCE_OSM_MAPNIK,
      "(CC) BY 2.0 OpenStreetMap contributors",
      "http://creativecommons.org/licenses/by/2.0/", 0, 18, 256,
      CHAMPLAIN_MAP_PROJECTION_MERCATOR,
      "http://tile.openstreetmap.org/#Z#/#X#/#Y#.png"));
}

static ChamplainMapSource *
champlain_map_source_new_oam (void)
{
  return CHAMPLAIN_MAP_SOURCE (champlain_network_map_source_new_full (CHAMPLAIN_MAP_SOURCE_OAM,
      "(CC) BY 3.0 OpenArialMap contributors",
      "http://creativecommons.org/licenses/by/3.0/", 0, 17, 256,
      CHAMPLAIN_MAP_PROJECTION_MERCATOR,
      "http://tile.openaerialmap.org/tiles/1.0.0/openaerialmap-900913/#Z#/#X#/#Y#.jpg"));
}

static ChamplainMapSource *
champlain_map_source_new_mff_relief (void)
{
  return CHAMPLAIN_MAP_SOURCE (champlain_network_map_source_new_full (CHAMPLAIN_MAP_SOURCE_MFF_RELIEF,
      "Map data available under GNU Free Documentation license, Version 1.2 or later",
      "http://www.gnu.org/copyleft/fdl.html", 0, 11, 256,
      CHAMPLAIN_MAP_PROJECTION_MERCATOR,
      "http://maps-for-free.com/layer/relief/z#Z#/row#Y#/#Z#_#X#-#Y#.jpg"));
}