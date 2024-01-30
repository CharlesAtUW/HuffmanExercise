#ifndef _COMPRESSEDWRITER_H_
#define _COMPRESSEDWRITER_H_

#include <cstddef>
#include <string>
#include "TreeNode.h"

namespace huffman {

#define PARENT_CHAR '\0'
#define MAGIC_NUMBER 0xcafef00d

// This struct represents how the tree mapping bits to bytes is represented in the compressed file.
struct TreeFileRepr {
    int16_t num_nodes;
    int16_t special_leaf_location;
    std::string tree_data;

    // Returns the number of bytes that the metadata of a TreeFileRepr takes up in ToBytes().
    static size_t MetadataSize() { return sizeof(num_nodes) + sizeof(special_leaf_location); }

    // Returns what the bytes of this TreeFileRepr will be in the compressed file.
    std::string ToBytes() const;
};

// Constructs a TreeFileRepr that represents the given tree. 
TreeFileRepr TreeToFileRepr(const TreeNode &root);

// This struct represents how the compressed data (not including the tree or header)
// is represented in the compressed file.
struct CompressedFileRepr {
    uint64_t num_bits;
    std::string compressed_bits;

    // Returns the number of bytes that the metadata of a CompressedFileRepr takes up in ToBytes().
    static size_t MetadataSize() { return sizeof(num_bits); }

    // Returns what the bytes of this CompressedFileRepr will be in the compressed file.
    std::string ToBytes() const;
};

// Constructs a CompressedFileRepr based on the given uncompressed data (file_to_bytes)
// and a mapping of different bytes to bit sequences.
CompressedFileRepr CompressFileBytes(
    const std::unordered_map<unsigned char, std::unique_ptr<Bits>> &char_to_bits,
    const std::string &file_bytes);

// This struct represents how the file header is represented in the compressed file.
struct FileHeader {
    uint32_t magic_number;
    uint32_t checksum;
    uint64_t content_length;

    // Returns the number of bytes that the metadata of a FileHeader takes up in ToBytes().
    static size_t MetadataSize() { return sizeof(FileHeader); }

    // Returns what the bytes of this FileHeader will be in the compressed file.
    std::string ToBytes() const;
};

// Calculates a (simple) checksum based on the contents of the given string.
uint32_t ComputeChecksum(const std::string &data);

// Creates and returns the contents of the compressed file that's based on tree_data and file_data.
std::string BuildFile(const TreeFileRepr &tree_data, CompressedFileRepr &file_data);

}  // namespace huffman

#endif  // _COMPRESSEDWRITER_H_
