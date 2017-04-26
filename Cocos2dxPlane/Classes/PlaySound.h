#ifndef _PLAYSOUND_H_
#define _PLAYSOUND_H_

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class PlaySound
{
public :
	static void playSound(int index);
	static void playBackground();	//≤•∑≈≤Àµ•
	static void playBackgroundG();	//≤•∑≈±≥æ∞“Ù¿÷
	static void resumeBackground();//ª÷∏¥±≥æ∞…˘“Ù
	static void pauseBackground();		//‘›Õ£±≥æ∞“Ù¿÷
	static void playEffect(string soundName);
};
#endif
