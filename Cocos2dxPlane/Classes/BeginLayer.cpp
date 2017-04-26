#include "BeginLayer.h"
#include "AppMacros.h"
#include "ParticleManager.h"
#include "PlaySound.h"
#include "math.h"
#include "cocos2d.h"
#include <string>

//USING_NS_CC;
using namespace cocos2d;

bool BeginLayer::init()
{
	//调用父类的初始化
    if ( !Layer::init() )
    {
        return false;
    }

    //创建一个精灵对象
    back1 = Sprite::create(MAP_PATH+std::string("beginLayerBG.jpg"));
    back1->setPosition(Point(270,0));
    back1->setAnchorPoint(Point(0.5,0));
    this->addChild(back1, BACKGROUND_LEVEL);

    back2 = Sprite::create(GAME_PATH+std::string("gameTitle.png"));
    back2->setPosition(Point(270,0));
    back2->setAnchorPoint(Point(0.5,0));
    this->addChild(back2, 100);

	//粒子系统
	ParticleManager* pm=new ParticleManager(this);
	pm->initRain();

	plane=Sprite3D::create(C3B_PATH+std::string("plane01.obj"),C3B_PATH+std::string("plane01.jpg"));
	plane->setPosition3D(Vec3(260,600,10));
	plane->setScale(4.0f);
	plane->setGlobalZOrder(1);
	plane->setRotation3D(Vec3(-105,180,0));
	this->addChild(plane);

	BillBoard *billboard=BillBoard::create("selectplane/plane0.png");
	billboard->setPosition(Point(270,100));
	this->addChild(billboard,MENU_LEVEL);

	//创建一个单点触摸监听
	auto listener = EventListenerTouchOneByOne::create();
	//设置下传触摸
	listener->setSwallowTouches(true);
	//开始触摸时回调onTouchBegan方法
	listener->onTouchBegan = CC_CALLBACK_2(BeginLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(BeginLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(BeginLayer::onTouchEnded, this);
	//添加到监听器
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	PlaySound::playBackground();				//播放背景音乐

	//定时回调方法移动
	schedule(schedule_selector(BeginLayer::update), 0.01f);
    return true;
}
void BeginLayer::update(float delta)
{
	//============飞机旋转===========
	perRate+=0.012;
	plane->setRotation3D(Vec3(-105-perXA*sin(perXW*perRate),180,0-perZA*sin(perZW*perRate)));
}

bool BeginLayer::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}
void BeginLayer::onTouchMoved(Touch *touch, Event *event)
{

}
void BeginLayer::onTouchEnded(Touch *touch, Event *event)
{
	gsm->goLoading();
}

