#include "champlain-perl.h"


MODULE = Champlain::Tile  PACKAGE = Champlain::Tile  PREFIX = champlain_tile_


ChamplainTile*
champlain_tile_new (class)
	C_ARGS: /* No args */


ChamplainTile*
champlain_tile_new_full (class, gint x, gint y, guint size, gint zoom_level)
	C_ARGS: x, y, size, zoom_level


gint
champlain_tile_get_x (ChamplainTile *self)


gint
champlain_tile_get_y (ChamplainTile *self)


gint
champlain_tile_get_zoom_level (ChamplainTile *self)


guint
champlain_tile_get_size (ChamplainTile *self)


ChamplainState
champlain_tile_get_state (ChamplainTile *self)


const gchar*
champlain_tile_get_uri (ChamplainTile *self)


const gchar*
champlain_tile_get_filename (ChamplainTile *self)


ClutterActor*
champlain_tile_get_actor (ChamplainTile *self)


void
champlain_tile_set_x (ChamplainTile *self, gint x)


void
champlain_tile_set_y (ChamplainTile *self, gint y)


void
champlain_tile_set_zoom_level (ChamplainTile *self, gint zoom_level)


void
champlain_tile_set_size (ChamplainTile *self, guint size)


void
champlain_tile_set_state (ChamplainTile *self, ChamplainState state)


void
champlain_tile_set_uri (ChamplainTile *self, const gchar* uri)


void
champlain_tile_set_filename (ChamplainTile *self, const gchar* filename)


#void
#champlain_tile_set_actor (ChamplainTile *self, ClutterActor* actor)

