#ifndef _COMPRESSED_WRITER_H_
#define _COMPRESSED_WRITER_H_

#include <cstddef>
#include <string>
#include "TreeNode.h"

namespace huffman {

#define PARENT_CHAR '\0'
#define MAGIC_NUMBER 0xcafef00d

struct TreeFileRepr {
    int16_t num_nodes;
    int16_t special_leaf_location;
    std::string tree_data;

    size_t MetadataSize() const { return sizeof(num_nodes) + sizeof(special_leaf_location); }

    std::string ToString() const;
};

TreeFileRepr TreeToFileRepr(const TreeNode &root);

struct CompressedFileRepr {
    uint64_t num_bits;
    std::string compressed_bits;

    size_t MetadataSize() const { return sizeof(num_bits); }

    std::string ToString() const;
};

CompressedFileRepr CompressFileBytes(
    const std::unordered_map<unsigned char, std::unique_ptr<Bits>> &char_to_bits,
    const std::string &file_bytes);

struct FileHeader {
    uint32_t magic_number;
    uint32_t checksum;
    uint64_t content_length;

    size_t MetadataSize() const { return sizeof(FileHeader); }

    std::string ToString() const;
};

uint32_t ComputeChecksum(const std::string &data);

std::string BuildFile(const TreeFileRepr &tree_data, CompressedFileRepr &file_data);

}  // namespace

#endif