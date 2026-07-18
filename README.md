Vise - fast, embeddable, statically typed, compiled programming language

## General info
- Name: Vise; No reason, just sounds cool
- File extension: .vz - Shortened version of Vise
- Syntax: mostly C; C is really simple, yet powerful, nothing is hidden from you, nor abstracted away

## Plans on improving C
- No header files, they are meaningless
- Decorators - built-in syntax allowing to extend functionality and meaning of anything you want
- Avoid the complexity of OOP from languages like C++ or Java, if you want it - you keep it simple
- defer - the dream of C programmers, keyword that simplifies code readability while keeping its idea the same
- Type aliasing - allows creating standalone types, this will help with keeping development of complex architectures by allowing you to customize types to read
- Custom allocations - you will have a simplified version of malloc, that works the same, but provides better experience such as additional optional args for allocation behavior
- import - since there are no macros you get a separate keyword to include modules somewhere
- Build scripts/recipies - i truly believe build system should not be complex and should not involve anything else rather than language compiler itself
- null as a type - this will allow full compile-time type-safety
- comptime - things that can be evaluated during compilation process can be marked as comptime to explicitly tell the compiler that this can be done during compilation process, this way you can make functions or expression being reduced in the final binary
- Generics - C lacks code generation for dynamic types, which means you have to use its weak type system to implicitly cast void* down to something you need, which isnt safe at all
- std will just be a huge collection of modules, so instead of importing stdio or stdlib you import "std.math" and so on (or even import "std" and use std.math.floor)
- string interpolation - a way to make your string more readable and without a need to memorize different flags and their unique features
- Exports; static wont be used as a mark to "leave the function or global variable private to this file", but only to make variable preserve its memory across function calls / different struct instances.

## Documentation & Deep Dives

For detailed explanations of Vise's design philosophy and internal features, please check out the `docs/` folder:

*   [Language Features](docs/language_features.md): Deep dives into Decorators, Memory Allocators, Visibilty/Exports, and OOP differences.
*   [Compiler Phases](docs/compiler_phases.md): Step-by-step breakdown of how the Vise compiler works internally.
*   [Grammar](docs/grammar.md): Formal EBNF grammar definitions for the Vise language syntax.

### Naming
In Vise just like in C you can pretty much do whatever you want, no one will force you to write what is "better", but there are some suggestions that will help in development:

1. **Variables and Functions**:
   - Variables (local and global) should use `snake_case`.
   - Functions should use `snake_case` (e.g., `print_hello()`).

2. **Constants**:
   - Global constants should be named `SCREAMING_SNAKE_CASE`.
   - Local constants (if contextually small) can be named `snake_case`.

3. **Types and Structs**:
   - Structs, Enums, and Type Aliases should use `PascalCase` (e.g., `AppState`, `HashMap`, `Player`).

4. **Decorators**:
   - Built-in and custom decorators should use `PascalCase` to distinguish them from standard functions (e.g., `@ExportAll`, `@Serializable`).
