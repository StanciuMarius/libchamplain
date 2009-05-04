#include "champlain-perl.h"


MODULE = Champlain::MapSourceFactory  PACKAGE = Champlain::MapSourceFactory  PREFIX = champlain_map_source_factory_


ChamplainMapSourceFactory*
champlain_map_source_factory_get_default (class)
	C_ARGS: /* No args */


void
champlain_map_source_factory_get_list (ChamplainMapSourceFactory *factory)
	PREINIT:
		gchar **list = NULL;
		gchar *item = NULL;
		gint i = 0;
	
	PPCODE:
		list = champlain_map_source_factory_get_list(factory);
		
		item = list[0];
		while (item != NULL) {
			XPUSHs(sv_2mortal(newSVGChar(item)));
			item = list[++i];
		}
		
		g_strfreev(list);


ChamplainMapSource*
champlain_map_source_factory_create (ChamplainMapSourceFactory *factory, const gchar *id)


#gboolean
#champlain_map_source_factory_register (ChamplainMapSourceFactory *factory, const gchar *id, ChamplainMapSourceConstructor callback)


const gchar*
OSM_MAPNIK (class)
	CODE:
		RETVAL = CHAMPLAIN_MAP_SOURCE_OSM_MAPNIK;

	OUTPUT:
		RETVAL


const gchar*
OSM_OSMARENDER (class)
	CODE:
		RETVAL = CHAMPLAIN_MAP_SOURCE_OSM_OSMARENDER;

	OUTPUT:
		RETVAL


const gchar*
OSM_CYCLEMAP (class)
	CODE:
		RETVAL = CHAMPLAIN_MAP_SOURCE_OSM_CYCLEMAP;

	OUTPUT:
		RETVAL


const gchar*
OAM (class)
	CODE:
		RETVAL = CHAMPLAIN_MAP_SOURCE_OAM;

	OUTPUT:
		RETVAL


const gchar*
MFF_RELIEF (class)
	CODE:
		RETVAL = CHAMPLAIN_MAP_SOURCE_MFF_RELIEF;

	OUTPUT:
		RETVAL