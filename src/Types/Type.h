#pragma once
#include <vector>
namespace NestedNode
{
	using String = std::basic_string<char>;

	class Binary: public std::vector<unsigned char>
	{
	public:
		Binary() = default;
		Binary(const String& str)
		{
			for (auto c : str)
				push_back(c);
		}
		Binary(const Binary&) = default;
		Binary(const std::initializer_list<unsigned char>& list) :std::vector<unsigned char>{ list } {}
		String toString()
		{
			String str;
			for (auto c : *this)
				str.push_back(c);
			return str;
		}
		
		template<typename T>
		bool convertTo(T& value) const
		{
			//throw error
			ERROR1("undefined convert Type");
			return false;
		}
		template<typename T>
		bool serializeFrom(const T& value)
		{
			//throw error
			ERROR1("undefined parse Type");
			return false;
		}
	};

	/*
	 * template<>
		inline bool Binary::convertTo(T& value)
		{
			int v;
			return false;
		}
		template<>
		inline bool Binary::serializeFrom(const T& value)
		{
			return false;
		}
	 */
	//for String
	template<>
	inline bool Binary::convertTo(String& value) const
	{
		for (auto c : *this)
			value.push_back(c);
		return true;
	}
	template<>
	inline bool Binary::serializeFrom(const String& value)
	{
		clear();
		for (auto c : value)
			push_back(c);
		return true;
	}
	using _CHAR_PTR = const char *;
	template<>
	inline bool Binary::serializeFrom(const _CHAR_PTR &value)
	{
		return serializeFrom(String(value));
	}
	//for int
	template<>
	inline bool Binary::convertTo(int& value) const
	{
		if (size() == 0)
			return false;
		auto& data = *this;
		size_t i = 1;
		if (data[0] == '-')
			if (size() == 1)
				return false;
			else
				value = data[1]-'0', i = 2;
		else
			value = data[0]-'0';
		for(;i<size();i++)
		{
			if ('0' <= data[i] && data[i] <= '9')
				value = value * 10 + data[i] - '0';
			else
				return false;
		}
		if (data[0] == '-')
			value = -value;
		return true;
	}

	template<>
	inline bool Binary::serializeFrom(const int& value)
	{
		int v = value;
		clear();
		if(v<0)
		{
			v = -value;
		}
		while (v>0)
		{
			push_back('0' + v % 10);
			v = v / 10;
		}
		if(value<0)
			push_back('-');
		int len = size();
		for(size_t i=0;i<len/2;i++)
			std::swap(operator[](i), operator[](len-1-i));
		return true;
	}


	/*template<>
	inline bool Binary::convertTo(JsonVariant& value) const
	{
		DynamicJsonBuffer t;
		value = t.parseObject(*this);
		return value.success();
	}
	template<>
	inline bool Binary::serializeFrom(const JsonVariant& value)
	{
		clear();
		value.printTo(*this);
		return true;
	}*/

	class NodeType
	{
	public:
		virtual String toString() const = 0;
	};

}