#ifndef _TREENODE_H_
#define _TREENODE_H_

#include <memory>
#include <unordered_map>
#include "Bits.h"

namespace huffman {

// This class represents a node in a tree used for representing the bytes in a string/file.
class TreeNode {
 public:
    // Constructs a non-leaf node whose left and right children are the given nodes.
    TreeNode(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right) :
        weight_(left->GetWeight() + right->GetWeight()),
        left_(std::move(left)),
        right_(std::move(right)),
        is_leaf_(false) { }

    // Constructs a leaf node that has the given key and weight.
    TreeNode(const unsigned char key, const int weight)
        : weight_(weight), key_(key), is_leaf_(true) { }

    // Returns this node's weight (for non-leaf nodes, this is the sum of the children's weights).
    int GetWeight() const { return weight_; }
    // Gets the key of this node if it's a leaf node; throws invalid_argument otherwise.
    unsigned char GetKey() const;
    // Gets the left child node if this node is not a leaf node; throws invalid_argument otherwise.
    const std::unique_ptr<TreeNode>& GetLeft() const;
    // Gets the right child node if this node is not a leaf node; throws invalid_argument otherwise.
    const std::unique_ptr<TreeNode>& GetRight() const;
    // Returns whether this node is a leaf node.
    bool IsLeaf() const { return is_leaf_; }

 private:
    int weight_;
    unsigned char key_;
    std::unique_ptr<TreeNode> left_;
    std::unique_ptr<TreeNode> right_;
    bool is_leaf_;
};

// Returns true if lhs's weight is less than rhs's weight, false otherwise.
bool operator<(TreeNode const& lhs, TreeNode const& rhs);
// Returns true if lhs's weight is greater than rhs's weight, false otherwise.
bool operator>(TreeNode const& lhs, TreeNode const& rhs);

// Returns a string representation of the contents and structure of the given tree.
std::string TreeContentsRepr(const TreeNode &root);
// Creates a mapping of the keys of the given tree's leaf nodes to Bits objects,
// whose bits are based on the placement of the leaf node in the tree. 
std::unordered_map<unsigned char, std::unique_ptr<Bits>> TreeCharToBits(const TreeNode &root);

}  // namespace huffman

#endif  // _TREENODE_H_
