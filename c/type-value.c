/*
 * Copyright (c) 2013 xmms2-guile workers, All rights reserved.
 *
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file type-value.c
 * @brief Guile representation for xmmsv_t
 *
 * We don't need to free this data container. With synchronous connections,
 * it'll get wiped away with the corresponding `xmmsc_result_t'. With
 * asynchronous connections it gets freed after a callback is done with it.
 *
 * That sounds awesome, but makes our job a bit harder. Because the garbage
 * collection process might clean away a parent result container, while the C
 * glue code works on a value container. We need a back-reference to the parent
 * result container to be able to reference it together with the value. See
 * `xmms2-guile.h' for details.
 */

#include "compiler.h"
#include "xmms2-guile.h"

#include <libguile.h>
#include <xmmsclient/xmmsclient.h>

static int print_x2_value(SCM, SCM, scm_print_state *);
static size_t free_x2_value(SCM);
static scm_t_bits x2_value_tag;

SCM
make_x2_value(void)
{
    struct x2_value *v;
    SCM smob;

    v = (struct x2_value *)scm_gc_malloc(sizeof (struct x2_value),
                                         "xmms2:type/value");
    v->value = (xmmsv_t *)NULL;
    v->parent_result = SCM_EOL;
    SCM_NEWSMOB(smob, x2_value_tag, v);
    return smob;
}

static int
print_x2_value(UNUSED SCM smob, SCM port, UNUSED scm_print_state *pstate)
{
    scm_puts("#<xmms2:type/value>", port);
    return 1;
}

static size_t
free_x2_value(SCM smob)
{
    struct x2_value *v;

    v = (struct x2_value *) SCM_SMOB_DATA(smob);
    scm_gc_free(v, sizeof (struct x2_value), "xmms2:type/value");
    return 0;
}

static SCM
mark_x2_value(SCM smob)
{
    struct x2_value *v;

    v = (struct x2_value *) SCM_SMOB_DATA(smob);
    return v->parent_result;
}

void
init_x2_type_value(void)
{
    x2_value_tag = scm_make_smob_type("xmms2:type/value", 0);
    scm_set_smob_mark(x2_value_tag, mark_x2_value);
    scm_set_smob_print(x2_value_tag, print_x2_value);
    scm_set_smob_free(x2_value_tag, free_x2_value);
    xg_scm_define_and_export("xmms2:type/make-value", 0, 0, 0, make_x2_value);
}
