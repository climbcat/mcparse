# mcparse

Custom parser for mcstas .instr and .comp files with great error messages.

## build & run

First run the script lib/getdeps.sh which will download a required helper library.

Next, run build.sh, which will output the 'mcparse' tool. Execute this tool for further instructions.
A component library with instrument examples is located in the mcstas-comps folder.

(Some of the files are slighty modified, see below.)

## mcstas-comps

Component- and instrument files were copied verbatim from a version of the McCode main repository (see mcstas.org).

Some component files were omited, and some modified. There are still hundreds of files with a very wide range of .comp and
.inst examples, and they can be considered a "pretty large" test set.

Edits to component files:
- don't allow any DEFINITION_PARAMETERS ()
- no arithmetic in the parameter default values - only verifiable types
- no use of '[]' in a parameter; parameter names must be valid identifiers

