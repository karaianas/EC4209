#pragma once

#include <vector>

#include "Graph.h"
#include "Course.h"

Graph* build_simple_graph(Graph* multi_graph, vector<Course*> course_list)
{
	Graph* simple_graph = new Graph(multi_graph);
	for (int i = 0; i < course_list.size(); i++)
	{
		for (int j = i + 1; j < course_list.size(); j++)
		{
			Course* cour_i = course_list[i];
			Course* cour_j = course_list[j];

			float corr_ij = simple_graph->get_correlation(cour_i, cour_j);
			float corr_ji = simple_graph->get_correlation(cour_j, cour_i);

			/* simplest model that adds up two weights */
			float new_corr;
			
			if (corr_ij < 0 || corr_ji < 0)
				new_corr = -1;
			else
				new_corr = corr_ij + corr_ji;

			simple_graph->modify_correlation(cour_i, cour_j, new_corr);
			simple_graph->modify_correlation(cour_j, cour_i, new_corr);
		}
	}


	return simple_graph;
}