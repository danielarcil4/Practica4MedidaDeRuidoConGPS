#ifndef __TIME_BASE_
#define __TIME_BASE_

#include <stdint.h>
#include <stdbool.h>

#include "hardware/timer.h"

typedef struct{
    uint64_t next;   ///< Struct member with the time for the next temporal event
    uint64_t delta;  ///< Struct member with the event period in us
    bool en;         ///< Enabler of the time base
} time_base_t;


void tb_init(time_base_t *t, uint64_t us, bool en);

static inline bool tb_check(time_base_t *t);

static inline void tb_update(time_base_t *t);

static inline void tb_next(time_base_t *t);

static inline void tb_enable(time_base_t *t);

static inline void tb_disable(time_base_t *t);

#endif // __TIME_BASE_