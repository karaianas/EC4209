#include <iostream>
#include <vector>

#include "Graph.h"
#include "Course.h"
#include "Tree.h"

using namespace std;

//vector<Tree::TreeNode*>* color_order;

vector<Course*>* max_sorting(Graph* G, vector<Course*>* crs_list, \
	Course* cur, vector<Course*>* visited) {
	vector<Course*>* sorted = new vector<Course*>();
	Graph* G_cp = new Graph(G);

	// preprocessing - delete edges between cur & visited
	for (int i = 0; i < visited->size(); i++) {
		G_cp->modify_correlation(cur, visited->at(i), 0);
	}

	while (sorted->size() < crs_list->size()) {
		float max = -1;
		Course* max_crs = new Course();
		for (int i = 0; i < crs_list->size(); i++) {
			float corr = G_cp->get_correlation(cur, crs_list->at(i));
			if (max < corr) {
				max = corr;
				max_crs = crs_list->at(i);
				G_cp->modify_correlation(cur, max_crs, 0);
			}
		}
		sorted->push_back(max_crs);
	}

	return sorted;
}

Tree* build_tree(Graph* weighted_graph, vector<Tree::TreeNode*>* order_of_coloring) {
	Course* root = weighted_graph->get_max_degree_vertex();
	Tree::TreeNode* root_node = new Tree::TreeNode(root);
	Tree* T = new Tree(root_node);
	vector<Course*>* node_list = weighted_graph->get_course_list();
	vector<Course*>* visited = new vector<Course*>();
	vector<Course*>* to_visit = new vector<Course*>();
	vector<Course*>* parents = new vector<Course*>();
	vector<Course*>* neighbors = new vector<Course*>();
	vector<Tree::TreeNode*>* coloring_order = new vector<Tree::TreeNode*>();
	//order_of_coloring = new vector<Tree::TreeNode*>();
	//vector<Course*>* max_weight_order = new vector<Course*>();

	//Graph* graph_copy = new Graph(weighted_graph);
	Course* visiting = new Course();
	bool cont = false;
	Tree::TreeNode* parent_ptr = root_node;

	to_visit->push_back(root);
	parents->push_back(root);
	//coloring_order->push_back(root_node);

	while (to_visit->size()) {
		visiting = to_visit->back();
		to_visit->pop_back();

		cont = false;
		for (int i = 0; i < visited->size(); i++) {
			if (visited->at(i) == visiting) {
				cont = true;
				break;
			}
		}
		if (cont)
			continue;

		// create TreeNode and attach to tree
		visited->push_back(visiting);
		Tree::TreeNode* to_add = new Tree::TreeNode(visiting);
		parent_ptr->add_child(to_add);
		coloring_order->push_back(to_add);
		order_of_coloring->push_back(to_add);
		to_add->set_parent(parent_ptr);

		cout << "[build_tree] TreeNode : ";
		to_add->get_TreeNode()->print_course_info(); cout << endl;

		Course* cur_par = parents->back();
		for (int i = coloring_order->size() - 1; i >= 0; i--) {
			Tree::TreeNode* ptr = coloring_order->at(i);
			if (ptr->get_TreeNode() == cur_par) {
				parent_ptr = ptr;
				parents->pop_back();
				break;
			}
		}

		neighbors = weighted_graph->get_neighbors(visiting);
		neighbors = max_sorting(weighted_graph, neighbors, visiting, visited);

		for (int i = neighbors->size() - 1; i >= 0; i--) {
			to_visit->push_back(neighbors->at(i));
			parents->push_back(visiting);
		}
	}

	//order_of_coloring = coloring_order;
	//color_order = coloring_order;

	cout << "Coloring_order" << endl;
	for (int i = 0; i < coloring_order->size(); i++)
		coloring_order->at(i)->get_TreeNode()->print_course_info();
	cout << endl;
	cout << "Coloring_order size: " << coloring_order->size() << endl;

	cout << "ordering_of_coloring" << endl;
	for (int i = 0; i < order_of_coloring->size(); i++)
		order_of_coloring->at(i)->get_TreeNode()->print_course_info();
	cout << endl;
	cout << "Coloring_order size: " << order_of_coloring->size() << endl;

	// print test
	cout << "[build_tree] visited printing..." << endl;
	for (int i = 0; i < visited->size(); i++)
	{
		visited->at(i)->print_course_info();
		cout << " ";
	} cout << endl;

	return T;
}

/*
Graph* weight_graph(Graph* G)
{
	vector<Course*>* cptr = G->get_course_list();
	int csize = cptr->size();
	Graph* WG = new Graph(G, csize);

	for (int x = 0; x < csize; x++)
	{
		vector<Course*>* neighbors = G->get_neighbors(cptr->at(x));
		int nsize = neighbors->size();
		float weight = 0;

		for (int y = 0; y < nsize; y++)
			weight += G->get_correlation(cptr->at(x), neighbors->at(y));

		// whatever you like
	}
}
*/

bool lets_color(Tree* T, vector<Tree::TreeNode*>* coloring_order)
{
	Tree::TreeNode* curr;
	int count = 0;

	while (count < coloring_order->size())
	{
		for (int i = 0; i < coloring_order->size(); i++)
		{
			curr = coloring_order->at(i);
			Tree::TreeNode* ptr = T->color_parent(curr);
			
			// there was a problem coloring
			if (ptr)
			{
				if (!T->backtrack(ptr, coloring_order))
				{
					cout << "Impossible to solve" << endl;
					return false;
				}
				else
				{
					count = 0;
					break;
				}
			}
			count++;
		}
	}

	cout << "Done" << endl;
	for (int j = 0; j < coloring_order->size(); j++)
	{
		coloring_order->at(j)->get_TreeNode()->print_course_info();
		cout << " " << coloring_order->at(j)->get_selected() << endl;
	}
	return true;
}

//void subgraphs_coloring(vector<Graph*>* subgraphs, )
//{

//}