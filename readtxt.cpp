//#include <iostream>
//#include <fstream>
//#include <cassert>
//#include <string>
//#include <filesystem>//C++ 17
//
//using namespace std;
//
//int last = -1;
//int num = -1;
//bool once = true;
//bool once2 = true;
//
//void readTxt(string file)
//{
//    ifstream infile;
//    infile.open(file.data());   //���ļ����������ļ��������� 
//    assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
//    cout << file << endl;
//
//    string s;
//    if (once2)
//    {
//        getline(infile, s);//��һ�в���
//        once2 = false;
//    }
//    int line = 1;
//    while (getline(infile, s))
//    {
//        line++;
//
//        std::vector<std::string> tokens;
//        std::istringstream iss(s);
//        std::string token;
//
//        while (std::getline(iss, token, ',')) {
//            tokens.push_back(token);
//        }
//
//        for (int i = 1; i < tokens.size() - 1; i++)//��Ҫ���һ�к͵�һ��
//        {
//	        if (std::stoi(tokens.at(i)) != 65536)
//	        {
//                cout << "error:" << "!= 65536 and line : " << line << endl;
//	        }
//        }
//
//        //s = s.substr(0, s.find(','));  // ������7��ʼ��ȡ����Ϊ5�����ַ���
//
//        num = std::stoi(tokens.at(0));
//        last++;
//        if(once)
//        {
//            last = num;
//            once = false;
//            if (num == 255)
//                last = -1;
//        }
//        else
//        {
//            if (last != num)
//            {
//                cout << "error:" << last << "_" << num << " line : " << line << endl;
//                once = true;
//            }
//        }
//
//        if (last == 255)
//            last = -1;
//    }
//    infile.close();             //�ر��ļ�������
//}
//
//
//int main()
//{
//    //string path = "C:\\data\\2023_06_27_17_09_06\\";
//    string path = filesystem::current_path().string();
//    //string path = "D:\\data\\delete\\";
//
//    int count = 0;
//    for (const auto& entry : filesystem::directory_iterator(path)) {
//        //std::cout << entry.path() << std::endl;
//      /*  string name = entry.path().string();
//        cout << name << endl;*/
//        count++;
//    }
//
//    count = count - 3;//��ȥ����3��Ӧ�ó����ļ�
//    for(int i = 1; i < count + 1; i++)
//    {
//        readTxt(path + "\\" + std::to_string(i) + ".csv");
//    }
//    //readTxt("C:\\Users\\13541\\Desktop\\1.csv");
//    system("pause");
//}