
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
typedef void (*librdf_gvalue_adaptor_func)(const gchar *lv, GValue *value_out);
typedef librdf_gvalue_adaptor_func (*librdf_gvalue_adaptor_map_func)(librdf_uri *datatype_uri);
librdf_gvalue_adaptor_func librdf_default_gvalue_adaptor_map(librdf_uri *datatype_uri);
void librdf_node_get_literal_gvalue(librdf_node *node,
        librdf_gvalue_adaptor_map_func adaptor_map,
        GValue *value_out);

#endif
