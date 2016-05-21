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
	Course* cur_node;

	while (G_copy->get_num_edge())
	{
		// find new root
		root = G_copy->get_random_vertex();

		// find connected component (i.e., subgraph)
		sub_nodes = bfs(G_copy, root);

		/*cout << "sub_nodes: connected component of the graph" << endl;
		for (int k = 0; k < sub_nodes->size(); k++)
		{
			sub_nodes->at(k)->print_course_info();
			cout << " ";
		} cout << endl;*/
		
		// remove subnodes from graph G
		// and copy the subnodes to new graph then push back
		
		// subgraph, a copy of G_copy
		//Graph* subgraph = new Graph(G_copy);
		Graph* subgraph = new Graph();

		/*cout << "root: ";
		root->print_course_info(); cout << endl;
		cout << "initializing subgraph: " << endl;*/

		// initialize subgraph
		//sub_nodes->push_back(root);
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
		//subgraph->print_graph();

		//sub_nodes->pop_back();
		//int size = sub_nodes->size();
		while (sub_nodes->size()) {
			cur_node = sub_nodes->back();
			//subgraph->add_index(cur_node);
			int i = G_copy->get_index(cur_node);
			neighbor = G_copy->get_neighbors(cur_node);

			// >>>>>>>> tbd start
			/*cout << "[list_subgraphs] G_copy" << endl;
			G_copy->print_graph();
			
			cout << "neighbors of a subnode of a subgraph" << endl;
			for (int k = 0; k < neighbor->size(); k++) {
				neighbor->at(k)->print_course_info();
				cout << " ";
			} cout << endl;
			cout << "subnode info: ";
			cur_node->print_course_info(); cout << endl;*/
			// >>>>>>>> tbd end

			for (int j = 0; j < neighbor->size(); j++)
			{
				// copy subnodes to subgraph first
				Course* crs_i = G_copy->get_course(i);
				Course* crs_j = neighbor->at(j);

				//subgraph->add_index(crs_i);
				//subgraph->add_index(crs_j);

				subgraph->modify_correlation(crs_i, crs_j, G_copy->get_correlation(crs_i, crs_j));
				subgraph->modify_correlation(crs_j, crs_i, G_copy->get_correlation(crs_j, crs_i));

				/*cout << "[list_subgraphs] subgraphs" << endl;
				subgraph->print_graph();*/

				//Course* crs_j = G_copy->get_course(j);
				//subgraph->modify_correlation(crs_i, crs_j, G_copy->get_correlation(crs_i, crs_j));
				//subgraph->modify_correlation(crs_j, crs_i, G_copy->get_correlation(crs_i, crs_j));

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