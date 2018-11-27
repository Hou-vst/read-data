#include <iostream>
#include<math.h>
#include<algorithm>
#include"usercf.h"
using namespace std;

 //用户-物品 转换 物品-用户 倒排表
 void TransferTo_ItemToUser(map<int, set<int>>& ori, map<int, set<int>>& dst)
 {
	 if (!dst.empty())
	 {
		 dst.clear();
	 }

	 map<int, set<int>>::iterator iter = ori.begin();
	 while (iter != ori.end())
	 {
		 set<int>& item_set = iter->second;
		 set<int>::iterator iter_set = item_set.begin();
		 for (; iter_set != item_set.end(); iter_set++)
		 {
			 dst[*iter_set].insert(iter->first);
		 }
		 iter++;
	 }
 }

 // 根据倒排表建立稀疏矩阵
 void Create_CoRated_table(map<int, set<int>>& dst, map<int, map<int, int>>& CoRated_table)
 {
	 //遍历所有物品
	 map<int, set<int>>::iterator iter_begin = dst.begin();
	 map<int, set<int>>::iterator iter_End = dst.end();
	 for (; iter_begin != iter_End; iter_begin++)
	 {
		 //连续遍历两次用户表
		 set<int>& user_set = iter_begin->second;
		 set<int>::iterator set_begin1 = user_set.begin();
		 for (; set_begin1 != user_set.end(); set_begin1++)
		 {
			 set<int>::iterator set_begin2 = user_set.begin();
			 int user1 = *set_begin1;
			 for (; set_begin2 != user_set.end(); set_begin2++)
			 {
				 int user2 = *set_begin2;
				 if (user1 == user2)
				 {
					 CoRated_table[user1][user2] = 0;
					 continue;
				 }
				 bool init = false;
				 map<int, map<int, int>>::iterator aim1 = CoRated_table.find(user1);
				 if (aim1 != CoRated_table.end())
				 {
					 map<int, int>& temp = aim1->second;
					 map<int, int>::iterator aim2 = temp.find(user2);
					 if (aim2 != temp.end())
					 {
						 init = true;
					 }
				 }

				 if (!init)
				 {
					 CoRated_table[user1][user2] = 1;
				 }
				 else
				 {
					 CoRated_table[user1][user2]++;
				 }
			 }
		 }
	 }
 }

 // 计算用户与用户之间的相似度
 void Calculate_Similarity(const map<int, map<int, int>>& CoRated_table, map<int, set<int>>& user_to_item, map<int, map<int, float>>& result)
 {
	 map<int, map<int, int>>::const_iterator iter_corated1 = CoRated_table.begin();
	 for (; iter_corated1 != CoRated_table.end(); iter_corated1++)
	 {
		 const int& user1 = iter_corated1->first;
		 const map<int, int>& table = iter_corated1->second;
		 map<int, int>::const_iterator iter_corated2 = table.begin();
		 int size1 = user_to_item[user1].size();
		 for (; iter_corated2 != table.end(); iter_corated2++)
		 {
			 const int& user2 = iter_corated2->first;
			 const int score = iter_corated2->second;
			 int size2 = user_to_item[user2].size();

			 if (user1 == user2)
			 {
				 result[user1][user2] = 0;
			 }
			 else
			 {
				 result[user1][user2] = (float)score / sqrt(size1*size2);
			 }
		 }
	 }
 }

 void PrintResult(const map<int, map<int, float>>& result)
 {
	 map<int, map<int, float>>::const_iterator iter = result.begin();
	 for (; iter != result.end(); iter++)
	 {
		 const int user1 = iter->first;
		 const map<int, float>& score = iter->second;
		 map<int, float>::const_iterator iter2 = score.begin();
		 for (; iter2 != score.end(); iter2++)
		 {
			 cout << "W[" << user1 << "][" << iter2->first << "]" << " : " << iter2->second << endl;
		 }
	 }
 }

 // 指定一个用户和物品，倒排其他使用该物品的用户和该用户的相似度
 vector<float> SortSimilarUser(const map<int, map<int, float>>& table, const int& user_name, const int& item_name, map<int, set<int>>& item_to_user)
 {
	 vector<float> v;

	 if (table.find(user_name) == table.end())
	 {
		 return v;
	 }

	 if (item_to_user[item_name].find(user_name) != item_to_user[item_name].end())
	 {
		 v.push_back(1);
		 return v;
	 }

	 const map<int, float>& user_map = table.find(user_name)->second;
	 map<int, float>::const_iterator iter = user_map.begin();
	 const set<int>& user_set = item_to_user[item_name];
	 for (; iter != user_map.end(); iter++)
	 {
		 const int& user_name1 = iter->first;

		 // 判断user_name1用户有没有使用过该商品
		 if (user_set.find(user_name1) != user_set.end())
		 {
			 v.push_back(iter->second);
		 }
	 }

	 sort(v.begin(), v.end(), greater<float>());
	 return v;
 }

 float Calculate(const vector<float>& result, int k)
 {
	 float t = 0;
	 float r = 1;
	 for (int i = 0; i < result.size() && i < k; i++)
	 {
		 t += result[i] * r;
	 }

	 return t;
 }