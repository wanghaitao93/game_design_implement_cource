#ifndef __GameSceneManager_H__
#define __GameSceneManager_H__

#include "cocos2d.h"
using namespace cocos2d;
//���ڴ�����������
class GameSceneManager
{
public:
	//������������ķ���
//    static cocos2d::Scene* createScene();
	Scene* openScene;
	static Camera* getCamera();
	//������������ķ���
    void createScene();
    static void goMainMenu();
    void goWeapon();
    void goSelectPlane();
    void goChoose();
    void goGame();			//��Ϸģʽ����

    void deleteNetLayer();
    void goHelp();
    void goSelectGame();
    void goFirst();
    void goWeb();
    void goBegin();
    void goLoading();
    void initShadersAnim(Sprite3D *sprite,std::string Texture);
};

#endif
