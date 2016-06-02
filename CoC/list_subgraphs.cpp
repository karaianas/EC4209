/* list_subgraphs
 *	BFS search the graph G until it finds all the connected components
 *	then add the found component to vector, 
 */
#pragma once

#include <iostream>
#include <vector>

#include "Graph.h"
#include "Course.h"

vector<Course*>* bfs_connected_component(Graph* G, Course* root);

void list_subgraphs(Graph* G, vector<Graph*>* sub_list)
{
	Course* root = new Course();
	vector<Course*>* sub_nodes = new vector<Course*>();
	Graph* G_copy = new Graph(G);
	vector<Course*>* neighbor = new vector<Course*>();
	Course* cur_node;

	while (G_copy->get_num_edge())
	{
		// find new root
		root = G_copy->get_random_vertex();

		// find connected component (i.e., subgraph)
		sub_nodes = bfs_connected_component(G_copy, root);
		
		Graph* subgraph = new Graph();

		// initialize subgraph
		for (int i = 0; i < sub_nodes->size(); i++)
		{
			subgraph->add_index(sub_nodes->at(i));
			for (int j = i; j < sub_nodes->size(); j++)
			{
				if (i == j)
				{
					subgraph->set_correlation(sub_nodes->at(i), sub_nodes->at(j), 0);
					continue;
				}
				
				subgraph->add_index(sub_nodes->at(j));
				subgraph->set_correlation(sub_nodes->at(i), sub_nodes->at(j), 0);
				subgraph->set_correlation(sub_nodes->at(j), sub_nodes->at(i), 0);
			}
		}
	
		while (sub_nodes->size()) {
			cur_node = sub_nodes->back();
			int i = G_copy->get_index(cur_node);
			neighbor = G_copy->get_neighbors(cur_node);

			for (int j = 0; j < neighbor->size(); j++)
			{
				// copy subnodes to subgraph first
				Course* crs_i = G_copy->get_course(i);
				Course* crs_j = neighbor->at(j);

				subgraph->modify_correlation(crs_i, crs_j, G_copy->get_correlation(crs_i, crs_j));
				subgraph->modify_correlation(crs_j, crs_i, G_copy->get_correlation(crs_j, crs_i));

				// remove subnodes from G_copy
				G_copy->modify_correlation(crs_i, crs_j, 0);
				G_copy->modify_correlation(crs_j, crs_i, 0);
			}

			sub_nodes->pop_back();
		}

		// push back the subgraph to sub_list
		sub_list->push_back(subgraph);
	}
}

vector<Graph*>* cut_subgraphs(Graph* G, float thres) {
	vector<Graph*>* cut_Gs = new vector<Graph*>();
	Graph* G_cp = new Graph(G);
	
	G_cp->remove_less_threshold(thres);
	list_subgraphs(G_cp, cut_Gs);

	return cut_Gs;
}