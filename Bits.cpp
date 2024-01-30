#include <sstream>
#include <string>
#include <stdexcept>
#include "Bits.h"

#include <bitset>

namespace huffman {

Bits::Bits(const unsigned char represents, const std::vector<bool> &bits)
    : num_bits_(bits.size()), representing_(represents) {
    if (GetNumBits() > BIT_LIMIT) {
        throw std::invalid_argument("Bits cannot hold more than BIT_LIMIT bits");
    }

    for (int i = 0; i < GetNumBits(); i++) {
        const char &bit_is_one = bits[i];

        if (bit_is_one) {
            unsigned char *to_write_bit_to = &bits_[i / BITS_PER_ELEM];
            int bit_offset = i % BITS_PER_ELEM;
            *to_write_bit_to |= 0x1 << bit_offset;
        }
    }
}

int Bits::ByteLength() const {
    int length = GetNumBits() / BITS_PER_LENGTH;
    return length + (GetNumBits() % BITS_PER_LENGTH != 0 ? 1 : 0);
}

std::string Bits::ToString() const {
    std::stringstream bits_str;
    for (int i = 0; i < GetNumBits(); i++) {
        bool bit_is_one = (bits_[i / BITS_PER_ELEM] >> i % BITS_PER_ELEM) & 0x1;
        bits_str << (bit_is_one ? "1" : "0");
    }
    return bits_str.str();
}

void BitWriter::AppendBits(const Bits &bits) {
    int num_final_iteration_bits = bits.GetNumBits() % BITS_PER_ELEM;
    if (num_final_iteration_bits == 0) {
        num_final_iteration_bits = BITS_PER_ELEM;
    }
    for (int i = 0; i < bits.ByteLength(); i++) {
        next_bits_ |= bits.bits_[i] << bit_offset_;

        bool leftover_bits_not_full_byte
            = i == bits.ByteLength() - 1 && num_final_iteration_bits + bit_offset_ < BITS_PER_ELEM;
        if (leftover_bits_not_full_byte) {
            break;
        }

        buffer << next_bits_;
        next_bits_ = bits.bits_[i] >> (BITS_PER_ELEM - bit_offset_);
    }
    bit_offset_ += num_final_iteration_bits;
    bit_offset_ %= BITS_PER_ELEM;

    total_num_bits_ += bits.GetNumBits();
}

std::string BitWriter::ToBytes() const {
    std::string bytes = buffer.str();
    if (bit_offset_ != 0) {
        bytes.push_back(next_bits_);
    }
    return bytes;
}

std::ostream &operator<<(std::ostream &lhs, const Bits &rhs) {
    return lhs << rhs.ToString();
}

}  // namespace huffman
