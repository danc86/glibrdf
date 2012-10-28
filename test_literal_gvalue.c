
/*
 * test_literal_gvalue.c, part of glibrdf
 * Copyright 2012 Dan Callaghan <djc@djc.id.au>
 * Licensed under GPLv3
 */

#include "glibrdf.h"

#define FIXTURE_TYPE LiteralGValueFixture
#define SETUP literal_gvalue_setup
#define TEARDOWN literal_gvalue_teardown
#define DATA const void *user_data G_GNUC_UNUSED
#define PACKAGE "/literal_gvalue/"
#define TEST_ADD(func) g_test_add(PACKAGE #func, FIXTURE_TYPE, NULL, SETUP, func, TEARDOWN)

typedef struct {
    librdf_world *world;
    librdf_storage *storage;
    librdf_model *model;
} FIXTURE_TYPE;

static void SETUP(FIXTURE_TYPE *fixture, DATA) {
    fixture->world = librdf_new_world();
    g_assert(fixture->world != NULL);
    librdf_world_open(fixture->world);
    fixture->storage = librdf_new_storage(fixture->world,
            "hashes", NULL, "hash-type='memory'");
    g_assert(fixture->storage != NULL);
    fixture->model = librdf_new_model(fixture->world, fixture->storage, "");
    g_assert(fixture->model != NULL);

    librdf_parser *parser = librdf_new_parser(fixture->world,
            NULL, "application/rdf+xml", NULL);
    g_assert(parser != NULL);
    librdf_uri *source_uri = librdf_new_uri_from_filename(
            fixture->world, "test-data.xml");
    g_assert(source_uri != NULL);
    int parse_error = librdf_parser_parse_into_model(parser,
            source_uri, NULL, fixture->model);
    g_assert(!parse_error);
    librdf_free_uri(source_uri);
    librdf_free_parser(parser);
}

static void TEARDOWN(FIXTURE_TYPE *fixture, DATA) {
    librdf_free_model(fixture->model);
    librdf_free_storage(fixture->storage);
    librdf_free_world(fixture->world);
}

static void test_untyped(FIXTURE_TYPE *fixture, DATA) {
    librdf_node *subj = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/Resource");
    g_assert(subj != NULL);
    librdf_node *prop = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/untyped");
    g_assert(prop != NULL);
    librdf_node *obj = librdf_model_get_target(fixture->model, subj, prop);
    g_assert(obj != NULL);
    GValue value = {0};
    librdf_node_get_literal_gvalue(obj, NULL, &value);
    g_assert(G_VALUE_HOLDS(&value, G_TYPE_STRING));
    g_assert_cmpstr(g_value_get_string(&value), ==, "blah");
    g_value_unset(&value);
    librdf_free_node(obj);
    librdf_free_node(prop);
    librdf_free_node(subj);
}

static void test_integer(FIXTURE_TYPE *fixture, DATA) {
    librdf_node *subj = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/Resource");
    g_assert(subj != NULL);
    librdf_node *prop = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/integer");
    g_assert(prop != NULL);
    librdf_node *obj = librdf_model_get_target(fixture->model, subj, prop);
    g_assert(obj != NULL);
    GValue value = {0};
    librdf_node_get_literal_gvalue(obj, NULL, &value);
    g_assert(G_VALUE_HOLDS(&value, G_TYPE_INT64));
    g_assert_cmpint(g_value_get_int64(&value), ==, 123);
    g_value_unset(&value);
    librdf_free_node(obj);
    librdf_free_node(prop);
    librdf_free_node(subj);
}

static void test_date(FIXTURE_TYPE *fixture, DATA) {
    librdf_node *subj = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/Resource");
    g_assert(subj != NULL);
    librdf_node *prop = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/date");
    g_assert(prop != NULL);
    librdf_node *obj = librdf_model_get_target(fixture->model, subj, prop);
    g_assert(obj != NULL);
    GValue value = {0};
    librdf_node_get_literal_gvalue(obj, NULL, &value);
    g_assert(G_VALUE_HOLDS(&value, G_TYPE_DATE));
    GDate *date = (GDate *)g_value_get_boxed(&value);
    g_assert_cmpuint(g_date_get_year(date), ==, 1986);
    g_assert_cmpuint(g_date_get_month(date), ==, 8);
    g_assert_cmpuint(g_date_get_day(date), ==, 16);
    g_value_unset(&value);
    librdf_free_node(obj);
    librdf_free_node(prop);
    librdf_free_node(subj);
}

static void test_datetime(FIXTURE_TYPE *fixture, DATA) {
    librdf_node *subj = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/Resource");
    g_assert(subj != NULL);
    librdf_node *prop = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/datetime");
    g_assert(prop != NULL);
    librdf_node *obj = librdf_model_get_target(fixture->model, subj, prop);
    g_assert(obj != NULL);
    GValue value = {0};
    librdf_node_get_literal_gvalue(obj, NULL, &value);
    g_assert(G_VALUE_HOLDS(&value, G_TYPE_DATE_TIME));
    GDateTime *datetime = (GDateTime *)g_value_get_boxed(&value);
    g_assert_cmpint(g_date_time_get_year(datetime),             ==, 2012);
    g_assert_cmpint(g_date_time_get_month(datetime),            ==, 7);
    g_assert_cmpint(g_date_time_get_day_of_month(datetime),     ==, 22);
    g_assert_cmpint(g_date_time_get_hour(datetime),             ==, 15);
    g_assert_cmpint(g_date_time_get_minute(datetime),           ==, 57);
    g_assert_cmpfloat(g_date_time_get_seconds(datetime),        ==, 41.0);
    g_assert_cmpint(g_date_time_get_utc_offset(datetime),       ==,
            /* 10 hours in us */ 10 * 60 * 60 * 1000000LL);
    g_value_unset(&value);
    librdf_free_node(obj);
    librdf_free_node(prop);
    librdf_free_node(subj);
}

static void custom_adaptor(const gchar *lv, GValue *value_out) {
    g_value_init(value_out, G_TYPE_STRING);
    g_value_set_static_string(value_out, "custom");
    return;
}
static librdf_gvalue_adaptor_func custom_adaptor_map(librdf_uri *datatype_uri) {
    const gchar *datatype_uri_string =
        (const gchar *)librdf_uri_as_string(datatype_uri);
    if (g_strcmp0(datatype_uri_string, "http://example.com/customType") == 0)
        return custom_adaptor;
    return librdf_default_gvalue_adaptor_map(datatype_uri);
}
static void test_custom_adaptor(FIXTURE_TYPE *fixture, DATA) {
    librdf_node *subj = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/Resource");
    g_assert(subj != NULL);
    librdf_node *prop = librdf_new_node_from_uri_string(fixture->world,
            (const unsigned char *)"http://example.com/custom");
    g_assert(prop != NULL);
    librdf_node *obj = librdf_model_get_target(fixture->model, subj, prop);
    g_assert(obj != NULL);
    GValue value = {0};
    librdf_node_get_literal_gvalue(obj, custom_adaptor_map, &value);
    g_assert(G_VALUE_HOLDS(&value, G_TYPE_STRING));
    g_assert_cmpstr(g_value_get_string(&value), ==, "custom");
    g_value_unset(&value);
    librdf_free_node(obj);
    librdf_free_node(prop);
    librdf_free_node(subj);
}

void add_literal_gvalue_tests(void) {
    TEST_ADD(test_untyped);
    TEST_ADD(test_integer);
    TEST_ADD(test_date);
    TEST_ADD(test_datetime);
    TEST_ADD(test_custom_adaptor);
}
