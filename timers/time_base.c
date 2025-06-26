#include <stdint.h>
#include "timers/time_base.h"


void tb_init(time_base_t *t, uint64_t us, bool en){
    t->next = time_us_64() + us ;
    t->delta = us;
    t->en = en;
}

static inline bool tb_check(time_base_t *t){
    return (time_us_64() >= t->next) && t->en;
}

static inline void tb_update(time_base_t *t){
    t->next = time_us_64() + t->delta;
}

static inline void tb_next(time_base_t *t){
    t->next = t->next + t->delta;
}


static inline void tb_enable(time_base_t *t){
    t->en = true;
}

static inline void tb_disable(time_base_t *t){
    t->en = false;
}