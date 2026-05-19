#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static FILE *global_log_file = NULL;

__attribute__((constructor))
static void __mypass_log_init(void) { // имена с "__" дабы соответсвовать стилю функций внутренней инфраструктуры // FIXME[flops]: It really makes sense, but stdlib reserves names with `__` prefix, so you are not supposed to use it. You can make `__` postfix instead to mark those funcs as your internal
    global_log_file = fopen("runtime_log.txt", "w");
    if (!global_log_file) {
        perror("Error: failed to open \"runtime_log.txt\".");
        return;
    }
}

__attribute__((destructor))
static void __mypass_log_finish(void) {
    if (global_log_file) {
        fclose(global_log_file);
        global_log_file = NULL;
    }
}

void __mypass_log_i32(int id, int32_t value) {
    if (global_log_file) {
        fprintf(global_log_file, "%d %d\n", id, (int)value);
    }
}

void __mypass_log_i64(int id, int64_t value) {
    if (global_log_file) {
        fprintf(global_log_file, "%d %lld\n", id, (long long)value);
    }
}
