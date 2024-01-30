# Huffman Exercise
This repository contains a working Huffman file compressor and decompressor, following the [Huffman exercise](https://www.smu.edu/-/media/Site/guildhallOLD/Documents/Huffman_Exercise.pdf).

## How to use
To use this repository,
- First, compile and link the files by using the Makefile (that is, run the command `make` while in the top-level directory of this repository). This will create the executable file `huffman`.
- Then, execute the program by running `./huffman` and passing in arguments based on whether you want to compress, decompress, or test a file:
```
USAGE: huffman -<c|d|t> [-v] <infile> [outfile]
    -c : compress infile, output to outfile (or stdout if not given)
    -d : decompress infile, output to outfile (or stdout if not given)
    -t : compress infile then decompress the compressed contents, 
         to test if it matches with original file; outfile is ignored
    -v : verbose; print additional (de)compression information for debug
```
- It is mandatory to pass in one of `-c` (to compress), `-d` (to decompress), or `-t` (to test), as well as an input filename (`infile`) into `huffman`. Verbose mode (`-v`) and the output file (`outfile`) are optional.

## Environment
- C++ 17 was the version used for the code for this exercise.
- `g++` (GCC) version 11.4.1 was the compiler used.
- GNU Make version 4.3, built for `x86_64-redhat-linux-gnu`, was used for managing file compilation/linking (the Makefile).
- Programming was done on a Linux (RockyLinux) environment.

## Repository Layout
- `uncompressed_data/`: various uncompressed files used for testing
- `Bits.h`: classes/methods that concern the representation of characters as (compressed) bits, and the writing of sequences of these bit sequences into byte strings
- `CompressedReader.h`: functions that concern the reading of compressed file data, and the outputting into decompressed representations
- `CompressedWriter.h`: structs/functions that concern the representation of compressed file data
- `TreeNode.h`: classes/methods concerning the mapping of individual characters to compressed bit sequences
- `UncompressedReader.h`: functions that concern the reading of uncompressed file data, and the outputting into various representations of that data
- `huffman.cpp`: `main` is located here; does the execution of compressing and decompressing
