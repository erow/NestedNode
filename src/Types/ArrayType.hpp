//
// Created by erow on 18-2-22.
//

#ifndef NODES_STRINGTYPE_HPP
#define NODES_STRINGTYPE_HPP
#include <sstream>
#include "Type.h"
namespace NestedNode{
	class _ArrayType:public NodeType
	{
		friend Binary;
	private:
		virtual bool parseFrom(const Binary& value)=0;
		virtual bool serializeTo(Binary& data)=0;
	};
	template<class T>
    class ArrayType : public _ArrayType {
	private:
		std::vector<T> value;
    public:
        String toString() const override {
			std::istringstream istr;
			for (int i = 0; i < value.size(); i++)
			{

			}
            return istr.str();
        }
		
		/*friend bool Binary::convertTo(ArrayType<T>& value);
		friend bool Binary::serializeFrom(const ArrayType<T>& value);*/

    private:
	    bool parseFrom(const Binary& value) override
	    {
			return false;
	    }
	    bool serializeTo(Binary& data) override
	    {
			return false;
	    }
    };
	using ArrayBase=_ArrayType*;
	//I can't implement explicit (full) template specialization
	//http://en.cppreference.com/w/cpp/language/template_specialization
	template<>
	inline bool Binary::convertTo(ArrayBase& value) const
	{
		return value->parseFrom(*this);
	}
	/*template<typename T>
		 inline bool Binary::convertTo(ArrayType<T>& value)
		{

			return false;
		}
		 template<typename T>
		inline bool Binary::serializeFrom(const ArrayType<T>& value)
		{
			return false;
		}*/
	
}


#endif //NODES_STRINGTYPE_HPP
