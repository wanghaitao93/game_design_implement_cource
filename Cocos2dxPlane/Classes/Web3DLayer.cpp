#include "Web3DLayer.h"
#include "GameData.h"
#include "BNSocketUtil.h"
#include "AppMacros.h"
#include "Constant.h"
#include "Enemies.h"

bool Web3DLayer::init()
{
	if ( !Layer::init())
	{
		return false;
	}
	back1 = Sprite3D::create(MENU_PATH+std::string("background.obj"),MENU_PATH+StringUtils::format("background%d.jpg",GameData::level).c_str());
	back1->setPosition3D(Vec3(270,1200,-500));
	this->addChild(back1,BACKGROUND_LEVEL);

	back2 = Sprite3D::create(MENU_PATH+std::string("background.obj"),MENU_PATH+StringUtils::format("background%d.jpg",GameData::level).c_str());
	back2->setPosition3D(Vec3(270,1200+2400,-500));
	this->addChild(back2,BACKGROUND_LEVEL);

	exManager=ExManager::create();
	this->addChild(exManager);

	//环境光  一般都是要的
	AmbientLight *ambientLight=AmbientLight::create(Color3B(150, 150, 150));
	ambientLight->setLightFlag(LightFlag::LIGHT1);
	this->addChild(ambientLight);
	//定向光
	directionLight= DirectionLight::create(Vec3(0.0f, -1.0f, -1.0f), Color3B(200, 200, 200));
	directionLight->setLightFlag(LightFlag::LIGHT2);
	this->addChild(directionLight);

	//保护
	Vector<SpriteFrame*> animFrames;
	Animation *anim;

	for(int i=0;i<5;i++)
	{
		SpriteFrame *frame=SpriteFrame::create(
				PROTECT_PATH+StringUtils::format("bh%d.png",(i)).c_str(),Rect(0,0,180,180));
		animFrames.pushBack(frame);
	}
	anim=Animation::createWithSpriteFrames(animFrames,0.2);
	protectAnim=Animate::create(anim);
	protectAnim->retain();
	//爆炸
	Vector<SpriteFrame*> animFrames1;
	Animation *anim1;

	for(int i=0;i<64;i++)
	{
		SpriteFrame *frame=SpriteFrame::create(
				BOOM_PATH+StringUtils::format("100%d.png",(i)).c_str(),Rect(0,0,256,256));
		animFrames1.pushBack(frame);
	}
	anim1=Animation::createWithSpriteFrames(animFrames1,0.02);
	boomAnim=Animate::create(anim1);
	boomAnim->retain();

	//两秒后开始游戏
	scheduleOnce(schedule_selector(Web3DLayer::startGame),2.0);

	this->setCameraMask((unsigned short)CameraFlag::USER1);

	return true;
}
//开始游戏
void Web3DLayer::startGame(float f)
{
	createPlane();											//创建飞机
	//创建一个单点触摸监听
	auto listener = EventListenerTouchOneByOne::create();
	//设置下传触摸
	listener->setSwallowTouches(true);
	//开始触摸时回调onTouchBegan方法
	listener->onTouchBegan = CC_CALLBACK_2(Web3DLayer::onMyTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Web3DLayer::onMyTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Web3DLayer::onMyTouchEnded, this);
//	//添加到监听器
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	//重力感应
	Device::setAccelerometerEnabled(true);			//开启重力传感器
	auto listener1 = EventListenerAcceleration::create(	//创建监听器
	CC_CALLBACK_2(Web3DLayer::onMyAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);	//注册监听器
	//拖尾效果===============
	//参一为消隐动画时长，参二为拖尾条带相邻顶点间的最小距离,参三为拖尾条带的宽度,参四为顶点颜色值，参五为所使用的纹理
	myStreak = MotionStreak::create(0.15, 1, 15, Color3B(255,255,200), GAME_PATH+"streak.png");//MotionStreak::create(0.4, 1, 17, Color3B(255,255,255), pics_PATH+"streak.png")
	myStreak->setBlendFunc(BlendFunc::ADDITIVE);//设置混合
	this->addChild(myStreak,20);//将myStreak添加到布景中
	//设置此布景层渲染用摄像机
	myStreak->setCameraMask((unsigned short)CameraFlag::USER1);
}
//创建飞机和导弹
void Web3DLayer::createPlane()
{
	//玩家飞机
	playerPlane=Sprite3D::create(C3B_PATH+StringUtils::format("plane0%d.obj",GameData::playerPlaneId+1),C3B_PATH+StringUtils::format("plane0%d.jpg",GameData::playerPlaneId+1));
	playerPlane->setScale(0.1);
	playerPlane->setRotation3D(Vec3(0,0,0));
	playerPlane->setPosition3D(Vec3(150,-100,0));
	this->addChild(playerPlane,PLANE_LEVEL);
	playerPlane->runAction(
			Sequence::create(
					Spawn::create(
							EaseBounceOut::create(MoveTo::create(1.2f,Point(150,180))),
							ScaleTo::create(1.2f,1.0f),
							RotateBy::create(1.2f,Vec3(0,-720,0)),
							nullptr),
					nullptr
			));
	playerPlane->setCameraMask((unsigned short)CameraFlag::USER1);		//添加摄像机

	//房主飞机
	housePlane=Sprite3D::create(C3B_PATH+StringUtils::format("plane0%d.obj",GameData::housePlaneId+1),C3B_PATH+StringUtils::format("plane0%d.jpg",GameData::housePlaneId+1));
	housePlane->setScale(0.1);
	housePlane->setRotation3D(Vec3(0,0,0));
	housePlane->setPosition3D(Vec3(390,-100,0));
	this->addChild(housePlane,PLANE_LEVEL);
	housePlane->runAction(
		Sequence::create(
				Spawn::create(
						EaseBounceOut::create(MoveTo::create(1.2f,Point(390,180))),
						ScaleTo::create(1.2f,1.0f),
						RotateBy::create(1.2f,Vec3(0,-720,0)),
						nullptr),
				nullptr
		));
	housePlane->setCameraMask((unsigned short)CameraFlag::USER1);		//添加摄像机
	//保护的进度条
	string route1 = GAME_PATH + "loadingBack.png";		//底座图片
	string route2 = GAME_PATH + "load.png";				//拖拉进度图片
	string route3 = GAME_PATH + "transparent.png";		//旋钮图片

	potentiometer = ControlPotentiometer::create(				//创建圆盘拖拉条对象
												 route1.c_str(),route2.c_str(),route3.c_str());
	potentiometer->setMinimumValue(0);
	potentiometer->setMaximumValue(100);
	potentiometer->setValue(100);
	potentiometer->setEnabled(false);
	tempProgress=BillBoard::create();
	tempProgress->addChild(potentiometer);
	tempProgress->setPosition3D(Vec3(0,-10,0));
	tempProgress->setVisible(false);
	if(GameData::playerNumber==0){
		playerPlane->addChild(tempProgress,MENU_LEVEL);
	}else{
		housePlane->addChild(tempProgress,MENU_LEVEL);
	}

	tempProgress->setCameraMask((unsigned short)CameraFlag::USER1);

	tempBoard1=BillBoard::create();
	tempBoard1->setPosition(0,-10);
	tempBoard1->runAction(RepeatForever::create(protectAnim->clone()));
	tempBoard1->setVisible(false);
	tempBoard2=BillBoard::create();
	tempBoard2->setPosition(0,-10);
	tempBoard2->runAction(RepeatForever::create(protectAnim->clone()));
	tempBoard2->setVisible(false);
	playerPlane->addChild(tempBoard1,MENU_LEVEL);
	housePlane->addChild(tempBoard2,MENU_LEVEL);
	tempBoard1->setCameraMask((unsigned short)CameraFlag::USER1);
	tempBoard2->setCameraMask((unsigned short)CameraFlag::USER1);

	schedule(schedule_selector(Web3DLayer::updateEnemyBullet),0.01);
	schedule(schedule_selector(Web3DLayer::updateEnemy),0.01);
	schedule(schedule_selector(Web3DLayer::updateMainBullet),0.01);
	schedule(schedule_selector(Web3DLayer::updateMissile),0.01);
	schedule(schedule_selector(Web3DLayer::updateAward),0.01);
	schedule(schedule_selector(Web3DLayer::updateBackground),0.01);
	schedule(schedule_selector(Web3DLayer::updatePlane),0.01);
	scheduleOnce(schedule_selector(Web3DLayer::startServer),1.2f);
	GameData::isPlaneCanMove=true;		//飞机可以移动
	//定时回调方法移动旋转Obj模型以及c3t模型
	schedule(schedule_selector(Web3DLayer::update), 0.01f);
}
void Web3DLayer::startServer(float f)
{
	CCLOG("------startserver");
	BNSocketUtil::sendInt(GameData::SEND_STARTACTION);
}
void Web3DLayer::deletePlane(int index)
{
	if(index==0){
		GameData::isRedLift=false;
		this->removeChild(playerPlane,true);
		if(GameData::playerNumber==0){
			this->removeChild(myStreak,true);
		}
	}else{
		GameData::isGreenLift=false;
		this->removeChild(housePlane,true);
		if(GameData::playerNumber==1){
			this->removeChild(myStreak,true);
		}
	}
}
void Web3DLayer::updatePlane(float f)
{
	if(GameData::isChangePlaneMove)
	{
		std::vector<float> dataPlane=GameData::planeData;
		GameData::isChangePlaneMove=false;
		std::vector<float>::iterator dataIndex=dataPlane.begin();

		playerPlaneX=*(dataIndex++);
		playerPlaneY=*(dataIndex++);
		housePlaneX=*(dataIndex++);
		housePlaneY=*(dataIndex++);
		playerAngleX=*(dataIndex++);
		playerAngleY=*(dataIndex++);
		playerAngleZ=*(dataIndex++);
		houseAngleX=*(dataIndex++);
		houseAngleY=*(dataIndex++);
		houseAngleZ=*(dataIndex++);
		redHealth=*(dataIndex++);
		greenHealth=*(dataIndex++);
		score=*(dataIndex++);
		redAttackCount=*(dataIndex++);
		greenAttackCount=*(dataIndex++);
		redProtectCount=*(dataIndex++);
		greenProtectCount=*(dataIndex++);

		if(GameData::isRedLift){
			playerPlane->setPosition(Point(playerPlaneX,playerPlaneY));
			playerPlane->setRotation3D(Vec3(playerAngleX,playerAngleY,playerAngleZ));
		}
		if(GameData::isGreenLift){
			housePlane->setPosition(Point(housePlaneX,housePlaneY));
			housePlane->setRotation3D(Vec3(houseAngleX,houseAngleY,houseAngleZ));
		}
		if(GameData::playerNumber==0&&GameData::isRedLift){
			myStreak->setPosition(Point(playerPlaneX,playerPlaneY)-Point(0,80));
		}else if(GameData::playerNumber==1&&GameData::isGreenLift){
			myStreak->setPosition(Point(housePlaneX,housePlaneY)-Point(0,80));
		}
		if(GameData::isRedLift){
			if(redHealth>0){
				GameData::redPlaneHealth=redHealth;
				GameData::redAttackCount=redAttackCount;
				GameData::redProtectCount=redProtectCount;
			}else{
				GameData::redPlaneHealth=0;
				this->deletePlane(0);
			}
		}
		if(GameData::isGreenLift){
			if(greenHealth>0){
				GameData::greenPlaneHealth=greenHealth;
				GameData::greenAttackCount=greenAttackCount;
				GameData::greenProtectCount=greenProtectCount;
			}else{
				GameData::greenPlaneHealth=0;
				this->deletePlane(1);
			}
		}
		GameData::totalScore=(int)score;
	}
}
void Web3DLayer::updateBackground(float dt)
{
	//两个背景滚动
	int yy=back1->getPosition().y;
	if(yy==-1200)
	{
		yy=1200+2400;
	}
	back1->setPositionY(yy-3);

	yy=back2->getPosition().y;
	if(yy==-1200)
	{
		yy=1200+2400;
	}
	back2->setPositionY(yy-3);

	if(GameData::isRedLift)
	{
		if(GameData::isRedAttack){
			GameData::isRedAttack=false;
			playBoom();
		}
		if(GameData::isRedProtect){
			tempBoard1->setVisible(true);
		}else{
			tempBoard1->setVisible(false);
		}
	}
	if(GameData::isGreenLift)
	{
		if(GameData::isGreenAttack){
			GameData::isGreenAttack=false;
			playBoom();
		}
		if(GameData::isGreenProtect){
			tempBoard2->setVisible(true);
		}else{
			tempBoard2->setVisible(false);
		}
	}

}
//播放爆炸
void Web3DLayer::playBoom()
{
	//创建精灵执行爆炸动画
	Sprite *temp=Sprite::create();
	temp->setPosition(270,480);
	temp->setScale(2.5f);
	this->addChild(temp);
	temp->runAction(Sequence::create(boomAnim->clone(),
		RemoveSelf::create(),nullptr));
}
void Web3DLayer::updateEnemyBullet(float dt)
{
	if(GameData::enemyBulletFlag)
	{
		GameData::enemyBulletFlag=false;
		//复制数据到临时数据
		std::vector<float> dataTemp=GameData::enemyBulletVec;
		GameData::enemyBulletFlag=true;
		if(dataTemp.size()%5!=0)
		{
			return;
		}
		//创建各种临时列表
		std::vector<float> createTemp;						//需要创建新子弹的数据李彪
		std::vector<float>::iterator iter=dataTemp.begin();	//数据列表的迭代器
		//遍历更新数据，更改已经存在的子弹数据，将不存在的子弹数据保存。
		iter=dataTemp.begin();
		for(;iter!=dataTemp.end();)
		{
			int ID=(int)(*iter);
			std::map<int, Bullet*>::iterator iterator=enemyBulletMap.find(ID);
			if(iterator!=enemyBulletMap.end())				//如果数据存在，更新位置数据
			{
				Bullet *bullet=iterator->second;
				iter++;
				bullet->setPositionX(*(++iter));
				bullet->setPositionY(*(++iter));
				iter+=2;
				bullet->liveFlag=1;
				continue;
			}else
			{
				for(int i=0;i<5;i++)
				{
					createTemp.push_back(*(iter++));
				}
			}
		}
		//遍历子弹列表，将无数据更新的子弹删除。
		std::map<int, Bullet*>::iterator deleteTemp=enemyBulletMap.begin();				//需要删除的子弹列表
		for(;deleteTemp!=enemyBulletMap.end();)
		{
			Bullet* bullet=deleteTemp->second;
			bullet->liveFlag--;
			if(bullet->liveFlag<0)
			{
				exManager->createPlane(bullet->getPosition3D());
				bullet->removeFromParent();
				deleteTemp=enemyBulletMap.erase(deleteTemp);
			}else
			{
				deleteTemp++;
			}
		}
		//遍历数据列表，创建新的子弹。
		std::vector<float>::iterator iter2=createTemp.begin();
		for(;iter2!=createTemp.end();)
		{
			int ID=(int)(*(iter2++));
			int num=(int)(*(iter2++));
			float x=*(iter2++);
			float y=*(iter2++);
			float angle=*(iter2++);
			Bullet *bullet=Bullet::webCreate(num,Point(x,y),angle);
			this->addChild(bullet);
			enemyBulletMap.insert(std::make_pair(ID, bullet));
		}
	}
}

void Web3DLayer::updateMainBullet(float dt)
{
	if(GameData::mainBulletFlag)
	{
		GameData::mainBulletFlag=false;
		//复制数据到临时数据
		std::vector<float> dataTemp=GameData::mainBulletVec;
		GameData::mainBulletFlag=true;
		if(dataTemp.size()%5!=0)
		{
			return;
		}
		//创建各种临时列表
		std::vector<float> createTemp;						//需要创建新子弹的数据李彪
		std::vector<float>::iterator iter=dataTemp.begin();	//数据列表的迭代器
		//遍历更新数据，更改已经存在的子弹数据，将不存在的子弹数据保存。
		iter=dataTemp.begin();
		for(;iter!=dataTemp.end();)
		{
			int ID=(int)(*iter);
			std::map<int, Bullet*>::iterator iterator=mainBulletMap.find(ID);
			if(iterator!=mainBulletMap.end())				//如果数据存在，更新位置数据
			{
				Bullet *bullet=iterator->second;
				iter++;
				bullet->setPositionX(*(++iter));
				bullet->setPositionY(*(++iter));
				iter+=2;
				bullet->liveFlag=1;
				continue;
			}else
			{
				for(int i=0;i<5;i++)
				{
					createTemp.push_back(*(iter++));
				}
			}
		}
		//遍历子弹列表，将无数据更新的子弹删除。
		std::map<int, Bullet*>::iterator deleteTemp=mainBulletMap.begin();				//需要删除的子弹列表
		for(;deleteTemp!=mainBulletMap.end();)
		{
			Bullet* bullet=deleteTemp->second;
			bullet->liveFlag--;
			if(bullet->liveFlag<0)
			{
				exManager->createSmall(bullet->getPosition3D());
				bullet->removeFromParent();
				deleteTemp=mainBulletMap.erase(deleteTemp);
			}else
			{
				deleteTemp++;
			}
		}
		//遍历数据列表，创建新的子弹。
		std::vector<float>::iterator iter2=createTemp.begin();
		for(;iter2!=createTemp.end();)
		{
			int ID=(int)(*(iter2++));
			int num=(int)(*(iter2++));
			float x=*(iter2++);
			float y=*(iter2++);
			float angle=*(iter2++);
			Bullet *bullet=Bullet::webCreate(num,Point(x,y),angle);
			this->addChild(bullet);
			mainBulletMap.insert(std::make_pair(ID, bullet));
		}
	}
}

void Web3DLayer::updateEnemy(float dt)
{
	if(GameData::enemyFlag)
	{
		GameData::enemyFlag=false;
		//复制数据到临时数据
		std::vector<float> dataTemp=GameData::enemyVec;
		GameData::enemyFlag=true;
		if(dataTemp.size()%8!=0)
		{
			return;
		}
		//创建各种临时列表
		std::vector<float> createTemp;						//需要创建新子弹的数据李彪
		std::vector<float>::iterator iter=dataTemp.begin();	//数据列表的迭代器
		//遍历更新数据，更改已经存在的敌机数据，将不存在的敌机数据保存。
		iter=dataTemp.begin();
		for(;iter!=dataTemp.end();)
		{
			int ID=(int)(*iter);
			std::map<int, EnemyPlane*>::iterator iterator=enemyMap.find(ID);
			if(iterator!=enemyMap.end())								//如果数据存在，更新位置数据
			{
				EnemyPlane *enemyPlane=iterator->second;
				iter+=2;
				enemyPlane->setPosition3D(Vec3(*(iter++),*(iter++),*(iter++)));
				enemyPlane->setRotation3D(Vec3(*(iter++),*(iter++),*(iter++)));
				enemyPlane->liveFlag=1;
				continue;
			}else
			{
				for(int i=0;i<8;i++)
				{
					createTemp.push_back(*(iter++));
				}
			}
		}
		//遍历敌机列表，将无数据更新的敌机删除。
		std::map<int, EnemyPlane*>::iterator deleteTemp=enemyMap.begin();				//需要删除的子弹列表
		for(;deleteTemp!=enemyMap.end();)
		{
			EnemyPlane* enemyPlane=deleteTemp->second;
			enemyPlane->liveFlag--;
			if(enemyPlane->liveFlag<0)
			{
				enemyPlane->die();
				deleteTemp=enemyMap.erase(deleteTemp);
			}else
			{
				deleteTemp++;
			}
		}
		//遍历数据列表，创建新的飞机。
		std::vector<float>::iterator iter2=createTemp.begin();
		for(;iter2!=createTemp.end();)
		{
			int ID=(int)(*(iter2++));
			int index=(int)(*(iter2++));
			float posX=*(iter2++);
			float posY=*(iter2++);
			float posZ=*(iter2++);
			float rotX=*(iter2++);
			float rotY=*(iter2++);
			float rotZ=*(iter2++);
			EnemyPlane *enemyPlane;
            switch(index)
			{
			case 0:
				enemyPlane=FirstEnemy::webCreate();
				this->addChild(enemyPlane);
				break;
			case 1:
				enemyPlane=SecondEnemy::webCreate(posX);
				this->addChild(enemyPlane);
				break;
			case 2:
			{
				enemyPlane=ThirdEnemy::webCreate();
				this->addChild(enemyPlane);
				auto scaleTemp=ScaleTo::create(1,0.5);
				enemyPlane->plane->runAction(scaleTemp);
				break;
			}
			case 3:
				enemyPlane=FourthEnemy::webCreate();
				this->addChild(enemyPlane);
				enemyPlane->startAnim();
				break;
			case 4:
			case 5:
			case 6:
				enemyPlane=Boss::webCreate();
				this->addChild(enemyPlane);
				enemyPlane->startAnim();
				break;
			}
			enemyPlane->setPosition3D(Vec3(posX,posY,posZ));
			enemyPlane->setRotation3D(Vec3(rotX,rotY,rotZ));
			enemyMap.insert(std::make_pair(ID, enemyPlane));
		}
	}
}
//更新导弹
void Web3DLayer::updateMissile(float f)
{
	if(GameData::missileFlag)
	{
		GameData::missileFlag=false;
		//复制数据到临时数据
		std::vector<float> dataTemp=GameData::missileVec;
		GameData::missileFlag=true;
		if(dataTemp.size()%7!=0)
		{
			return;
		}
		//创建各种临时列表
		std::vector<float> createTemp;						//需要创建新子弹的数据列表
		std::vector<float>::iterator iter=dataTemp.begin();	//数据列表的迭代器
		//遍历更新数据，更改已经存在的子弹数据，将不存在的子弹数据保存。
		iter=dataTemp.begin();
		for(;iter!=dataTemp.end();)
		{
			int ID=(int)(*iter);
			std::map<int,Missile*>::iterator iterator=missileMap.find(ID);
//			Missile *missile=missileMap.find(ID)->second;
//			CCLOG("-------------------------------------------20");
			if(iterator!=missileMap.end())								//如果数据存在，更新位置数据
			{
				Missile *missile=iterator->second;
				missile->setPositionX(*(++iter));
				missile->setPositionY(*(++iter));
				missile->setPositionZ(*(++iter));
				missile->setRotation3D(Vec3(*(++iter),*(++iter),*(++iter)));
				iter++;
				missile->liveFlag=1;
				continue;
			}else
			{
				for(int i=0;i<7;i++)
				{
					createTemp.push_back(*(iter++));
				}
			}
		}
		//遍历导弹列表，将无数据更新的导弹删除。
		std::map<int, Missile*>::iterator deleteTemp=missileMap.begin();				//需要删除的子弹列表
		for(;deleteTemp!=missileMap.end();)
		{
			Missile* missile=deleteTemp->second;
			missile->liveFlag--;
			if(missile->liveFlag<0)
			{
				exManager->createSmall(missile->getPosition3D());
				missile->removeFromParent();
				deleteTemp=missileMap.erase(deleteTemp);
			}else
			{
				deleteTemp++;
			}
		}
		//遍历数据列表，创建新的导弹。
		std::vector<float>::iterator iter2=createTemp.begin();
		for(;iter2!=createTemp.end();)
		{
			int ID=(int)(*(iter2++));
			float x=*(iter2++);
			float y=*(iter2++);
			float z=*(iter2++);
			float angleX=*(iter2++);
			float angleY=*(iter2++);
			float angleZ=*(iter2++);
			Missile *missile=Missile::createWebMissile(x,y,z,angleX,angleY,angleZ);
			this->addChild(missile);
			missileMap.insert(std::make_pair(ID, missile));
		}
	}
}
//更新奖励
void Web3DLayer::updateAward(float dt)
{
	if(GameData::awardFlag)
	{
		GameData::awardFlag=false;
		//复制数据到临时数据
		std::vector<float> dataTemp=GameData::awardVec;
		GameData::awardFlag=true;
		if(dataTemp.size()%4!=0)
		{// id x y sort
			return;
		}
		//创建各种临时列表
		std::vector<float> createTemp;						//需要创建新奖励的数据列表
		std::vector<float>::iterator iter=dataTemp.begin();	//数据列表的迭代器
		//遍历更新数据，更改已经存在的奖励数据，将不存在的子弹数据保存。
		iter=dataTemp.begin();
		for(;iter!=dataTemp.end();)
		{
			int ID=(int)(*iter);
			std::map<int, Award *>::iterator iterator=awardMap.find(ID);
			if(iterator!=awardMap.end())					//如果数据存在，更新位置数据
			{
				Award *award=iterator->second;
				iter++;
				award->setPositionX(*(iter++));
				award->setPositionY(*(iter++));
				iter++;
				award->liveFlag=1;
				continue;
			}else
			{
				for(int i=0;i<4;i++)
				{
					createTemp.push_back(*(iter++));
				}
			}
		}
		//遍历子弹列表，将无数据更新的子弹删除。
		std::map<int, Award*>::iterator deleteTemp=awardMap.begin();				//需要删除的子弹列表
		for(;deleteTemp!=awardMap.end();)
		{
			Award* award=deleteTemp->second;
			award->liveFlag--;
			if(award->liveFlag<0)
			{
				award->removeFromParent();
				deleteTemp=awardMap.erase(deleteTemp);
			}else
			{
				deleteTemp++;
			}
		}
		//遍历数据列表，创建新的子弹。
		std::vector<float>::iterator iter2=createTemp.begin();
		for(;iter2!=createTemp.end();)
		{
			int ID=(int)(*(iter2++));
			Award *award=Award::webCreate(Point(*(iter2++),*(iter2++)),*(iter2++));
			this->addChild(award);
			awardMap.insert(std::make_pair(ID, award));
		}
	}
}
//获取世界坐标
void Web3DLayer::getAnimPoint(Vec3 near,Vec3 far)
{
	float x0=near.x;
	float y0=near.y;
	float z0=near.z;
	float x1=far.x;
	float y1=far.y;
	float z1=far.z;

	float t=-z0/(z1-z0);
	float x=x0+(x1-x0)*t;
	float y=y0+(y1-y0)*t;
	animPoint=Vec2(x,y);
}
//触摸开始
bool Web3DLayer::onMyTouchBegan(Touch* touch, Event* event)
{
	if(touchFlag){								//正在触摸中
		return false;
	}else{										//已经抬起
		touchFlag=true;
	}
	bool isPlaneLift;							//飞机是否活着
	if(GameData::playerNumber==0){
		isPlaneLift=GameData::isRedLift;
	}else{
		isPlaneLift=GameData::isGreenLift;
	}
	if(isPlaneLift&&GameData::isPlaneCanMove&&(!Constant::touchFlag)){	//判断触摸点是否在目标的范围内
		GameData::isPlaneCanMove=false;
		auto target = static_cast<Sprite*>(	event->getCurrentTarget());//获取当前触摸对象，并转化为精灵类型
		Point location = target->convertToNodeSpace(touch->getLocation());//获取当前坐标
		//获取飞机的位置
		float planeX,planeY;
		if(GameData::playerNumber==0){
			planeX=playerPlane->getPositionX();
			planeY=playerPlane->getPositionY();
		}else{
			planeX=housePlane->getPositionX();
			planeY=housePlane->getPositionY();
		}

		Size size = Director::getInstance()->getWinSize();
		Vec3 nearP(location.x, size.height-location.y-100, -1.0f);
		Vec3 farP(location.x, size.height-location.y-100, 0.0f);

		camera->unproject(size, &nearP, &nearP);
		camera->unproject(size, &farP, &farP);
		getAnimPoint(nearP,farP);						//获取世界坐标系
		//拖尾效果===============
		//参一为消隐动画时长，参二为拖尾条带相邻顶点间的最小距离,参三为拖尾条带的宽度,参四为顶点颜色值，参五为所使用的纹理
//		myStreak = MotionStreak::create(0.15, 1, 15, Color3B(255,255,200), GAME_PATH+"streak.png");//MotionStreak::create(0.4, 1, 17, Color3B(255,255,255), pics_PATH+"streak.png")
//		myStreak->setBlendFunc(BlendFunc::ADDITIVE);//设置混合
//		this->addChild(myStreak,20);//将myStreak添加到布景中
		//设置此布景层渲染用摄像机
//		myStreak->setCameraMask((unsigned short)CameraFlag::USER1);
		location=animPoint;								//设置世界坐标系
		int directionLeft,directionFront;
		if((location.x-planeX)>1){
			directionLeft=1;							//右飞
		}else{
			directionLeft=-1;							//左飞
		}
		if((location.y-planeY)>0){						//向前倾斜90-
			directionFront=-1;
		}else if((planeY-location.y)<450){				//向后倾斜90+
			directionFront=1;
		}else{
			directionFront=0;							//后空翻
		}
		BNSocketUtil::sendInt(GameData::SEND_PLANEBEGAN);//发送飞机信息
		BNSocketUtil::sendInt(directionLeft);
		BNSocketUtil::sendInt(directionFront);
		BNSocketUtil::sendFloat(animPoint.x);			//目标点
		BNSocketUtil::sendFloat(animPoint.y);
	}
	return true;
}
//触摸
void Web3DLayer::onMyTouchMoved(Touch *touch, Event *event)
{
	bool isPlaneLift;							//飞机是否活着
	if(GameData::playerNumber==0){
		isPlaneLift=GameData::isRedLift;
	}else{
		isPlaneLift=GameData::isGreenLift;
	}
	if(isPlaneLift&&GameData::isPlaneCanMove&&(!Constant::touchFlag)&&touchFlag){
		GameData::isPlaneCanMove=false;						//移动标志位设置false
		auto target = static_cast<Sprite*>(	event->getCurrentTarget());//获取当前触摸对象，并转化为精灵类型
		Point location = target->convertToNodeSpace(touch->getLocation());//获取当前坐标
		Point delta=touch->getDelta();

		Size size = Director::getInstance()->getWinSize();
		Vec3 nearP(location.x, size.height-location.y-100, -1.0f);
		Vec3 farP(location.x, size.height-location.y-100, 0.0f);
		camera->unproject(size, &nearP, &nearP);
		camera->unproject(size, &farP, &farP);
		getAnimPoint(nearP,farP);
		location=animPoint;
		//获取飞机的位置
		float planeX,planeY;
		if(GameData::playerNumber==0){
			planeX=playerPlane->getPositionX();
			planeY=playerPlane->getPositionY();
		}else{
			planeX=housePlane->getPositionX();
			planeY=housePlane->getPositionY();
		}
		int directionLeft,directionFront;
		if((fabs(delta.x)>1||fabs(delta.y)>1)){

			if((delta.x)>0){
				directionLeft=1;				//右飞
			}else{
				directionLeft=-1;				//左飞
			}
			if(delta.y>0){							//向前倾斜  	90-
				directionFront=-1;
			}else if((delta.y)>-50){				//向后倾斜  90+
				directionFront=1;
			}else{
				directionFront=0;					//后空翻
			}
			BNSocketUtil::sendInt(GameData::SEND_PLANEMOVE);//发送飞机信息
			BNSocketUtil::sendFloat(animPoint.x);				//目标点
			BNSocketUtil::sendFloat(animPoint.y);

}}}

void Web3DLayer::onMyTouchEnded(Touch *touch, Event *event)
{
	touchFlag=false;
}
//重力感应器
void Web3DLayer::onMyAcceleration(Acceleration* acc, Event* unused_event)
{
	if(GameData::isPlaneCanMove&&(Constant::touchFlag)){
		GameData::isPlaneCanMove=false;
		float animPointX,animPointY;
		float planeX,planeY;
		if(GameData::playerNumber==0){					//玩家位置
			planeX=playerPlane->getPositionX();
			planeY=playerPlane->getPositionY();
		}else{											//房主位置
			planeX=housePlane->getPositionX();
			planeY=housePlane->getPositionY();
		}
		if((acc->x+0.6)>0){								//获取重力感应偏移量
			animPointX=-acc->y*(50+Constant::sensitivity);
			animPointY=(acc->x+0.6)*(50+Constant::sensitivity/2);
		}else{
			animPointX=-acc->y*(50+Constant::sensitivity);
			animPointY=(acc->x+0.6)*(50+Constant::sensitivity);
		}
		CCLOG("---------------animPointX=%f",animPointX);
		CCLOG("---------------animPointY=%f",animPointY);
		if(abs(animPointX)<=4&&abs(animPointY)<=4)
		{

		}else{
			animPointX=animPointX+planeX;					//获取目标点
			animPointY=animPointY+planeY;
			if(animPointX>540){
					animPointX=540;
				}else if(animPointX<0){
					animPointX=0;
				}
				if(animPointY>1400){
					animPointY=1400;
				}else if(animPointY<0){
					animPointY=0;
				}
			BNSocketUtil::sendInt(GameData::SEND_PLANEMOVE);//发送飞机信息
			BNSocketUtil::sendFloat(animPointX);			//目标点
			BNSocketUtil::sendFloat(animPointY);
		}
	}
}
