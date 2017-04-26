#ifndef _GAMESET_H_
#define _GAMESET_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;

class GameSet:public Node
{
public :
	int index;			//第几个界面调用此方法   0-MainMenuScene ,1--My2DLayer
	Layer *layer;
	GameSet(Layer *gameLayer);
	void createSet(int index);
	void sliderEvent(Ref* pSender,SliderEventType type);
private :
	Sprite *menuBg;
	MenuItemImage* touchItem;
	MenuItemImage* gravityItem;
	void menuSelect(int index);
};

#endif
