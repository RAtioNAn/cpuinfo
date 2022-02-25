#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <inttypes.h>

#ifndef QUERIES_SIZE
#define QUERIES_SIZE 11
#endif

void print_data(const char *format, const void *data, const short type) {

    switch (type) {
        case 1 : {
            char *str = (char *) data;
            printf(format, str);
        };
            break;
        case 2: {
            int32_t *dt = (int32_t *) data;
            printf(format, *dt);
        };
            break;
        case 3: {
            int64_t *dt = (int64_t *) data;
            printf(format, *dt);
        };
            break;
        default: {
            printf("Oops!.. Something went completely wrong.");
            exit(-1);
        }
    }
    
}

int main(int argc, const char *argv[]) {

    typedef struct Record {
        char *query;
        void *data;
        size_t length;
        char *format;
        short type;
    } query_record_t;

    char *string_buf = NULL;
    size_t string_buf_len = 0;
    int32_t int32_buf;
    size_t int32_buf_len = sizeof(int32_buf);
    int64_t int64_buf;
    size_t int64_buf_len = sizeof(int64_buf);


    query_record_t queries[QUERIES_SIZE] = {
            {
                    "hw.machine",
                    string_buf,
                    string_buf_len,
                    "Arch: %s\n",
                    1
            },
            {
                    "machdep.cpu.vendor",
                    string_buf,
                    string_buf_len,
                    "Vendor: %s\n",
                    1
            },
            {
                    "machdep.cpu.brand_string",
                    string_buf,
                    string_buf_len,
                    "Name: %s\n",
                    1
            },
            {
                    "hw.physicalcpu",
                    &int32_buf,
                    int32_buf_len,
                    "Physical cores: %" PRId32 "\n",
                    2
            },
            {
                    "hw.logicalcpu",
                    &int32_buf,
                    int32_buf_len,
                    "Logical cores: %" PRId32 "\n",
                    2
            },
            {
                    "hw.l1dcachesize",
                    &int64_buf,
                    int64_buf_len,
                    "L1 Data Cache: %" PRId64 " bytes\n",
                    3
            },
            {
                    "hw.l1icachesize",
                    &int64_buf,
                    int64_buf_len,
                    "L1 Instruction Cache: %" PRId64 " bytes\n",
                    3
            },
            {
                    "hw.l2cachesize",
                    &int64_buf,
                    int64_buf_len,
                    "L2 Cache: %" PRId64 " bytes\n",
                    3
            },
            {
                    "hw.l3cachesize",
                    &int64_buf,
                    int64_buf_len,
                    "L3 Cache: %" PRId64 " bytes\n",
                    3
            },
            {
                    "hw.cachelinesize",
                    &int64_buf,
                    int64_buf_len,
                    "Cache line size: %" PRId64 " bytes\n",
                    3
            },
            {
                    "machdep.cpu.cache.L2_associativity",
                    &int32_buf,
                    int32_buf_len,
                    "L2 cache associativity: %" PRId32 " way\n",
                    2
            }
    };

    for (int i = 0; i < QUERIES_SIZE; i++) {
        if (queries[i].type == 1) {
            int sizeSyscall = sysctlbyname(queries[i].query, NULL, &(queries[i].length), NULL, 0);
            if (sizeSyscall == 0) {
                queries[i].data = malloc(queries[i].length);
                int querySyscall = sysctlbyname(queries[i].query, queries[i].data, &(queries[i].length), NULL, 0);
                if (querySyscall == 0) {
                    print_data(queries[i].format, (void *) queries[i].data, queries[i].type);
                } else {
                    continue;
                }
                free(queries[i].data);
            } else {
                continue;
            }
        } else {
            int res = sysctlbyname(queries[i].query, queries[i].data, &(queries[i].length), NULL, 0);
            if (res == 0) {
                print_data(queries[i].format, (void *) queries[i].data, queries[i].type);
            } else {
                continue;
            }
        }
    }

    return 0;
}
