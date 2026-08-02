# Build System

The build system is based on [nob.h](https://github.com/tsoding/nob.h) by Tsoding. The build recipe is found in `nob.c` in the project root, and the Makefile builds with one simple command.

## Constants

All constants are located in the root `config.h` file:
- `BUILD_FOLDER` is created and used to store build / runtime logs, as well as final executable
- `EXEC_NAME` is used to create an executable
- `COMPILER_NAME` is basically a part of output, kind of an "official" name you could say
- `ENABLE_LOGS` toggles the logging system on (1) / off (0)
- `LOG_FILE_PATH` is the path where runtime logs get written

## Structs

- `Command` - single command, has a `name`, `signature` and `description`, used as an instance to describe compiler commands for visual output for help message
- `Commands` - dynamic array of `Command` structs, nothing else to say

## Functions

Functions are self-explanatory, but still:
- `ends_with` - returns a result of `strcmp` on string, to match its suffix
- `collect_sources` - appends all of the matching files (ends_with .c) to Nob_Cmd instance
- `command` - creates a `Command` instance at runtime
- `print_help` - takes a `Commands` array and displays all of its children
- `build_compiler` - executes a `cc` command with collected sources into a `BUILD_FOLDER`/`EXEC_NAME` path
- `run_compiler` - executes the compiler binary
- `main` - entry point, defines available compiler commands
