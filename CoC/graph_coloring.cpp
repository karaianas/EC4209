#pragma once

#include <iostream>
#include <vector>

#include "Color.h"
#include "Graph.h"
#include "Course.h"

using namespace std;

void graph_coloring(Graph* to_color, int color_limit)
{
	// Color* color_obj = new Color(to_color, color_limit);
	Color* color_obj = new Color(to_color);
	vector<Course*>* crs_list = to_color->get_course_list();
	
	cout << "[g_col_bfs]" << endl;
	for (int i = 0; i < crs_list->size(); i++)
	{
		crs_list->at(i)->print_course_info();
		cout << " initial color: " << crs_list->at(i)->get_select_color() << endl;
	}

	for (int i = 0; i < crs_list->size(); i++)
		color_obj->start_coloring(to_color->get_course_list(), color_limit);
}

void alone_coloring(vector<Course*>* alone_list, int color_limit)
{
	if (!alone_list)
		return;

	Graph* alone_graph = new Graph();
	int alone_num = alone_list->size();
	Course* crs_i = new Course();
	Course* crs_j = new Course();

	for (int i = 0; i < alone_num; i++)
	{
		crs_i = alone_list->at(i);
		alone_graph->add_index(crs_i);
		for (int j = i; j < alone_num; j++)
		{
			alone_graph->add_index(crs_j);
			alone_graph->set_correlation(crs_i, crs_j, 0);
			alone_graph->set_correlation(crs_j, crs_i, 0);
		}
	}

	alone_graph->set_index(alone_list);

	Color* color_obj = new Color(alone_graph);
	color_obj->start_coloring(alone_graph->get_course_list(), color_limit);
}