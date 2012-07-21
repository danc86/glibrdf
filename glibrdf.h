
/*
 * glibrdf.h
 * Copyright 2012 Dan Callaghan <djc@djc.id.au>
 * Licensed under GPLv3
 */

#ifndef _GLIBRDF_H
#define _GLIBRDF_H

#include <glib-object.h>
#include <librdf.h>

GType librdf_node_get_gtype(void);
#define G_TYPE_RDF_NODE librdf_node_get_gtype()
void librdf_node_get_literal_gvalue(librdf_node *node, GValue *value_out);

#endif
