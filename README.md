

NestedNode
=================

Many devices have some same parts. Most devices have a led light to show status.Why don't we reuse them? If we add more led lights to show more messages, we may change our protocol or program. Can we create a new device by adding them?

This project's aim is resolving it.

## First define a light node
```c++
class LightNode :public Leaf<int>
{
public:
	uint8_t pin;
	LightNode(uint8_t p, const char* name) :Leaf<int>(name, "light")
	{
		Require<Pin> l(Pin{ p });
		pin = p;
	}
	void setup() override
	{
		pinMode(pin, OUTPUT);
	}
	Msg handle_msg(NestedNode::MSG msg) override
	{
		switch (msg.message)
		{
		case Msg::accept_data:
			analogWrite(pin, _value);
			Serial.printf("analogwrite %d %d\n", pin, _value);
			return Msg::accept_data;
		default:
			Serial.printf("[%s] handle_msg (%d) unimplement\n", _name, msg.message);
			return msg.message;
		}
	}
};
```

## Second create an instance 
```c++
class RgbNode :public NestedNode::Nested
{
public:
	LightNode r = LightNode(D5, "r");
	LightNode g = LightNode(D6, "g");
	LightNode b = LightNode(D7, "b");
	RgbNode(const char* name) :Nested(name, "rgb")
	{
		add_child(&r);
		add_child(&g);
		add_child(&b);
	}

	Msg handle_msg(NestedNode::MSG msg) override
	{
		switch (msg.message)
		{
		case Msg::accept_data:
			Serial.printf("rgb control success\n");
			return Msg::handled;
		default:
			Serial.printf("[%s] handle_msg (%d) unimplement\n", _name, msg.message);
			return msg.message;
		}
	}
};
```

# Features
1. create nested node.
2. use node_path to control one node.
3. send json msg to control multi-node.
