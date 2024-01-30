#include <sstream>
#include <memory>
#include <unordered_map>
#include "TreeNode.h"
#include "Bits.h"

namespace huffman {

#define INDENT "  "

void BuildTreeContentsRepr(
    const TreeNode &current_node,
    const std::string &indent,
    std::stringstream &current_contents,
    const int depth);

void BuildTreeCharToBits(
    const TreeNode &current_node,
    std::vector<bool> &bits,
    std::unordered_map<unsigned char, std::unique_ptr<Bits>>&current_char_to_bits);

void BuildTreeFileRepr(
    const TreeNode &current_node,
    int16_t &node_count,
    std::stringstream &current_characters,
    int16_t &special_leaf_location);

// TreeNode::TreeNode(TreeNode &&node) {

// }

bool operator<(TreeNode const& lhs, TreeNode const& rhs) {
    return lhs.GetWeight() < rhs.GetWeight();
}

bool operator>(TreeNode const& lhs, TreeNode const& rhs) {
    return lhs.GetWeight() > rhs.GetWeight();
}

std::string TreeContentsRepr(const TreeNode &root) {
    std::stringstream tree_contents;
    BuildTreeContentsRepr(root, "", tree_contents, 0);
    return tree_contents.str();
}

void BuildTreeContentsRepr(
    const TreeNode &current_node,
    const std::string &indent,
    std::stringstream &current_contents,
    const int depth) {
    if (current_node.IsLeaf()) {
        current_contents << indent << "Leaf(key=" << current_node.GetKey()
            << ", weight=" << current_node.GetWeight()
            << ", depth=" << depth << ")" << std::endl;
    } else {
        std::string more_indent = indent + INDENT;
        current_contents << indent << "Node(weight=" << current_node.GetWeight() << ")" << std::endl
        << indent << "Left:" << std::endl;
        BuildTreeContentsRepr(*current_node.GetLeft(), more_indent, current_contents, depth + 1);
        current_contents << indent << "Right:" << std::endl;
        BuildTreeContentsRepr(*current_node.GetRight(), more_indent, current_contents, depth + 1);
    }
}

std::unordered_map<unsigned char, std::unique_ptr<Bits>> TreeCharToBits(const TreeNode &root) {
    std::unordered_map<unsigned char, std::unique_ptr<Bits>> char_to_bits;
    std::vector<bool> bits;
    BuildTreeCharToBits(root, bits, char_to_bits);
    return char_to_bits;
}

void BuildTreeCharToBits(
    const TreeNode &current_node,
    std::vector<bool> &bits,
    std::unordered_map<unsigned char, std::unique_ptr<Bits>>&current_char_to_bits) {
    if (current_node.IsLeaf()) {
        unsigned char key = current_node.GetKey();
        current_char_to_bits[key] = std::make_unique<Bits>(Bits(key, bits));
    } else {
        bits.push_back(false);
        BuildTreeCharToBits(*current_node.GetLeft(), bits, current_char_to_bits);
        bits.pop_back();

        bits.push_back(true);
        BuildTreeCharToBits(*current_node.GetRight(), bits, current_char_to_bits);
        bits.pop_back();
    }
}

}  // namespace