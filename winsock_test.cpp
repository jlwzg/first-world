#if 0

/*
#    Copyright By Schips, All Rights Reserved
#    代码链接来源：https://www.cnblogs.com/huty/p/8517279.html
#
#    File Name:  group_client.c
#    功能：组播客户端
*/
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#include <tchar.h>
#include<ws2tcpip.h>
#include <iostream>

using namespace std;
class CTimer
{
public:
    CTimer(void);
    ~CTimer(void);

    int time_in();
    double time_out();

private:
    LARGE_INTEGER litmp;
    LONGLONG qt1, qt2;
    double dft, dff, dfm;
};

CTimer::CTimer(void)
{
}


CTimer::~CTimer(void)
{
}

int CTimer::time_in()
{
    QueryPerformanceFrequency(&litmp);//获得时钟频率
    dff = (double)litmp.QuadPart;

    QueryPerformanceCounter(&litmp);//获得初始值
    qt1 = litmp.QuadPart;

    return 1;
}

double CTimer::time_out()
{
    QueryPerformanceCounter(&litmp);//获得终止值
    qt2 = litmp.QuadPart;

    dfm = (double)(qt2 - qt1);
    dft = dfm / dff;//获得对应的时间值

    return dft;
}

char HEX[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };


int main()
{

    //unsigned char f[] = {0x34,0xc4,0x12};
    //int a = (unsigned)f[0];
    //int b = (unsigned)f[1];
    //int c = (unsigned)f[2];

    //std::cout << a << std::endl;
    //std::cout << b << std::endl;
    //std::cout << c << std::endl;

    //return 0;
    //1.windows下要先初始化套接字
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("初始化套接字失败！\n");
        return -1;
    }
    printf("初始化套接字成功！\n");

    //2.建立客户端SOCKET
    SOCKET client;
    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client == INVALID_SOCKET)
    {
        printf("建立客户端套接字失败; %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    printf("建立客户端套接字成功！\n");

    sockaddr_in serveraddress;		//服务器地址，该变量用于存储recvfrom()捕获的数据发送源的地址

    //3.绑定端口（接收端需要绑定，发送端不需要绑定）
    //client = socket(AF_INET, SOCK_DGRAM, 0);		// 接收组播socket

    sockaddr_in  AddrClient;

    AddrClient.sin_family = AF_INET;
    AddrClient.sin_addr.s_addr = inet_addr("192.168.111.66");	
    AddrClient.sin_port = htons(5500);//组播监听端口号，需根据实际组播端口号设定


    int bufferSize = 64 * 1024;  // 设置缓冲区大小为 1MB
    if (setsockopt(client, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&bufferSize), sizeof(bufferSize)) == SOCKET_ERROR) {
        closesocket(client);
        WSACleanup();
        return 0;
    }

    int ret = bind(client, (sockaddr*)&AddrClient, sizeof(sockaddr_in));
    if (ret == SOCKET_ERROR)
    {
        printf("绑定广播接收端口1错误！\n");
    }


    //4.加入组播
    //struct ip_mreq mreq;
    //memset(&mreq, 0, sizeof(struct ip_mreq));
    //mreq.imr_multiaddr.S_un.S_addr = inet_addr("224.168.0.1");    //组播源地址
    //mreq.imr_interface.S_un.S_addr = inet_addr("192.168.111.66");	       //本地地址	// 将要添加到多播组的 IP，类似于 成员号
    //int m = setsockopt(client, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR*) & mreq, sizeof(mreq));	//关键函数，用于设置组播、链接组播。此时套接字client已经连上组播
    //if (m == SOCKET_ERROR)
    //{
    //    perror("setsockopt");
    //    return -1;
    //}



    //创建接收数据缓冲区
    unsigned char recvbuf[1024];  //回头注意重新设定缓冲区大小
    int n;
    DWORD dwWrite;    //DWORD在windows下常用来保存地址（或者存放指针）
    BOOL bRet;
    int len = sizeof(sockaddr_in);
    int count_recv = 0;
    int last_num = 0;
    bool once = true;
    //unsigned char ch_num;

    //int tempf = 0;

    //创建文件
    //HANDLE hFile = CreateFile(_T("接收数据.txt"), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    //if (hFile != INVALID_HANDLE_VALUE)
    //{
    //    printf("创建文件成功!\n");
    //}

    CTimer timer;
    timer.time_in();
#if 0
    while (1)//用两个while循环消除once这个变量这样第二个while循环last_num肯定都是从0开始的。
    {
        n = recvfrom(client, (char *)recvbuf, sizeof(recvbuf), 0, (sockaddr*)&serveraddress, &len);	//从组播socket接收数据
       /* if (n == SOCKET_ERROR)
        {
            printf("recvfrom error:%d\n", WSAGetLastError());
            printf("接收数据错误！\n");
        }*/
        last_num++;
        if(once)
        {
            last_num = recvbuf[2];
            once = false;
            std::cout << "once:  " << last_num << std::endl;
        }
        else
        {
            int hex = recvbuf[2];
	        if(last_num != hex)
	        {
                std::cout << "error:  " << last_num << "recvbuf"  << hex << std::endl;
                std::cout << "time :  " << timer.time_out() << std::endl;

                break;
	        }

            if (last_num == 255)
                last_num = -1;
        }
       /* printf("-%d-", n);*/

        //将接收到的数据写到hFile中
        //bRet = WriteFile(hFile, recvbuf, n, &dwWrite, NULL);		//如果有写数据错误，则跳出循环；否则一直接收数据
        //if (bRet == FALSE)
        //{
        //    MessageBox(NULL, _T("Write Buf ERROR!"), _T("ERROR"), MB_OK);
        //    break;
        //}
       /* if(count_recv == 0)
            timer.time_in();

        count_recv++;

        if(count_recv == 40000)
        {
            std::cout << timer.time_out() << std::endl;
            count_recv = 0;
        }*/
    }
#endif

    int tempi = 0;
    while (1)//用两个while循环消除once这个变量这样第二个while循环last_num肯定都是从0开始的。
    {
        n = recvfrom(client, (char*)recvbuf, sizeof(recvbuf), 0, (sockaddr*)&serveraddress, &len);	//从组播socket接收数据
        tempi = recvbuf[2];
        if (tempi == 255)
        {
            break;
        }
    }

    int  nn = 0;
    while (1)//用两个while循环消除once这个变量这样第二个while循环last_num肯定都是从0开始的。
    {
        n = recvfrom(client, (char*)recvbuf, sizeof(recvbuf), 0, (sockaddr*)&serveraddress, &len);	//从组播socket接收数据
        tempi = recvbuf[2];
        if (tempi == 78 || tempi == 79)
        {
            std::cout << tempi << std::endl;
        }
        //if(tempi != last_num)
        //{
        //    std::cout << "error:  " << last_num << "recvbuf" << tempi << std::endl;
        //    last_num = tempi;
        //    //std::cout << "time :  " << timer.time_out() << std::endl;
        //}
        //if (last_num == 255)
        //    last_num = -1;

        //last_num++;
        nn++;
        if (nn == 40000)
            break;
    }

    //传送成功
    //MessageBox(NULL, _T("Receive file OK!"), _T("OK"), MB_OK);

    closesocket(client);
    WSACleanup();
    system("pause");
    return 0;
}

#endif