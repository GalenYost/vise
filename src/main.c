#include "utils.h"

int main(void) {
    log_to_console(FATAL, "Build failed");
    log_to_file(FATAL, "build/build.log", "Build failed");
    return 0;
}
