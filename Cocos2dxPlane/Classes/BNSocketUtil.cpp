#include "BNSocketUtil.h"
#include "GameData.h"
#include "Web2DLayer.h"


int BNSocketUtil::socketHandle=0;
int BNSocketUtil::counter=0;

void BNSocketUtil::connect(const char* ip, unsigned short port)
{
	if(!GameData::connectFlag)
	{
		new std::thread(&BNSocketUtil::threadConnectTask,ip,port);
	}
}

void BNSocketUtil::sendInt(int si)
{
	 send(socketHandle,&si,4,0);
}

void BNSocketUtil::sendFloat(float sf)
{
	send(socketHandle,&sf,4,0);
}

void BNSocketUtil::sendStr(const char* str,int len)
{
	sendInt(len);
	send(socketHandle,str,len,0);
}

char* BNSocketUtil::receiveBytes(int len)
{
	char* result=new char[len];

	int status=0;
	status=recv(socketHandle, result, len, 0);
	if(status==0)
	{
		return "close";
	}

	while(status!=len)
	{
		int index=status;
		char b[len-status];
		int count=recv(socketHandle, b, len-status, 0);
		if(count==0)
		{
			return "close";
		}
		status=status+count;
		if(count!=0)
		{
			for(int i=0;i<count;i++)
			{
				result[index+i]=b[i];
			}
		}
	}
	return result;
}

int BNSocketUtil::receiveInt()
{
	char* a=receiveBytes(4);
	if(strcmp(a,"close")==0)
	{
		return 0;
	}
	int ri;

	memset(&ri, 0, sizeof(ri));
	memcpy((char*)(&ri), a,4);
    delete a;
	return ri;
}

float BNSocketUtil::receiveFloat()
{
	char* a=receiveBytes(4);
	if(strcmp(a,"close")==0)
	{
		return 0;
	}
	float ri;
	memset(&ri, 0, sizeof(ri));
	memcpy((char*)(&ri), a,4);
	delete a;
	return ri;
}

char* BNSocketUtil::receiveStr()
{
	int len=receiveInt();
	if(len==0)
	{
		return "0";
	}
	char* a=receiveBytes(len);
	if(strcmp(a,"close")==0)
	{
		return "0";
	}
	char* result=new char[len+1];
	memset(result, 0, len+1);
	memcpy(result, a,len);
	return result;
}

void  BNSocketUtil::closeConnect()
{
	if(GameData::connectFlag)
	{
		::close(socketHandle);
		GameData::connectFlag=false;
	}
}

