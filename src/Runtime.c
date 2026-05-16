#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static FILE *global_log_file = NULL;

__attribute__((constructor))
static void __my_log_init(void) { // имена с "__" дабы соответсвовать стилю функций внутренней инфраструктуры
    global_log_file = fopen("runtime_log.txt", "w");
    if (!global_log_file) {
        perror("Error: failed to open \"runtime_log.txt\".");
        return;
    }
}

__attribute__((destructor))
static void __my_log_finish(void) {
    if (global_log_file) {
        fclose(global_log_file);
        global_log_file = NULL;
    }
}

void __my_log_i32(int id, int32_t value) {
    if (global_log_file) {
        fprintf(global_log_file, "%d %d\n", id, (int)value);
    }
}

void __my_log_i64(int id, int64_t value) {
    if (global_log_file) {
        fprintf(global_log_file, "%d %lld\n", id, (long long)value);
    }
}