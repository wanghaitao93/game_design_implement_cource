#ifndef  _GameData_H_
#define  _GameData_H_

#include "cocos2d.h"

class GameData
{
public:
	static const int SEND_CONNECT;				//������Ϸ������Ϣ
	static const int RECEIVE_OK;				//�������ӳɹ���Ϣ
	static const int RECEIVE_FULL;				//���շ���������Ϣ
	static const int SEND_LEVEL;				//���͹ؿ��л���Ϣ
	static const int RECEIVE_LEVEL;				//���չؿ��л���Ϣ
	static const int SEND_SELECT;				//���͹ؿ�ȷ����Ϣ����������һ����
	static const int RECEIVE_SELECT;			//���չؿ�ȷ����Ϣ
	static const int SEND_PLANE;				//���ͷɻ�ѡ����Ϣ
	static const int RECEIVE_PLANE;				//���շɻ�ѡ����Ϣ
	static const int SEND_START;				//���Ϳ�ʼ��Ϸ��Ϣ
	static const int RECEIVE_START;				//���տ�ʼ��Ϸ��Ϣ
	static const int SEND_KEY;					//���Ͱ�����Ϣ
	static const int SEND_STATE;				//������Ϸ״̬��Ϣ
	static const int RECEIVE_STATE;				//������Ϸ״̬��Ϣ
	static const int SEND_EXIT;					//�����˳���Ϣ
	static const int RECEIVE_COUNT;				//���շ�����������Ϣ
	static const int RECEIVE_EXIT;				//������Ϸ�˳���Ϣ


	static const int RECEIVE_EXPLOSION;			//���ձ�ըλ������
	static const int RECEIVE_MAINBULLET;		//������ҷɻ��ӵ�����
	static const int RECEIVE_PLANEDATA;			//������ҷɻ��ӵ�����
	static const int RECEIVE_ENEMYBULLET;		//���յл��ӵ�����
	static const int RECEIVE_AWARD;				//���ս�������
	static const int RECEIVE_ENEMYPLANE;		//���յл�����
	static const int RECEIVE_BOSSFLAG;			//����boss��������

	static const int SEND_PLANEBEGAN;			//���͵����Ļ����
	static const int SEND_PLANEMOVE;			//���ʹ����ƶ�����
	static const int RECEIVE_PLANEMOVE;			//���շɻ��ƶ���Ŀ����Լ�����Ƕ�
	static const int RECEIVE_PLANECANMOVE;		//���շɻ��Ƿ�����ƶ�
	static const int SEND_STARTACTION;			//���ͷɻ��Ŀ�ʼ״̬
	static const int RECEIVE_MISSILEDATA;		//���͵�������

	static const int SEND_CONTINUE;				//���ͼ���״̬
	static const int RECEIVE_CANLEVEL;			//��������ؿ�������
	static const int SEND_USESKILL;				//ʹ�ü���
	static const int RECEIVE_USESKILL;			//���ռ���

	static const char* gameData[];

	static bool isPause;
	static bool serviceFull;
	static int level;
	static int oldLevel;
	static bool SelectedGame;			//�Ƿ�ѡ����ؿ�
	static int playerNumber;			//0--���ˣ�1--����
	static int playerCount;
	static bool connectFlag;
	static int playerPlaneId;			//0-- wanjia ,1--����
	static int housePlaneId;			//�����ɻ�Id
	static int oldPlayerPlaneId;		//0-- wanjia ,1--����
	static int oldHousePlaneId;			//�����ɻ�Id
	static int housePrepare;			//����׼�������
	static int playerPrepare;			//���׼�������
	static int canPlay;					//��������Ĺؿ�
	static int redAttackCount;
	static int greenAttackCount;
	static int redProtectCount;
	static int greenProtectCount;
	static bool isRedAttack;			//��ɫ�ɻ��Ĵ���
	static bool isGreenAttack;			//��ɫ�ɻ��Ĵ���
	static bool isRedProtect;			//��ɫ�ɻ��ı���
	static bool isGreenProtect;			//��ɫ�ɻ��ı���

	//----------LI----------------
	static std::vector<float> mainBulletVec;		//����ӵ���������
	static std::vector<float> enemyBulletVec;		//�л��ӵ���������
	static std::vector<float> enemyVec;				//�л���������
	static std::vector<float> explosionVec;			//��ը��������
	static std::vector<float> awardVec;				//������������
	static std::vector<float > missileVec;			//������������

	//���ݴ����־λ      0δ����   1�ѽ���δ����   2������
	static bool explosionFlag;						//��ը���ݴ����־λ
	static bool mainBulletFlag;						//����ӵ����ݴ����־λ
	static bool planeDataFlag;						//��ҷɻ����ݴ����־λ
	static bool enemyBulletFlag;					//�л��ӵ����ݴ����־λ
	static bool awardFlag;							//�������ݴ����־λ
	static bool enemyFlag;							//�л����ݴ����־λ
	static bool missileFlag;						//�������ݴ����־λ

	static int gameState;							//��Ϸ��־λ
	static int whoPause;							//��˭��ͣ��

	static const int GAME_WAIT;
	static const int GAME_START;
	static const int GAME_PAUSE;
	static const int GAME_WIN;
	static const int GAME_LOST;

	// ��Ϸ2D����
	static int oldSelfScore;			//�ϵķ���
	static int totalScore;				//�ܷ�
	static int money;					//��Ǯ
	static float oldPlaneHealth;		//�Լ���Ѫ��
	static float redPlaneHealth;		//��ҷɻ���Ѫ��
	static float greenPlaneHealth;		//������ҵ�Ѫ��
	//��Ϸ3D����
	static float playerPlaneX;			//��ҷɻ���X����
	static float playerPlaneY;			//��ҷɻ���Y����
	static float housePlaneX;			//�����ɻ���X����
	static float housePlaneY;			//�����ɻ���Y����

	static bool isPlaneCanMove;			//�Ƿ�ɻ����ƶ�
	static bool isChangePlaneMove;
	static bool isRedLift;
	static bool isGreenLift;

	static std::vector<float> planeData;

	static void reset();

};

#endif
