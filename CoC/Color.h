/*
 *	Functions for coloring a graph
 */
#pragma once

#include "Graph.h"
#include "Course.h"
#include <vector>

// number of colors that can be used
#define NUM 12
// length of the coloring history
#define COLORED_PATH_LENGTH 4

class Color{
public:

	// G 줄 때 이미 threshold 이하의 correlation은 전부 다 끊긴 G를 줘야 함
	// color a given graph G and color frequency limit
	Color(Graph* G, int _limit = 10)
	{
		gptr = G;
		set_color_limit(_limit);
		num_used_colors = 0;
	}
	
	// color a vertex
	bool color_vertex(Course* C, int limit)
	{
		/*cout << "[color_vertex] start coloring course ";
		C->print_course_info(); cout << endl;*/

		// 일단 되는 대로 색깔을 칠하고 있음
		for (int i = 0; i < NUM; i++)
		{
			if (color_frequency[i] <= limit && C->is_color_ok(i))
			{
				// (1) color a vertex
				C->set_color(i);
				/*cout << "\tcolored "; C->print_course_info();
				cout << ": " << C->get_select_color() << endl;*/

				// (2) increase color frequency
				color_frequency[i] += 1;

				// (3) remove the color from its neighbors
				vector<Course*>* neigbor = gptr->get_uncolored_neighbors(C);
				int num_neighbors = gptr->get_uncolored_neighbors(C)->size();

				cout << "Removing the color from uncolored neighbors..." << endl;
				for (int j = 0; j < num_neighbors; j++)
				{
					if (!neigbor->at(j)->remove_color(i))
					{
						cout << "Color clash at ";
						neigbor->at(j)->print_course_info();
						return false;
					}
				}
				cout << "Successfully removed the color from uncolored neighbors!" << endl;

				// (4) add the vertex to the colored path set
				colored_path.push_back(C);

				return true;
			}
		}

		// the vertex was not colored
		cout << "Cannot be colored: ";
		C->print_course_info();
		
		return false;
	}

	// color the graph
	bool start_coloring(vector<Course *>* course_list, int limit)
	{
		//Course* start = get_init_vertex(course_list);
		Course* start = get_max_degree_vertex(course_list);

		int size = gptr->get_size();
		int purity = 1;
		// (1) color starting vertex
		while (purity)
		{
			int check=color_vertex(start, limit);
			if (!check)
			{
				std::cout << "Choose another starting vertex!" << endl;
//				course_list.erase(start->get_id());
				start = get_min_colors_vertex(course_list);
				continue;	//다른 점이 색칠 될때까지 계속 진행.
			}
			else
			{
				cout << "Colored vertex is : ";
				start->print_course_info(); cout << endl;
				return true;
			}
		}
	}

	
	// get an optimal color
	int get_opt_color()
	{

	}


	// get starting vertex of graph G
	Course* get_init_vertex(vector<Course *>* list)
	{
		return NULL;
	}

	// Graph에 존재하는 get_vertex_degree로 짜서 만든건데 혹시 이게
	// color가 정해진 거를 degree에서 제외시키는 게 없으면 오류가 나요
	// MRV
	//vector<Course*>* get_max_degree_vertex( vector<Course*>* list )
	Course* get_max_degree_vertex(vector<Course*>* list)
	{
		//// Color 정해진 vertex들을 degree에서 제거
		//vector<int> net_degree;
		//for (int i = 0; i < list->size(); i++)
		//{

		//}

		// vector<Course*>* max_degree_course = NULL;
		int max_degree = -1;
		Course* max_vertex = new Course();
		for (int i = 0; i < list->size(); i++)
		{
			Course* temp = list->at(i);
			if ( (gptr->get_vertex_degree(temp) > max_degree) && \
				temp->get_select_color() == -1)
			{
				max_degree = gptr->get_vertex_degree(temp);
				max_vertex = temp;
			}
			/*if (temp->get_num() == 24) {
				temp->print_course_info();
				cout << "coloring: " << temp->get_select_color() << endl;
			}*/
		}

		/*cout << "[get_max_deg_ver] max_ver: ";
		max_vertex->print_course_info();
		cout << endl;*/

		return max_vertex;

		/*for (int i = 0; i < list->size(); i++)
		{
			Course* temp = list->at(i);
			if (gptr->get_vertex_degree(temp) == max_degree)
				max_degree_course->push_back(temp);
		}

		return max_degree_course;*/
	}
	
	// get vertex of minimum remaining colors i.e. LRV
	// MRV with LCV
	//vector<Course*>* get_min_colors_vertex( vector<Course*>* list )
	Course* get_min_colors_vertex(vector<Course*>* list)
	{
		vector<Course*>* min_remaining_colors_course = NULL;
		int min_remaining_colors = 99999999;
		Course* min_remaining_vertex = new Course();

		for (int i = 0; i < list->size(); i++)
		{
			Course* temp = list->at(i);
			if ( (temp->color_possible_num() < min_remaining_colors) &&
				temp->get_select_color() == -1) {
				min_remaining_colors = temp->color_possible_num();
				min_remaining_vertex = temp;
			}
		}

		/*cout << "[get_min_col_ver] min_ver: ";
		min_remaining_vertex->print_course_info();
		cout << endl;*/

		return min_remaining_vertex;

		/*for (int i = 0; i < list->size(); i++)
		{
			Course* temp = list->at(i);
			if (temp->color_possible_num() == min_remaining_colors)
				min_remaining_colors_course->push_back(temp);
		}

		return min_remaining_colors_course;*/
	}

	// get vertex of maximum correlation
	Course* get_max_corr_vertex( vector<Course*>* list)
	{
		Course* max_corr_vertex = NULL;
		float max_corr = -1.0;

		for (int i = 0; i < list->size(); i++)
		{
			Course* temp = list->at(i);
			float temp_corr = 0.0;
			for (int j = 0; j < gptr->get_size(); j++)
			{
				Course* temp2 = gptr->get_course(j);
				if (temp->get_id() != temp2->get_id())
					temp_corr = temp_corr + gptr->get_correlation(temp, temp2);
			}

			if (temp_corr > max_corr)
			{
				max_corr = temp_corr;
				max_corr_vertex = temp;
			}
		}
		return max_corr_vertex;
	}

	/*vector<Course*>* get_course_list()
	{
		return gptr->get_course_list();
	}*/

private:

	Graph* gptr; // pointer to the given graph for convenience
	int num_used_colors; // number of colors actually used
	int color_frequency[NUM];

	vector<Course*> colored_path;

	// set color frequency limit
	void set_color_limit(int limit)
	{
		assert(limit > 0);

		for (int i = 0; i < NUM; i++)
			color_frequency[i] = 0;
	}
};