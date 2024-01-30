#ifndef _COMPRESSEDREADER_H_
#define _COMPRESSEDREADER_H_

#include <string>
#include "CompressedWriter.h"

namespace huffman {

// Populates tree_data and file_data based on file_contents (which represents a compressed file)
bool PartitionFileContents(const std::string file_contents, TreeFileRepr &tree_data,
    CompressedFileRepr &file_data);

// Constructs a tree based on the contents of tree_repr.
// The weights of all nodes in this tree will be zero. Returns the root node of this tree.
std::unique_ptr<huffman::TreeNode> TreeReprToTree(const TreeFileRepr &tree_repr);

// Reconstructs the uncompressed contents of the given file_data.compressed_bits,
// using the given tree for mapping bits to characters.
// Returns the contents of the decompressed file as a string.
std::string DecompressFile(const huffman::TreeNode &root, const CompressedFileRepr &file_data);

}  // namespace huffman

#endif  // _COMPRESSEDREADER_H_
