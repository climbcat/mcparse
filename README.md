# mcparse

Custom parser for mcstas/mxtrace .instr and .comp files with great error
messages.

#### mcstas-comps

Component- and instrument files were copied verbatim from a version of 
the McCode main repository; see mcstas.org

250805:

Some components were not included in the test set for the parser, for various
reasons. There are still hundreds of files with a very wide range of .comp and
.inst examples, and I would consider it a "pretty large" test set.

250606:

Edits to component files:
- don't allow any DEFINITION_PARAMETERS ()
- no arithmetic in the parameter default values - only verifiable types
- no use of '[]' in a parameter; parameter names must be valid identifiers (see >


#### simcore

Some changes to the simulation core are noted.

There are two files: simlib.h and simcore.h. The first includes helper 
libraries, and the second the actual simulation core macros and functions.

simlib changes

mcstas macros: references to FLAVOUR_* were disabled
MPI: MPI_MASTER macro has been out-commented. Any multi-threaded use would re-im>
monitor output functions:

mcdetector_out_0D
mcdetector_out_1D
mcdetector_out_2D
mcdetector_out_list
These functions should be re-introduced from the mccode codebase.( They
belong to in mccode-r and were deleted during port of simcore.h.)

sprintf: some helper sprintf caused a warning, disabled
particle_getvar: Function should be pulled from mcstas-generated code


