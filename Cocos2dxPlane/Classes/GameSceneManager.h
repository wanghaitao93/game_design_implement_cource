#ifndef __GameSceneManager_H__
#define __GameSceneManager_H__

#include "cocos2d.h"
using namespace cocos2d;
//用于创建场景的类
class GameSceneManager
{
public:
	//创建场景对象的方法
//    static cocos2d::Scene* createScene();
	Scene* openScene;
	static Camera* getCamera();
	//创建场景对象的方法
    void createScene();
    static void goMainMenu();
    void goWeapon();
    void goSelectPlane();
    void goChoose();
    void goGame();			//游戏模式场景

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
