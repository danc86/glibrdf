
/*
 * tests.c, part of glibrdf
 * Copyright 2012 Dan Callaghan <djc@djc.id.au>
 * Licensed under GPLv3
 */

#include <glib-object.h>

void add_literal_gvalue_tests(void);

int main(int argc, char *argv[]) {
    g_type_init();
    g_test_init(&argc, &argv, NULL);
    add_literal_gvalue_tests();
    return g_test_run();
}
