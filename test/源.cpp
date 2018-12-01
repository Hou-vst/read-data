#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>
#include"usercf.h"

using namespace std;

void dealStrTemp(const string& temp,int& result1,int& result2)
{
	int i_start = 0;
	int size = static_cast<int>(temp.size());
	int find_num = 0;
	for (int i = 0; i < size && find_num<2; i++)
	{
		if (temp[i] == ':' && i+1<size && temp[i+1] == ':')
		{
			int t = atoi(temp.substr(i_start,i-i_start).c_str());
			i_start = i + 2;
			find_num++;
			if (find_num == 1)
			{
				result1 = t;
			}
			else if (find_num == 2)
			{
				result2 = t;
			}
			
		}
	}
}

void testUserCF(map<int, set<int>>& user_to_item,map<int, map<int, float>>& result)
{
	// �û�-��Ʒ ת�� ��Ʒ-�û� ���ű�
	map<int, set<int>> item_to_user;
	TransferTo_ItemToUser(user_to_item, item_to_user);

	// ���ݵ��ű���ϡ�����
	map<int, map<int, int>> CoRated_table;
	Create_CoRated_table(item_to_user, CoRated_table);

	// �����û����û�֮������ƶ�
	
	Calculate_Similarity(CoRated_table, user_to_item, result);

	// ������
	//PrintResult(result);


}

int main()
{
	string path = "ratings.dat";
	ifstream fin(path.c_str(), std::ios::binary);
	if (!fin)
	{
		return - 1;
	}
	//seekg�����Ƕ������ļ���λ������������������һ��������ƫ�������ڶ��������ǻ���ַ��
	//tellg������������Ҫ�������������ص�ǰ��λָ���λ�ã�Ҳ�������������Ĵ�С��
	std::streamsize size = fin.seekg(0, std::ios::end).tellg();
	char* buff = (char*)malloc(size);
	fin.seekg(0, std::ios::beg).read(&buff[0], size);
	string str_buff = buff;
	free(buff);
	buff = NULL;
	// userid movieid rating timestamp

	map<int, set<int>> user_to_item;
	int str_size = static_cast<int>(str_buff.size());
	int start = 0;
	bool skip = false;
	int count = 0;
	for (int i = 0; i < str_size; i++)
	{
		if (str_buff[i] == '\n')
		{
			string&& temp = str_buff.substr(start, i - start + 1);
			start = i + 1;
			i = i + 10;
		/*	if (!skip)
			{
				skip = true;
				continue;
			}*/
			int result1 = 0;
			int result2 = 0;
			dealStrTemp(temp,result1,result2);
			
			//13.301s
			user_to_item[result1].insert(result2);

			//13.897s
			/*map<int, set<int>>::iterator iter= user_to_item.find(result1);
			if (iter != user_to_item.end())
			{
				iter->second.insert(result2);
			}
			else
			{
				set<int> set_temp;
				set_temp.insert(result2);
				user_to_item.insert(pair<int, set<int>>(result1, set_temp));
			}*/
			
			count++;
			if (count % 10000 == 0)
			{
				cout << count << endl;
			}
   	 	}
	}

	//����usercf
	map<int, map<int, float>> result;
	testUserCF(user_to_item, result);

	return 0;
}