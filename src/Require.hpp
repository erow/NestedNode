#pragma once

namespace NestedNode {
	class Node1;
	struct Pin { int value; };

	enum Resource
	{
		pin
	};

	enum class Msg
	{
		internal_error,
		//request
		accept_data,
		bad_data,
		not_found,
		no_content,
		//handle
		handled,
		unhandled,
		unimplement
	};

	typedef struct tagMSG {
		const Node1* hnhd;
		Msg message;
		void* vParam;
		void* lParam;
		unsigned long time;
	} MSG;


	template <class T>
	class Require
	{
	};
	template <>
	class Require<Pin>
	{
	public:
		Pin pin;
		Require(Pin t)
		{
			pin = t;
		}
	};

	
}
