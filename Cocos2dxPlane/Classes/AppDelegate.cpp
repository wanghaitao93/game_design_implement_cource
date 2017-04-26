#include "AppDelegate.h"
#include "GameSceneManager.h"
#include "Constant.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

//初始化方法
bool AppDelegate::applicationDidFinishLaunching()
{
    //获取导演
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    //若不存在glview则重新创建
    if(!glview)
    {
    	glview = GLViewImpl::create("My Game");
    }
    //设置绘制用GLView
    director->setOpenGLView(glview);
//    //设置开启深度检测
//    director->setDepthTest(true);
    //设置目标分辨率,别的分辨率的屏幕将自动上下或左右留白进行多分辨率自适应
    glview->setDesignResolutionSize(540,960,ResolutionPolicy::SHOW_ALL);
	//设置为显示FPS等信息
    director->setDisplayStats(true);
    //系统模拟时间间隔
    director->setAnimationInterval(1.0 / 60);
    //读取数据
    this->readData();

    //创建欢迎场景
    GameSceneManager* gsm = new GameSceneManager();
    gsm->goBegin();
//    gsm->createScene();
//    //跌换到欢迎场景显示
//    director->runWithScene(gsm->openScene);
    return true;
}
//读取数据
void AppDelegate::readData()
{
	for(int i=0;i<3;i++)
	{
		Constant::attackGrade[i]=UserDefault::getInstance()->
			getIntegerForKey(StringUtils::format("attack%d",i).c_str());
		Constant::bloodGrade[i]=UserDefault::getInstance()->
			getIntegerForKey(StringUtils::format("blood%d",i).c_str());
		Constant::bulletsGrade[i]=UserDefault::getInstance()->
			getIntegerForKey(StringUtils::format("bullets%d",i).c_str());
		Constant::goldGrade[i]=UserDefault::getInstance()->
			getIntegerForKey(StringUtils::format("gold%d",i).c_str());
	}
	Constant::money=UserDefault::getInstance()->getIntegerForKey("money");
	Constant::money+=100000;
	Constant::canPlay=UserDefault::getInstance()->getIntegerForKey("canPlay");
	Constant::soundFlag=UserDefault::getInstance()->getIntegerForKey("soundFlag");		//声音是否播放
	Constant::canPlane=UserDefault::getInstance()->getIntegerForKey("canPlane");		//解锁的
}

//当程序进入后台时调用此方法
void AppDelegate::applicationDidEnterBackground()
{
	//停止动画
    Director::getInstance()->stopAnimation();
    //如果有声音的话要调用下面一句暂停声音播放
    //SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

//当程序进入前台时调用
void AppDelegate::applicationWillEnterForeground()
{
	//开始动画
    Director::getInstance()->startAnimation();
    //如果有声音的话要调用下面一句开始声音播放
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
