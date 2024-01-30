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

class Bits {
 public:
    Bits(const unsigned char representing, const std::vector<bool> &bits);

    int GetNumBits() const { return num_bits_; }
    char GetRepresenting() const { return representing_; }

    int Length() const;
    std::string ToString() const;

    void WriteBitsTo(std::stringstream &buffer, unsigned char &next_bits, int &bit_offset) const;

 private:
    unsigned char bits_[BIT_LIMIT / BITS_PER_ELEM] = { 0 };
    int num_bits_;
    unsigned char representing_;
};

std::ostream &operator<<(std::ostream &lhs, const Bits &rhs);

}  // namespace

#endif  // _BITS_H_