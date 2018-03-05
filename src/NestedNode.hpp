#pragma once
#include "Require.hpp"
#include "Types/Type.h"
#include <list>
#include "ArduinoJson.h"
namespace Json=ArduinoJson;
namespace NestedNode {
	typedef int(*MSGHANDLER)(MSG);
	void print(MSG msg);
	class Node1;
	class Party {
	public:
		Party() {
		}
		Node1* master=nullptr;
		DynamicJsonBuffer buffer;
		MSGHANDLER global_handler = [](MSG msg)->int {
			printf("global handler ---\n");
			print(msg);
			return 0;
		};
		static Party* instance() {
			static Party* _party = nullptr;
			if (_party)
				return _party;
			return _party = new Party();
		}
	};

	class Node1
	{
	public:
		virtual ~Node1() = default;
		Node1* _parent;
		Node1* _brother=nullptr;
		Node1* _child=nullptr;
		const char* _name;
		const char* _type;
		Node1(const char*name, const char* type, Node1* p = nullptr)
		{
			this->_name = name;
			_type = type;
			_parent = p;
		}
		void setParent(Node1* p)
		{
			_parent = p;
		}
		void add_child(Node1* n)
		{
			n->setParent(this);
			Node1* p = this->_child;
			if(p)
			{
				while (p->_brother)
				{
					p = p->_brother;
				}
				p->_brother = n;
			}
			else
			{
				this->_child = n;
			}
			
		}
		Node1* find_child(const char* name) const {
			if(_child)
			{
				if (strcmp(_child->_name, name) == 0)
					return _child;
				return _child->find_brother(name);
			}
			return nullptr;
		}
		Node1* find_brother(const char* name) const {
			auto brother = _brother;
			while (brother)
			{
				if (strcmp(brother->_name, name) == 0)
				{
					return brother;
				}
				brother = brother->_brother;
			}
			return brother;
		}

		virtual void setup() {}

		virtual Msg handle_binary(const Binary &data) {
			return Msg::unimplement;
		}
		/**
		 * \brief 
		 * \param data ::const JsonVariant &
		 * \return accept_data | no_content | bad_data
		 */
		virtual Msg handle_object(const JsonVariant &data) {
			return Msg::unimplement;
		}
		
		/**
		 * \brief user custom handler
		 * \param msg ::Msg
		 * \return Msg
		 */
		virtual Msg handle_msg(MSG msg) = 0;

		/**
		 * \brief message deliver
		 * \param msg :: handled|unhandled
		 * \return handled|unhandled
		 */
		Msg deliver_msg(MSG msg) {
			if (handle_msg(msg) == Msg::handled)
				return Msg::handled;
			if(this->_parent)
				return this->_parent->deliver_msg(msg);
			return Msg::unhandled;
		}
		void send_msg(Msg message)
		{
			MSG msg = { this,message };
			handle_msg(msg);
			Party::instance()->global_handler(msg);
		}
		Msg recv(const char* node_path, const Binary & data, bool is_Object = false)
		{
			
			MSG msg = { this,  Msg::internal_error };
			Msg& message =msg.message;
			if(strlen(node_path)==0)
			{
				//handle data
				message= is_Object ?
					this->handle_object(Party::instance()->buffer.parseObject(data.data())) :
					this->handle_binary(data);
				//deliver_msg
				
				//message=deliver_msg(msg);

				Party::instance()->global_handler(msg);
			}else
			{
				//find a child
				static char tmp[32] = { 0 };
				size_t i = 0;
				for (; i < strlen(node_path); i++)
				{
					if (node_path[i] == '/')
					{
						break;
					}
				}
				memcpy(tmp, node_path, i);
				tmp[i] = '\0';
				Node1 * n = this->find_child(tmp);
				if (n)
				{
					message = n->recv(node_path + i + 1, data, is_Object);
				}
				else
					message = Msg::not_found;
			}
			return message;
		}

	};


	void print(MSG msg) {
		printf("node:%s\n", msg.hnhd->_name);
		printf("message:%d\n", msg.message);
	}

	template <class T>
	class Leaf :public Node1
	{
	public:
		T _value;
		Leaf(const char*name, const char* type, Node1* p = nullptr) :Node1(name, type, p) {}
		Msg handle_binary(const Binary &data) override {
			if (data.convertTo(_value))
			{
				//send_msg(Msg::accept_data);
				deliver_msg(MSG{this,Msg::accept_data});
				return  Msg::accept_data;
			}
			else
				return Msg::bad_data;
		}
		Msg handle_object(const JsonVariant& data) override
		{
			//TODO if(data.is<char*>())
				return handle_binary(Binary(data.as<char*>()));
			return Msg::bad_data;
		}
	};
	
	class Nested:public Node1
	{
	public:
		Nested(const char*name, const char* type, Node1* p = nullptr) :Node1(name, type, p) {}
		Msg handle_object(const JsonVariant &data) override{
			// binary data
			if (data.is<JsonArray>()) {
			}
			// nested
			else if (data.is<JsonObject>()) {
				int c = 0;
				for (auto& t : data.asObject())
				{
					auto brother = find_child(t.key);
					if (brother) {
						brother->handle_object(t.value);
						c++;
					}
				}
				if (c > 0)
					return Msg::accept_data;
				else
					return Msg::no_content;
			}
			else {
				return handle_binary(Binary(data.asString()));
			}
			return Msg::bad_data;
		}

		void setup() override
		{
			if (!_child)
				return;
			auto p = _child;
			while (p)
			{
				p->setup();
				p = p->_brother;
			}
		}
	};

	class Master :public Nested
	{
	public:
		Master() :Nested("master", "master")
		{
			
		}
		Msg handle_msg(MSG msg) override
		{
			printf("----master----\n");
			print(msg);
			return Msg::handled;
		}
	};

}