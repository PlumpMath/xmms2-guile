#ifndef INC_XMMS2_GUILE_H
#define INC_XMMS2_GUILE_H

#include <libguile.h>
#include <xmmsclient/xmmsclient.h>

struct x2_connection {
    /** The client name to announce to the server upon connect */
    SCM clientname;
    /** The actual connection data. */
    xmmsc_connection_t *c;
};

void xg_scm_define_and_export(const char *, int, int, int, SCM (*)());

#define X2_SCM_EXPORT_CONSTANT(c_level, scm_level, value)               \
    do {                                                                \
        c_level = scm_permanent_object(scm_c_define(scm_level, value)); \
        scm_c_export(scm_level);                                        \
    } while (0)

#define X2_TRIVIAL_SERVER_ACTION(API_FNC, XMMS2_FNC)                    \
    static SCM API_FNC(SCM);                                            \
    static SCM API_FNC(SCM connection)                                  \
    {                                                                   \
        return x2_trivial_server_action(XMMS2_FNC, connection);         \
    }

SCM x2_trivial_server_action(xmmsc_result_t *(*)(xmmsc_connection_t *), SCM);

SCM make_x2_result(void);
SCM make_x2_value(void);

void init_x2_primitive_connect(void);
void init_x2_primitive_playback(void);
void init_x2_primitive_synchronous(void);
void init_x2_primitive_value(void);
void init_x2_type_connection(void);
void init_x2_type_result(void);
void init_x2_type_value(void);
void xmms2_guile_ext_init(void);

#endif /* INC_XMMS2_GUILE_H */
