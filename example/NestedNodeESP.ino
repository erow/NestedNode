
#include <string>
#include <Require.hpp>
#include <NestedNode.hpp>
#include <ESP8266WiFi.h>

#include <MQTTClient.h>
#include <ArduinoJson.hpp>
using namespace NestedNode;
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
Master master;
LightNode l1(D0, "l1");
RgbNode l2("l2");
void setup()
{
	Party::instance()->master = &master;
	Serial.begin(115200);
	delay(10);
	
	master.add_child(&l1);
	master.add_child(&l2);

	master.setup();
}

void loop()
{
	Party::instance()->master->recv("l2", Binary("{'r':12}"), true);
	delay(2000);
	Party::instance()->master->recv("l1", Binary("123"));
	delay(2000);
}
