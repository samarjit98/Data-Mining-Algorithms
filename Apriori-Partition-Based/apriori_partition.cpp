#include <bits/stdc++.h>
#include <pthread.h>
#include <sys/types.h>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
const int rel_min_sup = 50;

struct Partition
{
	int n;
	vector<set<int> > imaps;
	vector<vector<int> > itrans;
};

set<set<int> > global_itemsets;
void push_into_global_items(vector<vector<int> > min_items);
void get_in_memory(vector<set<int> > &maps, vector<vector<int> > &trans);
vector<int> ret_support(vector<vector<int> > items, vector<set<int> > maps);
vector<vector<int> > apriori_join(vector<vector<int> > min_items);
vector<vector<int> > apriori_prune(vector<vector<int> > items, vector<int> sup, int min_sup);

void* runner(void* param)
{
	Partition p = *((Partition*)param);
	printf("Thread: %d, Transactions: %d \n", p.n, (int)p.itrans.size());
	int min_sup = (rel_min_sup*(p.itrans.size()))/100;
	vector<vector<int> > items;
	for(int i=0; i<200; i++)
	{
		vector<int> a; a.push_back(i); items.push_back(a);
	}
	vector<int> sup;
	sup=ret_support(items, p.imaps);
	vector<vector<int> > min_items;
	min_items=apriori_prune(items, sup, min_sup);
	
	while(min_items.size()>0)
	{
		push_into_global_items(min_items);
		cout<<"Thread: "<<p.n<<" Iteration: "<<min_items[0].size()<< " Count: "<<min_items.size()<<endl;
		items = apriori_join(min_items);
		sup = ret_support(items, p.imaps);
		min_items.clear();
		min_items = apriori_prune(items, sup, min_sup);
	}
	cout<<"Exited"<<endl;
}

void push_into_global_items(vector<vector<int> > min_items)
{
	for(int i=0; i<min_items.size(); i++)
	{
		set<int> tmp;
		for(int j=0; j<min_items[i].size(); j++)tmp.insert(min_items[i][j]);
		global_itemsets.insert(tmp);
	}
}

void get_in_memory(vector<set<int> > &maps, vector<vector<int> > &trans)
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

vector<int> ret_support(vector<vector<int> > items, vector<set<int> > maps)
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
	ofstream fout("apriori_partition_out.txt", ios::out);
	fout<<items[0].size()<< " itemsets with minimum support are: "<< endl;

	for(int i=0; i<items.size(); i++)
	{
		for(int j=0; j<items[i].size(); j++)fout<<items[i][j]<<" ";
		fout<<endl;
	}
	fout.close();
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

vector<vector<int> > apriori_prune(vector<vector<int> > items, vector<int> sup, int min_sup)
{
	vector<vector<int> > min_items;
	for(int i=0; i<items.size(); i++)if(sup[i]>=min_sup)min_items.push_back(items[i]);
	return min_items;
}

int main(int argc, char** argv) {
	vector<set<int> > maps;
	vector<vector<int> > trans;

	get_in_memory(maps, trans);

	cout<<"No. of transactions: "<<trans.size()<<endl;
	int k;
	cin>>k;
	int trans_per_part;
	if(trans.size()%k==0)trans_per_part=trans.size()/k;
	else trans_per_part=trans.size()/k+1;

	pthread_t *tid = new pthread_t[k];
	int curr_start = 0;
	int curr_end = min((int)trans.size(), curr_start + trans_per_part);
	for(int i=0; i<k; i++)
	{
		vector<set<int> > imaps;
		vector<vector<int> > itrans;

		for(int j=curr_start; j<curr_end; j++)
		{
			itrans.push_back(trans[j]);
			imaps.push_back(maps[j]);
		}
		printf("Partition for %d to %d created...\n", curr_start, curr_end);
		Partition *param = new Partition; 
		param->n = i;
		param->imaps = imaps; param->itrans = itrans;
		cout<<"Before thread creation "<<i<<endl;
		pthread_create(&tid[i], NULL, runner, (void*)param);
		cout<<"Thread"<<i<<" created successfully"<<endl;

		curr_start = curr_end;
		curr_end = min((int)trans.size(), curr_start + trans_per_part);
	}
	for(int i=0; i<k; i++)pthread_join(tid[i], NULL);

	set<set<int> >::iterator it1;
	set<int>::iterator it2;
	vector<vector<int> > items;
	for(it1=global_itemsets.begin(); it1!=global_itemsets.end(); it1++)
	{
		set<int> tmp;
		tmp = *it1;
		vector<int> a;
		for(it2=tmp.begin(); it2!=tmp.end(); it2++)a.push_back(*it2);
		items.push_back(a);
	}
	int min_sup = (rel_min_sup*(trans.size()))/100;
	vector<int> sup;
	sup=ret_support(items, maps);
	vector<vector<int> > min_items;
	min_items=apriori_prune(items, sup, min_sup);
	print_items(items);

	return 0;
}

/*

No. of transactions: 50
3
Partition for 0 to 17 created...
Before thread creation 0
Thread0 created successfully
Thread: 0, Transactions: 17 
Partition for 17 to 34 created...
Before thread creation 1
Thread1 created successfully
Thread: 1, Transactions: 17 
Partition for 34 to 50 created...
Thread: 0 Iteration: 1 Count: 77
Before thread creation 2
Thread2 created successfully
Thread: 2, Transactions: 16 
Thread: 1 Iteration: 1 Count: 155
Thread: 2 Iteration: 1 Count: 175
Thread: 0 Iteration: 2 Count: 219
Thread: 0 Iteration: 3 Count: 69
Thread: 0 Iteration: 4 Count: 4
Exited
Thread: 1 Iteration: 2 Count: 3196
Thread: 2 Iteration: 2 Count: 6651
Thread: 1 Iteration: 3 Count: 7764
Thread: 1 Iteration: 4 Count: 7744
Thread: 1 Iteration: 5 Count: 4771
Thread: 2 Iteration: 3 Count: 34939
Thread: 1 Iteration: 6 Count: 2007
Thread: 1 Iteration: 7 Count: 570
Thread: 1 Iteration: 8 Count: 99
Thread: 1 Iteration: 9 Count: 8
Exited
Thread: 2 Iteration: 4 Count: 55137
Thread: 2 Iteration: 5 Count: 52014
Thread: 2 Iteration: 6 Count: 38807
Thread: 2 Iteration: 7 Count: 24179
Thread: 2 Iteration: 8 Count: 12248
Thread: 2 Iteration: 9 Count: 4851
Thread: 2 Iteration: 10 Count: 1470
Thread: 2 Iteration: 11 Count: 325
Thread: 2 Iteration: 12 Count: 46
Thread: 2 Iteration: 13 Count: 3
Exited


*/