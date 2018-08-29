#include <bits/stdc++.h>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
const int min_sup = 18;

vector<set<int> > maps;
vector<vector<int> > trans;

ofstream fout("apriori1_out.txt", ios::out);
ofstream fout2("apriori1_performance.txt", ios::out);

void get_in_memory()
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
		vector<int> cpy2;
		for(int j=0; j<num_of_items; j++)
		{
			int tmp; fin>>tmp; cpy.insert(tmp); cpy2.push_back(tmp); 
		}
		sort(cpy2.begin(), cpy2.end());
		trans.push_back(cpy2);
		maps.push_back(cpy);
	}
}

vector<int> ret_support(vector<vector<int> > items)
{
	vector<int> sup(items.size(), 0);
	
	for(int i=0; i<items.size(); i++)
	{
		for(int j=0; j<maps.size(); j++)
		{
			int flag=0;
			
			for(int k=0; k<items[i].size(); k++)
			if(maps[j].find(items[i][k])==maps[j].end())
			{
				flag=1; break;
			}
			if(flag==0)sup[i]++;
		}
	}
	return sup;
}

void print_items(vector<vector<int> > items)
{
	fout<<items[0].size()<< " itemsets with minimum support are: "<< endl;
	for(int i=0; i<items.size(); i++)
	{
		for(int j=0; j<items[i].size(); j++)fout<<items[i][j]<<" ";
		fout<<endl;
	}
	
}

vector<vector<int> > apriori_join(vector<vector<int> > min_items)
{
	vector<vector<int> > items;
	set<set<int> > pre_subs;
	for(int i=0; i<min_items.size(); i++)
	{
		set<int> a;
		for(int j=0; j<min_items[i].size(); j++)a.insert(min_items[i][j]);
		pre_subs.insert(a);
	}
	for(int i=0; i<min_items.size()-1; i++)
	{
		for(int j=i+1; j<min_items.size(); j++)
		{
			int flag=0;
			for(int k=0; k<min_items[i].size()-1; k++)
			if(min_items[i][k]!=min_items[j][k])
			{
				flag=1; break;
			}
			if(flag==0)
			{
				vector<int> a;
				for(int k=0; k<min_items[i].size(); k++)a.push_back(min_items[i][k]);
				a.push_back(min_items[j][min_items[j].size()-1]);
				int flag2=0;
				for(int ii=0; ii<a.size(); ii++)
				{
					set<int> subs;
					for(int jj=0; jj<a.size(); jj++)if(ii!=jj)subs.insert(a[jj]);
					if(pre_subs.find(subs)==pre_subs.end())
					{
						flag2=1; break;
					}
				}
				if(flag2==1)break;
				sort(a.begin(), a.end());
				items.push_back(a);
			}
		}
	}
	return items;
}

vector<vector<int> > apriori_prune(vector<vector<int> > items, vector<int> sup)
{
	vector<vector<int> > min_items;
	for(int i=0; i<items.size(); i++)if(sup[i]>=min_sup)min_items.push_back(items[i]);
	return min_items;
}

int main(int argc, char** argv) {
	get_in_memory();
	
	vector<vector<int> > items;
	for(int i=0; i<200; i++)
	{
		vector<int> a; a.push_back(i); items.push_back(a);
	}
	vector<int> sup;
	sup=ret_support(items);
	vector<vector<int> > min_items;
	min_items=apriori_prune(items, sup);
	
	while(min_items.size()>0)
	{
		print_items(min_items);
		fout2<<"Iteration: "<<min_items[0].size()<< " Count: "<<min_items.size()<<endl;
		cout<<"Iteration: "<<min_items[0].size()<< " Count: "<<min_items.size()<<endl;
		items = apriori_join(min_items);
		sup = ret_support(items);
		min_items.clear();
		min_items = apriori_prune(items, sup);
	}
	fout.close();
	fout2.close();
	return 0;
}

/*
Output:-

Iteration: 1 Count: 199
Iteration: 2 Count: 7850
Iteration: 3 Count: 28560
Iteration: 4 Count: 30317
Iteration: 5 Count: 15569
Iteration: 6 Count: 4717
Iteration: 7 Count: 988
Iteration: 8 Count: 179
Iteration: 9 Count: 27
Iteration: 10 Count: 2

*/
