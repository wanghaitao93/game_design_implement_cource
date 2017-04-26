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

//��ʼ������
bool AppDelegate::applicationDidFinishLaunching()
{
    //��ȡ����
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    //��������glview�����´���
    if(!glview)
    {
    	glview = GLViewImpl::create("My Game");
    }
    //���û�����GLView
    director->setOpenGLView(glview);
//    //���ÿ�����ȼ��
//    director->setDepthTest(true);
    //����Ŀ��ֱ���,��ķֱ��ʵ���Ļ���Զ����»��������׽��ж�ֱ�������Ӧ
    glview->setDesignResolutionSize(540,960,ResolutionPolicy::SHOW_ALL);
	//����Ϊ��ʾFPS����Ϣ
    director->setDisplayStats(true);
    //ϵͳģ��ʱ����
    director->setAnimationInterval(1.0 / 60);
    //��ȡ����
    this->readData();

    //������ӭ����
    GameSceneManager* gsm = new GameSceneManager();
    gsm->goBegin();
//    gsm->createScene();
//    //��������ӭ������ʾ
//    director->runWithScene(gsm->openScene);
    return true;
}
//��ȡ����
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
	Constant::soundFlag=UserDefault::getInstance()->getIntegerForKey("soundFlag");		//�����Ƿ񲥷�
	Constant::canPlane=UserDefault::getInstance()->getIntegerForKey("canPlane");		//������
}

//����������̨ʱ���ô˷���
void AppDelegate::applicationDidEnterBackground()
{
	//ֹͣ����
    Director::getInstance()->stopAnimation();
    //����������Ļ�Ҫ��������һ����ͣ��������
    //SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

//���������ǰ̨ʱ����
void AppDelegate::applicationWillEnterForeground()
{
	//��ʼ����
    Director::getInstance()->startAnimation();
    //����������Ļ�Ҫ��������һ�俪ʼ��������
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
