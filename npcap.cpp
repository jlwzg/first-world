/*
 * Copyright (c) 1999 - 2005 NetGroup, Politecnico di Torino (Italy)
 * Copyright (c) 2005 - 2006 CACE Technologies, Davis (California)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Politecnico di Torino, CACE Technologies
 * nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifdef _MSC_VER
/*
 * we do not want the warnings about the old deprecated and unsecure CRT functions
 * since these examples can be compiled under *nix as well
 */
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
//#include "Packet32.h"
#include <pcap.h>
#include <time.h>
#include <winsock.h>
//#include <iostream>


#pragma comment(lib,"ws2_32.lib")

#ifdef _WIN32
#include <tchar.h>

BOOL LoadNpcapDlls()
{
	_TCHAR npcap_dir[512];
	UINT len;
	len = GetSystemDirectory(npcap_dir, 480);
	if (!len)
	{
		fprintf(stderr, "Error in GetSystemDirectory: %x", GetLastError());
		return FALSE;
	}
	_tcscat_s(npcap_dir, 512, _T("\\Npcap"));
	if (SetDllDirectory(npcap_dir) == 0)
	{
		fprintf(stderr, "Error in SetDllDirectory: %x", GetLastError());
		return FALSE;
	}
	return TRUE;
}
#endif

/* 4 bytes IP address */
using ip_address = struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
};

/* IPv4 header */
using ip_header = struct ip_header
{
	u_char ver_ihl; // Version (4 bits) + Internet header length (4 bits)
	u_char tos; // Type of service 
	u_short tlen; // Total length 
	u_short identification; // Identification
	u_short flags_fo; // Flags (3 bits) + Fragment offset (13 bits)
	u_char ttl; // Time to live
	u_char proto; // Protocol
	u_short crc; // Header checksum
	ip_address saddr; // Source address
	ip_address daddr; // Destination address
	u_int op_pad; // Option + Padding
};

/* UDP header*/
using udp_header = struct udp_header
{
	u_short sport; // Source port
	u_short dport; // Destination port
	u_short len; // Datagram length
	u_short crc; // Checksum
};

/* prototype of the packet handler */
void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data);

