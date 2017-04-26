#include "PlaySound.h"
#include "AppMacros.h"
#include "Constant.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace std;

void PlaySound::playSound(int index)
{
	if(Constant::soundFlag){
		std::string soundName="";
		switch(index){
			case kButtonGo: soundName="buttomGo.mp3";break;
			case kMenuSure: soundName="sure.mp3";break;
			case kCancel: soundName="word.mp3";break;
			case kMenuSelect: soundName="select.mp3";break;
			case kMenuButton: soundName="button.mp3";break;
			case kBack: soundName="back.mp3";break;
			case kTransform: soundName="transform.mp3";break;
			case kPlane_go: soundName="plane_go.mp3";break;
			case kBoomS: soundName="boom_s.mp3";break;
			case kBoomB: soundName="boom_m.mp3";break;
			case kBullet: soundName="bullet.mp3";break;
			case kGetCoin: soundName="getCoin.mp3";break;
			case kWarning: soundName="warning.mp3";break;
		}
		playEffect(soundName);
	}


}
void PlaySound::playEffect(string soundName)
{
	if(Constant::soundFlag){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect
		(
			(std::string("sound/"+soundName)).c_str()
		);
	}

}
//���Ų˵�����ı�������
void PlaySound::playBackground()
{
	if(Constant::soundFlag){
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic
		(
			(std::string("sound/bgm_menu.mp3")).c_str(),-1
		);
	}

}
//������Ϸ�еı�������
void PlaySound::playBackgroundG()
{
	if(Constant::soundFlag){
		std::string soundName="";				//���ű������ֵ��ַ�
		switch(Constant::custom){
			case 0:soundName="bgm_boss.mp3";break;
			case 1:soundName="bgm_boss.mp3";break;
			case 2:soundName="bgm_boss.mp3";break;
			case 3:soundName="bgm_boss.mp3";break;			//����boss��������
		}
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic
		(
			(SOUND_PATH+std::string(soundName)).c_str(),-1
		);
	}
}
//��ͣ���ű�������
void PlaySound::pauseBackground()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
//�ָ����ű�������
void PlaySound::resumeBackground()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
