#include <bits/stdc++.h>
#include <cmath>
#include <float.h>
using namespace std;

struct DTreeNode
{
	string attr;
	map<string, DTreeNode*> child;
	bool leaf;
};

vector<string> attrs;
map<string, int> attr_loc;
map<int, string> loc_attr;
vector<vector<string> > tuples;
map<string, set<string> > attr_values;

void get_data()
{
	int no_of_attr;
	int no_of_tups;
	ifstream fin("database.txt", ios::in);
	fin>>no_of_attr;
	fin>>no_of_tups;
	for(int i=0; i<no_of_attr; i++)
	{
		string attr; fin>>attr; attrs.push_back(attr);
		attr_loc[attr]=i;
		loc_attr[i]=attr;
	}

	for(int i=0; i<no_of_tups; i++)
	{
		vector<string> tup;
		for(int j=0; j<no_of_attr; j++)
		{
			string val; fin>>val; tup.push_back(val);
			attr_values[loc_attr[j]].insert(val);
		}
		tuples.push_back(tup);
	}
}

string select_attr(vector<string> rem_attrs, vector<vector<string> > rem_tups)
{
	double infoD=0.0;

	map<string, int> ci;

	for(int i=0; i<rem_tups.size(); i++)
	{
		ci[rem_tups[i][rem_tups[i].size()-1]]++;
	}
	
	map<string, int>::iterator it;
	for(it=ci.begin(); it!=ci.end(); it++)
	{
		double logprob = ((double)it->second / (double)rem_tups.size()) *  log2((double)it->second / (double)rem_tups.size());
		infoD -= logprob;
	}

	string best_attr;
	double max_info_gain = DBL_MIN;

	for(int i=0; i<rem_attrs.size()-1; i++)
	{
		set<string>::iterator it2;
		double infoAD=0.0;
		for(it2=attr_values[rem_attrs[i]].begin(); it2!=attr_values[rem_attrs[i]].end(); it2++)
		{
			double infoDj=0.0;
			string curr_val=*it2;

			vector<vector<string> > rem_tups2;

			for(int j=0; j<rem_tups.size(); j++)
				if(rem_tups[j][attr_loc[rem_attrs[i]]]==curr_val)
					rem_tups2.push_back(rem_tups[j]);

			if(rem_tups2.size()==0)continue;

			map<string, int> ci2;

			for(int k=0; k<rem_tups2.size(); k++)
			{
				ci[rem_tups2[k][rem_tups2[k].size()-1]]++;
			}

			map<string, int>::iterator it2;
			for(it2=ci2.begin(); it2!=ci2.end(); it2++)
			{
				double logprob2 = ((double)it2->second / (double)rem_tups2.size()) *  log2((double)it2->second / (double)rem_tups2.size());
				infoDj -= logprob2;
			}

			infoAD +=  ((double)rem_tups2.size() / (double)rem_tups.size() ) * infoDj ;
		}

		if( (infoD - infoAD) > max_info_gain)
		{
			max_info_gain = (infoD - infoAD);
			best_attr=rem_attrs[i];
		}
	}

	return best_attr;
}

DTreeNode* createTree(vector<string> rem_attrs, vector<vector<string> > rem_tups)
{
	if(rem_tups.size()==1)return NULL;
	
	DTreeNode* nn = new DTreeNode;

	int flag=0;
	string cls = rem_tups[0][rem_tups[0].size()-1];
	for(int i=1; i<rem_tups.size(); i++)
	{
		if(rem_tups[i][rem_tups[i].size()-1]!=cls)
		{
			flag=1; break;
		}
	}

	if(flag==0)
	{
		nn->leaf=true;
		nn->attr=cls;
		return nn;
	}

	if(rem_attrs.size()==0)
	{
		map<string, int> max_probs;

		for(int i=0; i<rem_tups.size(); i++)
		{
			max_probs[rem_tups[i][rem_tups[i].size()-1]]++;
		}

		map<string, int>::iterator it;
		int max_freq=0;
		string max_cls;
		for(it=max_probs.begin(); it!=max_probs.end(); it++)
		{
			if(it->second > max_freq)
			{
				max_freq=it->second;
				max_cls=it->first;
			}
		}

		nn->leaf=true;
		nn->attr=max_cls;
		return nn;
	}
	//cout<<"Here0"<<endl;

	string best_splitter=select_attr(rem_attrs, rem_tups);

	//cout << best_splitter << "k" << endl;

	nn->attr=best_splitter;
	nn->leaf=false;
	vector<string> rem_attrs2;

	for(int i=0; i<rem_attrs.size(); i++)
		if(rem_attrs[i]!=best_splitter)
			rem_attrs2.push_back(rem_attrs[i]);

	set<string> dist_vals = attr_values[best_splitter];
	set<string>::iterator it;
	
	for(it=dist_vals.begin(); it!=dist_vals.end(); it++)
	{
		string curr_attr=*it;
		
		vector<vector<string> > rem_tups2;
		for(int i=0; i<rem_tups.size(); i++)
		{
			if(rem_tups[i][attr_loc[best_splitter]]==curr_attr)rem_tups2.push_back(rem_tups[i]);
		}
		//cout<<"Rem"<<rem_tups2.size()<<endl;
		if(rem_tups2.size()==0)
		{
			DTreeNode* nn2 = new DTreeNode;
			map<string, int> max_probs;

			for(int i=0; i<rem_tups.size(); i++)
			{
				max_probs[rem_tups[i][rem_tups[i].size()-1]]++;
			}

			map<string, int>::iterator it2;
			int max_freq=0;
			string max_cls;
			for(it2=max_probs.begin(); it2!=max_probs.end(); it2++)
			{
				if(it2->second > max_freq)
				{
					max_freq=it2->second;
					max_cls=it2->first;
				}
			}

			nn2->leaf=true;
			nn2->attr=max_cls;
			nn->child[curr_attr]=nn2;
		}
		else nn->child[curr_attr]=createTree(rem_attrs2, rem_tups2);
	}
	
	return nn;
}

void inorder(DTreeNode* root)
{
	if(root==NULL)return;

	if(!root->leaf)cout << root->attr <<" ";

	map<string, DTreeNode*>::iterator it;

	for(it=root->child.begin(); it!=root->child.end(); it++)
	{
		inorder(it->second);
	}
}

int main()
{
	get_data();

	DTreeNode* root=NULL;
	vector<string> rem_attrs=attrs;
	//rem_attrs.pop_back();
	vector<vector<string> > rem_tups=tuples;
	root=createTree(rem_attrs, rem_tups);
	inorder(root);
	return 0;
}