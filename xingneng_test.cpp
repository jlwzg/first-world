////#include<bits/stdc++.h>//����ͷ�ļ�
//
//#include<stdio.h>
//#include <string>
//#include <chrono>
//
//
//#include <vector>
//#include <fstream>
//#include <iostream>
//#include <sstream>
//
//using namespace std;
//
//
//const string trainFile = "train_data.txt";
//
//bool loadTrainData() {
//
//	ifstream infile(trainFile.c_str());
//	string line;
//
//	if (!infile) {
//		cout << "��ѵ���ļ�ʧ��" << endl;
//		exit(0);
//	}
//
//	while (infile) {
//		getline(infile, line);
//	}
//	infile.close();
//	return true;
//}
//
//string outputFile = "output_data.txt";
//int storeData() {
//
//	string line;
//	int i;
//	ofstream fout(outputFile.c_str());
//	if (!fout.is_open()) {
//		cout << "������ļ�ʧ��" << endl;
//	}
//
//	fout.close();
//	return 0;
//}
//
//
//int main(int argc, char* argv[]) {
//	//auto s1 = chrono::steady_clock::now();
//	//loadTrainData();
//	auto s2 = chrono::steady_clock::now();
//	//cout << "time elapsed: " << chrono::duration_cast<chrono::duration<double>>(s2 - s1).count() << " s" << endl;
//	storeData();
//	auto s3 = chrono::steady_clock::now();
//	cout << "time elapsed: " << chrono::duration_cast<chrono::duration<double>>(s3 - s2).count() << " s" << endl;
//
//}
