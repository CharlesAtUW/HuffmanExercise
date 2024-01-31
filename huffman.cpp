#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <unordered_map>
#include "TreeNode.h"
#include "Bits.h"
#include "UncompressedReader.h"
#include "CompressedWriter.h"
#include "CompressedReader.h"

#define COMPRESS 0
#define DECOMPRESS 1
#define TEST 2

#define STDOUT_FILENAME "\0"

void parse_args(int argc, char **argv, int &mode, bool &verbose,
    std::string &input_filename, std::string &output_filename);
void usage();

void print_characters_information(
    const std::unordered_map<unsigned char, int> &byte_to_frequency,
    const std::unordered_map<unsigned char, std::unique_ptr<huffman::Bits>> &char_to_bits);
void print_character_tree(const huffman::TreeNode &root);
void print_compressed_data_info(const huffman::TreeFileRepr &tree_data,
    const huffman::CompressedFileRepr &file_data,
    const std::string &compressed_file);

std::string compress_file_content(const std::string &file_bytes, const bool verbose);
std::string decompress_file_content(const std::string &file_bytes, const bool verbose);
bool test_compression_decompression(const std::string &file_bytes, const bool verbose);

int main(int argc, char **argv) {
    int mode;
    bool verbose;
    std::string input_filename;
    std::string output_filename;
    parse_args(argc, argv, mode, verbose, input_filename, output_filename);

    std::string file_bytes;
    if (!huffman::ReadFileContents(input_filename, file_bytes)) {
        return EXIT_FAILURE;
    }

    std::string output_file_data;
    if (mode == COMPRESS) {
        output_file_data = compress_file_content(file_bytes, verbose);
    } else if (mode == DECOMPRESS) {
        output_file_data = decompress_file_content(file_bytes, verbose);
    } else if (mode == TEST) {
        return test_compression_decompression(file_bytes, verbose)
            ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    if (output_filename.compare(STDOUT_FILENAME)) {
        std::ofstream outstream(output_filename, std::ofstream::out | std::ofstream::binary);
        outstream.write(output_file_data.data(), output_file_data.size());
        outstream.close();
    } else {
        std::cout.write(output_file_data.data(), output_file_data.size());
    }

    return EXIT_SUCCESS;
}

void parse_args(int argc, char **argv, int &mode, bool &verbose,
    std::string &input_filename, std::string &output_filename) {
    if (argc != 3 && argc != 4 && argc != 5) {
        usage();
    }

    std::string mode_str(argv[1]);
    if (!mode_str.compare("-c") || !mode_str.compare("-C")) {
        mode = COMPRESS;
    } else if (!mode_str.compare("-d") || !mode_str.compare("-D")) {
        mode = DECOMPRESS;
    } else if (!mode_str.compare("-t") || !mode_str.compare("-T")) {
        mode = TEST;
    } else {
        usage();
    }

    std::string verbose_str(argv[2]);
    int input_index = 2;
    verbose = !verbose_str.compare("-v") || !verbose_str.compare("-V");
    if (verbose) {
        input_index++;
    }

    input_filename = argv[input_index];
    output_filename = (input_index + 1 == argc) ? STDOUT_FILENAME : argv[input_index + 1];
}

void usage() {
    std::cerr << "USAGE: huffman -<c|d|t> [-v] <infile> [outfile]" << std::endl
        << "    -c : compress infile, output to outfile (or stdout if not given)" << std::endl
        << "    -d : decompress infile, output to outfile (or stdout if not given)" << std::endl
        << "    -t : compress infile then decompress the compressed contents, " << std::endl
        << "         to test if it matches with original file; outfile is ignored" << std::endl
        << "    -v : verbose; print additional (de)compression information for debug" << std::endl;
    exit(EXIT_FAILURE);
}

void print_character_tree(const huffman::TreeNode &root) {
    std::cout << "Character tree:" << std::endl
        << huffman::TreeContentsRepr(root) << std::endl;
}

void print_characters_information(
    const std::unordered_map<unsigned char, int> &byte_to_frequency,
    const std::unordered_map<unsigned char, std::unique_ptr<huffman::Bits>> &char_to_bits) {
    std::cout << "Character information" << std::endl;
    for (const auto &kv : char_to_bits) {
        std::cout << "char: '" << kv.first
            << "', bits: " << *kv.second
            << ", frequency: " << byte_to_frequency.at(kv.first) <<  std::endl;
    }
}

void print_compressed_data_info(const huffman::TreeFileRepr &tree_data,
    const huffman::CompressedFileRepr &file_data,
    const std::string &compressed_file) {
    std::cout << "Num tree nodes: " << tree_data.num_nodes << std::endl
        << "Special leaf location: " << tree_data.special_leaf_index << std::endl
        << "Tree data size (bytes): " << tree_data.tree_data.size() << std::endl
        << "All TreeFileRepr size (bytes): " << tree_data.ToBytes().size() << std::endl
        << "Number of bits in compressed content: " << file_data.num_bits << std::endl
        << "Compressed content size (bytes): " << file_data.compressed_bits.size() << std::endl
        << "All CompressedFileRepr size (bytes): " << file_data.ToBytes().size() << std::endl
        << "Total compressed file size: " << compressed_file.size() << std::endl;
}

std::string compress_file_content(const std::string &file_bytes, const bool verbose) {
    if (file_bytes.empty()) {
        if (verbose) {
            std::cout << "Compressing an empty file!";
        }
        return "";
    }

    // creating compressed representations
    std::unordered_map<unsigned char, int> byte_to_frequency
        = huffman::GetByteFrequencies(file_bytes);
    std::unique_ptr<huffman::TreeNode> root = huffman::CreateTree(byte_to_frequency);
    std::unordered_map<unsigned char, std::unique_ptr<huffman::Bits>> char_to_bits
        = TreeCharToBits(*root);

    // making bytes for compressed file
    huffman::TreeFileRepr tree_data = TreeToFileRepr(*root);
    huffman::CompressedFileRepr file_data = CompressFileBytes(char_to_bits, file_bytes);
    std::string compressed_file = BuildFile(tree_data, file_data);

    if (verbose) {
        std::cout << "File compression info:" << std::endl;
        print_character_tree(*root);
        print_characters_information(byte_to_frequency, char_to_bits);
        print_compressed_data_info(tree_data, file_data, compressed_file);
    }

    return compressed_file;
}

std::string decompress_file_content(const std::string &file_bytes, const bool verbose) {
    if (file_bytes.empty()) {
        if (verbose) {
            std::cout << "Decompressing an empty file!";
        }
        return "";
    }

    // separate compressed file into respective sections
    huffman::TreeFileRepr tree_data;
    huffman::CompressedFileRepr file_data;
    if (!PartitionFileContents(file_bytes, tree_data, file_data)) {
        exit(EXIT_FAILURE);
    }

    std::unique_ptr<huffman::TreeNode> root = TreeReprToTree(tree_data);

    if (verbose) {
        std::cout << "File decompression info:" << std::endl;
        print_character_tree(*root);
        print_compressed_data_info(tree_data, file_data, file_bytes);
    }

    return DecompressFile(*root, file_data);
}

bool test_compression_decompression(const std::string &file_bytes, const bool verbose) {
    std::string compressed_file_data = compress_file_content(file_bytes, verbose);
    std::string decompressed_file_data = decompress_file_content(compressed_file_data, verbose);
    if (!file_bytes.compare(decompressed_file_data)) {
        std::cout << "Test passed! Compressed-then-decompressed file is the same!" << std::endl;
        return true;
    } else {
        std::cerr << "Test failed! Compressed-then-decompressed file is not the same!"
            << std::endl << decompressed_file_data << std::endl;
        return false;
    }
}
