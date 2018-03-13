#pragma once

#include <vector>

class BitSet
{
public:
	BitSet(size_t n = 0, bool val = false);

	bool operator[](size_t pos) const;

	void Set(size_t pos);
	void Reset(size_t pos);

	// n bits set to true
	size_t CountTrue() const;

	// capacity
	size_t Size() const;

private:

	std::vector<bool> vec;   /* it's likely that std::vector<bool> compresses bool as bit
								but this is implementation defined
								TODO implement my own bitset or boost:dynamic_bitset ? */
	size_t cntTrue;
};
