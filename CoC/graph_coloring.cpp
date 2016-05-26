#pragma once

#include <iostream>
#include <vector>

#include "Color.h"
#include "Graph.h"
#include "Course.h"

using namespace std;

/* init_coloring
*	본격 coloring을 시작하기 전에 앞서
*	모든 Course의 selected_color를 -1로 initialize.
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
	
	/*cout << "[g_col_bfs]" << endl;
	for (int i = 0; i < crs_list->size(); i++)
	{
		crs_list->at(i)->print_course_info();
		cout << " initial color: " << crs_list->at(i)->get_select_color() << endl;
	}*/

	for (int i = 0; i < crs_list->size(); i++)
		color_obj->start_coloring(to_color->get_course_list(), color_limit);

	//coloring_start(to_color, color_limit);
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

	//coloring_start(alone_graph, color_obj, color_limit);
}

//// color a vertex
//bool vertex_coloring(Course* C, Color* col, int limit)
//{
//	/*cout << "[color_vertex] start coloring course ";
//	C->print_course_info(); cout << endl;*/
//
//	// 일단 되는 대로 색깔을 칠하고 있음
//	for (int i = 0; i < NUM_COLORS; i++)
//	{
//		if (color_frequency[i] <= limit && C->is_color_ok(i))
//		{
//			// (1) color a vertex
//			C->set_color(i);
//			/*cout << "\tcolored "; C->print_course_info();
//			cout << ": " << C->get_select_color() << endl;*/
//
//			// (2) increase color frequency
//			color_frequency[i] += 1;
//
//			// (3) remove the color from its neighbors
//			vector<Course*>* neigbor = gptr->get_uncolored_neighbors(C);
//			int num_neighbors = gptr->get_uncolored_neighbors(C)->size();
//
//			cout << "Removing the color from uncolored neighbors..." << endl;
//			for (int j = 0; j < num_neighbors; j++)
//			{
//				if (!neigbor->at(j)->remove_color(i))
//				{
//					cout << "Color clash at ";
//					neigbor->at(j)->print_course_info();
//					return false;
//				}
//			}
//			cout << "Successfully removed the color from uncolored neighbors!" << endl;
//
//			// (4) add the vertex to the colored path set
//			colored_path.push_back(C);
//
//			return true;
//		}
//	}
//
//	// the vertex was not colored
//	cout << "Cannot be colored: ";
//	C->print_course_info();
//
//	return false;
//}

// color the graph
//bool coloring_start(Graph* to_color, Color* col, int limit)
//{
//	vector<Course*>* crs_list = to_color->get_course_list();
//	Course* root = col->get_max_degree_vertex(crs_list);
//
//	int size = to_color->get_size();
//	int purity = 1;
//
//	// color the graph using bfs
//	vector<Course*>* visited = new vector<Course*>();
//	vector<Course*> to_visit;
//	vector<Course*>* neighbors;
//	Course* tmp_crs;
//	Course* nom_crs;
//	int first = 0;
//	int i, j;
//	bool not_visited;
//
//	// initialize
//	to_visit.push_back(root);
//
//	while (first < to_visit.size())
//	{
//		/* loop the neighbors of to_visit[first]
//		* if not in visited list, push back to to_visit
//		*/
//		neighbors = to_color->get_neighbors(to_visit[first]);
//
//		nom_crs = col->get_min_colors_vertex(neighbors);
//
//		if (!nom_crs)
//			cout << "[coloring_start] NO AVAILABLE VERTEX W/ MIN COLORS" << endl;
//
//		for (i = 0; i < neighbors->size(); i++)
//		{
//			tmp_crs = neighbors->at(i);
//
//			if (tmp_crs == to_visit[first]) 
//				continue;
//	
//			not_visited = true;
//
//			// if tmp_crs not in visited list, push back to to_visit
//			for (j = 0; j < visited->size(); j++)
//			{
//				if (visited->at(j) == tmp_crs) {
//					not_visited = false;
//					break;
//				}
//			}
//
//			if (not_visited) {
//				to_visit.push_back(tmp_crs);
//				visited->push_back(tmp_crs);
//
//				// color the vertex (since it is not visited yet)
//
//			}
//		}
//		first++;
//	}
//
//	while (purity)
//	{
//		int check = vertex_coloring(start, col, limit);
//		if (!check)
//		{
//			std::cout << "Choose another starting vertex!" << endl;
//			start = get_min_colors_vertex(course_list);
//			continue;	//다른 점이 색칠 될때까지 계속 진행.
//		}
//		else
//		{
//			cout << "Colored vertex is : ";
//			start->print_course_info(); cout << endl;
//			return true;
//		}
//	}
//}