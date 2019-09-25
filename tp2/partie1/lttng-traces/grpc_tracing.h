#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER grpc_tracing

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./grpc_tracing.h"

#if !defined(_TP_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TP_H

#include <lttng/tracepoint.h>

/*
 * Use TRACEPOINT_EVENT(), TRACEPOINT_EVENT_CLASS(),
 * TRACEPOINT_EVENT_INSTANCE(), and TRACEPOINT_LOGLEVEL() here.
 */

TRACEPOINT_EVENT(
    grpc_tracing,
    server_start,
    TP_ARGS(
        int, id
    ),
    TP_FIELDS(
        ctf_integer(int, id, id)
    )
)

TRACEPOINT_EVENT(
    grpc_tracing,
    server_end,
    TP_ARGS(
        int, id
    ),
    TP_FIELDS(
        ctf_integer(int, id, id)
    )
)

TRACEPOINT_EVENT(
    grpc_tracing,
    manager_send,
    TP_ARGS(
        const char *, message
    ),
    TP_FIELDS(
        ctf_string(string, message)
    )
)

TRACEPOINT_EVENT(
    grpc_tracing,
    manager_recv,
    TP_ARGS(
        const char *, message
    ),
    TP_FIELDS(
        ctf_string(string, message)
    )
)




#endif /* _TP_H */

#include <lttng/tracepoint-event.h>