void* BNSocketUtil::threadConnectTask(const char* ip, unsigned short port)
{
	struct sockaddr_in sa;
	struct hostent* hp;
	hp = gethostbyname(ip);
	if(!hp)
	{
	     return 0;
	}
	memset(&sa, 0, sizeof(sa));
	memcpy((char*)&sa.sin_addr, hp->h_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons(port);
	socketHandle = socket(sa.sin_family, SOCK_STREAM, 0);
	if(socketHandle < 0)
	{
		return 0;
	}
	if(::connect(socketHandle, (sockaddr*)&sa, sizeof(sa)) < 0)
	{
	    ::close(socketHandle);
	    return 0;
	}
    
    GameData::connectFlag=true;					//连接上
	new std::thread(&BNSocketUtil::threadReceiveTask);
	sendInt(GameData::SEND_CONNECT);
}

void* BNSocketUtil::threadReceiveTask()
{
	while(GameData::connectFlag)
	{
		int data=receiveInt();
		CCLOG("asdfDATA=%d",data);
		if(data==0)
		{
			closeConnect();
			break;
		}
//		CCLOG("ASDFdata=%s",GameData::gameData[data-1]);
		if(GameData::gameData[data-1]==nullptr)
		{
			break;
		}
		if(data==GameData::RECEIVE_FULL)
		{
			GameData::serviceFull=true;
		}
		if(data==GameData::RECEIVE_OK)//连接成功信息
		{
			GameData::serviceFull=false;
			GameData::playerNumber=receiveInt();			//玩家编号
			GameData::playerCount=receiveInt();				//玩家个数
            CCLOG("recept-----socket");
		}else if(data==GameData::RECEIVE_COUNT){
			GameData::playerCount=receiveInt();;					//玩家数量
		}else if(data==GameData::RECEIVE_LEVEL)				//选择关卡的信息
		{
			GameData::level=receiveInt();
		}else if(data==GameData::RECEIVE_SELECT){			//选择完关卡
			GameData::SelectedGame=true;
			GameData::level=receiveInt();
		}else if(data==GameData::RECEIVE_PLANE)				//接收房主，玩家选择的飞机
		{
			GameData::playerPlaneId=receiveInt();			//接收玩家的ID
			GameData::housePlaneId=receiveInt();			//接收房主的ID
		}else if(data==GameData::RECEIVE_START)				//开始
		{
			GameData::playerPrepare=receiveInt();			//接收玩家是否准备
			GameData::housePrepare=receiveInt();			//接收房主是否准备
		}else if(data==GameData::RECEIVE_PLANECANMOVE){		//是否可以移动
			if(GameData::playerNumber==receiveInt()){		//如果
				GameData::isPlaneCanMove=true;
			}
		}else if(data==GameData::RECEIVE_PLANEMOVE)			//接收飞机数据
		{
			int count=receiveInt();
			float a;
			std::vector<float> temp;
			for(int i=0;i<count;i++){
				a=receiveFloat();
				temp.push_back(a);
			}
			GameData::planeData=temp;
			GameData::isChangePlaneMove=true;				//改变飞机的状态
		}else if(data==GameData::RECEIVE_EXPLOSION)
		{
			int count=receiveInt();
			std::vector<float> temp;
			for(int i=0;i<count;i++)
			{
				temp.push_back(receiveFloat());
			}
			if(GameData::explosionFlag)
			{
				GameData::explosionFlag=false;
				GameData::explosionVec=temp;
				GameData::explosionFlag=true;
			}
		}else if(data==GameData::RECEIVE_USESKILL)		//接收技能
		{
			int serverNumber=receiveInt();				//接收是哪个飞机发过来的
			int sort=receiveInt();						//那个技能
			if(serverNumber==0)							//红色飞机
			{
				if(sort==0){
					GameData::isRedAttack=true;
					GameData::enemyBulletFlag=false;
					GameData::enemyBulletVec.clear();
					GameData::enemyBulletFlag=true;
					GameData::enemyFlag=false;
					GameData::enemyVec.clear();
					GameData::enemyFlag=true;
				}else if(sort==1){
					GameData::isRedProtect=true;
				}else if(sort==2){
					GameData::isRedProtect=false;
				}
			}else if(serverNumber==1)
			{
				if(sort==0){
					GameData::isGreenAttack=true;
					GameData::enemyBulletFlag=false;
					GameData::enemyBulletVec.clear();
					GameData::enemyBulletFlag=true;
					GameData::enemyFlag=false;
					GameData::enemyVec.clear();
					GameData::enemyFlag=true;
				}else if(sort==1){
					GameData::isGreenProtect=true;
				}else if(sort==2){
					GameData::isGreenProtect=false;
				}
			}
		}else if(data==GameData::RECEIVE_MISSILEDATA)
		{
			int count=receiveInt();
			std::vector<float> temp;
			for(int i=0;i<count;i++)
			{
				temp.push_back(receiveFloat());
			}
			if(GameData::explosionFlag)
			{
				GameData::missileFlag=false;
				GameData::missileVec=temp;
				GameData::missileFlag=true;
		}
		}else if(data==GameData::RECEIVE_MAINBULLET)
		{
			int count=receiveInt();
			std::vector<float> temp;
			for(int i=0;i<count;i++)
			{
				temp.push_back(receiveFloat());
			}
			if(GameData::mainBulletFlag)
			{
				GameData::mainBulletFlag=false;
				GameData::mainBulletVec=temp;
				GameData::mainBulletFlag=true;
			}
		}else if(data==GameData::RECEIVE_PLANEDATA)
		{
			int count=receiveInt();
			for(int i=0;i<count;i++)
			{
				receiveFloat();
			}
		}else if(data==GameData::RECEIVE_ENEMYBULLET)
		{
			int count=receiveInt();
			std::vector<float> temp;
			for(int i=0;i<count;i++)
			{
				temp.push_back(receiveFloat());
			}
			if(GameData::enemyBulletFlag)
			{
				GameData::enemyBulletFlag=false;
				GameData::enemyBulletVec=temp;
				GameData::enemyBulletFlag=true;
			}
		}else if(data==GameData::RECEIVE_AWARD)
		{
			int count=receiveInt();
			std::vector<float> temp;
			for(int i=0;i<count;i++)
			{
				temp.push_back(receiveFloat());
			}
			if(GameData::awardFlag)
			{
				GameData::awardFlag=false;
				GameData::awardVec=temp;
				GameData::awardFlag=true;
			}
		}else if(data==GameData::RECEIVE_ENEMYPLANE)
		{
			int count=receiveInt();
			std::vector<float> temp;
			for(int i=0;i<count;i++)
			{
				temp.push_back(receiveFloat());
			}
			if(GameData::enemyFlag)
			{
				GameData::enemyFlag=false;
				GameData::enemyVec=temp;
				GameData::enemyFlag=true;
			}
		}else if(data==GameData::RECEIVE_BOSSFLAG)
		{

		}else if(data==GameData::RECEIVE_STATE)
		{
			GameData::gameState=receiveInt();
		}else if(data==GameData::RECEIVE_CANLEVEL)			//接收能玩的关卡信息
		{
			GameData::canPlay=receiveInt();
			GameData::level=GameData::canPlay;
		}else if(data==GameData::RECEIVE_EXIT)
		{
			closeConnect();
			GameData::canPlay=0;
		}
	}
}

