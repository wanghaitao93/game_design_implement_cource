#ifndef _LoadingLayer_H_
#define _LoadingLayer_H_

#define PRELOAD_NEWPLANE_COUNT 1
#define PRELOAD_ENEMYNEWPLANE_COUNT 3
#define PRELOAD_MISSILE_COUNT 1
#define PRELOAD_BOSS_COUNT 1
#include "GameSceneManager.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AppMacros.h"
using namespace cocos2d;
using namespace ui;

class LoadingLayer:public cocos2d::Layer
{
public:
    LoadingBar* loadingbar;	//进度条
    int curPreload_NewPlane_count=0;
    int curPreload_boss_count=0;
    int curPreload_missile_count=0;
    int curPreload_enemyNewPlane_count=0;
    int currentNum=0;			//已经加载的个数
    int totalNum=179;			//需要加载的总个数
    
    std::string picGameNames[35]={
        "attack_down.png","attack.png","bullets.png","coinAward1.png",
        "coinAward2.png","coinAward3.png","coinAward4.png","coinAward5.png",
        "coinAward6.png","coinAward7.png","coinAward8.png","coinAward9.png",
        "defen.png","game0.png","game1.png","game2.png",
        "gameTitle.png","gameTitle2.png","injuredBG.png","load.png",
        "loadingBack.png","numbers.png","nums.png","pause.png",
        "progress_game_bk.png","progress_game_track.png","protect_down.png","protect.png",
        "resumeNumber.png","sliderProgress_game.png","sliderTrack_game.png","streak.png",
        "transparent.png","warning0.png","warning1.png"
    };
    std::string picC3bNames[23]={
        "3p.png","boss2.jpg","boss3.jpg","caustics.png",
        "coin_blue.png","coin_gold.png","coin_gray.png","coin_gold.jpg",
        "dj11.png","dj12.png","dj13.png","dj21.png",
        "dj22.png","dj23.png","dj31.png","dj32.png",
        "dj33.png","enemy.png","plane01.jpg","plane02.jpg",
        "plane03.jpg","Missile.png","gun.jpg"
    };
    std::string picGameModelNames[10]={
        "close.png","gameModeBG.png","MenuBg.png","open.png",
        "setClose.png","setClose_down.png","sliderballnormal.png","sliderballpressed.png",
        "sliderProgress.png","sliderTrack.png"
    };
    std::string picHelpNames[6]={
        "back_btn_down.png","back_btn.png","buttomBG.png",
        "page.jpg","point1.png","point2.png"
    };
    std::string picMenuNames[45]={
        "background0.jpg","background1.jpg","background2.jpg",
        "bnum1.png","closeVoice_normal.png","closeVoice_pressed.png","close_btn.png",
        "close_btn_down.png","continue.png","continue_disabled.png","continue_down.png",
        "goMenu.png","goMenu_down.png","gameLose.png","gameSuccess.png",
        "goMenu_down.png","goMenu.png","help_btn.png","help_btn_down.png",
        "help.png","incomplete.png","openVoice_normal.png","openVoice_pressed.png",
        "pause_down.png","pf0.png","pf1.png","pf2.png",
        "pf3.png","pf4.png","pf5.png","reset_disabled.png",
        "reset_down.png","reset.png","set_btn_down.png","set_btn.png",
        "soundClosed_btn_down.png","soundClosed_btn.png",
        "soundOpened_btn_down.png","soundOpened_btn.png","start_btn.png","start.png",
        "strengthen_btn_down.png","strengthen_btn.png","strengthen_down.png","strengthen.png"
    };
    std::string picNetNames[17]={
        "background.png","bg.png","board.png","deadBig.png","deadSmall.png",
        "noConnected.png","onePlayer.png","progress_net_bk.png",
        "progress_net_track.png","servicerFull.png","twoPlayer_unenable.png",
        "twoPlayer.png","waiting.png","waittingConnect0.png","waittingConnect1.png","waittingConnect2.png",
        "waittingConnect3.png"
    };
    std::string picSgNames[16]={
        "blend.png","blendBig.png","blendBig1.png","blendSelect.png",
        "suo.png","changeGame.png","changeGameBG.png","fight_btn.png",
        "fight_btn_down.png","gameSelect.png","level1.png","level2.png",
        "level3.png","planeSelect.png","selectGameBG.jpg","tstars.png"
    };
    std::string picSelectNames[18]={
        "blendSmall.png","houseOwn.png","left_btn_down2.png","left_btnMove1.png",
        "plane0.png","plane1.png","plane2.png","planeContent.png","planeRotate.png",
        "planeRotateBG.png","player.png","right_btn_down2.png","right_btnMove1.png",
        "selectGameButtom.png","selectImageBig.png","selectImageSmall.png",
        "selectPlane.png","selectPlaneBG.jpg",
    };
    std::vector<std::string> objPath;
    GameSceneManager *gsm;
    Label* load_Percent;	//加载的百分比
    
    Sprite *loadingTitle;	//加载的文字
    Sprite *coverUp;		//加载背景上
    Sprite *coverDown;		//加载背景下
    virtual bool init();
    void InitBk();			//初始化加载界面e
    
    void printf(Sprite3D* sprite, void* index);		//打印加载的OBJ个数
    void LoadingResource();	//加载资源
    void LoadingPic();		//加载图片
    void LoadingCallback();	//更新资源加载中
    void LoadingOBJ();		//加载OBJ
    void GotoNextScene();	//跳转下一个界面
    
    CREATE_FUNC(LoadingLayer);
};
#endif
