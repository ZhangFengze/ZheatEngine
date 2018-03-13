#include "BitSet.h"

BitSet::BitSet(size_t n, bool val)
	:vec(std::vector<bool>(n, val)), cntTrue(val ? n : 0)
{
}

bool BitSet::operator[](size_t pos) const
{
	return vec[pos];
}

void BitSet::Set(size_t pos)
{
	if (!vec[pos])
	{
		vec[pos] = true;
		++cntTrue;
	}
}

void BitSet::Reset(size_t pos)
{
	if (vec[pos])
	{
		vec[pos] = false;
		--cntTrue;
	}
}

size_t BitSet::CountTrue() const
{
	return cntTrue;
}

size_t BitSet::Size() const
{
	return vec.size();
}
