/************************************************
 * @author Liu Yunfei
 * @stuID  BY1706126
 * @email  lyunfei@buaa.edu.cn
 * All rights reserved
 ***********************************************/
#include <cmath>
#include <queue>
#include <deque>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <algorithm>

using namespace std;

namespace algor
{
    const int MAX_CITIES = 50;
    const int MAX_INT    = 9999;    // unreachable between cities
    const int MAX_COST   = 1500;
    const int INDEX_A    = 0;
    const int INDEX_B    = 49;
    const string PATH_DIST_FILE = "../data/m1.txt";
    const string PATH_COST_FILE = "../data/m2.txt";

    struct Node
    {
        int index;
        int length;
        int cost;
        Node()
        {
            index = length = cost = 0;
        }
        Node(int _index, int _length, int _cost)
        {
            index = _index;
            length = _length;
            cost = _cost;
        }
    };

    /**
    * @func     split
    * @param    str     source string to be split
    * @param    pattern split pattern
    */
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

    /**
    * @func     readDataToMatrix
    * @param    filePath    
    * @param    m           The matrix retured
    */
    inline int readDataToMatrix(string filePath, vector<vector<int> >& m)
    {
        fstream in;
        in.open(filePath, ios::in);
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
            if (row_vec.size() == MAX_CITIES)
                m.push_back(row_vec);
            ret = 0;
        }

