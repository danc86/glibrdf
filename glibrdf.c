
/*
 * glibrdf.c
 * Copyright 2012 Dan Callaghan <djc@djc.id.au>
 * Licensed under GPLv3
 */

#include <stdbool.h>
#include <time.h>
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

// glib should do this for me >:(
static GDate *parse_iso8601_date(const gchar *s) {
    struct tm tm = {0};
    if (*strptime(s, "%F", &tm) != '\0')
        return NULL;
    return g_date_new_dmy(tm.tm_mday, 1 + tm.tm_mon, 1900 + tm.tm_year);
}

// g_time_val_from_iso8601 doesn't preserve timezone >:(((
static GDateTime *parse_iso8601_datetime(const gchar *s) {
    struct tm tm = {0};
    // parse the date and time, timezone will be left at the end
    const gchar *rest = strptime(s, "%FT%T", &tm);
    GTimeZone *tz = g_time_zone_new(rest);
    g_return_val_if_fail(tz != NULL, NULL);
    GDateTime *result = g_date_time_new(tz,
            1900 + tm.tm_year,
            1 + tm.tm_mon,
            tm.tm_mday,
            tm.tm_hour,
            tm.tm_min,
            (gdouble) tm.tm_sec);
    g_time_zone_unref(tz);
    return result;
}

// XXX handle parse failures more gracefully?
// XXX should have some kind of registry so that callers can add new types
void librdf_node_get_literal_gvalue(librdf_node *node, GValue *value_out) {
    g_return_if_fail(librdf_node_is_literal(node));
    const gchar *lv = (const gchar *)librdf_node_get_literal_value(node);
    g_return_if_fail(lv != NULL);
    librdf_uri *datatype_uri = librdf_node_get_literal_value_datatype_uri(node);
    if (datatype_uri == NULL) {
        g_value_init(value_out, G_TYPE_STRING);
        g_value_set_string(value_out, lv);
        return;
    }
    const gchar *datatype_uri_string =
        (const gchar *)librdf_uri_as_string(datatype_uri);
    if (g_strcmp0(datatype_uri_string,
            "http://www.w3.org/2001/XMLSchema#integer") == 0) {
        g_value_init(value_out, G_TYPE_INT64);
        gchar *lv_unconsumed;
        g_value_set_int64(value_out, g_ascii_strtoll(lv, &lv_unconsumed, 10));
        g_return_if_fail(*lv_unconsumed == '\0');
        return;
    }
    if (g_strcmp0(datatype_uri_string,
            "http://www.w3.org/TR/xmlschema-2/#date") == 0) {
        GDate *date = parse_iso8601_date(lv);
        g_return_if_fail(date != NULL);
        g_value_init(value_out, G_TYPE_DATE);
        g_value_set_boxed(value_out, date);
        return;
    }
    if (g_strcmp0(datatype_uri_string,
            "http://www.w3.org/TR/xmlschema-2/#datetime") == 0) {
        GDateTime *datetime = parse_iso8601_datetime(lv);
        g_return_if_fail(datetime != NULL);
        g_value_init(value_out, G_TYPE_DATE_TIME);
        g_value_set_boxed(value_out, datetime);
        return;
    }
    g_warning("Unhandled RDF type %s", librdf_uri_as_string(datatype_uri));
    g_value_init(value_out, G_TYPE_STRING);
    g_value_set_string(value_out, lv);
}
