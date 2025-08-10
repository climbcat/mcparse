# mcparse

Custom parser for the mcstas McStas/McXtrace DSL: A low-dependency .instr and .comp
file parser with improved usability and maintainability.

(See mccode.org.)

## build & run

Download this repository, and execute the following commands: 

<pre>
./lib/getdeps.sh
./build.sh
./mcparse --help
</pre>

Run the tool by build doing, e.g.: 

<pre>
./mcparse mcstas-comps/
</pre>

Currently targetting Linux only.


### Parsing

Improvements as compared to the default parser:

- Prints concise error messages, allowing users to quickly pin-point the location and nature of any DSL usage error.
- Much reduced library dependencies; And no parser generator pre-build step, or abstract grammar-rule lex/yacc configuration.
- Modify or augment the parser by reading and modifying concise source code.
- Build & debug light-weight and readable parser code in seconds.

This repository includes a set of ~ 350 components and ~ 150 instruments as a test set.
Some of these were slightly modified from their original state, in order to omit certain
legacy DSL features. Yet, it implements most of the DSL in terms of parsing.

Notable omisions are function pointers in struct definitions, and the JUMP keyword.

The mcparse parser also improves on parsing in some respects, for example, it parses code 
in DECLARE sections as though they were C struct members.

### Code Generation

In addition to parsing, this project includes code generation, which outputs C++
function and type wrapper for components.

The current generator would instead output one .h file for each component, plus a single 
component meta file, and an instrument configuration .h file for each instrument.

In combination With the <code>simlib.h</code> and <code>simcore.h</code> source files
(given elsewhere), that include most of the mcstas runtime simulation code and shared 
simulation libraries, this allows for a much more accessible debugging- and development
environment.

- Wrap component code in a C++ context.
- Development access to .comp and .instr code in an easilly debug-able runtime context.

This project is part of an exploratory project to re-image the mcstas core, and a stand-alone
demonstration of the parser (and code-generation) section of that idea.

### Building high-level functionality

The generated components meta file provides access to all of the parsed components 
through a unified API: Thus it becomes possible to build high-level operations, 
e.g. simulation strategies or user interface, directly on top of the generated 
code.

For those who are familiar, this includes mcdisplay-like functionality and visual,
live simulation execution and monitoring.

It also generates configuration code for instruments, which sets up the component geometry,
custom instrument initialization and EXTEND code, and initializes everything into a
simulation-ready state.

This is of course exactly what the mcstas-generated code does, but in a less accessible
format: As a comparison, the mcstas-generator easily output 10000
lines of hard-to-read generated code from your average, garden-variety non-trivial
neutron- or x-ray instrument.
