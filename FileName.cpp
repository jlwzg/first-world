//#include <iostream>
//#include <Windows.h>
//#include <iomanip>
//#include <fstream>
//#include<cstdlib>
//#include<string>
//
//using namespace std;
//
//char HEX[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
//void setIndex(int num, char* hexNumber)
//{
//	// ������±�
//	for (int i = 0; i < 8; i++) {
//		hexNumber[i] = '0';
//	}
//
//	// �����µ����±�
//	int index = 7;
//	while (num != 0 && index >= 0)
//	{
//		hexNumber[index--] = HEX[num % 16];
//		num = num / 16;
//	}
//}
//
//int main()
//{
//	// ���ļ�
//	string path_r = "C:\\Users\\13541\\Desktop\\save6\\1_00010_20230609153318.pcappng";
//	ifstream in = ifstream(path_r, ios::binary);
//	if (!in.is_open()) { cout << "Error: File Path is Wrong" << endl; }
//
//	// ��ȡ�ļ���С���ļ���
//	long long Beg = in.tellg();
//	in.seekg(0, ios::end);
//	long long End = in.tellg();
//	long long fileSize = End - Beg;
//	in.seekg(0, ios::beg);
//	cout << "File Size: " << fileSize / 1024.0 << "KB" << endl;
//
//	// ���ļ���ÿ��ѭ����ȡ 1 �ֽڣ�
//	int byteBeenRead = 0;
//	char hexNumber[9] = "00000000";
//	unsigned char temp;
//	bool fb = false;
//	int fb_index = 0;
//	int last_data = -1;
//	//int current_data = -1;
//	bool once = true;
//	int errory_recovery = -1;
//	bool onec_error_recovery = true;
//	while (in.read((char*)&temp, 1))
//	{
//		// ÿ�� 16 ���ֽڻ���
//		//if (byteBeenRead % 16 == 0)
//		//{
//		//	// �������±�
//		//	cout << endl;
//		//	setIndex(byteBeenRead, hexNumber);
//		//	cout << hexNumber << ":\t";
//		//}
//		byteBeenRead++;
//
//		// �� 1 �ֽ�
//		int hex = (unsigned)temp;
//	/*	char a = HEX[hex / 16];
//		char b = HEX[hex % 16];*/
//		fb_index++;
//
//		if (fb && (fb_index == 1))
//		{
//			if(hex != 40)
//			{
//				fb = false;
//			}
//		}
//		if (fb && (fb_index == 2))
//		{
//			fb = false;
//			last_data++;
//			
//			if(once)
//			{
//				last_data = hex;
//				once = false;
//			}
//
//			if(hex != last_data)
//			{
//				std::cout << "error:" << last_data << "and hex:" << hex << std::endl;
//			}
//
//			if (last_data == 255)
//			{
//				last_data = -1;
//			}
//		}
//
//		if(hex == 246)
//		{
//			if (errory_recovery + 184 != byteBeenRead)
//			{
//				if(onec_error_recovery)
//				{
//					onec_error_recovery = false;
//				}
//				else
//				{
//					continue;
//				}
//			}
//
//			fb = true;
//			fb_index = 0;
//			errory_recovery = byteBeenRead;
//		}
//		
//
//		//cout << a << b << " ";
//	}
//	// �ر��ļ���
//	in.close();
//	cout << "Read Successfully" << endl;
//
//	getchar();
//	return 0;
//}
