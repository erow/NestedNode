//
// Created by erow on 18-2-22.
//

#ifndef NODES_RANGETYPE_HPP
#define NODES_RANGETYPE_HPP

#include "Type.h"
#include <stdio.h>
namespace NestedNode
{
class RangeType : public NodeType
{
	friend Binary;
    int lower,upper, value;
  public:
	int get_lower() const { return lower; }
	int get_upper() const { return upper; }
	int get_value() const { return value; }
	RangeType(int l=0,int u=100):lower(l),upper(u),value(0){}
	RangeType(const RangeType&) = default;
    String toString() const override
    {
        char tmp[10];
        sprintf(tmp, "%d", value);
        return tmp;
    }
};
template<>
inline bool Binary::convertTo(RangeType& value) const
{
	int v;
	if(convertTo(v)&&(value.get_lower() <= v) && (v <= value.get_upper()))
	{
		value.value = v;
		return true;
	}
	return false;
}
template<>
inline bool Binary::serializeFrom(const RangeType& value)
{
	return serializeFrom(value.value);
}
}

#endif //NODES_RANGETYPE_HPP
