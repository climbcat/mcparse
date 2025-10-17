
Test output for stub instrument files in 'test/fail_tests/':

Tests mimic typical typing mistakes chosen to trigger various parse errors.

<pre>
./build.sh
./mcparse --instrs test/fail_tests/test_01.instr
</pre>


### 01 - Missing start paranthesis after instrument name

01-mcstas:

<pre>
ERROR: syntax error at line 5.
McStas: 1 Errors encountered during parse of test_01.instr.
</pre>

01-mcparse:

<pre>
ERROR: Expected '(', got 'identifier'

5,26| DEFINE INSTRUMENT PSI_DMC lambda=2.5666, R=0.87, R_curve=0.87, string filename="Na2Ca3Al2F14.laz")
                                ^
</pre>


### 02 - Missing comma in INSTRUMENT parameter list

02-mcstas:

It passes, and the output file 'test_02.c' compiles with 'gcc test_02.c -lm'.

Inspecting test_02.c reveals that the parameter R is not being generated. In this case, the
output compiles, but running the binary yields an error:

<pre>
Error: Invalid value '2.5666 R = 0.87' for floating point parameter lambda (mcparmerror_double)
       Change lambda default value in instrument definition.
</pre>

The sequence '2.5666 R=0.87' (missing a comma as compared with "2.5666, R=0.87') is not a
valid "arithmetic expression", and thus not a valid default value.
The mcparse tool is able to parse the default value correctly, and detect and display the error:

02-mcparse:

<pre>
ERROR: Expected ',' or ')', got 'identifier'

5,40| DEFINE INSTRUMENT PSI_DMC(lambda=2.5666 R=0.87, R_curve=0.87, string filename="Na2Ca3Al2F14.laz")
                                              ^
</pre>

### 03 - Miss-spelled DECLARE as DECLARES

03-mcstas:

<pre>
ERROR: syntax error at line 12.
McStas: 1 Errors encountered during parse of test_03.instr.
</pre>

03-mcparse:

<pre>
ERROR: Expected 'DECLARE', 'INITIALIZE, 'TRACE' or 'DEPENDENCY got 'DECLARES'

12,0| DECLARES
      ^
</pre>


### 04 - INITIALIZE section before DECLARE section

04-mcstas:

<pre>
ERROR: syntax error at line 19.
McStas: 1 Errors encountered during parse of test_04.instr.
</pre>

04-mcparse:

<pre>
ERROR: Expected 'TRACE', got 'DECLARE'

19,0| DECLARE 
      ^
</pre>


### 05 - FINALLY section before TRACE

05-mcstas:

<pre>
ERROR: syntax error at line 26.
McStas: 1 Errors encountered during parse of test_05.instr.
</pre>

05-mcparse:

<pre>
ERROR: Expected 'TRACE', got 'FINALLY'

26,0| FINALLY
      ^
</pre>


### 06 - Missing default value after '=' when using COMPONENT

06-mcstas:

<pre>
ERROR: syntax error at line 13.
McStas: 1 Errors encountered during parse of test_06.instr.
</pre>

06-mcparse:

<pre>
ERROR: Expected arithmetic expression, got ','

13,27|         yheight = /*0.156*/, xwidth = 0.126,
                                  ^
</pre>






