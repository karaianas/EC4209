#include <iostream>
#include <vector>

#include "Graph.h"
#include "Course.h"
#include "Tree.h"

#define THRESHOLD 0.05
#define DEFAULT_THRES 0.6

using namespace std;

void list_subgraphs(Graph* G, vector<Graph*>* sub_list);
void init_coloring(vector<Graph*>* to_init);

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

	Course* visiting = new Course();
	bool cont = false;
	Tree::TreeNode* parent_ptr = root_node;

	to_visit->push_back(root);
	parents->push_back(root);

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

	return T;
}

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

	for (int j = 0; j < coloring_order->size(); j++)
	{
		int color = coloring_order->at(j)->get_selected();
		coloring_order->at(j)->get_TreeNode()->sudo_color(color);
	}
	return true;
}

vector<Graph*>* cut_subgraphs(Graph* G, float thres) {
	vector<Graph*>* cut_Gs = new vector<Graph*>();
	Graph* G_cp = new Graph(G);

	G_cp->remove_less_threshold(thres);
	list_subgraphs(G_cp, cut_Gs);

	return cut_Gs;
}

vector<Tree*>* main_coloring(Graph* G, vector<Graph*>* subgraphs, vector<Course*>* alone_list)
{
	vector<Tree::TreeNode*>* coloring_order;
	vector<Tree*>* sub_trees = new vector<Tree*>();

	G->remove_less_threshold(DEFAULT_THRES);
	list_subgraphs(G, subgraphs);
	init_coloring(subgraphs);

	cout << "# of default subgraphs: " << subgraphs->size() << endl;

	for (int i = 0; i < subgraphs->size(); i++) {
		// transform every subgraph into a tree
		coloring_order = new vector<Tree::TreeNode*>();
		Tree* to_color = build_tree(subgraphs->at(i), coloring_order);
		sub_trees->push_back(to_color);

		bool no_answer = lets_color(to_color, coloring_order);
		if (!no_answer) {	// cannot solve. need to split up.
			vector<Graph*>* cut_graphs = cut_subgraphs(subgraphs->at(i), \
				subgraphs->at(i)->get_curr_thres() + THRESHOLD);
			init_coloring(cut_graphs);
			for (int j = 0; j < cut_graphs->size(); j++)
				subgraphs->push_back(cut_graphs->at(i));
		}
		cout << i << "th subgraph colored (total # subG: " << subgraphs->size() << ")" << endl;
		cout << "graph size: " << subgraphs->at(i)->get_size() << endl;
	}

	// alone list coloring
	alone_list = G->get_alone_crs();

	for (int i = 0; i < alone_list->size(); i++) {
		Tree::TreeNode* root_node = new Tree::TreeNode(alone_list->at(i), NULL, NULL);
		Tree *alone_tree = new Tree(root_node);
		sub_trees->push_back(alone_tree);
	}

	if (alone_list) {
		cout << "alone_list size: " << alone_list->size() << endl;
		// color alone_list
	}

	return sub_trees;
}