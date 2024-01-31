# Huffman Exercise
This repository contains a working Huffman file compressor and decompressor, following the [Huffman exercise](https://www.smu.edu/-/media/Site/guildhallOLD/Documents/Huffman_Exercise.pdf).

This file compressor works by mapping more common bytes in the file to shorter bit sequences (i.e., less than 8 bits). (This means that less common bytes might be mapped to a bit sequence greater than 8 bits, but since they're less common, we overall get a smaller compressed file size.)

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
- It is mandatory to pass in one of `-c` (to compress), `-d` (to decompress), or `-t` (to test) into `huffman`. It is also mandatory to pass in an input filename (`infile`). Verbose mode (`-v`) and the output file (`outfile`) are optional.

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

## Compressed file layout
Here is the layout of compressed files produced by this repository. Documentation on each field can be found in `CompressedWriter.h` under their respective structs. Note that any `std::string` fields in the structs are represented as the raw bytes of the string data in the actual file.
```
(start of file; start of FileHeader region)
+-----------------------------------------------+
|   magic_number (4 bytes)                      |
+-----------------------------------------------+
|   checksum (4 bytes)                          |
+-----------------------------------------------+
|   content_length (8 bytes)                    |
+-----------------------------------------------+
(start of TreeFileRepr region)
+-----------------------------------------------+ 
|   num_nodes (2 bytes)                         |
+-----------------------------------------------+
|   special_leaf_index (2 bytes)                |
+-----------------------------------------------+
|   tree_data (num_nodes bytes)                 |
+-----------------------------------------------+
(start of CompressedFileRepr region)
+-----------------------------------------------+
|   num_bits (8 bytes)                          |
+-----------------------------------------------+
|   compressed_bits (ceil(num_bits / 8.) bytes) |
+-----------------------------------------------+
```
The only exception to this is the compression of an empty file. A compressed empty file is instead another empty file.
