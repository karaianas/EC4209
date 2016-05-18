/* list_subgraphs
 *	BFS search the graph G until it finds all the connected components
 *	then add the found component to vector, 
 */

#include <iostream>
#include <vector>

#include "Graph.h"
#include "Course.h"

vector<Course*>* bfs(Graph* G, Course* root);

void list_subgraphs(Graph* G, vector<Graph*>* sub_list)
{
	Course* root = new Course();
	vector<Course*>* sub_nodes = new vector<Course*>();
	Graph* G_copy = new Graph(G);
	vector<Course*>* neighbor = new vector<Course*>();

	while (G_copy->get_num_edge())
	{
		// find new root
		root = G_copy->get_random_vertex();

		// find connected component (i.e., subgraph)
		sub_nodes = bfs(G_copy, root);
		
		// remove subnodes from graph G
		// and copy the subnodes to new graph then push back
		
		// subgraph initialized to zeros
		Graph* subgraph = new Graph(G_copy);
		//int size = sub_nodes->size();
		while (sub_nodes->size()) {
			int i = G_copy->get_index(sub_nodes->back());
			neighbor = G_copy->get_neighbors(sub_nodes->back());

			G_copy->print_graph();
			subgraph->print_graph();

			for (int j = 0; j < neighbor->size(); j++)
			{
				// copy subnodes to subgraph first
				Course* crs_i = G_copy->get_course(i);
				Course* crs_j = neighbor->at(j);
				//Course* crs_j = G_copy->get_course(j);
				subgraph->modify_correlation(crs_i, crs_j, G_copy->get_correlation(crs_i, crs_j));

				// remove subnodes from G_copy
				G_copy->modify_correlation(crs_i, crs_j, 0);
			}

			sub_nodes->pop_back();
		}

		// push back the subgraph to sub_list
		sub_list->push_back(subgraph);
	}
}