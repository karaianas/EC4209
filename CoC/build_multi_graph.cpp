#pragma once

#include <vector>

#include "Graph.h"
#include "Course.h"

void compute_correlation(Graph*, int, int, Course*, Course*);

Graph* build_multi_graph(vector<Course*> course_list)
{
	Graph* G = new Graph();
	for (int i = 0; i < course_list.size(); i++)
	{
		// add index (node)
		G->add_index(course_list[i]);

		// add 
		for (int j = i; j < course_list.size(); j++)
		{
			// add index if j is not found in index
			if (i == 0 || i != j)
				G->add_index(course_list[j]);

			compute_correlation(G, i, j, course_list[i], course_list[j]);

		}
	}

	return G;
}