int main()
{


	pcap_if_t* alldevs;
	pcap_if_t* d;
	int inum;
	int i = 0;
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	char packet_filter[] =
		"udp and src host 192.168.111.36 and src port 5500 and dst host 192.168.111.66 and dst port 5500";
	struct bpf_program fcode_w;

#ifdef _WIN32
	/* Load Npcap and its functions. */
	if (!LoadNpcapDlls())
	{
		fprintf(stderr, "Couldn't load Npcap\n");
		exit(1);
	}
#endif

	/* Retrieve the device list */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* Print the list */
	char destination[128]; // 目标数组

	for (d = alldevs; d; d = d->next)
	{
		printf("%d. %s", ++i, d->name);

		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if (i == 0)
	{
		printf("\nNo interfaces found! Make sure Npcap is installed.\n");
		return -1;
	}

	printf("Enter the interface number (1-%d):", i);
	scanf("%d", &inum);

	/* Check if the user specified a valid adapter */
	if (inum < 1 || inum > i)
	{
		printf("\nAdapter number out of range.\n");

		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

	/* Open the adapter */
	if ((adhandle = pcap_open_live(d->name, // name of the device
	                               65536, // portion of the packet to capture. 
	                               // 65536 grants that the whole packet will be captured on all the MACs.
	                               1, // promiscuous mode (nonzero means promiscuous)
	                               1000, // read timeout
	                               errbuf // error buffer
	)) == nullptr)
	{
		fprintf(stderr, "\nUnable to open the adapter: %s\n", errbuf);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Check the link layer. We support only Ethernet for simplicity. */
	if (pcap_datalink(adhandle) != DLT_EN10MB)
	{
		fprintf(stderr, "\nThis program works only on Ethernet networks.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	if (d->addresses != nullptr)
		/* Retrieve the mask of the first address of the interface */
		netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask = 0xffffff;


	{

		if (d->addresses != NULL) {
			// 遍历网络地址列表
			for (pcap_addr_t* addr = d->addresses; addr != NULL; addr = addr->next) {
				// 检查是否是IPv4地址
				if (addr->addr->sa_family == AF_INET) {
					// 获取IPv4地址
					struct sockaddr_in* ip = (struct sockaddr_in*)addr->addr;
					char ip_str[INET_ADDRSTRLEN];
					const char* ip_address = inet_ntop(AF_INET, &(ip->sin_addr), ip_str, INET_ADDRSTRLEN);
					if (ip_address == NULL) {
						printf("Error converting IP address to string\n");
						continue;
					}

					printf("Interface: %s\n", d->name);
					printf("Host IP: %s\n", ip_address);
				}
			}
		}

		//strcpy(destination, d->name);
		////char dev[] = "eth0"; // 你要查询的网络接口名
		//char errbuf[PCAP_ERRBUF_SIZE] = {};
		//bpf_u_int32 net; // 存储网络地址
		//bpf_u_int32 mask; // 存储子网掩码

		//// 获取网络地址和子网掩码
		//if (pcap_lookupnet(destination, &net, &mask, errbuf) == -1) {
		//	printf("Error getting network info for device %s: %s\n", destination, errbuf);
		//	return -1;
		//}

		//// 将网络地址和子网掩码转换成字符串形式
		//char net_str[INET_ADDRSTRLEN];
		//char mask_str[INET_ADDRSTRLEN];
		//if (inet_ntop(AF_INET, &net, net_str, sizeof(net_str)) == nullptr ||
		//	inet_ntop(AF_INET, &mask, mask_str, sizeof(mask_str)) == nullptr) {
		//	printf("Error converting network info to string\n");
		//	return -1;
		//}

		//// 打印结果
		//printf("Device: %s\n", destination);
		//printf("IP Address: %s\n", net_str);
		//printf("Subnet Mask: %s\n", mask_str);
		////printf("1111111111: %s\n", d->addresses->addr->sa_data);
		//printf("1111111111: %d\n", net);
		//int a = 12;
	}
	system("pause");
	return 0;
	//compile the filter
	if (pcap_compile(adhandle, &fcode_w, packet_filter, 1, netmask) < 0)
	{
		fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	//set the filter
	if (pcap_setfilter(adhandle, &fcode_w) < 0)
	{
		fprintf(stderr, "\nError setting the filter.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	printf("\nlistening on %s...\n", d->description);

	/* At this point, we don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);

	/* start the capture */
	pcap_loop(adhandle, 0, packet_handler, nullptr);

	return 0;
}


int count = 0;
int last_num = 0;
bool once = true;
/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	/*if (count > 10000)
	{
		return;
	}
	count++;*/
	struct tm* ltime;
	char timestr[16];
	ip_header* ih;
	udp_header* uh;
	u_int ip_len;
	u_short sport, dport;
	time_t local_tv_sec;

	/*
	 * unused parameter
	 */
	(VOID)(param);

	/* convert the timestamp to readable format */
	//local_tv_sec = header->ts.tv_sec;
	//ltime = localtime(&local_tv_sec);
	//strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

	///* print timestamp and length of the packet */
	//printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);

	unsigned char ch = *(pkt_data + 44);
	//int num = ch;
	/*printf("%d\n", num);*/
	last_num++;
	if (once)
	{
		last_num = ch;
		once = false;
		if(last_num == 255)
			last_num = -1;
	}
	else
	{
		int hex = ch;
		if (last_num != hex)
		{
			printf("%d_%d\n", last_num, hex);
		}
	/*	else
		{
			printf("succ: %d_%d\n", last_num, hex);
		}*/

		if (last_num == 255)
		{
			last_num = -1;
			//printf("succ: %d_%d\n", last_num, hex);
		}
	}


	///* retireve the position of the ip header */
	//ih = (ip_header*)(pkt_data +
	//    14); //length of ethernet header

	///* retireve the position of the udp header */
	//ip_len = (ih->ver_ihl & 0xf) * 4;
	//uh = (udp_header*)((u_char*)ih + ip_len);

	///* convert from network byte order to host byte order */
	//sport = ntohs(uh->sport);
	//dport = ntohs(uh->dport);

	///* print ip addresses and udp ports */
	//printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
	//    ih->saddr.byte1,
	//    ih->saddr.byte2,
	//    ih->saddr.byte3,
	//    ih->saddr.byte4,
	//    sport,
	//    ih->daddr.byte1,
	//    ih->daddr.byte2,
	//    ih->daddr.byte3,
	//    ih->daddr.byte4,
	//    dport);
}
