
#include "glibrdf.h"

GType librdf_node_get_gtype(void) {
    static volatile gsize _librdf_node_type_id = 0;
    if (g_once_init_enter(&_librdf_node_type_id)) {
        GType type_id = g_boxed_type_register_static("librdf_node",
                (GBoxedCopyFunc) librdf_new_node_from_node,
                (GBoxedFreeFunc) librdf_free_node);
        g_once_init_leave(&_librdf_node_type_id, type_id);
    }
    return _librdf_node_type_id;
}
