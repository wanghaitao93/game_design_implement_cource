#include "Constant.h"


//LoadingScene
bool Constant::soundLoaded=false;
bool Constant::particleLoaded=false;
bool Constant::soundFlag=true;				//声音是否开始

//孙策=========
int Constant::planeNumber=0;				//plane number 0 1 2
float Constant::plane002X=598.779;			//封装飞机的长宽高
float Constant::plane002Y=477.619;
float Constant::plane002Z=154.618;
//bool Constant::touchFlag=true;				//触摸开启、重力感应关闭，相反

//
/*************游戏中的数据**************
 * 半径数组：敌机和子弹123，boss123,玩机123、子弹、导弹
 * 尺寸数组：上面数据是否放大缩小
 * 伤害数组：敌机123号伤害相同、boss伤害、玩家伤害（可加成）、导弹（可加成）
 */
int Constant::radiusData[14]={1,2,3,1,2,3,1,2,3,1,2,3,1,1};
int Constant::scaleData[14]={1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int Constant::hurtData[4]={1,2,1,1};
//当前飞机子弹、导弹的临时伤害值
int Constant::bulletHurt=1;
int Constant::missileHurt=1;
//*************游戏中玩家、敌机、boss的血量数据
//int Constant::playerBlood[3]={7,7,7};
//int Constant::bossBlood[3]={20,30,40};
bool Constant::isPause=false;							//暂停标志
//end=============
//--------------------------------------------------LI
int Constant::attackGradePre[3]={1,2,3};					//基础攻击力等级					1---8
int Constant::bloodGradePre[3]={2,3,4};						//基础防御力等级					1---8
int Constant::bulletsGradePre[3]={1,1,2};					//基础攻速等级					1---8
int Constant::goldGradePre[3]={0,1,2};						//基础金币加成等级				1---8

int Constant::attackGrade[3]={0,0,0};					//攻击力等级					1---8
int Constant::bloodGrade[3]={0,0,0};					//防御力等级					1---8
int Constant::bulletsGrade[3]={0,0,0};					//攻速等级					1---8
int Constant::goldGrade[3]={0,0,0};						//金币加成等级				1---8
int Constant::score=0;									//玩家分数
int Constant::money=10000;								//玩家金钱
//--------------------------------------------------LI
//-----------------------------------------------------wang
std::string Constant::title[]={"\u7b2c\u4e00\u5173","\u7b2c\u4e8c\u5173","\u7b2c\u4e09\u5173"};
std::string Constant::titleC[]={"\u5b87\u5b99\u8bd5\u70bc","\u5730\u9762\u8ffd\u51fb","\u6700\u540e\u4e00\u6218"};
std::string Constant::content[]={"\u654c\u4eba\u5df2\u7ecf\u903c\u8fd1\u5927\u6c14\u5c42\uff0c\u4e3a\u4e86\u4fdd\u536b\u6211\u4eec\u5171\u540c\u7684\u5bb6\u56ed\uff0c\u62e6\u622a\u7684\u4efb\u52a1\u5c31\u4ea4\u7ed9\u4f60\u4eec\u4e86\u3002\u4f5c\u4e3a\u4e00\u540d\u65e0\u754f\u7684\u52c7\u8005\uff0c\u52a0\u5165\u6d88\u706d\u654c\u4eba\u7684\u6218\u6597\u5427\uff0c\u4e3a\u4e86\u6211\u4eec\u7684\u8363\u8a89\u800c\u6218\u5427\uff01",
								"\u96f6\u6563\u5e1d\u541b\u5df2\u7ecf\u51b2\u5165\u5730\u7403\uff0c\u6211\u4eec\u5df2\u7ecf\u77e5\u9053\u5b83\u4eec\u5728\u54ea\u91cc\u3002\u4f60\u53ea\u67091\u5929\u7684\u65f6\u95f4\u53bb\u62ef\u6551\u4eba\u7c7b\uff0c\u4e3a\u4e86\u8d76\u5728\u5b83\u4e4b\u524d\u5230\u8fbe\u4eba\u4eec\u7684\u5730\u65b9\uff0c\u4e3a\u4e86\u6211\u4eec\u5bb6\u56ed\uff0c\u53bb\u6d88\u706d\u4ed6\u4eec\u5427\uff01",
								"\u654c\u4eba\u5c31\u5728\u524d\u9762\uff0c\u8ba9\u6211\u4eec\u8fdb\u884c\u6700\u540e\u4e00\u640f\u3002\u76f8\u4fe1\u4f60\u81ea\u5df1\uff0c\u76f8\u4fe1\u4f60\u7684\u4f19\u4f34\uff0c\u62ef\u6551\u4eba\u7c7b\u7684\u5e0c\u671b\u5c31\u4ea4\u7ed9\u4f60\u4e86\u3002\u4f5c\u4e3a\u4e00\u540d\u65e0\u754f\u7684\u52c7\u8005\uff0c\u5411\u524d\u51b2\uff0c\u6d88\u706d\u5b83\u4eec\uff01"
								};
std::string Constant::planeName[]={"\u6597\u6218\u80dc\u673a",
		"\u66b4\u529b\u725b\u673a",
		"\u65e0\u754f\u66b4\u673a"};
std::string Constant::planeContent[]={};

int Constant::custom=0;					//当前关卡
int Constant::canPlay=0;				//能玩的
int Constant::planeId=0;				//飞机的编号
int Constant::canPlane=0;				//最多能选的飞机树
bool Constant::loaded=false;
bool Constant::touchFlag=false;			//false--触摸，true--重力感应
int Constant::planeHealth=200;			//玩家生命值
int Constant::sensitivity=50;			//重力感应灵敏度
int Constant::planeSkill[3][3]={{50,70,20},{70,80,40},{80,90,50}};	//飞机的性能
int Constant::bossSkill[3][3]={{50,70,40},{70,80,30},{80,90,80}};	//boss的性能


//-----------------新加
int Constant::bulletLevel=0;									//子弹多的等级12		0,1,2,3,4
int Constant::bulletPower=0;									//子弹的威力	31		0,1,2,3,4
int Constant::missileLevel=0;									//导弹的等级	13		0,1,2,3
int Constant::attackCount=0;										//大招的个数			0,1,2,3		最多3个
int Constant::protectCount=0;										//防御的等级			0,1,2,3		做多3个

int Constant::enemyBulletSpeed=1;								//敌机子弹的速度	0.1.2
Point Constant::planePoint=Point(270,180);							//飞机的位置
bool Constant::isBulletPower=false;								//子弹威力加成
int Constant::bulletPowerTime=0;								//子弹威力时间

bool Constant::openCustom=false;								//新开关卡
bool Constant::gameingFlag=false;								//是否在游戏中
//--------------------------------------------------------wang
