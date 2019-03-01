# VAMPIRE Tests
Vampire uses a basic custom test frame work for verifying functional
correctness.

Normal VAMPIRE development flow using these test would be:
1. Make changes to source code.
2. Compile: `make debug -j4`
   1. Fix any compilation bugs and warnings
3. Run the tests: `make tests`
   1. Fix the error by studying corresponding test file if any of the
      test fails (non-zero exit code).
	  
## Tests organization
This directory has the following structure:
```
.
├── configs/
│	: Holds configuration files for different test runs
├── helper.py
│	: Contains function which allow easy execution and result comparison
├── run_tests.sh
│	: Script that handles all the tests and reports their exit codes
├── test_*.py
│	: Files for corresponding test
├── traces
│	: Contains tarballs of traces correponding to different tests
└── vampireAsciiToBin.py
	: Script to convert traces between VAMPIRE's ASCII and BIN format
```
