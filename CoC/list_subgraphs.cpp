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
Course* find_crs_with_index(int index, vector<Course*>* crs_list);

void list_subgraphs(Graph* G, vector<Graph*>* sub_list, vector<Course*>* alone_list)
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

		//
		// push back the subgraph to sub_list
		subgraph->set_threshold(G_copy->get_curr_thres());
		sub_list->push_back(subgraph);
	}

	// Compare G and subgraphs to get alone courses
	assert(alone_list);
	//vector<Course*>* alone_list = new vector<Course*>();

	const int size = G->get_size();
	/*bool crs_existence[size];
	for (int i = 0; i < size; i++) {
		*crs_existence++ = false;
	}*/
	vector<Course*>* crs_exists = new vector<Course*>();
		//crs_existence[i] = false;

	for (int i = 0; i < sub_list->size(); i++) {
		vector<Course*>* crs_list = sub_list->at(i)->get_course_list();
		for (int j = 0; j < crs_list->size(); j++) {
			//crs_existence[crs_list->at(j)->get_id()] = true;
			crs_exists->push_back(crs_list->at(j));
		}
	}

	// check the redundancy in crs_exists list
	// push back to alone_list if not already in it
	bool add_ok;
	bool exists;
	vector<Course*>* entire_crs = G->get_course_list();
	for (int i = 0; i < size; i++) {
		exists = false;
		for (int k = 0; k < crs_exists->size(); k++) {
			if (entire_crs->at(i) == crs_exists->at(k)) {
				exists = true;
			}
		}
		if (!exists) {
			add_ok = true;
			Course* cur = crs_exists->at(i);
			for (int j = 0; j < alone_list->size(); j++) {
				if (cur == alone_list->at(j)) {
					add_ok = false;
					break;
				}
			}
			if (add_ok)
				alone_list->push_back(cur);
		}
	}
	/*vector<Course*>* entire_crs = G->get_course_list();
	for (int i = 0; i < size; i++) {
		if (crs_existence[i])
			continue;
		Course* alone = find_crs_with_index(i, entire_crs);
		alone_list->push_back(alone);
	}*/

	//delete[] crs_existence;
	//return alone_list;
}

Course* find_crs_with_index(int index, vector<Course*>* crs_list)
{
	for (int i = 0; i < crs_list->size(); i++) {
		if (crs_list->at(i)->get_id() == index)
			return crs_list->at(i);
	}
	return NULL;
}