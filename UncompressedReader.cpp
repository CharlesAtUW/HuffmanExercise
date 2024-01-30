#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <queue>
#include "TreeNode.h"
#include "UncompressedReader.h"

namespace huffman {

typedef std::unique_ptr<huffman::TreeNode> NodePtr;

template <class T> class UniquePtrReverseComparer {
 public:
    bool operator()(std::unique_ptr<T> const &lhs, std::unique_ptr<T> const &rhs) {
        return *lhs > *rhs;
    }
};

bool ReadFileContents(const std::string &file_name, std::string &file_bytes) {
    std::ifstream byte_reader(file_name, std::ifstream::in | std::ifstream::binary);
    if (!byte_reader) {
        std::cerr << "Invalid file" << std::endl;
        return false;
    }

    std::stringstream file_bytes_builder;
    unsigned char file_byte = byte_reader.get();
    while (byte_reader.good()) {
        file_bytes_builder << file_byte;
        file_byte = byte_reader.get();
    }
    byte_reader.close();

    file_bytes = file_bytes_builder.str();

    return true;
}

std::unordered_map<unsigned char, int> GetByteFrequencies(const std::string &content) {
    std::unordered_map<unsigned char, int> byte_to_frequency;
    for (const unsigned char c : content) {
        byte_to_frequency[c] += 1;
    }

    return byte_to_frequency;
}

NodePtr CreateTree(const std::unordered_map<unsigned char, int>& byte_to_frequency) {
    std::priority_queue<
        NodePtr, std::vector<NodePtr>, UniquePtrReverseComparer<huffman::TreeNode>> sorter;
    for (const auto &kv : byte_to_frequency) {
        sorter.emplace(std::make_unique<huffman::TreeNode>(huffman::TreeNode(kv.first, kv.second)));
    }

    while (sorter.size() != 1) {
        // Have to use const_cast since we can't move (not copy) an element out of a priority_queue:
        // https://stackoverflow.com/questions/20149471/move-out-element-of-std-priority-queue-in-c11
        NodePtr lowest = std::move(const_cast<NodePtr&>(sorter.top()));
        sorter.pop();
        NodePtr second_lowest = std::move(const_cast<NodePtr&>(sorter.top()));
        sorter.pop();

        sorter.emplace(std::make_unique<huffman::TreeNode>(
            huffman::TreeNode(std::move(lowest), std::move(second_lowest))));
    }

    NodePtr root = std::move(const_cast<NodePtr&>(sorter.top()));
    sorter.pop();

    return root;
}

}  // namespace huffman
