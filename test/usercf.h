#pragma once
#include<map>
#include<vector>
#include<string>
#include<set>

using namespace std;

void TransferTo_ItemToUser(map<int, set<int>>& ori, map<int, set<int>>& dst);
void Create_CoRated_table(map<int, set<int>>& dst, map<int, map<int, int>>& CoRated_table);
void Calculate_Similarity(const map<int, map<int, int>>& CoRated_table, map<int, set<int>>& user_to_item, map<int, map<int, float>>& result);
void PrintResult(const map<int, map<int, float>>& result);
vector<float> SortSimilarUser(const map<int, map<int, float>>& table, const int& user_name, const int& item_name, map<int, set<int>>& item_to_user);
float Calculate(const vector<float>& result, int k);