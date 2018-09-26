#include <bits/stdc++.h>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
vector<vector<int> > generate_transaction()
{
	vector<vector<int> > h;
	
	for(int i=0; i<50; i++)
	{
		int no_of_item=0;
		while(no_of_item==0)no_of_item=rand()%200;
		
		set<int> taken;
		vector<int> willtake;
		for(int j=0; j<no_of_item; j++)
		{
			int curr_item=rand()%200;
			while(taken.find(curr_item)!=taken.end())
			{
				curr_item = rand()%200;
			}
			taken.insert(curr_item);
			willtake.push_back(curr_item);
		}
		
		h.push_back(willtake);
	}
	
	return h;
}

void put_in_file(vector<vector<int> > h)
{
	ofstream fout("data.txt", ios::out);
	
	fout<<h.size()<<endl;
	for(int i=0; i<h.size(); i++)
	{
		fout<< "T" << " " << i << " " << "N" << " " << h[i].size()<<" ";
		sort(h[i].begin(), h[i].end());
		for(int j=0; j<h[i].size(); j++)fout<<h[i][j]<<" ";
		fout<<"\n";
	}
	fout.close();
}

int main(int argc, char** argv) {
	vector<vector<int> > trans;
	trans = generate_transaction();
	put_in_file(trans);
	return 0;
}
