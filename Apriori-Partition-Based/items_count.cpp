#include <bits/stdc++.h>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void get_in_memory(vector<set<int> > &maps)
{
	ifstream fin("data.txt", ios::in);
	
	int num_of_trans;
	fin>>num_of_trans;
	
	for(int i=0; i<num_of_trans; i++)
	{
		int tid, num_of_items; char dummy;
		fin>>dummy;
		fin>>tid;
		fin>>dummy;
		fin>>num_of_items;
		
		set<int> cpy;
		for(int j=0; j<num_of_items; j++)
		{
			int tmp; fin>>tmp; cpy.insert(tmp);
		}
		maps.push_back(cpy);
	}
}

void print_table(vector<set<int> > maps)
{
	for(int i=0; i<maps.size(); i++)
	{
		set<int>::iterator it;
		
		cout<<"T"<<" "<<"Items"<<" ";
		for(it=maps[i].begin(); it!=maps[i].end(); it++)
		{
			cout<<*it<<" ";
		}
		cout<<endl;
	}
}

void print_items_maps_trans(vector<set<int> > maps)
{
	ofstream fout("items.txt", ios::out);
	for(int i=0; i<50; i++)
	{
		cout<<"Item"<<" "<<i<<" Transactions ";
		fout<<"Item"<<" "<<i<<" Transactions ";
		for(int j=0; j<maps.size(); j++)if(maps[j].find(i)!=maps[j].end())
		{
			cout<<j<<" ";
			fout<<j<<" ";
		}
		fout<<endl;
		cout<<endl;
	}
	fout.close();
}

int main(int argc, char** argv) {
	vector<set<int> > maps;
	get_in_memory(maps);
	print_table(maps);
	print_items_maps_trans(maps);
	return 0;
}
