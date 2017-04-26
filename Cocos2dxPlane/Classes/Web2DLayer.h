#ifndef __Web2DLayer_H__
#define __Web2DLayer_H__

#include "GameSceneManager.h"
#include "Web3DLayer.h"
#include "GameSet.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;


class Web2DLayer : public cocos2d::Layer
{
	public:
	static Web2DLayer* getInstance();
	GameSceneManager* gsm;
	LabelAtlas* goldLabel;
	LabelAtlas *attackLabel;	//����
	LabelAtlas *protectLabel;	//����

	Web3DLayer * layer3D;
	GameSet *gameSet;

	MenuItemImage* continueGame;
	MenuItemImage* gameset;
	MenuItemImage* mainMenu;
	//��ʼ���ķ���
	virtual bool init();
	void completeGame(int flag);		//������Ϸ
	void pauseGame();			//��ͣ��Ϸ
	void sharkInjuredBG();			//�������˵ı���
	void setVisibleBG();			//���ر���
	void initPauseCode();		//��ʼ����ͣ����
	void continueCallback();	//�ص���Ϸ�ص�����
    CREATE_FUNC(Web2DLayer);
private:
    Sprite3D* gold;
	Sprite* pauseBG;
	Sprite* continuePauseBG;
	Sprite* waitting;
	MenuItemImage *deadBigBG;
	Sprite *deadSmallBG;
	Sprite3D *coin;
	Sprite *cloud[4];					//�Ʋ�

	Sprite *injureBG;				//���˵ı���
	Sprite *progress;
	LoadingBar *loadingbar;		// �ӵ��Ľ�����
	LoadingBar *loadingbar_plane;			// ��ҵĽ�����
	LoadingBar *progressOPlane;		//���ŵĽ�����
	Menu* menu;

    float angle=0;
	void goPause();				//��ת����ͣ����
	void update();				//��ʱ�ص�--�����ת
	void continueAnim();		//�ص���Ϸ�ĵ���ʱ
	void voiceChangeCallback();	//�����ı�ص�����
	void goMainMenu();			//��ת�����˵�����
	void createCloud();			//������Ⱥ
	void setGame();
	void changePro();			// �ı����
	void useSkill(int number);			//ʹ�ü���
};
class CompleteGameLayer : public Layer
{
public :
	virtual bool init();		//��ʼ������
	static CompleteGameLayer* create(int _flag);
private:
    void goMainMenu();			//��ת�����˵�����
    void createBoard();			//��Ϸ�����
    void moveBoard();			//�ƶ������
    void updateCoin();			//���½������
	void goNext();				//����һ��
	void goSelf();				//���汾��
    void setMenu();				//ִ�ж���
    void setGameDate();			//������Ϸ����
    void updateWeb();			//��ط���
    void menuCallback(int index);	//�˵��ص�����
    Web3DLayer * layer3D;		//3Dlayer
    Sprite *board;
    LabelAtlas* goldLabel;
    int coinCounter=0;
    int coinStep=0;
    int flag;
};

#endif
