#include "GameData.h"

const int GameData::SEND_CONNECT=1;					//������Ϸ������Ϣ
const int GameData::RECEIVE_OK=2;					//�������ӳɹ���Ϣ
const int GameData::RECEIVE_FULL=3;					//���շ���������Ϣ
const int GameData::SEND_LEVEL=4;					//���͹ؿ��л���Ϣ
const int GameData::RECEIVE_LEVEL=5;				//���չؿ��л���Ϣ
const int GameData::SEND_SELECT=6;					//���͹ؿ�ȷ����Ϣ����������һ����
const int GameData::RECEIVE_SELECT=7;				//���չؿ�ȷ����Ϣ
const int GameData::SEND_PLANE=8;					//���ͷɻ�ѡ����Ϣ
const int GameData::RECEIVE_PLANE=9;				//���շɻ�ѡ����Ϣ
const int GameData::SEND_START=10;					//���Ϳ�ʼ��Ϸ��Ϣ
const int GameData::RECEIVE_START=11;				//���տ�ʼ��Ϸ��Ϣ
const int GameData::SEND_KEY=12;					//���Ͱ�����Ϣ
const int GameData::SEND_STATE=13;					//������Ϸ״̬��Ϣ
const int GameData::RECEIVE_STATE=14;				//������Ϸ״̬��Ϣ
const int GameData::SEND_EXIT=15;					//�����˳���Ϣ
const int GameData::RECEIVE_COUNT=16;				//���շ�����������Ϣ
const int GameData::RECEIVE_EXIT=17;				//������Ϸ�˳���Ϣ

const int GameData::RECEIVE_EXPLOSION=18;			//���ձ�ըλ������
const int GameData::RECEIVE_MAINBULLET=19;			//������ҷɻ��ӵ�����
const int GameData::RECEIVE_PLANEDATA=20;			//������ҷɻ��ӵ�����
const int GameData::RECEIVE_ENEMYBULLET=21;			//���յл��ӵ�����
const int GameData::RECEIVE_AWARD=22;				//���ս�������
const int GameData::RECEIVE_ENEMYPLANE=23;			//���յл�����
const int GameData::RECEIVE_BOSSFLAG=24;			//����boss��������
const int GameData::SEND_PLANEBEGAN=25;				//���ʹ�����ʼ�ɻ�����
const int GameData::SEND_PLANEMOVE=26;				//���ʹ����ƶ�����
const int GameData::RECEIVE_PLANEMOVE=27;			//���շɻ�Ŀ���
const int GameData::RECEIVE_PLANECANMOVE=28;		//���շɻ��Ƿ�����ƶ�
const int GameData::SEND_STARTACTION=29;			//���Ϳ�ʼ״̬
const int GameData::RECEIVE_MISSILEDATA=30;			//���յ�������
const int GameData::SEND_CONTINUE=31;				//���ͼ�����Ϸ״̬
const int GameData::RECEIVE_CANLEVEL=32;			//�����µĹؿ�����Ϣ
const int GameData::SEND_USESKILL=33;				//ʹ�ü���
const int GameData::RECEIVE_USESKILL=34;			//���ռ���

const char* GameData::gameData[]=
{
	"SEND_CONNECT","RECEIVE_OK","RECEIVE_FULL","SEND_LEVEL",
	"RECEIVE_LEVEL","SEND_SELECT","RECEIVE_SELECT","SEND_PLANE",
	"RECEIVE_PLANE","SEND_START","RECEIVE_START","SEND_KEY",
	"SEND_STATE","RECEIVE_STATE","SEND_EXIT","RECEIVE_COUNT",
	"RECEIVE_EXIT",

	"RECEIVE_EXPLOSION","RECEIVE_MAINBULLET","RECEIVE_PLANEDATA",
	"RECEIVE_ENEMYBULLET","RECEIVE_AWARD","RECEIVE_ENEMYPLANE",

	"RECEIVE_BOSSFLAG","SEND_PLANEBEGAN","SEND_PLANEMOVE",
	"RECEIVE_PALNEMOVE","RECEIVE_PLANECANMOVE","SEND_STARTACTION",
	"RECEIVE_MISSILEDATA","SEND_CONTINUE","RECEIVE_CANLEVEL","SEND_USESKILL","RECEIVE_USESKILL"
};												//������Ϣ����

