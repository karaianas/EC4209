#pragma once

#include <iostream>
#include <vector>

#include "Color.h"
#include "Graph.h"
#include "Course.h"

using namespace std;

/* init_coloring
*	본격 coloring을 시작하기 전에 앞서
*	모든 Course의 selected_color를 -1로 initialize
*	아무 것도 하지 않았는데 coloring이 되어 있는
*	vertex들이 있어서 준비했음. 데헷-
*/
void init_coloring(vector<Graph*>* to_init)
{
	for (int i = 0; i < to_init->size(); i++) {
		vector<Course*>* crs_list_i = to_init->at(i)->get_course_list();
		for (int j = 0; j < crs_list_i->size(); j++) {
			crs_list_i->at(j)->init_selected_color();
		}
	}
}

void graph_coloring(Graph* to_color, int color_limit)
{
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