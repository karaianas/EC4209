/*
	Functions for coloring a graph
*/
#pragma once

#include "Graph.h"
#include "Course.h"

// number of colors that can be used
#define NUM 12

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
		// 일단 되는 대로 색깔을 칠하고 있음
		for (int i = 0; i < NUM; i++)
		{
			if (color_frequency[i] <= limit && C->is_color_ok(i))
			{
				// (1) color a vertex
				C->set_color(i);

				// (2) decrease color frequency
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
	bool start_coloring(vector<Course *> course_list, int limit)
	{
		Course* start = get_init_vertex(course_list);
		int size = gptr->get_size();
		int purity = 1;
		// (1) color starting vertex
		while (purity)
		{
			int check=color_vertex(start, limit);
			if (!check)
			{
				cout << "Choose another starting vertex!" << endl;
				course_list.erase(start->get_id());
				start = get_init_vertex(course_list);
				continue;	//다른 점이 색칠 될때까지 계속 진행.
			}
			else
			{
				cout << "Colored vertex is : " << start->get_course_name() << endl;
				return TRUE;
			}
		}
	}

	
	// get an optimal color
	int get_opt_color()
	{

	}


	// get starting vertex of graph G
	Course* get_init_vertex(vector<Course *> list)
	{

	}

	// get vertex of maximum degree i.e. MRV
	Course* get_max_degree_vertex()
	{

	}

	// get vertex of minimum remaining colors i.e. LRV
	Course* get_min_colors_vertex()
	{

	}

	// get vertex of maximum correlation
	Course* get_max_corr_vertex()
	{

	}

private:

	Graph* gptr;// pointer to the given graph for convenience
	int num_used_colors;// number of colors actually used
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