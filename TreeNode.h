#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include <memory>
#include <unordered_map>
#include "Bits.h"

namespace huffman {

class TreeNode {
 public:
    // non-leaf node
    TreeNode(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right) :
        weight_(left->GetWeight() + right->GetWeight()),
        left_(std::move(left)),
        right_(std::move(right)),
        is_leaf_(false) { }
    
    // leaf node
    TreeNode(const unsigned char key, const int weight)
        : weight_(weight), key_(key), is_leaf_(true) { }

    // TreeNode(TreeNode &&node);
    
    int GetWeight() const { return weight_; }
    unsigned char GetKey() const;
    const std::unique_ptr<TreeNode>& GetLeft() const;
    const std::unique_ptr<TreeNode>& GetRight() const;
    bool IsLeaf() const { return is_leaf_; }

 private:
    int weight_;
    unsigned char key_;
    std::unique_ptr<TreeNode> left_;
    std::unique_ptr<TreeNode> right_;
    bool is_leaf_;
};

bool operator<(TreeNode const& lhs, TreeNode const& rhs);
bool operator>(TreeNode const& lhs, TreeNode const& rhs);

std::string TreeContentsRepr(const TreeNode &root);
std::unordered_map<unsigned char, std::unique_ptr<Bits>> TreeCharToBits(const TreeNode &root);

}  // namespace

#endif  // _TREE_NODE_H