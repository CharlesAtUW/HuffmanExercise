#include <cstring>
#include <string>
#include <sstream>
#include "CompressedWriter.h"

namespace huffman {

#define CHARS_PER_CHECKSUM_ELEM 8

void BuildTreeFileRepr(
    const TreeNode &current_node,
    int16_t &node_count,
    std::stringstream &current_characters,
    int16_t &special_leaf_location);

std::string TreeFileRepr::ToBytes() const {
    char number_buffer[TreeFileRepr::MetadataSize()];
    memcpy(number_buffer, &num_nodes, sizeof(num_nodes));
    memcpy(
        number_buffer + sizeof(num_nodes), &special_leaf_location, sizeof(special_leaf_location));

    return std::string(number_buffer, TreeFileRepr::MetadataSize()) + tree_data;
}

TreeFileRepr TreeToFileRepr(const TreeNode &root) {
    std::stringstream characters_builder;
    int16_t node_count = 0;
    int16_t special_leaf_location = -1;
    BuildTreeFileRepr(root, node_count, characters_builder, special_leaf_location);

    return {
        node_count,
        special_leaf_location,
        characters_builder.str()
    };
}

void BuildTreeFileRepr(
    const TreeNode &current_node,
    int16_t &node_count,
    std::stringstream &current_characters,
    int16_t &special_leaf_location) {
    if (current_node.IsLeaf()) {
        unsigned char key = current_node.GetKey();
        current_characters << key;
        if (key == PARENT_CHAR) {
            special_leaf_location = node_count;
        }
    } else {
        BuildTreeFileRepr(
            *current_node.GetLeft(), node_count, current_characters, special_leaf_location);
        BuildTreeFileRepr(
            *current_node.GetRight(), node_count, current_characters, special_leaf_location);

        current_characters << PARENT_CHAR;
    }

    node_count++;
}

std::string CompressedFileRepr::ToBytes() const {
    char number_buffer[CompressedFileRepr::MetadataSize()];
    memcpy(number_buffer, &num_bits, sizeof(num_bits));

    return std::string(number_buffer, CompressedFileRepr::MetadataSize()) + compressed_bits;
}

CompressedFileRepr CompressFileBytes(
    const std::unordered_map<unsigned char, std::unique_ptr<Bits>> &char_to_bits,
    const std::string &file_bytes) {

    huffman::BitWriter compressed_builder;
    for (const unsigned char b : file_bytes) {
        compressed_builder.AppendBits(*char_to_bits.at(b));
    }

    return {
        compressed_builder.GetTotalNumBits(),
        compressed_builder.ToBytes()
    };
}

std::string FileHeader::ToBytes() const {
    char number_buffer[FileHeader::MetadataSize()];
    memcpy(number_buffer, &magic_number, sizeof(magic_number));
    memcpy(number_buffer + sizeof(magic_number), &checksum, sizeof(checksum));
    memcpy(number_buffer + sizeof(magic_number) + sizeof(checksum),
        &content_length, sizeof(content_length));
    return std::string(number_buffer, FileHeader::MetadataSize());
}

uint32_t ComputeChecksum(const std::string &data) {
    uint32_t aggregate_hash_number = 0;
    uint32_t current_hash_number = 1;
    for (size_t i = 0; i < data.length(); i++) {
        const unsigned char c = data.at(i);
        current_hash_number = 31 * current_hash_number + c;
        if ((i + 1) % CHARS_PER_CHECKSUM_ELEM == 0) {
            aggregate_hash_number ^= current_hash_number;
            current_hash_number = 1;
        }
    }
    aggregate_hash_number ^= current_hash_number;
    return aggregate_hash_number;
}

std::string BuildFile(
    const huffman::TreeFileRepr &tree_data, huffman::CompressedFileRepr &file_data) {
    std::string file_content = tree_data.ToBytes() + file_data.ToBytes();
    uint32_t checksum = ComputeChecksum(file_content);

    FileHeader header = {
        MAGIC_NUMBER,
        checksum,
        file_content.size()
    };

    std::string compressed_file = header.ToBytes() + file_content;

    return compressed_file;
}

}  // namespace huffman
