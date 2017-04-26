#include "SelectPlaneLayer.h"
#include "AppMacros.h"
#include "Constant.h"
#include "PlaySound.h"
#include "GameData.h"
#include "BNSocketUtil.h"
#include "cocos2d.h"

using namespace std;
bool SelectPlane::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    //背景
    Sprite *background=Sprite::create(SELECT_PATH+std::string("selectPlaneBG.jpg"));
	background->setPosition3D(Vec3(270,480,-50));
	background->setScale(1.06f);
	this->addChild(background,BACKGROUND_LEVEL);
	//初始化 锁
	lock=Sprite::create(SELECTGAME_PATH+std::string("suo.png"));
	lock->setPosition3D(Vec3(270,320,120));
	lock->setVisible(false);
	this->addChild(lock,MENU_LEVEL);

	//星星的纹理
	starsTexture=Director::getInstance()->getTextureCache()->addImage(SELECTGAME_PATH+std::string("tstars.png"));
	//飞机
	initPlane3D();

	//飞机旋转背景
	Sprite *rotateBack=Sprite::create(SELECT_PATH+std::string("planeRotateBG.png"));
	rotateBack->setPosition3D(Vec3(270,290,-50));
	rotateBack->setScale(1.15f);
	this->addChild(rotateBack,BACKGROUND_LEVEL);

	//飞机上方旋转的图
	Sprite *rotatePlaneBG=Sprite::create(SELECT_PATH+std::string("planeRotate.png"));
	rotatePlaneBG->setPosition3D(Vec3(269,312,100));
	rotatePlaneBG->setScale(0.86f);
	rotatePlaneBG->setVisible(false);
	this->addChild(rotatePlaneBG,BACKGROUND_LEVEL);
	rotatePlaneBG->runAction(RepeatForever::create(RotateBy::create(0.5,180)));

	//飞机的信息
	Sprite *planeContent=Sprite::create(SELECT_PATH+std::string("planeContent.png"));
	planeContent->setPosition(178,500);
	this->addChild(planeContent,BACKGROUND_LEVEL);

	//选关信息
	Sprite *selectCustom=Sprite::create(SELECT_PATH+std::string("selectImageSmall.png"));
	selectCustom->setPosition(223+128*Constant::custom,822);
	this->addChild(selectCustom,MENU_LEVEL);

	//选择机型信息
	selectPlaneSort=Sprite::create(SELECT_PATH+std::string("selectImageBig.png"));
	selectPlaneSort->setPosition(223,878);
	this->addChild(selectPlaneSort,BACKGROUND_LEVEL);
	//机型奔跑动画
	ActionInterval *actionMove=Sequence::create(MoveBy::create(0.5f,Point(128,0)),DelayTime::create(0.5f),nullptr);
	ActionInterval *actionMove1=Sequence::create(MoveBy::create(0.5f,Point(-128,0)),DelayTime::create(0.5f),nullptr);
	selectPlaneSort->runAction(
				RepeatForever::create(
						Sequence::create(
							actionMove->clone(),
							actionMove,
							actionMove1->clone(),
							actionMove1,
							nullptr
				)));

	//初始化低栏
	backButtom=Sprite::create(SELECT_PATH+std::string("selectGameButtom.png"));
	backButtom->setAnchorPoint(Point(0.5,0));
	backButtom->setPosition3D(Vec3(270,0,0));
	this->addChild(backButtom,MENU_LEVEL);

	//初始化菜单项
	initMenu();
	//初始化飞机信息
	initPlaneStar();
	//初始化进度条
