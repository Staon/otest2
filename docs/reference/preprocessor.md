---
title: OTest2 Preprocessor
permalink: /reference/preprocessor/
---

## Running of the Preprocessor

```plaintext
Usage: otest2 -s srcfile [-o outfile] [-d domain] [-- compiler options...]

  -h          --help             Print this help message.
  -s infile   --srcfile=infile   Name of the input file.
  -o outfile  --outfile=outfile  Name of the output file. By default, the name
                                 is derived from the input filename by adding
                                 .cpp suffix.
  -d domain   --domain=domain    Name of the test domain. The default value is
                                 'default'.

  compiler options...            Compiler flags passed into the clang tool.
                                 The main usage is passing additive include
                                 directories -Idir. Don't forget to precede
                                 -- to the compiler options.
```
