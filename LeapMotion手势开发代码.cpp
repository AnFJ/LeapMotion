#include<stdafx.h>
#include<iostream>
#include<fstream>
#include<string.h>
#include "Leap.h"
#define FrameNum 15
#define IceboxTime 50
#define IceboxDistance 50
#define CupboardTime 30
#define CupboardDistance 50
#define LightBeforeFrame 15
#define SmokeTime 15
#define SmokeDistance 90
#define WatertapTime 57
#define WatertapDistanceY 40
#define WatertapDistanceX 30
#define HorizonTime 15
#define XHorizonDistance 120
#define ZHorizonDistance 70
bool iceboxopen=true;
bool iceboxclose=false;
bool cupboardopen = true;
bool cupboardclose = false;
bool lightopen = true;
int smokes = 0;
int smokecount = 0;
int watertaps = 0;
int watertapcount = 0;
int horizonmovecount = 0;
using namespace Leap;
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };
int HorizonMove(const Controller& controller)
{
	float xmove[60],zmove[60],ymove[60];
	int j = 0, count = 0;
	for (j = HorizonTime; j >= 0; j--)
	{
		Frame framei = controller.frame(j);
		HandList hands = framei.hands();
		HandList::const_iterator hl = hands.begin();
		const Hand hand = *hl;
		if (hand.isValid())//判断手存在
		{
				xmove[count] = hand.palmPosition().x;
				zmove[count] = hand.palmPosition().z;
				ymove[count] = hand.palmPosition().y;
				//std::cout << "数据：" << out[count] << std::endl;
				count++;
		}
	}
	count = 1;
	for (j = 1; j < HorizonTime; j++)
		if ((xmove[j] >= xmove[j - 1]))
			count++;
	if ((count == HorizonTime) && ((xmove[HorizonTime] - xmove[0]) >= XHorizonDistance)&&((ymove[HorizonTime]-ymove[0])<SmokeDistance/2&& (ymove[HorizonTime] - ymove[0])>-SmokeDistance/2))
		return 0;
	count = 1;
	for (j = 1; j < HorizonTime; j++)
		if ((xmove[j] <= xmove[j - 1]))
			count++;
	if ((count == HorizonTime) && ((xmove[0] - xmove[HorizonTime]) >= XHorizonDistance) && ((ymove[HorizonTime] - ymove[0])<SmokeDistance/2 && (ymove[HorizonTime] - ymove[0])>-SmokeDistance/2))
		return 1;
	count = 1;
	for (j = 1; j < HorizonTime; j++)
		if ((zmove[j]>= zmove[j - 1]))
			count++;
	if ((count == HorizonTime) && ((zmove[HorizonTime] - zmove[0]) >= ZHorizonDistance) && ((ymove[HorizonTime] - ymove[0])<SmokeDistance / 2 && (ymove[HorizonTime] - ymove[0])>-SmokeDistance / 2))
		return 2;
	count = 1;
	for (j = 1; j < HorizonTime; j++)
		if ((zmove[j] <= zmove[j - 1]))
			count++;
	if ((count == HorizonTime) && ((zmove[0] - zmove[HorizonTime]) >= ZHorizonDistance) && ((ymove[HorizonTime] - ymove[0])<SmokeDistance / 2 && (ymove[HorizonTime] - ymove[0])>-SmokeDistance / 2))
		return 3;
	return -1;
}
int BoolToInt(bool& b)
{
	if (b)
		return 1;
	else
		return 0;
}
int IceBox(const Controller& controller)
{
	float in[60];
	int j=0,count=0;
	for (j = IceboxTime; j >= 0; j--)
	{
		Frame framei = controller.frame(j);
		HandList hands = framei.hands();
		HandList::const_iterator hl = hands.begin();
		const Hand hand = *hl;
		if (hand.isValid())//判断手存在
		{
			//std::cout << "IsValid" << std::endl;
			//if ((hand.palmNormal().x>0.15||hand.palmNormal().x<-0.15)&&hand.confidence()<5)//判断手掌法线向量的x轴方向范围，用于调试
			if(1)
			{
				//std::cout <<" palmNormal" <<framei.id()<< std::endl;
				bool f[IceboxTime] = {false};
				int i = 0;
				const FingerList fingers = hand.fingers();
				for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl)
				{
					const Finger finger = *fl;
					f[i] = finger.isExtended();
				//	std::cout <<"fingerid:"<< finger.id ()<< std::endl;
					i++;
				}
				f[0] = true;
				for (i = 1; i < IceboxTime; i++)
					if (f[i])
						f[0] = false;
				if (f[0])
				{
					in[count] = hand.palmPosition().z;
					//std::cout << "数据：" << out[count] << std::endl;
					count++;
				}
				else
					return -1;
			}
			else
				return -1;
		}
		else
			return -1;
	}
	count = 1;