//	initProgress();
	//创建一个单点触摸监听
	EventListenerTouchOneByOne* listenerTouches = EventListenerTouchOneByOne::create();
	//设置下传触摸
	listenerTouches->setSwallowTouches(true);
	//开始触摸时回调onTouchBegan方法
	listenerTouches->onTouchBegan = CC_CALLBACK_2(SelectPlane::onMyTouchBegan, this);
	//开始触摸时回调onTouchMoved方法
	listenerTouches->onTouchMoved = CC_CALLBACK_2(SelectPlane::onMyTouchMoved, this);
	//触摸结束时回调onTouchEnded方法
	listenerTouches->onTouchEnded = CC_CALLBACK_2(SelectPlane::onMyTouchEnded, this);
	//添加到监听器//
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouches, this);

	if(GameData::playerNumber!=-1){				//联网模式
		//剪裁
        DrawNode* shap = DrawNode::create();
        Point point[4] = {Point(95,620), Point(235,620), Point(235,760), Point(95, 760)};
        shap->drawPolygon(point, 4,Color4F(255, 255, 255, 255),2,Color4F(255, 255, 255, 255));
        ClippingRectangleNode* cliper = ClippingRectangleNode::create(Rect(95,620,140,140));
        cliper->setPosition(Point::ZERO);
        this->addChild(shap,1);
        this->addChild(cliper);
        
        //剪裁
        DrawNode* shap2 = DrawNode::create();
        Point point2[4] = {Point(305,620), Point(445,620), Point(445,760), Point(305,760)};
        shap2->drawPolygon(point2, 4, Color4F(255, 255, 255, 255), 2, Color4F(255, 255, 255, 255));
        ClippingRectangleNode* cliper2 = ClippingRectangleNode::create(Rect(305,620,140,140));
        cliper2->setPosition(Point::ZERO);
        this->addChild(shap2,1);
        this->addChild(cliper2);
        //房主
        Sprite *houseSprite=Sprite::create(SELECT_PATH+std::string("houseOwn.png"));
        houseSprite->setPosition(Vec2(20,760));
        houseSprite->setAnchorPoint(Point(0,1));
        this->addChild(houseSprite,MENU_LEVEL);
        
        //玩家
        Sprite *playerSprite=Sprite::create(SELECT_PATH+std::string("player.png"));
        playerSprite->setPosition(Vec2(520,760));
        playerSprite->setAnchorPoint(Point(1,1));
        this->addChild(playerSprite,MENU_LEVEL);
        
        //房主选择的飞机
        housePlane=Sprite::create(SELECT_PATH+std::string("selectPlane.png"));
        housePlane->setPosition(Vec2(235,760));
        housePlane->setAnchorPoint(Point(1,1));
        cliper->addChild(housePlane,MENU_LEVEL);
        
        //玩家选择的飞机
        playerPlane=Sprite::create(SELECT_PATH+std::string("selectPlane.png"));
        playerPlane->setPosition(Vec2(305,760));
        playerPlane->setAnchorPoint(Point(0,1));
        cliper2->addChild(playerPlane,MENU_LEVEL);
        
        //打开定时更新选择飞机
        schedule(schedule_selector(SelectPlane::changeSelectPlane),0.1f);
        
        //绘制两个边框
        DrawNode* shapHouse = DrawNode::create();
        Point pointHouse[4] = {Point(95,620), Point(235,620), Point(235,760), Point(95, 760)};
        Point pointPlayer[4] = {Point(305,620), Point(445,620), Point(445,760), Point(305,760)};
        shapHouse->drawPolygon(pointHouse, 4,Color4F(255, 255, 255, 255),3,(GameData::playerNumber==1)?Color4F(0.95,0.8,0.2, 1):Color4F(0.08, 0.6, 0.97, 1));
        shapHouse->drawPolygon(pointPlayer, 4,Color4F(255, 255, 255, 255),3,(GameData::playerNumber==0)?Color4F(0.95,0.8,0.2, 1):Color4F(0.08, 0.6, 0.97, 1));
        this->addChild(shapHouse,20);
        //按钮的颜色混合效果
        Point PointHouse(235-70,760-70);			//房主的位置
        Point PointPlayer(445-70,760-70);			//玩家的位置
        Point PointStart(0,0);						//开始的位置
        Sprite *blend=Sprite::create(SELECTGAME_PATH+std::string("blendSelect.png"));	//创建颜色混合精灵
        BlendFunc cbl = {GL_DST_COLOR, GL_ONE};		//设置颜色混合方案
        blend->setBlendFunc(cbl);					//设置颜色混合
        //			blend->setAnchorPoint(Point(1,1));		//设置颜色混合的锚点
        if(GameData::playerNumber==0){				//根据是房主还是玩家设置位置
            PointStart=PointPlayer+Point(0,140);	//设置玩家混合的起始点
            cliper2->addChild(blend,MENU_LEVEL+1);
        }else if(GameData::playerNumber==1){		//设置房主的混合起始点
            PointStart=PointHouse+Point(0,140);
            cliper->addChild(blend,MENU_LEVEL+1);
        }
        blend->setPosition(PointStart);				//设置混合的位置
        blend->runAction(							//混合执行动画
            RepeatForever::create(
                Sequence::create(
                    MoveBy::create(1.0f,Vec2(0,-280)),
                    DelayTime::create(0.5f),
                    Place::create(PointStart),
                    nullptr
                    )));
    }

	//环境光  一般都是要的
	AmbientLight *ambientLight=AmbientLight::create(Color3B(150, 150, 150));
	this->addChild(ambientLight);
	//定向光
	DirectionLight *directionLight= DirectionLight::create(Vec3(0.0f, -1.0f, -1.0f), Color3B(200, 200, 200));
	this->addChild(directionLight);
	return true;
}
//创建飞机
void SelectPlane::initPlane3D()
{
	for(int i=0;i<3;i++)
	{
		planeobj[i] = Sprite3D::create(C3B_PATH+StringUtils::format("plane0%d.obj",i+1).c_str(),C3B_PATH+StringUtils::format("plane0%d.jpg",i+1).c_str());
		planeobj[i]->setPosition3D( Vec3(270+540*(i-Constant::planeId),0,70) );
		planeobj[i]->setRotation3D(Vec3(-60,0,0));
		planeobj[i]->setScale(1.5f);
		this->addChild(planeobj[i]);
		planeobj[i]->runAction(Spawn::create(MoveBy::create(1.2f,Vec2(0,320)),
				ScaleTo::create(1.0f,1.2f),
				nullptr));
//		planeobj[i]->setGlobalZOrder(1);
	}
	lock->setVisible(false);
}
//初始化菜单项
void SelectPlane::initMenu()
{
//	//离开
	menuItem[0]=MenuItemImage::create(
			HELP_PATH+"back_btn.png",
			HELP_PATH+"back_btn_down.png",
    		CC_CALLBACK_0(SelectPlane::menuSelect,this,0)
    		);
	menuItem[0]->setAnchorPoint(Point(0,0));
	menuItem[0]->setPosition(Vec2(25,40));
	//战斗
    menuItem[1]=MenuItemImage::create(
    		SELECTGAME_PATH+std::string("fight_btn.png"),
    		SELECTGAME_PATH+std::string("fight_btn_down.png"),
    		CC_CALLBACK_0(SelectPlane::menuSelect,this,1)
    		);
    menuItem[1]->setAnchorPoint(Point(1,0));
    menuItem[1]->setPosition(Vec2(515,40));

    Menu *menu1=Menu::create(menuItem[0],menuItem[1],nullptr);
	menu1->setPosition(Point::ZERO);
	backButtom->addChild(menu1,MENU_LEVEL+1);

	//按钮的颜色混合效果
	Sprite *blend[2];
	for(int i=0;i<2;i++){
		blend[i]=Sprite::create(SELECTGAME_PATH+std::string("blend.png"));
			BlendFunc cbl = {GL_DST_COLOR, GL_ONE};
			blend[i]->setBlendFunc(cbl);
			blend[i]->setAnchorPoint(Point(0,0.5));
	}
	blend[0]->setPosition(65,30);
	blend[0]->runAction(
			RepeatForever::create(
					Sequence::create(
							MoveBy::create(1.0f,Vec2(-70,0)),
							MoveBy::create(1.0f,Vec2(70,0)),
//							Place::create(Vec2(65,30)),
							nullptr
							)));
	blend[1]->setPosition(-5,30);
	blend[1]->runAction(
			RepeatForever::create(
					Sequence::create(
							MoveBy::create(1.0f,Vec2(70,0)),
							MoveBy::create(1.0f,Vec2(-70,0)),
//							Place::create(Vec2(-5,30)),
							nullptr
							)));
	menuItem[0]->addChild(blend[0]);
	menuItem[1]->addChild(blend[1]);
    //向左
    menuItem[2]=MenuItemImage::create(
    		SELECT_PATH+std::string("left_btn_down2.png"),
    		SELECT_PATH+std::string("left_btn_down2.png"),
    		CC_CALLBACK_0(SelectPlane::menuSelect,this,2)
    		);
    menuItem[2]->setAnchorPoint(Point(0,0.5));
    menuItem[2]->setPosition(Point(35,290));
    //向右
    menuItem[3]=MenuItemImage::create(
    		SELECT_PATH+std::string("right_btn_down2.png"),
    		SELECT_PATH+std::string("right_btn_down2.png"),
    		CC_CALLBACK_0(SelectPlane::menuSelect,this,3)
    		);
    menuItem[3]->setAnchorPoint(Point(1,0.5));
    menuItem[3]->setPosition(Point(505,290));

	menu=Menu::create(menuItem[2],menuItem[3],nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,MENU_LEVEL+1);

	//左侧选择按钮 向右移动
	leftMove=Sprite::create(SELECT_PATH+std::string("left_btnMove1.png"));
	leftMove->setAnchorPoint(Point(0,0.5));
	leftMove->setPosition(Point(70,290));
	this->addChild(leftMove,20);

	//左侧按钮移动
	ActionInterval *action_leftM=EaseSineIn::create(MoveBy::create(0.5f,Point(-20,0)));  //慢变快
	ActionInterval *action_rightM=EaseSineOut::create(MoveBy::create(0,Point(20,0)));//快变慢
	ActionInterval *leftAction=Sequence::create(action_leftM->clone(),action_rightM->clone(),nullptr);
	if(Constant::custom<=Constant::canPlay){
		leftMove->runAction(RepeatForever::create(leftAction->clone()));
	}
	//右侧选择按钮 向右移动
	rightMove=Sprite::create(SELECT_PATH+std::string("right_btnMove1.png"));
	rightMove->setAnchorPoint(Point(1,0.5));
	rightMove->setPosition(Point(470,290));
	this->addChild(rightMove,20);
	//按钮
	ActionInterval *action_right=EaseSineIn::create(MoveBy::create(0.5f,Point(20,0)));  //慢变快
	ActionInterval *action_left=EaseSineOut::create(MoveBy::create(0,Point(-20,0)));//快变慢
	ActionInterval *rightAction=Sequence::create(action_right->clone(),action_left->clone(),nullptr);
	if(Constant::custom<=Constant::canPlay){
		rightMove->runAction(RepeatForever::create(rightAction->clone()));
	}

	if(GameData::playerNumber!=-1){					//如果是网络版
		menuItem[0]->setVisible(false);				//隐藏返回按钮
	}
}
//初始化飞机的信息
void SelectPlane::initPlaneStar()
{
	for(int i=0;i<3;i++){
		stars[i]=Sprite::createWithTexture(starsTexture,Rect(0,0,98.0*Constant::planeSkill[Constant::planeId][i]/100,15));
		stars[i]->setPosition(165-49,586-66*i);
		stars[i]->setAnchorPoint(Point(0,0.5));
		this->addChild(stars[i],MENU_LEVEL);
	}
}
//改变飞机信息 星星
void SelectPlane::changePlaneStar()
{
	for(int i=0;i<3;i++){
		stars[i]->setTextureRect(Rect(0,0,98.0*Constant::planeSkill[Constant::planeId][i]/100,15));
	}
}
//选择
void SelectPlane::menuSelect(int index)
{
	menuItem[index]->runAction(Sequence::create(
		ScaleTo::create(0.1f, 1.3f),
		ScaleTo::create(0.2f, 1.1f),
        ScaleTo::create(0.2f, 1.0f),
        nullptr));

	switch(index){
	case 0:						//返回
		gsm->goSelectGame();
		PlaySound::playSound(kCancel);
		break;
	case 1:						//开始游戏
		menu->setEnabled(false);
		if(GameData::playerNumber!=-1){		//联网模式

			Sprite *background=Sprite::create(GAMEMODE_PATH+std::string("MenuBg.png"));
			background->setScale(2);
			background->setPosition3D(Vec3(270,480,201));
			this->addChild(background,MENU_LEVEL);

			waittingConnect=Sprite::create(SELECTGAME_PATH+std::string("waittingConnect0.png")); //等待中
			this->addChild(waittingConnect,MENU_LEVEL);
			waittingConnect->setPosition3D(Vec3(270,480,201));
			schedule(schedule_selector(SelectPlane::changWaitting),0.1f);

			BNSocketUtil::sendInt(GameData::SEND_START);				//开始游戏

		}else{								//单机模式
			planeFly();						//飞机飞行
			PlaySound::playSound(kPlane_go);//加载声音
		}
		break;
	case 2:
		changePlane(-1);			//左移
		break;
	case 3:
		changePlane(1);
		break;
	}
}
void SelectPlane::changWaitting(float f)
{
	waitIndex++;
	if(waitIndex==4){
		waitIndex=0;
	}
	waittingConnect->setTexture(NET_PATH+StringUtils::format("waittingConnect%d.png",waitIndex));
	if(GameData::housePrepare==1&&GameData::playerPrepare==1){			//都已经准备
		this->unschedule(SEL_SCHEDULE(&SelectPlane::changWaitting));
		this->unschedule(SEL_SCHEDULE(&SelectPlane::changeSelectPlane));
//		this->unschedule(schedule_selector(&SelectPlane::changWaitting));
		gsm->goWeb();
	}
}
void SelectPlane::planeFly()
{
	selectPlaneSort->stopAllActions();
	Point pTemp=Point(223+128*Constant::planeId,878);
	selectPlaneSort->runAction(Sequence::create(
		MoveTo::create(0.3,pTemp),Blink::create(0.7,3),nullptr));			//选择框闪动
	planeobj[Constant::planeId%3]->setRotation3D(Vec3(-60,0,0));
//	ActionInterval *planeRotate1=RotateTo::create(1.0f,Vec3(-90,270,0));
//	ActionInterval *planeRotate2=RotateTo::create(2.0f,Vec3(-90,400,0));
//	ActionInterval *planeMove1=MoveBy::create(1.0f,Vec2(100,-100));
//	ActionInterval *planeMove2=MoveBy::create(2.0f,Vec2(100,600));
//	ActionInterval *planeScale1=ScaleTo::create(1.0,1.5f);
//	ActionInterval *planeScale2=ScaleTo::create(2.0,0.001f);
//	ActionInterval *planeCome=Spawn::create(planeRotate1,planeMove1,planeScale1,nullptr);
//	ActionInterval *planeGo=Spawn::create(planeRotate2,planeMove2,planeScale2,nullptr);

	ActionInterval *planeRotate=RotateTo::create(2.0f,Vec3(-20,-45,0));
	ActionInterval *planeMove=MoveBy::create(2.0f,Vec2(200,500));
	ActionInterval *planeScale=ScaleTo::create(2.0,0.001f);
	ActionInterval *planeGo=Spawn::create(planeRotate,planeMove,planeScale,nullptr);

	planeobj[Constant::planeId%3]->runAction(Sequence::create(
			planeGo,
			CallFunc::create(CC_CALLBACK_0(SelectPlane::goNextScene,this)),
			nullptr));
}
//改变选飞机信息
void SelectPlane::changePlane(int index)
{
	PlaySound::playSound(kTransform);	//加载声音
	//获取当前飞机
	int Index=Constant::planeId+index;
	if(Index==-1){
		Index=2;
		planeobj[Index]->setPosition(Vec2(270+540*index,310));
		planeobj[Index-1]->setPosition(Vec2(270+(-540)*index,310));
//		}
	}else if(Index==3){
		Index=0;
		planeobj[Index]->setPosition(Vec2(270+540*index,310));
		planeobj[Index+1]->setPosition(Vec2(270+(-540)*index,310));
//		}
	}else{
		planeobj[Index]->setPosition(Vec2(270+540*index,310));
		if(Index==2){
			planeobj[0]->setPosition(Vec2(270+(-540)*index,310));
		}else if(Index==0){
			planeobj[2]->setPosition(Vec2(270+(-540)*index,310));
		}
	}
	//关卡有变
	Constant::planeId=Index;
	changeContent(index);
	if(GameData::playerNumber==0){			//玩家改变选择飞机
		GameData::playerPlaneId=Index;
		BNSocketUtil::sendInt(GameData::SEND_PLANE);
		BNSocketUtil::sendInt(Index);
		CCLOG("-------------SEND_PLANE=%d",Index);
	}else if(GameData::playerNumber==1){	//房主改变选择飞机
		GameData::housePlaneId=Index;
		BNSocketUtil::sendInt(GameData::SEND_PLANE);
		BNSocketUtil::sendInt(Index);
		CCLOG("-------------SEND_PLANE=%d",Index);
	}
	//战斗按钮设置可用
	if(GameData::playerNumber==-1)					//单机版
	{
		if(Constant::planeId>Constant::canPlane){
			menuItem[1]->setVisible(false);
			lock->setVisible(true);
		}else{
			menuItem[1]->setVisible(true);
			lock->setVisible(false);
		}
	}
}
// //改变飞机的选择
void SelectPlane::changeSelectPlane(float f)
{
	if(GameData::oldHousePlaneId!=GameData::housePlaneId){
		housePlane->stopAllActions();
		housePlane->runAction(MoveTo::create(0.5,Vec2(235,760+GameData::housePlaneId*140)));
		GameData::oldHousePlaneId=GameData::housePlaneId;
		CCLOG("-------------changselectpalne   GameData::housePlaneId=%d,",GameData::housePlaneId);
	}
	if(GameData::oldPlayerPlaneId!=GameData::playerPlaneId){
		playerPlane->stopAllActions();
		playerPlane->runAction(MoveTo::create(0.5,Vec2(305,760+GameData::playerPlaneId*140)));
		GameData::oldPlayerPlaneId=GameData::playerPlaneId;
		CCLOG("-------------changselectpalne   GameData::playerPlaneId=%d,",GameData::playerPlaneId);
	}
}
void SelectPlane::changeContent(int index)
{
	changePlaneStar();			//更改飞机的信息 星星
	//改变飞机的位置
	for(int i=0;i<3;i++){
			planeobj[i]->stopAllActions();
			if(Constant::planeId==i){
				planeobj[Constant::planeId]->runAction(EaseBounceOut::create(MoveTo::create(0.5f,Point(270,310))));
			}else{
				planeobj[i]->runAction(EaseBounceOut::create(MoveTo::create(0.5f,Point(270+index*540,310))));
			}

	}
}
void SelectPlane::setPoint3D(float dt)
{
	step+=10;
	planeobj[Constant::planeId]->setPosition3D(Vec3(270,150+step,70));		//更新位置
	if(step>=1100)
	{
		unschedule(schedule_selector(SelectPlane::setPoint3D));
		goNextScene();												//跳转到下一界面
	}
}
//触摸监听
bool SelectPlane::onMyTouchBegan(Touch* touch, Event* event)
{
	return true;
}
void SelectPlane::onMyTouchMoved(Touch *touch, Event *event)
{
	if(Constant::planeId<=Constant::canPlane){
		auto target = static_cast<Sprite*>(	event->getCurrentTarget());//获取当前触摸对象，并转化为精灵类型
		Point startPoint = target->convertToNodeSpace(touch->getLocation());//获取当前坐标
		Point delta=touch->getDelta();
		if((startPoint.x-270)*(startPoint.x-270)+(startPoint.y-290)*(startPoint.y-290)<=22500)
			if(fabs(delta.x)>0){
				float planeX=planeobj[Constant::planeId]->getRotation3D().x-delta.y;
				float planeY=planeobj[Constant::planeId]->getRotation3D().y+delta.x;
					planeobj[Constant::planeId]->setRotation3D(Vec3(planeX,planeY,0));
	}}
}
void SelectPlane::onMyTouchEnded(Touch *touch, Event *event)
{
}
void SelectPlane::goNextScene()
{
	if(GameData::playerNumber==-1){			//单机版
		gsm->goFirst();
		CCLOG("----------------单机");
	}else{									//网络版
		CCLOG("----------------网络");
		gsm->goWeb();
	}

}

