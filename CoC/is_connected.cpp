/* is_connected function
 *	Checks if the two nodes in Graph* G are connected using BFS.
 *	Returns true if they are, false otherwise.
 */
#pragma once

#include <vector>

#include "Graph.h"
#include "Course.h"

bool is_connected(Graph* G, Course* root, Course* dest)
{
	vector<Course*> visited;
	vector<Course*> to_visit;
	Course* tmp_crs;
	int first = 0;
	int i, j;
	bool not_visited;

	// if root == dest return true
	if (root == dest)
		return true;

	// initialize
	to_visit.push_back(root);

	while (first < to_visit.size())
	{
		for (i = 0; i < G->get_size(); i++)
		{
			tmp_crs = G->get_course(i);

			if (tmp_crs == to_visit[first])
				continue;

			// if tmp_crs == neighbor
			if (G->get_correlation(to_visit[first], tmp_crs) > 0)
			{
				if (tmp_crs == dest)
					return true;

				not_visited = true;

				// if tmp_crs not in visited list, push back to to_visit
				for (j = 0; j < visited.size(); j++)
				{
					if (visited[j] == tmp_crs) {
						not_visited = false;
						break;
					}
				}

				if (not_visited) {
					to_visit.push_back(tmp_crs);
					visited.push_back(tmp_crs);
				}
			}
		}
		first++;
	}

	return false;
}