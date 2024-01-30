#ifndef _COMPRESSED_READER_H_
#define _COMPRESSED_READER_H_

#include <string>
#include "CompressedWriter.h"

namespace huffman {

bool PartitionFileContents(
    const std::string file_contents,
    TreeFileRepr &tree_data,
    CompressedFileRepr &file_data);

std::unique_ptr<huffman::TreeNode> TreeReprToTree(const TreeFileRepr &tree_repr);

std::string DecompressFile(const huffman::TreeNode &root, const CompressedFileRepr &file_data);

}  // namespace

#endif