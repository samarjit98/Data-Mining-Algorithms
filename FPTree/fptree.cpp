#include <bits/stdc++.h>
using namespace std;
#define DEBUG printf("debug\n")
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct Itemstats
{
	int itemno;
	int freq;
};

bool cmp1(Itemstats a, Itemstats b)
{
	return a.freq>b.freq;
}


vector<set<int> > maps;
vector<vector<Itemstats> > trans;
vector<map<int, int> > h;
vector<Itemstats> itemfreqs;

void get_in_memory()
{
	ifstream fin("data2.txt", ios::in);
	
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
		vector<Itemstats> cpy2;
		map<int, int> v;
		for(int j=0; j<num_of_items; j++)
		{
			int tmp; fin>>tmp; cpy.insert(tmp); 
			Itemstats a; a.itemno=tmp; a.freq=0;
			cpy2.push_back(a);
			v[tmp]=j;
		}
		
		trans.push_back(cpy2);
		maps.push_back(cpy);
		h.push_back(v);
	}
}

void print_memory_contents()
{
	for(int i=0; i<trans.size(); i++)
	{
		printf("T %d : Size: %d --> ", i, (int)trans[i].size());
		for(int j=0; j<trans[i].size(); j++)cout<<trans[i][j].itemno<<" ";
		cout<<endl;
	}
}

struct FPTreeNode
{
	int itemno;
	int currfreq;
	map<int, FPTreeNode*> h;
};

FPTreeNode* head;

void insert_into_tree(vector<Itemstats> a, int i, FPTreeNode* &T)
{
	if(i>=a.size())
	{
		T->currfreq++; return;
	}
	else if((T->h).find(a[i].itemno)!=(T->h).end())
	{
		T->currfreq++;
		insert_into_tree(a, i+1, (T->h)[a[i].itemno]);
	}
	else
	{
		T->currfreq++;
		FPTreeNode* tmp=new FPTreeNode;
		tmp->itemno=a[i].itemno; tmp->currfreq=0;
		(T->h)[a[i].itemno]=tmp;
		insert_into_tree(a, i+1, (T->h)[a[i].itemno]);
	}
}

ofstream fout("tre2.txt", ios::out);

void order(FPTreeNode* T, int i)
{
	fout<<"[ "<<T->itemno<<" "<<T->currfreq<<"]-->"<<i<<endl;
	map<int, FPTreeNode*>::iterator it;
	map<int, FPTreeNode*> tmp=T->h;
	for(it=tmp.begin(); it!=tmp.end(); it++)
	{
		order(it->second, i+1);
	}
}

int main(int argc, char** argv) 
{
	get_in_memory();
	
	//Build FP Tree
	//	DEBUG;
	
	for(int i=0; i<14; i++)
	{
		Itemstats tmp; tmp.itemno=i; tmp.freq=0;
		
		for(int j=0; j<maps.size(); j++)
		{
			if(maps[j].find(i)!=maps[j].end())tmp.freq++;
		}
		
		itemfreqs.push_back(tmp);
	}
	
	for(int i=0; i<itemfreqs.size(); i++)
	{
		for(int j=0; j<maps.size(); j++)
		{
			if(maps[j].find(itemfreqs[i].itemno)!=maps[j].end())
			{
				trans[j][h[j][itemfreqs[i].itemno]].freq=itemfreqs[i].freq;
			}
		}
	}
	
	for(int i=0; i<trans.size(); i++)sort(trans[i].begin(), trans[i].end(), cmp1);
	
	print_memory_contents();
	
	head=new FPTreeNode;
	head->itemno=-1;
	head->currfreq=0;
	
	for(int i=0; i<trans.size(); i++)
	{
		insert_into_tree(trans[i], 0, head);
	}
	order(head, 0);
	fout.close();
	
	//Mine FP Tree
	DEBUG;
	
	return 0;
}
