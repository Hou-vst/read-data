#include<string>
#include<map>
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

vector<int> dealStrTemp(const string& temp)
{
	int i_start = 0;
	int size = temp.size();
	vector<int> result;
	for (int i = 0; i < size && result.size()<2; i++)
	{
		if (temp[i] == ':' && i+1<size && temp[i+1] == ':')
		{
			int t = atoi(temp.substr(i_start,i-i_start).c_str());
			result.push_back(t);
			i_start = i + 2;
		}
	}

	return result;
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

	map<int, vector<int>> user_to_item;
	int str_size = str_buff.size();
	int start = 0;
	bool skip = false;
	int count = 0;
	for (int i = 0; i < str_size; i++)
	{
		if (str_buff[i] == '\n')
		{
			string&& temp = str_buff.substr(start, i - start + 1);
			start = i + 1;
			if (!skip)
			{
				skip = true;
				continue;
			}
			
			vector<int>&& temp_result = dealStrTemp(temp);
			user_to_item[temp_result[0]].push_back(temp_result[1]);
			cout << count++ << endl;
   	 	}
	}

	return 0;
}