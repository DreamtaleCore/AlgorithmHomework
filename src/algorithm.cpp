/************************************************
 * @author Liu Yunfei
 * @stuID  BY1706126
 * @email  lyunfei@buaa.edu.cn
 * All rights reserved
 ***********************************************/
#include<iostream>
#include<cmath>
#include<vector>
#include <cassert>

using namespace std;

namespace algor2
{
	// Convex polygon's edges number
	const int N = 8;
	// Every weight of edge which connected two vertex
	const int TABLE[N][N] = {				
		{0, 14, 25, 27, 10, 11, 24, 16},
		{14, 0, 18, 15, 27, 28, 16, 14},
		{25, 18, 0, 19, 14, 19, 16, 10},
		{27, 15, 19, 0, 22, 23, 15, 14},
		{10, 27, 14, 22, 0, 14, 13, 20},
		{11, 28, 19, 23, 14, 0, 15, 18},
		{24, 16, 16, 15, 13, 15, 0, 27},
		{16, 14, 10, 14, 20, 18, 27, 0}
	};

	/************************************************************************/
	/* Using minimum triangle weight method, return the total weight of it. */
	/************************************************************************/
	inline int getTriangleWeight(int a, int b, int c)
	{
		assert(a < N && b < N && c < N);
		assert(a >= 0 && b >= 0 && c >= 0);

		return TABLE[a][b] + TABLE[b][c] + TABLE[a][c];
	}

	/************************************************************************/
	/* Show the result edges that share the minimal weights                 */
	/************************************************************************/
	inline void tracebackEdges(int i, int j, vector<vector<int> > edges)
	{
		if (i == j) return;
		tracebackEdges(i, edges[i][j], edges);
		tracebackEdges(edges[i][j] + 1, j, edges);
		cout << "The split triangle points: v" << i - 1 << ", v" << j << ", v" << edges[i][j] << endl;
	}

	/**
	* @brief	Compute the minimum weight of triangles which split a convex polygon
	* @params	edges	the result of vertex of triangles 
	* @return	The optimal result(numeric)
	*/
	int minWeightTriangulation(vector<vector<int>>& edges)
	{
		// Init processing matrix and result edge matrix
		int t[N][N];
		for (int i = 1; i <= N; i++)	t[i][i] = 0;

		// Begin to generate the minimal weight triangle points: edges
		// and the sum of edges
		for (int r = 1; r < N; r++)
		{
			// The first vertex of the triangle index
			for (int i = 1; i < N - r; i++)
			{
				// The end vertex of the triangle index
				int j = i + r;
				// Init the cost of weight [i][j]
				t[i][j] = t[i + 1][j] + getTriangleWeight(i - 1, i, j);
				// Init the chosen edges' vertex as i
				edges[i][j] = i;
				// Searching the best(minimum) weight of sub-question
				for (int k = i + 1; k < j; k++)
				{
					int tmp = t[i][k] + t[k + 1][j] + getTriangleWeight(i - 1, k, j);
					// If the sub-question is better, update the result
					if (tmp < t[i][j])
					{
						t[i][j] = tmp;
						edges[i][j] = k;
					}
				}
			}
		}
		// So the t[1][N-1] storage the sum of all minimum weight of triangles
		return t[1][N - 1];
	}

	void minWeightTriangulation_test()
	{
		// Init and get storage space of edges
		vector<vector<int> > edges;
		for (int i = 0; i < N; i++)
		{
			edges.push_back(vector<int>(N));
		}
		// Run it
		int rlt = minWeightTriangulation(edges);
		cout << "The optimal result is: " << rlt << endl;
		tracebackEdges(1, 7, edges);
	}
}

namespace algor1
{

}

int main()
{
	algor2::minWeightTriangulation_test();

	getchar();

	return 0;
}
