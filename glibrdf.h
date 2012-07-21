
#ifndef _GLIBRDF_H
#define _GLIBRDF_H

#include <glib-object.h>
#include <librdf.h>

GType librdf_node_get_gtype(void);
#define G_TYPE_RDF_NODE librdf_node_get_gtype()

#endif
