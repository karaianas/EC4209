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

	/*printf("[is_conn] start connectivity check\n");
	printf("[is_conn] root = %d%d\n", root->get_track(), root->get_num());*/

	// initialize
	to_visit.push_back(root);

	while (first < to_visit.size())
	{
		/*printf("[is_conn] (while %d) first = %d\n", first+1, first);
		printf("[is_conn]          to_visit.size = %d\n", to_visit.size());
		printf("[is_conn]          graph.size = %d\n", G->get_size());*/


		/* loop the neighbors of to_visit[first]
		 * if not in visited list, push back to to_visit
		 */
		for (i = 0; i < G->get_size(); i++)
		{
			tmp_crs = G->get_course(i);

			/*printf("[is_conn] (big_for_%d) neighbor tmp_crs: %d%d\n", \
				i, tmp_crs->get_track(), tmp_crs->get_num());
		

			printf("%d%d\n", \
				tmp_crs->get_track(), tmp_crs->get_num());*/

			if (tmp_crs == to_visit[first])
				continue;

			/*if (tmp_crs == dest) {
				printf("[is_conn] tmp_crs = %d%d\t == dest = %d%d\n", \
					tmp_crs->get_track(), tmp_crs->get_num(), \
					dest->get_track(), dest->get_num());
				return true;
			}*/

			// if tmp_crs == neighbor
			if (G->get_correlation(to_visit[first], tmp_crs) > 0)
			{
				if (tmp_crs == dest)
					return true;

				not_visited = true;

				//printf("[is_conn] corr > 0\n");

				// if tmp_crs not in visited list, push back to to_visit
				for (j = 0; j < visited.size(); j++)
				{
					if (visited[j] == tmp_crs) {
						not_visited = false;
						break;
					}
				}

				/*for (int k = first; k < to_visit.size(); k++)
					printf("to_visit: %d%d ", to_visit[k]->get_track(), to_visit[k]->get_num());
				printf("\n");*/

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