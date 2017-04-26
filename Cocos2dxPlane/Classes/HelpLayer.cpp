#include "HelpLayer.h"
#include "AppMacros.h"
#include "BNSocketUtil.h"
#include "cocos2d.h"
#include <string>

bool HelpLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    Sprite3D *bg = Sprite3D::create(
                                    HELP_PATH+string("backgroundH.obj"),
                                    LOAD_PATH+string("loadingBG.jpg"));
    bg->setPosition3D(Vec3(270,530,-400));
    bg->setScale(2.3);
    this->addChild(bg);
    initMenu();
    
    rotateHelp = Sprite3D::create(
                                  HELP_PATH+string("helpRotate.obj"),
                                  HELP_PATH+string("page.jpg"));
    rotateHelp->setScale(0.6f);
    rotateHelp->setPosition3D(Vec3(273,460,-382));
    rotateHelp->setRotation3D(Vec3(0,0,0));
    this->addChild(rotateHelp,MENU_LEVEL);
    //创建一个单点触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    //设置下传触摸
    listener->setSwallowTouches(true);
    //开始触摸时回调onTouchBegan方法
    listener->onTouchBegan = CC_CALLBACK_2(HelpLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelpLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelpLayer::onTouchEnded, this);
    //添加到监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    //	new std::thread(&BNSocketUtil::threadConnectTask,"172.18.106.1", 59421);
    
    return true;
}

void HelpLayer::initMenu()
{
    Sprite *buttomBG=Sprite::create(HELP_PATH+std::string("buttomBG.png"));		//提示文本框
    buttomBG->setPosition(Vec2(270,0));
    buttomBG->setAnchorPoint(Point(0.5,0));
    buttomBG->setScaleY(0.7f);
    this->addChild(buttomBG,MENU_LEVEL);
    
    MenuItemImage* back =MenuItemImage::create(
                                               HELP_PATH+std::string("back_btn.png"),
                                               HELP_PATH+std::string("back_btn_down.png"),
                                               CC_CALLBACK_0(HelpLayer::menuExit,this)
                                               );
    back->setAnchorPoint(Point(0,0));
    back->setPosition(Point(20,20));
    
    Menu *menu=Menu::create(back,nullptr);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,MENU_LEVEL);
    
    point=Sprite::create(HELP_PATH+std::string("point1.png"));		//提示文本框
    point->setPosition(Vec2(270,175));
    point->setAnchorPoint(Point(0.5,1));
    point->setScale(1.2);
    this->addChild(point,MENU_LEVEL);
    point->runAction(
                     RepeatForever::create(
                                           Sequence::create(
                                                            FadeOut::create(1),
                                                            FadeIn::create(1),
                                                            nullptr
                                                            ))
                     );
}
bool HelpLayer::onTouchBegan(Touch* touch, Event* event)
{
    if(isClicked){				//双击事件
        isClicked=false;
        CCLOG("============shaungji");
        if(!isLook){
            rotateHelp->runAction(	//放大帮助信息
                                  Spawn::create(
                                                ScaleTo::create(0.25,0.86f),
                                                MoveBy::create(0.25,Vec2(0,60)),
                                                nullptr
                                                ));
            point->setTexture(HELP_PATH+std::string("point2.png"));
            isLook=true;
        }else{
            rotateHelp->runAction(	//放大帮助信息
                                  Spawn::create(
                                                ScaleTo::create(0.25,0.6f),
                                                MoveBy::create(0.25,Vec2(0,-60)),
                                                nullptr
                                                ));
            isLook=false;
            point->setTexture(HELP_PATH+std::string("point1.png"));
        }
    }else{						//单机事件
        isClicked=true;
        scheduleOnce(schedule_selector(HelpLayer::judgeCallback),0.3);
    }
    return true;
}

void HelpLayer::onTouchMoved(Touch* touch, Event* event)
{
    //	int offsetMax=helpCount*540;
    //	int startX=touch->getStartLocationInView().x;
    //	int nowX=touch->getLocationInView().x;
    //	int offsetTemp=(int)(offsetX-(nowX-startX)*0.8+offsetMax)%offsetMax;	//计算偏移总量
    //	int helpNum=offsetTemp/540;										//计算帮助图片编号
    //	int helpOffset=offsetTemp%540;									//计算当前偏移量
    //	CCLOG("offsetTemp=%d,helpNum=%d,helpOffset=%d,d=%d",offsetTemp,helpNum,helpOffset,-nowX+startX);
    //	if(helpOffset>270)												//调整偏移量与当前图片编号
    //	{
    //		helpOffset-=540;
    //		helpNum=(helpNum+1)%helpCount;
    //	}
    //	int helpPre = (helpNum-1+helpCount)%helpCount;					//计算相关帮助图片
    //	int helpNext = (helpNum+1)%helpCount;
    //	CCLOG("offsetTemp=%d,helpNum=%d,helpOffset=%d",offsetTemp,helpNum,helpOffset);
    //
    //	int dx=-helpOffset;
    //	double radian=asin((float)dx/270);
    //	int degree=CC_RADIANS_TO_DEGREES(radian);
    //	int dz=270*cos(radian);
    //	CCLOG("dx=%d,dz=%d,degree=%d,radian=%f",dx,dz,degree,radian);
    //	help[helpNum]->setPosition3D(Vec3(dx+270,480,dz-270));	//设置当前帮助图片的旋转及偏移
    //	help[helpNum]->setRotation3D(Vec3(0,degree,0));
    //
    //	help[helpPre]->setPosition3D(Vec3(-dz+270,480,dx-270));	//设置上一张帮助图片的旋转及偏移
    //	help[helpPre]->setRotation3D(Vec3(0,degree-90,0));
    //
    //	help[helpNext]->setPosition3D(Vec3(dz+270,480,-dx-270));	//设置后一张帮助图片的旋转及偏移
    //	help[helpNext]->setRotation3D(Vec3(0,degree+90,0));
    if(touchFlag&&Rect(0,120,540,800).containsPoint(touch->getLocation()))
    {
        Point delta=touch->getDelta();
        float helpY=rotateHelp->getRotation3D().y+delta.x/2;
        rotateHelp->setRotation3D(Vec3(0,helpY,0));
    }
    
}
void HelpLayer::onTouchEnded(Touch* touch, Event* event)
{
    int angle=int(rotateHelp->getRotation3D().y)%60;
    if(angle!=0){
        rotateHelp->runAction(RotateBy::create(0.3f,Vec3(0,-angle,0)));
    }
}
//触摸一次事件
void HelpLayer::judgeCallback(float)
{
    if(isClicked){
        isClicked=false;
    }
}
//改变帮助信息
void HelpLayer::goNextHelp(int index)
{
    rotateHelp->runAction(
                          Sequence::create(
                                           ScaleTo::create(0.15,0.6f),
                                           RotateBy::create(0.4f,Vec3(0,-60*index,0)),
                                           EaseBounceOut::create(ScaleTo::create(0.15,0.8)),
                                           CallFunc::create(CC_CALLBACK_0(HelpLayer::canChangeContent,this)),
                                           nullptr
                                           ));
}
void HelpLayer::canChangeContent()
{
    touchFlag=true;
}
void HelpLayer::menuExit()
{
    gsm->goMainMenu();
}

