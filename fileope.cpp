//#include <iostream>
//#include <fstream>
//#include <cassert>
//#include <string>
//
//using namespace std;
//
//int last = -1;
//int num = -1;
//bool once = true;
//
//void readTxt(string file)
//{
//    ifstream infile;
//    infile.open(file.data());   //���ļ����������ļ��������� 
//    assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
//
//    string s;
//    while (getline(infile, s))
//    {
//        //cout << s << endl;
//        num = std::stoi(s);
//        last++;
//        if(once)
//        {
//            last = num;
//            once = false;
//            if (num == 255)
//                last = -1;
//        }
//
//        if(last != num)
//        {
//            cout << "error:" << last << "_" << num << endl;
//            once = true;
//        }
//
//        if (last == num)
//            cout << "i" << endl;
//
//        if (last == 255)
//            last = -1;
//      
//
//    }
//    infile.close();             //�ر��ļ������� 
//}
//
//
//int main()
//{
//    readTxt("D:\\delete\\20230612155255.txt");
//}