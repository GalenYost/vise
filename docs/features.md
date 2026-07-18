# Vise Language Features

### Clarifications on OOP
OOP should be simple, it shouldnt complicate things, especially taking the fact that OOP is not necessary, so in Vise theres no inheritance, no private fields, just pure structs that contain fields and functions.
If you want to inherit something - you can attach decorator to a struct, this way you can have a struct that can be used as sort of a "template" for your future structs that will use that exact decorator as a strict path to follow

### Clarifications on build scripts
There are rust and zig as good examples of how build scripts should work, but in my opinion they are a bit more complicated, it feels like learning yet another part of the language.
Vise build system will allow you to simply define what you want from a compiler

### Clarifications on std library
Language should be more or less self-explorable, sure documentation can be a way to solve the problem, but thats not really convenient, far better is when you can simply try out different words in order to try hit the nail on the head.
So basically, instead of having whole std in different files like in C/C++, as well as having huge different complicated long names like in Java, Vise will provide more or less simple and easy to figure out names.

#### A few examples:
1. Hashmap - a high-efficiency, commonly used container to store complex data
```cpp
#include <unordered_map>

int main(void) {
    std::unordered_map<string, string> names;
}
```
Not really self-explorable and understandable without a documentation if you ask me

```java
import java.util.HashMap;

public class Main {
    public static void main(String[] args) {
        HashMap<String, String> map = new HashMap<>();
    }
}
```
That's better, but there is still room for improvements, Zig is actually a perfect example, but it has different allocators and so self-exploration is still being dropped dramatically:
Rust is also a nice example:
```rs
use std::collections::HashMap;

fn main() {
    let map: HashMap<&str, &str> = HashMap::new();
}
```
Nice and simple.
But Vise takes C as a simplicity standard, so we can do even better:
```vz
import std.collections.HashMap;

int main(void) {
    HashMap<char*, char*> map = HashMap.new();
}
```
So basically it will combine simplicity of C, "more modern simplicity" of Java and self-explanatory names of Rust

### Clarifications on decorators
Basically decorators are macros in a way, but they operate on nodes, not strings, and the type checker runs downstream of
them, this is exactly perfect to enforce strict rules, while maintaining flexibility of concept as it is.

A nice example of decorators is Zig, but Zig has them built-in using the compile-time metaprogramming.
Vise uses similar approach, but allows creating custom decorators that will allow better flexibility.
A few examples:
```vz
import std.tag; // existing in stdlib decorator
import std.io.print;

const Extern = @tag("Extern");
const Flag = @tag("Flag");

// @Extern can be used for single tag
// or
// @[Extern, Flag] for multiple tags
// no one stops you from doing @[Extern] tho
void print_hello() {
    print("Hello, world!");
}

int main(void) {
    return 0;
}
```
This is perfect system for tagging functions, this will help with embeddability (such as game engines for example) and custom marking of functions, structs or even expressions

How custom decorators will work? Basically they will utilize comptime feature, you add comptime to function which returns what you need.
Small example:
```vz
import std.meta.StructDecl;

comptime StructDecl Serializable(StructDecl node) {
    node.inject_method("
        char* to_json() {}
    ");
    node.inject_fields("
        static int static_var1;
        int var2;
    "); // multiple fields at once
    node.inject_field("static bool static_var3"); // one field
    return node;
}

@Serializable
struct Player {
    int health;
    char* name;

    Player new(int health, char* name) {
        return {
            .health = health,
            .name = name,
        };
    }
};

int main(void) {
    Player p = Player.new(100, "Player1");

    char* json = p.to_json();
}
```
Another example with functions:
```vz
import std.meta.FuncDecl;

comptime FuncDecl Route(FuncDecl node, char* path) {
    // create a registry for web framework to use later
    return node;
}

@Route("/api/users/get")
char*[] get_users() {
    return [];
}
```
Those examples are planned to exist in stdlib, but
hope you get the idea, they work similar to Rust macros and Python decorators, they can extend functionality, but dont create a chaos by enforcing strict AST rules.

### Clarifications on visibility (public/private)
By default all of the expressions and functions are private, no need to mark them static (unless needed to preserve memory address).
Instead you will have an std library decorators: Export and ExportAll. At first glance it looks like a crutch, but it actually is perfectly valid to solve the visibility problem.
ExportAll is module-wide decorator, such decorators apply to the whole file where they are being used, can be used in case of creating big type / constants declaration modules.
Single Export as opposed can be used if you know you need this constant of function to be visible and usable from outside.
Example:
```vz
// state.vz
import std.ExportAll;
@[ExportAll]

static const int DEFAULT_THREADS = 12; // this is not exported, Export decorator simply ignores static variables and functions

// this struct is exported
struct AppState {
    static bool is_running = false;
    static int threads = DEFAULT_THREADS; // this can be changed, but DEFAULT_THREADS is always the same
};

static void private_helper() {} // this function stays private

void init() {} // this function is exported and can be accessed via state.init();
// can be moved directly to AppState tho, but the idea does not change
```
Basic idea is:
1. you can ExportAll if you know you will have to export a lot of things, but you can still mark something static to prevent its export
2. you can Export idividual parts, this adds some readability complexity, but not really a big issue and you wont have to mark everything else as static