bool GameData::isPause=false;			//��ͣ
bool GameData::serviceFull=false;
int GameData::level=0;
int GameData::oldLevel=0;
bool GameData::SelectedGame=false;
int GameData::playerNumber=-1;					//0-- wanjia ,1--����
int GameData::housePlaneId=0;					//�����ɻ�Id
int GameData::playerPlaneId=0;					//��ҷɻ�Id
int GameData::oldHousePlaneId=0;				//�����ɻ�Id
int GameData::oldPlayerPlaneId=0;				//��ҷɻ�Id
int GameData::playerCount=0;
bool GameData::connectFlag=false;				//����
int GameData::housePrepare=0;					//������׼�����  1--��׼��
int GameData::playerPrepare=0;					//��ҵ�׼�����  1--��׼��
int GameData::canPlay=0;						//�������ؿ���ѡ��
int GameData::redAttackCount=0;
int GameData::greenAttackCount=0;
int GameData::redProtectCount=0;
int GameData::greenProtectCount=0;
bool GameData::isRedAttack=false;			//��ɫ�ɻ��Ĵ���
bool GameData::isGreenAttack=false;			//��ɫ�ɻ��Ĵ���
bool GameData::isRedProtect=false;			//��ɫ�ɻ��ı���
bool GameData::isGreenProtect=false;			//��ɫ�ɻ��ı���

//----------LI----------------
std::vector<float> GameData::mainBulletVec;		//����ӵ���������
std::vector<float> GameData::enemyBulletVec;	//�л��ӵ���������
std::vector<float> GameData::enemyVec;			//�л���������
std::vector<float> GameData::explosionVec;		//��ը��������
std::vector<float> GameData::awardVec;			//������������
std::vector<float> GameData::planeData;			//�ɻ���������
std::vector<float> GameData::missileVec;		//�ɻ���������

//���ݴ����־λ
bool GameData::explosionFlag=true;				//��ը���ݴ����־λ
bool GameData::mainBulletFlag=true;				//����ӵ����ݴ����־λ
bool GameData::planeDataFlag=true;				//��ҷɻ����ݴ����־λ
bool GameData::enemyBulletFlag=true;			//�л��ӵ����ݴ����־λ
bool GameData::awardFlag=true;					//�������ݴ����־λ
bool GameData::enemyFlag=true;					//�л����ݴ����־λ
bool GameData::missileFlag=true;				//�������ݴ����־λ
int GameData::gameState=0;						//��Ϸ��־λ

const int GameData::GAME_WAIT=0;
const int GameData::GAME_START=1;
const int GameData::GAME_PAUSE=2;
const int GameData::GAME_WIN=3;
const int GameData::GAME_LOST=4;
//-------------------WANG
//��Ϸ2D����
int GameData::oldSelfScore=0;				//�ϵ��Լ��ķ���
int GameData::totalScore=0;					//�ܷ�
int GameData::money=0;						//Ǯ
float GameData::oldPlaneHealth=200;			//�Լ����ϵ�Ѫ��
float GameData::redPlaneHealth=200;			//��ɫѪ��
float GameData::greenPlaneHealth=200;		//��ɫ�ɻ���Ѫ��
//3D
bool GameData::isPlaneCanMove=false;		//�ɻ��Ƿ�����ƶ�
bool GameData::isChangePlaneMove=false;		//�ɻ��Ƿ�����ƶ�
bool GameData::isRedLift=true;				//��ɫ�ɻ��Ƿ����
bool GameData::isGreenLift=true;			//��ɫ�ɻ��Ƿ����

void GameData::reset()						//��ʼ������
{
	isPause=false;
	level=0;
	oldLevel=0;
	SelectedGame=false;
	housePlaneId=0;
	playerPlaneId=0;
	oldHousePlaneId=0;
	oldPlayerPlaneId=0;
	housePrepare=0;
	playerPrepare=0;
	explosionFlag=true;
	mainBulletFlag=true;
	planeDataFlag=true;
	enemyBulletFlag=true;
	awardFlag=true;
	enemyFlag=true;
	missileFlag=true;
	gameState=0;
	oldSelfScore=0;
	totalScore=0;
	money=0;
	oldPlaneHealth=200;
	redPlaneHealth=200;
	greenPlaneHealth=200;
	isPlaneCanMove=false;
	isChangePlaneMove=false;
	isRedLift=true;
	isGreenLift=true;

	mainBulletVec.clear();
	enemyBulletVec.clear();
	enemyVec.clear();
	explosionVec.clear();
	awardVec.clear();
	planeData.clear();
	missileVec.clear();
}






