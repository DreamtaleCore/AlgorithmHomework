/************************************************
 * @author Liu Yunfei
 * @stuID  BY1706126
 * @email  lyunfei@buaa.edu.cn
 * All rights reserved
 ***********************************************/
#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include<cmath>
#include<vector>

using namespace std;

namespace prim
{
	const int MAX = 100000;
	int VNUM = 10 + 1;			// The sum of vertices
	vector<vector<int>> edge;	// The matrix of adjacent
	vector<int> low_cost;		// Record the min edge between adjacent vertex of every vertex in V_new
	vector<int> add_vnew;		// Mark point whether add in V_new
	vector<int> adjacent;		// Record the vertex in V which share the min edge with vertex in V_new

	int loadEdgeFromFile(string filename)
	{
		fstream file_in;
		file_in.open(filename, ios::in);
		if (file_in.is_open())
		{
			while (!file_in.eof())
			{
				char line_buf[512];
				file_in.getline(line_buf, 512);
				string line = line_buf;
				stringstream ss;
				ss << line;
				int w;
				vector<int> weight_line;
				while (ss >> w)
				{
					weight_line.push_back(w);
				}
				edge.push_back(weight_line);
			}
			file_in.close();
			// init the global parameters
			VNUM = edge.size();
			low_cost = vector<int>(VNUM);
			for (auto& elem : low_cost) elem = 0;
			add_vnew = vector<int>(VNUM);
			adjacent = vector<int>(VNUM);
			return 0;
		}
		else
		{
			cout << "Cannot open the file: " << filename << endl;
			return -1;
		}
	}

	void compute(int start)
	{
		int sum_weight = 0;
		// Step 1: init
		for (int i = 0; i < VNUM; i++)
		{
			low_cost[i] = edge[start][i];
			add_vnew[i] = -1;		// -1 denotes to out of V_new
		}
		// Step 2: begin
		add_vnew[start] = 0;
		adjacent[start] = start;
		int min_index = start;
		// Step 3: process
		for (int i = 0; i < VNUM - 1; i++)
		{
			int min_value = MAX;
			int v = -1;
			// Step 3.1 find the min vertex's edge beyond V_new
			for (int j = 0; j < VNUM; j++)
			{
				if (add_vnew[j] == -1 && low_cost[j] < min_value)
				{
					min_value = low_cost[j];
					v = j;
				}
			}
			// Step 3.2: add the v_min_index to V_new
			if (v != -1)
			{
				adjacent[v] = min_index;
				cout << adjacent[v] + 1 << "--" << v + 1 << ", weight: " << low_cost[v] << endl;
				add_vnew[v] = 0;
				// Count total path's length
				sum_weight += low_cost[v];
				for (int j = 0; j < VNUM; j++)
				{
					if (add_vnew[j] == -1 && edge[v][j] < low_cost[j])
					{
						low_cost[j] = edge[v][j];
						min_index = v;
					}
				}
			}
		}
		cout << "The minimum weight is " << sum_weight << endl;
	}
}


namespace test
{
	// Test function and modules here
	void run_prim()
	{
		//cout << "Please input the data file path:" << endl;
		string path;
		path = "E:\\home\\BUAA\\¿Î³Ì\\Ëã·¨\\homework\\data\\mst_data.txt";
		//cin >> path;
		int ret = prim::loadEdgeFromFile(path);
		if (ret == 0)
		{
			prim::compute(1);
		}
	}
}

int main()
{
	// Main algorithm structure here
	test::run_prim();
	getchar();
	return 0;
}
