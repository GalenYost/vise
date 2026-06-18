Vise - fast, embeddable, statically typed, compiled programming language

## General info
- Name: Vise; No reason, just sounds cool
- File extension: .vz - Shortened version of Vise
- Syntax: mostly C; C is really simple, yet powerful, nothing is hidden from you, nor abstracted away

## Plans on improving C
- No header files, they are meaningless
- No static keyword, its highly overloaded
- Decorators - built-in systax allowing to extend functionality and meaning of anything you want
- OOP - crucial mistake of languages like C++ or Java, if you want it - you keep it simple
- defer - the dream of C programmers, keyword that simplifies code readance while keeping its idea the same
- Type aliasing - allows creating standalone types, this will help with keeping development of complex architectures by allowing you to customize types to read
- Custom allocations - you will have a simplified version of malloc, that works the same, but provides better experience such as additional optional args for allocation behavior
- No macros - (at least for now), they are powerful, thats true, but they abstract away too much and when used wrong - might confuse and slow down developers, instead you will have much stricter and focused on their own task decorators
- export - since there are no header files you get a separate keyword to mark expression as exported, allowing you to reuse them
- import - since there are no macros you get a separate keyword to include modules somewhere
- Build scripts/recipies - i truly believe build system should not be complex and should not involve anything else rather than language compiler itself
- null as a type - this will allow full compile-time type-safety
- comptime - things that can be evaluated during compilation process can be marked as comptime to explicitly tell the compiler that this can be done during compilation process, this way you can make functions or expression being reduced in the final binary
- Generics - C lacks code generation for dynamic types, which means you have to use its weak type system to implicitly cast void* down to something you need, which isnt safe at all
- std will just be a huge collection of modules, so instead of importing stdio or stdlib you import "std.math" and so on (or even import "std" and use std.math.floor)

### Clarifications on OOP
OOP should be simple, it shouldnt complicate things, especially taking the fact that OOP is not necessary, so in Vise theres no inheritance, no private fields, just pure structs that contain fields and functions.
If you want to inherit something - you can attach decorator to a struct, this way you can have a struct that can be used as sort of a "template" for your future structs that will use that exact decorator as a strict path to follow

### Clarifications on build scripts
There are rust and zig as good examples of how build scripts should work, but in my opinion they are a bit more complicated, it feels like learning yet another part of the language.
Vise build system will allow you to simply define what you want and when you wont from a compiler
