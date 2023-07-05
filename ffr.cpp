//#include <iostream>
//#include <Windows.h>
//#include <iomanip>
//#include <fstream>
//#include<cstdlib>
//#include<string>
//#include <iomanip> // 包含 std::hex 格式控制器
//
//using namespace std;
//
//char HEX[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
//void setIndex(int num, char* hexNumber)
//{
//	// 清空行下标
//	for (int i = 0; i < 8; i++) {
//		hexNumber[i] = '0';
//	}
//
//	// 设置新的行下标
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
//	// 打开文件
//	string path_r = "C:\\Users\\13541\\Desktop\\SAVE2023_6_16_13-32-32.DAT";
//	ifstream in = ifstream(path_r, ios::binary);
//	if (!in.is_open()) { cout << "Error: File Path is Wrong" << endl; }
//
//	// 获取文件大小、文件名
//	long long Beg = in.tellg();
//	in.seekg(0, ios::end);
//	long long End = in.tellg();
//	long long fileSize = End - Beg;
//	in.seekg(0, ios::beg);
//	cout << "File Size: " << fileSize / 1024.0 << "KB" << endl;
//
//	// 读文件（每次循环读取 1 字节）
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
//	in.read((char*)&temp, 1);
//	in.read((char*)&temp, 1);
//	in.read((char*)&temp, 1);
//	in.read((char*)&temp, 1);
//	in.read((char*)&temp, 1);
//	in.read((char*)&temp, 1);
//	in.read((char*)&temp, 1);
//	in.read((char*)&temp, 1);
//	in.read((char*)&temp, 1);
//	in.read((char*)&temp, 1);
//
//	fstream f;
//	f.open("data.txt", ios::out);
//	//输入你想写入的内容 
//	//f << "123323" << " " << "abs" << endl;
//
//	while (in.read((char*)&temp, 1))
//	{
//		// 每读 16 个字节换行
//		if (byteBeenRead % 19 == 0)
//		{
//			int hex = (unsigned)temp;
//			if (in.read((char*)&temp, 1))
//			{
//				int hex2 = (unsigned)temp;
//				byteBeenRead++;
//
//				int hex3 = hex << 8 | hex2;
//
//				f << std::dec << hex3 << std::endl; // 输出10进制数
//			}
//			else
//			{
//				break;
//			}
//		}
//		byteBeenRead++;
//	}
//
//	// 关闭文件流
//	in.close();
//	f.close();
//
//	cout << "Read Successfully" << endl;
//
//	getchar();
//	return 0;
//}
