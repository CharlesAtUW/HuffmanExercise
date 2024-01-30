#ifndef _UNCOMPRESSED_READER_H_
#define _UNCOMPRESSED_READER_H_

#include <string>
#include <unordered_map>
#include <memory>
#include "TreeNode.h"

namespace huffman {

bool ReadFileContents(const std::string &file_name, std::string &file_bytes);

std::unordered_map<unsigned char, int> GetByteFrequencies(const std::string &content);

std::unique_ptr<TreeNode> CreateTree(
    const std::unordered_map<unsigned char, int>& byte_to_frequency);

}  // namespace

#endif