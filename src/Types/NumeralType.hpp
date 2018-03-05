#pragma once
#include "Type.h"
namespace NestedNode
{
	class NumeralType :public NodeType {
	private:

	public:
		String toString() const override
		{
			return "";
		}
	};

	template<>
	inline bool Binary::convertTo(NumeralType& value) const
	{
		return false;
	}
	template<>
	inline bool Binary::serializeFrom(const NumeralType& value)
	{
		return false;
	}
}