//	std::cout << std::endl;
	for (j = 1; j < IceboxTime; j++)
		if (in[j] >= in[j - 1])
			count++;
	if ((count == IceboxTime)&&((in[IceboxTime]-in[0])>= IceboxDistance))
		return 0;//icebox open
	count = 1;
	for (j = 1; j < IceboxTime; j++)
		if (in[j] <= in[j - 1])
			count++;
	if ((count == IceboxTime)&&((in[0]-in[IceboxTime]) >= IceboxDistance))
		return 1;//icebox close
	return -1;
}
int Cupboard(const Controller& controller)
{
	float in[60] = { 0 };
	int j = 0, count = 0;
	for (j = CupboardTime; j >= 0; j--)
	{
		Frame framei = controller.frame(j);
		HandList hands = framei.hands();
		HandList::const_iterator hl = hands.begin();
		const Hand hand = *hl;
		if (hand.isValid())//判断手存在
		{
			//std::cout << hand.grabStrength() << "   " << hand.pinchStrength() << std::endl;
			if ((hand.grabStrength() >= 0.99) && (hand.pinchStrength() <= 0.01))
			{
				in[count] = hand.palmPosition().z;
				//std::cout << "数据：" << in[count] << std::endl;
				count++;
			}
			else
				return -1;
		}
		else
			return -1;
	}
	count = 1;
	for (j = 1; j < CupboardTime; j++)
		if (in[j] >= in[j - 1])
			count++;
	if ((count == CupboardTime) && ((in[CupboardTime] - in[0]) >= CupboardDistance))
		return 0;//cupboard open
	count = 1;
	for (j = 1; j < CupboardTime; j++)
		if (in[j] <= in[j - 1])
			count++;
	if ((count == CupboardTime) && ((in[0] - in[CupboardTime]) >= CupboardDistance))
		return 1;//cupboard close	
	return -1;
}
int Light(const Controller& controller)
{
	
	Frame framebefore = controller.frame(LightBeforeFrame);
	HandList bhands = framebefore.hands();
	HandList::const_iterator bhl = bhands.begin();
	const Hand bhand = *bhl;
	Frame framenow = controller.frame();
	HandList nhands = framenow.hands();
	HandList::const_iterator nh1 = nhands.begin();
	const Hand nhand = *nh1;
	if ((bhand.palmNormal().y < -0.9) && (nhand.palmNormal().y > 0.9))
		return 0;
	else if ((bhand.palmNormal().y > 0.9) && (nhand.palmNormal().y < -0.9))
		return 1;
	else
		return -1;
}
/*
int Smoke(const Controller& controller)
{
	float up[60], grab[60], pinch[60];
	int j = 0, count = 0;
	bool start = false, isup = false;
	for (j = SmokeTime; j >= 0; j--)
	{
		Frame framei = controller.frame(j);
		HandList hands = framei.hands();
		HandList::const_iterator hl = hands.begin();
		const Hand hand = *hl;
		if (hand.isValid())//判断手存在
		{
			if (j == SmokeTime)
			{
				if (hand.palmNormal().y < -0.99&&hand.grabStrength() < 0.01&&hand.pinchStrength() < 0.01)
				{
					isup = true;
					up[count] = hand.palmPosition().y;
					grab[count] = hand.grabStrength();
					pinch[count] = hand.pinchStrength();
					//std::cout << "数据：" << out[count] << std::endl;
					count++;
				}
			}
			else
			{
				up[count] = hand.palmPosition().y;
				grab[count] = hand.grabStrength();
				pinch[count] = hand.pinchStrength();
				//std::cout << "数据：" << out[count] << std::endl;
				count++;
			}
		}
	}
	count = 1;
	for (j = 1; j < SmokeTime; j++)
		if ((up[j] >= up[j - 1]) && (grab[j] >= grab[j - 1]) && (pinch[j]>=pinch[j-1]))
			count++;
	if ((count == SmokeTime) && isup&&((up[SmokeTime]-up[0])>=SmokeDistance))
		return 0;
	count = 1;
	for (j = 1; j < SmokeTime; j++)
		if ((up[j] <= up[j - 1]) && (grab[j] <= grab[j - 1]) && (pinch[j] <= pinch[j - 1]))
			count++;
	if ((count == SmokeTime)&& ((up[0] - up[SmokeTime]) >= SmokeDistance))
		return 1;
	return -1;
}
*/
int Smoke(const Controller& controller)
{
	float up[60];
	int j = 0, count = 0;
	bool start = false, isup = false;
	for (j = SmokeTime; j >= 0; j--)
	{
		Frame framei = controller.frame(j);
		HandList hands = framei.hands();
		HandList::const_iterator hl = hands.begin();
		const Hand hand = *hl;
		if (hand.isValid())//判断手存在
		{
			{
				up[count] = hand.palmPosition().y;
				//std::cout << "数据：" << out[count] << std::endl;
				count++;
			}
		}
	}
	count = 1;
	for (j = 1; j < SmokeTime; j++)
		if ((up[j] >= up[j - 1]))
			count++;
	if ((count == SmokeTime)&&((up[SmokeTime] - up[0]) >= SmokeDistance))
		return 0;
	count = 1;
	for (j = 1; j < SmokeTime; j++)
		if ((up[j] <= up[j - 1]))
			count++;
	if ((count == SmokeTime) && ((up[0] - up[SmokeTime]) >= SmokeDistance))
		return 1;
	return -1;
}
int Watertap(const Controller& controller)
{
	float index[60];
	int j = 0, count = 0;
	Frame framei = controller.frame();
	HandList hands = framei.hands();
	HandList::const_iterator hl = hands.begin();
	const Hand hand = *hl;
	const FingerList fingers = hand.fingers();
	FingerList::const_iterator f = fingers.begin();
	f++;
	const Finger Index = *f;
	const GestureList gestures = framei.gestures();
	for (int g = 0; g < gestures.count(); ++g)
	{
		Gesture gesture = gestures[g];
		switch (gesture.type())
		{
		case Gesture::TYPE_CIRCLE:
		{
			CircleGesture circle = gesture;
			if (circle.pointable().direction().angleTo(circle.normal()) <= PI / 2) {
				if (circle.radius() >= 40)
					//std::cout << circle.radius() << std::endl;
					return 0;//std::cout << "顺时针"<<std::endl;
			}
			else {
				if (circle.radius() >= 40)
					//std::cout << circle.radius() << std::endl;
					return 1;//std::cout << "counterclockwise" << std::endl;
			}
		}
		default:
			break;
		}
	}
	return -1;
}
class SampleListener :public Listener {
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void  onFrame(const Controller&);
};
void SampleListener::onInit(const Controller &controller)
{
	std::cout << "初始化" << std::endl;
}
void SampleListener::onExit(const Controller& controller) {
	std::cout << "结束" << std::endl;
}
void SampleListener::onConnect(const Controller& controller) {
	std::cout << "已连接到设备" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
}
void SampleListener::onDisconnect(const Controller& controller)
{
	std::cout << "取消连接" << std::endl;
}
void SampleListener::onFrame(const Controller& controller){
	const Frame frame = controller.frame();
/*	for (int i = 0; i < 100; i++)
	{
		frame = controller.frame(i);
		std::cout << frame.id() <<","<<i<<"\t"<<frame.currentFramesPerSecond()<< std::endl;
	}*/
	//循环测试
	//std::cout << "ok";
	int icebox = IceBox(controller);
	int cupboard = Cupboard(controller);
	icebox = cupboard = -1;
	int light = Light(controller);
	int smoke = Smoke(controller);
	int watertap = Watertap(controller);
	int horizonmove = HorizonMove(controller);
	HandList hands = frame.hands();
	HandList::const_iterator hl = hands.begin();
	const Hand hand = *hl;
	
	if(hand.isValid())
	{
/*		
		if (cupboardopen)
		{
			if (cupboard == 0)
			{
				std::cout << "橱柜打开" << std::endl;
				cupboardopen = false;
				cupboardclose = true;
			}
			else
			{
				if (iceboxopen)
				{
					if (icebox == 0)
					{
						std::cout << "冰箱打开" << std::endl;
						iceboxopen = false;
						iceboxclose = true;
					}
				}
				if (iceboxclose)
				{
					if (icebox == 1)
					{
						std::cout << "冰箱关闭" << std::endl;
						iceboxopen = true;
						iceboxclose = false;
					}
				}
			}
		}
		if (cupboardclose)
		{
			if (cupboard == 1)
			{
				std::cout << "橱柜关闭" << std::endl;
				cupboardopen = true;
				cupboardclose = false;
			}
			else
			{
				if (iceboxopen)
				{
					if (icebox == 0)
					{
						std::cout << "冰箱打开" << std::endl;
						iceboxopen = false;
						iceboxclose = true;
					}
				}
				if (iceboxclose)
				{
					if (icebox == 1)
					{
						std::cout << "冰箱关闭" << std::endl;
						iceboxopen = true;
						iceboxclose = false;
					}
				}
			}
		}*/
		if (lightopen)
		{
			if (light == 0)
			{
				std::cout << "灯打开" << std::endl;
				lightopen = false;
			}
		}
		else
		{
			if (light == 1)
			{
				std::cout << "灯关闭" << std::endl;
				lightopen = true;
			}
		}
		if (smokecount == 150)
		{
			if (smoke == 0 && smokes < 5)
			{
				smokes++;
				smokecount = 0;
				std::cout << "吸油烟机加强为：" << smokes << std::endl;
			}
			if (smoke == 1 && smokes > 0)
			{
				smokes--;
				smokecount = 0;
				std::cout << "吸油烟机减弱为：" << smokes << std::endl;
			}
		}
		else
			smokecount++;
		if (watertapcount == 200)
		{
			if (watertap == 0 && watertaps < 5)
			{
				watertaps++;
				watertapcount = 0;
				std::cout << "水流增大：" << watertaps << std::endl;
			}
			if (watertap == 1 && watertaps > 0)
			{
				watertaps--;
				watertapcount = 0;
				std::cout << "水流减少：" << watertaps << std::endl;
			}
		}
		else
		{
			watertapcount++; 
			//std::cout << watertapcount << std::endl;
		}
		if (horizonmovecount == 200)
		{
			switch (horizonmove)
			{

			case 0:
				if (iceboxopen)
				{
					std::cout << "冰箱打开" << std::endl;
					horizonmovecount = 0;
					iceboxopen = false;
				}
				break;
			case 1:
				if (!iceboxopen)
				{
					std::cout << "冰箱关闭" << std::endl;
					horizonmovecount = 0;
					iceboxopen = true;
				}
				break;
			case 2:
				if (cupboardopen)
				{
					std::cout << "橱柜打开" << std::endl;
					horizonmovecount = 0;
					cupboardopen = false;
				}
				break;
			case 3:
				if (!cupboardopen)
				{
					std::cout << "橱柜关闭" << std::endl;
					horizonmovecount = 0;
					cupboardopen = true;
				}
				break;
			default:
				break;
			}
		}
		else
			horizonmovecount++;
	}

}
int main()
{
	SampleListener listener;
	Controller controller;
	controller.addListener(listener);
	std::cin.get();
	controller.removeListener(listener);
	return 0;
}
