#ifndef _BITS_H_
#define _BITS_H_

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace huffman {

#define BIT_LIMIT 256
#define BITS_PER_ELEM 8
#define BITS_PER_LENGTH 8

// This class represents a char and a (compressed) sequence of bits representing the char.
class Bits {
 public:
    // Creates a Bits whose bits are the given 0/1 sequence, which represents the given char.
    Bits(const unsigned char representing, const std::vector<bool> &bits);

    // Returns the number of bits in the compressed sequence of bits.
    int GetNumBits() const { return num_bits_; }
    // Returns the char that the compressed sequence of bits represents.
    char GetRepresenting() const { return representing_; }

    // Returns the minimum number of bytes needed to store all the bits of this Bits.
    int ByteLength() const;
    // Returns a text string of "0" and "1" characters that corresponds to the bits of this Bits.
    std::string ToString() const;

    // Writes this Bits' bits into the given string buffer.
    void WriteBitsTo(std::stringstream &buffer, unsigned char &next_bits, int &bit_offset) const;

 private:
    unsigned char bits_[BIT_LIMIT / BITS_PER_ELEM] = { 0 };
    int num_bits_;
    unsigned char representing_;
};

// Appends the given Bits' ToString representation to the given ostream.
std::ostream &operator<<(std::ostream &lhs, const Bits &rhs);

}  // namespace huffman

#endif  // _BITS_H_
