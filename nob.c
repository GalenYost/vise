#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build"
#define EXEC_NAME "vise"
#define COMPILER_NAME "Vise"

int ends_with(const char *str, const char *suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (str_len < suffix_len) return 0;
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

void collect_sources(Nob_Cmd *cmd, const char *dir_path) {
    Nob_File_Paths children = {0};
    if (!nob_read_entire_dir(dir_path, &children)) return;

    for (size_t i = 0; i < children.count; ++i) {
        const char *name = children.items[i];
        if (strcmp(name, ".") == 0) continue;
        if (strcmp(name, "..") == 0) continue;

        const char *path = nob_temp_sprintf("%s/%s", dir_path, name);

        if (nob_get_file_type(path) == NOB_FILE_DIRECTORY) {
            collect_sources(cmd, path);
        } else {
            if (ends_with(name, ".c")) {
                nob_cmd_append(cmd, path);
            }
        }
    }
}

typedef struct {
    const char *name;
    const char *signature;
    const char *description;
} Command;

typedef struct {
    Command *items;
    size_t count;
    size_t capacity;
    bool picked;
} Commands;

bool command(const char *arg, Commands *commands, const char *name, const char *signature, const char *description) {
    if (commands->picked) return false;
    
    Command cmd = { .name = name, .signature = signature, .description = description };
    nob_da_append(commands, cmd);
    
    if (strcmp(arg, name) == 0) {
        commands->picked = true;
        return true;
    }
    return false;
}

void print_help(Commands commands) {
    nob_log(INFO, "Usage: ./nob <command>");
    nob_log(INFO, "Commands:");
    for (size_t i = 0; i < commands.count; ++i) {
        nob_log(INFO, "    %-10s %-15s - %s", 
            commands.items[i].name, 
            commands.items[i].signature, 
            commands.items[i].description);
    }
}

bool build_compiler() {
    nob_log(INFO, "--- Building %s Compiler ---", COMPILER_NAME);
    
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return false;

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "cc");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-O3", "-g", "-I.");
    nob_cmd_append(&cmd, "-o", nob_temp_sprintf("%s/%s", BUILD_FOLDER, EXEC_NAME));

    collect_sources(&cmd, "src");
    
    return nob_cmd_run(&cmd);
}

bool run_compiler(int argc, char **argv) {
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, nob_temp_sprintf("%s/%s", BUILD_FOLDER, EXEC_NAME));
    
    for (int i = 0; i < argc; ++i) nob_cmd_append(&cmd, argv[i]);
    
    return nob_cmd_run(&cmd);
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *program = nob_shift_args(&argc, &argv);
    const char *subcmd = (argc > 0) ? nob_shift_args(&argc, &argv) : "help";

    Commands commands = {0};

    if (command(
        subcmd,
        &commands,
        "build", "",
        nob_temp_sprintf("Compile the %s compiler", COMPILER_NAME))
    )
    {
        if (!build_compiler()) return 1;
        return 0;
    }

    if (command(subcmd, &commands, "run", "[args...]", "Build and run the compiler")) {
        if (!build_compiler()) return 1;
        if (!run_compiler(argc, argv)) return 1;
        return 0;
    }
    
    if (command(subcmd, &commands, "test", "", "Run test suite")) {
        nob_log(INFO, "TODO: Implement test runner");
        return 0;
    }

    if (command(subcmd, &commands, "help", "", "Print this message")) {
        print_help(commands);
        return 0;
    }

    print_help(commands);
    nob_log(ERROR, "Unknown command: %s", subcmd);
    return 1;
}
