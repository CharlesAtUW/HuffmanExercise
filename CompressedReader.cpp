#include <cstring>
#include <stack>
#include <memory>
#include <sstream>
#include "CompressedReader.h"
#include "CompressedWriter.h"

namespace huffman {

typedef std::unique_ptr<huffman::TreeNode> NodePtr;

bool PartitionHeader(
    const std::string &file_contents, std::string &partitioned_tree_and_file_data);

bool PartitionTree(
    const std::string &tree_and_data,
    TreeFileRepr &tree_data,
    std::string &partitioned_file_data);

bool ProcessFileReprData(const std::string &file_repr_data, CompressedFileRepr &file_repr);

size_t MinContentSize() {
    return FileHeader::MetadataSize() + TreeFileRepr::MetadataSize()
        + CompressedFileRepr::MetadataSize();
}

bool PartitionFileContents(
    const std::string file_contents,
    TreeFileRepr &tree_repr,
    CompressedFileRepr &file_repr) {
    std::string tree_and_file_data;
    if (!PartitionHeader(file_contents, tree_and_file_data)) {
        return false;
    }

    std::string file_repr_data;
    if (!PartitionTree(tree_and_file_data, tree_repr, file_repr_data)) {
        return false;
    }

    return ProcessFileReprData(file_repr_data, file_repr);
}

bool PartitionHeader(
    const std::string &file_contents, std::string &partitioned_tree_and_file_data) {
    if (file_contents.size() < MinContentSize()) {
        return false;
    }

    const char *contents_buffer = file_contents.data();
    FileHeader header;
    memcpy(&header.magic_number, contents_buffer, sizeof(header.magic_number));
    if (header.magic_number != MAGIC_NUMBER) {
        std::cerr << "The file's magic number doesn't match what's expected!" << std::endl;
        return false;
    }

    memcpy(
        &header.checksum,
        contents_buffer + sizeof(header.magic_number),
        sizeof(header.checksum));
    memcpy(
        &header.content_length,
        contents_buffer + sizeof(header.magic_number) + sizeof(header.checksum),
        sizeof(header.content_length));
    if (file_contents.size() - FileHeader::MetadataSize() < header.content_length) {
        std::cerr << "The file's content length field doesn't match actual length!" << std::endl;
        return false;
    }

    partitioned_tree_and_file_data
        = std::string(file_contents, sizeof(FileHeader), header.content_length);
    if (header.checksum != ComputeChecksum(partitioned_tree_and_file_data)) {
        std::cerr << "Expected checksum does not match actual checksum!" << std::endl;
        return false;
    }

    return true;
}

bool PartitionTree(
    const std::string &tree_and_file_data,
    TreeFileRepr &tree_repr,
    std::string &partitioned_file_data) {
    const char *contents_buffer = tree_and_file_data.data();

    int remaining_size = tree_and_file_data.size() - TreeFileRepr::MetadataSize();

    int16_t num_nodes;
    memcpy(&num_nodes, contents_buffer, sizeof(num_nodes));
    if (num_nodes > remaining_size) {
        std::cerr << "Number of nodes exceeds remaining file size!" << std::endl;
        return false;
    }

    int16_t special_leaf_location;
    memcpy(
        &special_leaf_location, contents_buffer + sizeof(num_nodes), sizeof(special_leaf_location));
    if (special_leaf_location >= num_nodes) {
        std::cerr << "Special leaf location is not in tree region!" << std::endl;
        return false;
    }

    tree_repr = TreeFileRepr {
        num_nodes,
        special_leaf_location,
        std::string(tree_and_file_data, TreeFileRepr::MetadataSize(), num_nodes)
    };

    partitioned_file_data
        = std::string(tree_and_file_data, TreeFileRepr::MetadataSize() + num_nodes);

    if (partitioned_file_data.size() < CompressedFileRepr::MetadataSize()) {
        std::cerr << "Remaining file not big enough for CompressedFileRepr region!" << std::endl;
        return false;
    }


    return true;
}

bool ProcessFileReprData(const std::string &file_repr_data, CompressedFileRepr &file_repr) {
    const char *contents_buffer = file_repr_data.data();
    uint64_t remaining_size = file_repr_data.size() - CompressedFileRepr::MetadataSize();

    uint64_t num_bits;
    memcpy(&num_bits, contents_buffer, sizeof(num_bits));
    uint64_t num_bytes = num_bits / BITS_PER_ELEM + static_cast<int>(num_bits % BITS_PER_ELEM != 0);
    if (num_bytes > remaining_size) {
        std::cerr << "Number of bits exceeds remaining file size!" << std::endl;
        return false;
    }

    file_repr = CompressedFileRepr {
        num_bits,
        std::string(file_repr_data, CompressedFileRepr::MetadataSize(), num_bytes)
    };

    return true;
}

NodePtr TreeReprToTree(const TreeFileRepr &tree_repr) {
    std::stack<NodePtr> tree_organizer;
    for (size_t i = 0; i < tree_repr.tree_data.size(); i++) {
        const unsigned char key = tree_repr.tree_data.at(i);
        if (key == PARENT_CHAR && (int16_t) i != tree_repr.special_leaf_location) {
            NodePtr right = std::move(tree_organizer.top());
            tree_organizer.pop();
            NodePtr left = std::move(tree_organizer.top());
            tree_organizer.pop();

            tree_organizer.emplace(std::make_unique<huffman::TreeNode>(
                std::move(left), std::move(right)));
        } else {
            tree_organizer.emplace(std::make_unique<huffman::TreeNode>(huffman::TreeNode(key, 0)));
        }
    }

    return std::move(tree_organizer.top());
}

std::string DecompressFile(const huffman::TreeNode &root, const CompressedFileRepr &file_data) {
    std::stringstream decompressed_output;
    huffman::TreeNode const *current_node = &root;

    std::string::const_iterator byte_iterator = file_data.compressed_bits.begin();
    unsigned char current_byte = *byte_iterator;
    for (uint64_t i = 0; i < file_data.num_bits; i++) {
        int bit_offset = i % BITS_PER_ELEM;
        if (i != 0 && bit_offset == 0) {
            byte_iterator++;
            current_byte = *byte_iterator;
        }

        bool bit_is_one = (current_byte >> bit_offset) & 0x1;

        current_node = (bit_is_one ? current_node->GetRight() : current_node->GetLeft()).get();
        if (current_node->IsLeaf()) {
            decompressed_output << current_node->GetKey();
            current_node = &root;
        }
    }

    return decompressed_output.str();
}

}  // namespace huffman