        assert(m.size() == MAX_CITIES && m[0].size() == MAX_CITIES);
        return ret;
    }
    
    /**
    * @func floyd
    * @param l      the length matrix, or called m1
    * @param prev   [return] record the shortest node's previous node's index
    * @param dist   [return] record the minimum distance between cities
    */
    void floyd(vector<vector<int> > l, vector<vector<int> > &prev, vector<vector<int> > &dist)
    {
        // Init the prev and path matrix
        dist.clear();
        prev.clear();
        for (int i = 0; i < MAX_CITIES; i++)
        {
            vector<int> tmp(MAX_CITIES);
            dist.push_back(tmp);
            prev.push_back(tmp);
        }
        for (int i = 0; i < MAX_CITIES; i++)
            for (int j = 0; j < MAX_CITIES; j++)
            {
                dist[i][j] = l[i][j];
                prev[i][j] = j;
            }

        for (int k = 0; k < MAX_CITIES; k++)
            for (int i = 0; i < MAX_CITIES; i++)
                for (int j = 0; j < MAX_CITIES; j++)
                {
                    if (dist[i][k] != MAX_INT &&
                        dist[k][j] != MAX_INT &&
                        dist[i][k] + dist[k][j] < dist[i][j])
                    {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        prev[i][j] = prev[i][k];
                    }
                }
    }

    /**
    * @func branchAndBound
    * @param queueMinPath
    * @param minPath
    * @param costOfPath
    */
    void branchAndBound(deque<Node>& queueMinPath, int& minPath, int& costOfMinPath)
    {
        cout << "Begin to compute ..." << endl;

        vector<vector<int> > m1, m2;
        cout << "Loading data from txt file ..." << endl;
        readDataToMatrix(PATH_DIST_FILE, m1);
        readDataToMatrix(PATH_COST_FILE, m2);

        // Init m2 depend on m1, if unreachable between cities
        for (int i = 0; i < MAX_CITIES; i++)
            for (int j = 0; j < MAX_CITIES; j++)
                if (m1[i][j] == MAX_INT)
                    m2[i][j] = MAX_INT;

        // Init minPath to a large number
        minPath = MAX_INT;
        // Record one city whether in a path or not
        vector<int> node_in(MAX_CITIES);

        cout << "Core algorithm begin ..." << endl;
        vector<vector<int> > pdm, mdm, pcm, mcm;
        // We use mdm(minimum distance matrix) to record the minimum
        // distance between cities, pm(previous matrix) to record the 
        // node's minimum path -- privious node's index
        // pcm(previous cost matrix), mcm(minimum cost matrix)
        cout << "Computing every two cities minimum distance and cost ..." << endl;
        floyd(m1, pdm, mdm);
        floyd(m2, pcm, mcm);
        for (int i = 0; i < MAX_CITIES; i++)
        {
            mdm[i][i] = mcm[i][i] = 0;
        }

        cout << "Bounding condition: max cost must be less than " << MAX_COST << endl;
        deque<Node> deque_nodes;
        // At first, push the A city's index
        Node cur_node(INDEX_A, 0, 0);
        deque_nodes.push_back(cur_node);
        // Prepare for pruning
        bool is_find_next_city = false;
        vector<int> n_path(MAX_CITIES, 0);
        n_path[0] = 1;
        Node last_out_node(-1, 0, 0);
        while (!deque_nodes.empty())
        {
            cur_node = deque_nodes.back();
            for (int i = 0; i < MAX_CITIES; i++)
            {
                // Bounding condition
                if (n_path[i] == 1 || i <= last_out_node.index)
                    continue;
                if (m1[cur_node.index][i] != MAX_INT)
                {
                    // Deside whether prune or not depend on curret node's length and cost
                    // Case 1: current path's optimal solution is worse than before
                    //          or current path's cost larger than MAX_COST
                    if (cur_node.length + mdm[i][INDEX_B] >= minPath ||
                        cur_node.cost + mcm[i][INDEX_B] > MAX_COST)
                        continue;
                    // Case 2: the exist solution is also not so good, do prune
                    if (cur_node.length + mdm[cur_node.index][i] >= minPath ||
                        cur_node.cost + mcm[cur_node.index][i] > MAX_COST)
                        continue;
                    // If find a better path, update minPath and cur_node and put it to deque
                    if (i == INDEX_B)
                    {
                        minPath = cur_node.length + m1[cur_node.index][i];
                        cur_node.length = minPath;
                        costOfMinPath = cur_node.cost + m2[cur_node.index][i];
                        cur_node.cost = costOfMinPath;
                        cur_node.index = i;
                        deque_nodes.push_back(cur_node);
                        // Find the result!
                        queueMinPath = deque_nodes;
                        break;
                    }
                    else
                    {
                        is_find_next_city = true;
                        n_path[i] = 1;
                        cur_node.length += m1[cur_node.index][i];
                        cur_node.cost += m2[cur_node.index][i];
                        cur_node.index = i;
                        deque_nodes.push_back(cur_node);
                        last_out_node.index = -1;
                        break;
                    }
                }
            }
            // If there is not a better path, current city go out of deque
            if (!is_find_next_city)
            {
                last_out_node = deque_nodes.back();
                deque_nodes.pop_back();
                if (deque_nodes.empty())
                    break;
                cur_node = deque_nodes.back();
                n_path[last_out_node.index] = 0;
                cur_node.length = last_out_node.length - m1[cur_node.index][last_out_node.index];
                cur_node.cost = last_out_node.cost - m2[cur_node.index][last_out_node.index];
                // Update the back of deque
                deque_nodes.pop_back();
                deque_nodes.push_back(cur_node);
            }
            is_find_next_city = false;
        }
        cout << "Branch and bound completed." << endl;
    }
    
    void branchAndBound_test()
    {
        time_t time_start, time_end;
        time_start = clock();

        deque<Node> queue_min_path;
        int min_path, cost_of_path;
        branchAndBound(queue_min_path, min_path, cost_of_path);

        cout << "---------------- Result -------------------" << endl;
        cout << "The shortest path's length is: " << min_path << endl;
        cout << "The shortest path's cost is:   " << cost_of_path << endl;
        cout << "The path is as follows: " << endl;
        cout << queue_min_path[0].index + 1;
        for (int i = 1; i < queue_min_path.size(); i++)
            cout << " -> " << queue_min_path[i].index + 1;
        time_end = clock();
        cout << "\nTotal cost time: " << (double)(time_end - time_start) / CLOCKS_PER_SEC 
             << " s." << endl;
    }
}

int main()
{
	// Main algorithm structure here
    algor::branchAndBound_test();

	getchar();
	return 0;
}
