#include <iostream>
#include <vector>

#include "Graph.h"
#include "Course.h"

vector<Course*>* bfs(Graph* G, Course* root)
{
	vector<Course*>* visited = new vector<Course*>();
	vector<Course*> to_visit;
	Course* tmp_crs;
	int first = 0;
	int i, j;
	bool not_visited;

	// initialize
	to_visit.push_back(root);

	while (first < to_visit.size())
	{
		cout << "[bfs] start" << endl;
		/* loop the neighbors of to_visit[first]
		* if not in visited list, push back to to_visit
		*/
		for (i = 0; i < G->get_size(); i++)
		{
			tmp_crs = G->get_course(i);

			if (tmp_crs == to_visit[first])
				continue;

			// if tmp_crs == neighbor
			if (G->get_correlation(to_visit[first], tmp_crs) > 0)
			{
				not_visited = true;

				// if tmp_crs not in visited list, push back to to_visit
				for (j = 0; j < visited->size(); j++)
				{
					if (visited->at(j) == tmp_crs) {
						not_visited = false;
						break;
					}
				}

				if (not_visited) {
					to_visit.push_back(tmp_crs);
					visited->push_back(tmp_crs);
					cout << tmp_crs->get_course_name() << " ";
				}
			}
		}
		first++;
	}

	cout << endl;

	return visited;
}