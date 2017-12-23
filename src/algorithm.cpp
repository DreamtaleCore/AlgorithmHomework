/************************************************
 * @author Liu Yunfei
 * @stuID  BY1706126
 * @email  lyunfei@buaa.edu.cn
 * All rights reserved
 ***********************************************/
#include <cmath>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <algorithm>

using namespace std;

namespace test
{
	const string CITY_PATH_LENGTH_FILE = "../data/m1.txt";
	const string CITY_PATH_COST_FILE = "../data/m2.txt";
	const int SUM_CITIES = 50;
	const int SUM_MAX_COST = 1500;
	const int CANNOT_ACHIEVE = 9999;
	const int INDEX_A = 0;
	const int INDEX_B = 49;

	struct QueueItem
	{
		int index;
		int length;
		int cost;
		QueueItem() 
		{
			index = length = cost = 0;
		}
		QueueItem(int a, int b, int c)
		{
			index = a;
			length = b;
			cost = c;
		}
		friend bool operator<(QueueItem a, QueueItem b)
		{
			return  ((a.length >  b.length));
		}
	};

	/************************************************************************/
	/* Split a string into strings by a pattern                             */
	/************************************************************************/
	inline vector<string> split(const string &str, const string &pattern)
	{
		//const char* convert to char*
		char * strc = new char[strlen(str.c_str()) + 1];
		strcpy(strc, str.c_str());
		vector<string> rlts;
		char* tmp_str = strtok(strc, pattern.c_str());
		while (tmp_str != NULL)
		{
			rlts.push_back(string(tmp_str));
			tmp_str = strtok(NULL, pattern.c_str());
		}

		delete[] strc;

		return rlts;
	};

	/************************************************************************/
	/* Load data to matrix from txt file                                    */
	/************************************************************************/
	inline int readDataToMatrix(string file_path, vector<vector<int> >& m)
	{
		fstream in;
		in.open(file_path, ios::in);
		char line_buf[512];
		int ret = -1;
		// Read string from file as line
		while (in.is_open() && !in.eof())
		{
			in.getline(line_buf, 512);
			string line = line_buf;
			// Split line into number strings by table
			vector<string> line_vec = split(line, "\t ");
			vector<int> row_vec;
			for (string elem : line_vec)
			{
				// Convert string to number and append it to 
				stringstream ss;
				ss << elem;
				int a;
				ss >> a;
				row_vec.push_back(a);
			}
			if(row_vec.size() == SUM_CITIES)
				m.push_back(row_vec);
			ret = 0;
		}

		assert(m.size() == SUM_CITIES && m[0].size() == SUM_CITIES);
		return ret;
	}

	/************************************************************************/
	/* Generate minimum path and record them into matrix                    */
	/************************************************************************/
	void floyd(vector<vector<int> > l, vector<vector<int> > &prev, vector<vector<int> > &path)
	{
		// Init the prev and path matrix
		for (int i = 0; i < SUM_CITIES; i++)
			for (int j = 0; j < SUM_CITIES; j++)
			{
				path[i][j] = l[i][j];
				prev[i][j] = j;
			}
		for (int k = 0; k < SUM_CITIES; k++)
			for (int i = 0; i < SUM_CITIES; i++)
				for (int j = 0; j < SUM_CITIES; j++)
				{
					if(path[i][k] != CANNOT_ACHIEVE && 
					   path[k][j] != CANNOT_ACHIEVE &&
					   path[i][k] + path[k][j] < path)
				}
	}

	/************************************************************************\
	|* Using branch and bound algorithm to solve                             |
	|*       the min transport path between cities	         				 |
	\************************************************************************/
	int branchAndBound(vector<int>& path, int &min_dis, int &min_cost)
	{
		int ret = 0;
		min_cost = min_dis = 0;
		vector<vector<int> > m_length, m_cost;
		vector<int> dist(SUM_CITIES), prev(SUM_CITIES), cost(SUM_CITIES);
		readDataToMatrix(CITY_PATH_LENGTH_FILE, m_length);
		readDataToMatrix(CITY_PATH_COST_FILE, m_cost);

		// Make a priority queue to storage the process result
		priority_queue <QueueItem> proc_queue;
		// Init the current node as index 0(city A's index) and path cost is 0
		// current node's first is node index, and second is current cost
		QueueItem cnode;
		// Init all dist as cannot achieve and cost is 0
		for (int i = 0; i < SUM_CITIES; i++)
		{
			dist[i] = CANNOT_ACHIEVE;
			cost[i] = 0;
		}
		// Init distance's first cost is 0 
		dist[cnode.index] = 0;
		// Of course, the first node is A, pre-index is -1;
		prev[0] = -1;

		while (true)
		{
			// Build the search queue
			for (int j = 0; j <= INDEX_B; j++)
			{	
				if (m_length[cnode.index][j] != CANNOT_ACHIEVE)
				{
					int tmp_length = cnode.length + m_length[cnode.index][j];
					int tmp_cost = cnode.cost + m_cost[cnode.index][j];
					//if (tmp_length < dist[j] && tmp_cost <= SUM_MAX_COST)
					if (tmp_length < dist[j])
					{
						dist[j] = tmp_length;
						prev[j] = cnode.index;
						cost[j] = tmp_cost;
						QueueItem node = QueueItem(j, dist[j], cost[j]);
						proc_queue.push(node);  
					}
				}
			}
			// Bound condition
			if (proc_queue.empty()) break;
			else
			{
				cnode = proc_queue.top();
				proc_queue.pop();
			}
		}
		// Generate the path
		int index = INDEX_B;
		while (prev[index] != -1)
		{
			path.push_back(index);
			index = prev[index];
		}
		reverse(path.begin(), path.end());

		//Compute the minimum distance and cost
		int prev_idx = INDEX_A;
		for (int index : path)
		{
			min_dis += m_length[prev_idx][index];
			min_cost += m_cost[prev_idx][index];
			prev_idx = index;
		}
		return ret;
	}

}

int main()
{
	// Main algorithm structure here
	vector<int> path;
	int min_distance, min_cost;
	test::branchAndBound(path, min_distance, min_cost);
	
	cout << "Minimum distance is: " << min_distance << endl;
	cout << "Minimum cost is: " << min_cost << endl;
	cout << "The path is ";
	bool is_first = true;
	cout << "1";
	for (int p : path)
	{
		cout << " -> " << p + 1;
	}
	cout << "\n and total minimal cost is: " << min_cost <<
		(min_cost < test::SUM_MAX_COST ?
			". it's a path that matches the question." :
			". but sorry that cannot fit the question.") << endl;

	getchar();
	return 0;
}
