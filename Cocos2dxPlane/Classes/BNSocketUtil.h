#ifndef  _BNSocketUtil_H_
#define  _BNSocketUtil_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include "cocos2d.h"

#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/errno.h>

USING_NS_CC;

class BNSocketUtil
{
  public:

	static int socketHandle;
	static int counter;
	//是否连接服务器成功标志
	static bool connectFlag;
	//连接服务器的方法
	static void connect(const char* ip, unsigned short port);

	static void sendInt(int si);

	static char* receiveBytes(int len);

	static void sendFloat(float sf);

	static void sendStr(const char* str,int len);

	static int receiveInt();

	static float receiveFloat();

	static char* receiveStr();
	//连接任务方法，供线程执行
	static void* threadConnectTask(const char* ip, unsigned short port);
	//接收任务方法，供线程执行
	static void* threadReceiveTask();

	static void closeConnect();

};

#endif
