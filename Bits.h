#ifndef _BITS_H_
#define _BITS_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>

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

    friend class BitWriter;

 private:
    unsigned char bits_[BIT_LIMIT / BITS_PER_ELEM] = { 0 };
    int num_bits_;
    unsigned char representing_;
};

// Appends the given Bits' ToString representation to the given ostream.
std::ostream &operator<<(std::ostream &lhs, const Bits &rhs);

// This class represents a buffer for bits to be appended to.
class BitWriter {
 public:
   // Returns the total number of bits that have been written to this BitWriter.
   uint64_t GetTotalNumBits() const { return total_num_bits_; }

   // Appends the given Bits' bits to this BitWriter.
   void AppendBits(const Bits &bits);
   // Returns a string with all the bits (as bits in the string data) appended onto this BitWriter.
   // There will be unused bits in the last byte of the string if the number of bits appended
   // isn't evenly divisible by BITS_PER_LENGTH.
   std::string ToBytes() const;

 private:
   uint64_t total_num_bits_ = 0;
   std::stringstream buffer;
   unsigned char next_bits_ = 0;
   int bit_offset_ = 0;
};

}  // namespace huffman

#endif  // _BITS_H_
