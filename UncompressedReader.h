#ifndef _UNCOMPRESSEDREADER_H_
#define _UNCOMPRESSEDREADER_H_

#include <string>
#include <unordered_map>
#include <memory>
#include "TreeNode.h"

namespace huffman {

// Reads the file at file_name and writes its contents to file_bytes.
// Returns whether the file read was successful.
bool ReadFileContents(const std::string &file_name, std::string &file_bytes);

// Creates a mapping of each different byte in the given string to its number of occurrences.
std::unordered_map<unsigned char, int> GetByteFrequencies(const std::string &content);

// Creates a binary tree where leaf nodes' keys and weights are byte_to_frequency's keys and values.
// Chars with lower values are placed into leaf nodes that are deeper within the tree.
// Returns the root node of this tree.
std::unique_ptr<TreeNode> CreateTree(
    const std::unordered_map<unsigned char, int> &byte_to_frequency);

}  // namespace huffman

#endif  // _UNCOMPRESSEDREADER_H_
