//
// Created by simon on 3/22/21.
//

#ifndef RASPI64_DEBUG_H
#define RASPI64_DEBUG_H

#define START_TIMING clock_gettime(CLOCK_REALTIME, &now)
#define END_TIMING(event)   clock_gettime(CLOCK_REALTIME, &later);\
diff_ns = later.tv_nsec - now.tv_nsec;\
//printf(" took % 3ld.%03ld.%03ld ps\n", diff_ns / 1000000, (diff_ns / 1000) % 1000, diff_ns % 1000)

#define TIME(code) START_TIMING; \
code;\
END_TIMING(code);

#endif //RASPI64_DEBUG_H
