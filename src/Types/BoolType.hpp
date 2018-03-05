//
// Created by erow on 18-2-23.
//

#ifndef NODES_BOOLTYPE_HPP
#define NODES_BOOLTYPE_HPP
#include "Type.h"
namespace NestedNode
{
class BoolType : public NodeType
{
  public:
    bool value;
    String toString() const override
    {
        return value ? "true" : "false";
    }
};
template<>
inline bool Binary::convertTo(BoolType& value) const
{
	String str;
	if(convertTo(str))
	{
		if (str == "true")
			value.value = true;
		else if (str == "false")
			value.value = false;
		else
			return false;
		return true;
	}
	return false;
}
template<>
inline bool Binary::serializeFrom(const BoolType& value)
{
	return serializeFrom(String(value.value ? "true" : "false"));
}
}
#endif