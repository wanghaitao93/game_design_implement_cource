#ifndef _PLAYSOUND_H_
#define _PLAYSOUND_H_

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class PlaySound
{
public :
	static void playSound(int index);
	static void playBackground();	//���Ų˵�
	static void playBackgroundG();	//���ű�������
	static void resumeBackground();//�ָ���������
	static void pauseBackground();		//��ͣ��������
	static void playEffect(string soundName);
};
#endif
