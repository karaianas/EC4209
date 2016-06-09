#include <iostream>
#include <vector>

#include "Graph.h"
#include "Course.h"
#include "Tree.h"

#define THRESHOLD 0.05
#define DEFAULT_THRES 0.2

using namespace std;

void list_subgraphs(Graph* G, vector<Graph*>* sub_list, vector<Course*>* alone_list);
void init_coloring(vector<Graph*>* to_init);
void set_neighbors(Tree* T, Graph* subG);

Course* get_max_crs(vector<Course*>* crs_list, vector<float>* values, vector<Course*>* visited)
{
	vector<Course*> visited_removed;
	vector<float> removed_vals;
	bool to_add;
	for (int i = 0; i < crs_list->size(); i++) {
		to_add = true;
		for (int j = 0; j < visited->size(); j++) {
			if (crs_list->at(i) == visited->at(j))
			{
				to_add = false;
				break;
			}
		}
		if (to_add) {
			visited_removed.push_back(crs_list->at(i));
			removed_vals.push_back(values->at(i));
		}
	}

	float max = -100;
	Course* max_crs = new Course();
	for (int i = 0; i < visited_removed.size(); i++) {
		if (max < removed_vals[i]) {
			max = removed_vals[i];
			max_crs = visited_removed[i];
		}
	}

	return max_crs;

}

vector<Course*>* sort_neighbor(vector<Course*>* neighbor_list, Graph* G, Course* cur)
{
	// sort the neighbors by their correlation
	vector<float> neighbor_corr;
	
	for (int i = 0; i < neighbor_list->size(); i++) {
		float corr = G->get_correlation(cur, neighbor_list->at(i));
		neighbor_corr.push_back(corr);
	}

	float max;
	vector<Course*>* sorted_list = new vector<Course*>();

	for (int i = 0; i < neighbor_list->size(); i++) {
		Course* next_max = get_max_crs(neighbor_list, &neighbor_corr, sorted_list);
		sorted_list->push_back(next_max);
	}

	return sorted_list;
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
	Tree::TreeNode* to_add;

	int G_size = weighted_graph->get_size();

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

		to_add = new Tree::TreeNode(visiting);
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

		neighbors = sort_neighbor(neighbors, weighted_graph, visiting);

		for (int i = neighbors->size() - 1; i >= 0; i--) {
			to_visit->push_back(neighbors->at(i));
			parents->push_back(visiting);
		}
	}

	T->get_root()->set_parent(NULL);

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
					return false;
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

vector<Graph*>* cut_subgraphs(Graph* G, float thres, vector<Course*>* alone_list) {
	vector<Graph*>* cut_Gs = new vector<Graph*>();
	Graph* G_cp = new Graph(G);

	G_cp->remove_less_threshold(thres);
	list_subgraphs(G_cp, cut_Gs, alone_list);

	return cut_Gs;
}

vector<Tree*>* main_coloring(Graph* G, vector<Graph*>* subgraphs, vector<Course*>* alone_list)
{
	vector<Tree::TreeNode*>* coloring_order;
	vector<Tree*>* sub_trees = new vector<Tree*>();
	alone_list = new vector<Course*>();

	G->remove_less_threshold(DEFAULT_THRES);
	list_subgraphs(G, subgraphs, alone_list);
	init_coloring(subgraphs);

	for (int i = 0; i < subgraphs->size(); i++) {
		// transform every subgraph into a tree
		coloring_order = new vector<Tree::TreeNode*>();
		Tree* to_color = build_tree(subgraphs->at(i), coloring_order);
		sub_trees->push_back(to_color);

		// order input
		to_color->set_order(coloring_order);

		set_neighbors(to_color, subgraphs->at(i)); // newly added
		

		bool no_answer = lets_color(to_color, coloring_order);
		if (!no_answer) {	// cannot solve. need to split up.
			vector <Graph*>* cut_graphs = cut_subgraphs(subgraphs->at(i), \
				subgraphs->at(i)->get_curr_thres() + THRESHOLD, alone_list);

			init_coloring(cut_graphs);
			for (int j = 0; j < cut_graphs->size(); j++)
				subgraphs->push_back(cut_graphs->at(i));
		}
	}

	// alone list coloring
	G->get_alone_crs(alone_list);

	for (int i = 0; i < alone_list->size(); i++) {
		Tree::TreeNode* root_node = new Tree::TreeNode(alone_list->at(i), NULL, NULL);
		Tree *alone_tree = new Tree(root_node);
		sub_trees->push_back(alone_tree);
	}

	return sub_trees;
}

Tree::TreeNode* find_treenode(vector<Tree::TreeNode*>* node_list, Course* crs) 
{
	for (int i = 0; i < node_list->size(); i++)
	{
		if (node_list->at(i)->get_TreeNode()->get_id() == crs->get_id())
			return node_list->at(i);
	}
	assert(false);
	return NULL;
}

void set_neighbors(Tree* T, Graph* subG)
{
	vector<Tree::TreeNode*>* col_ord = T->get_order();
	Course* cur_crs;
	vector<Course*>* cur_neighbors;
	vector<Tree::TreeNode*>* neighbor_to_set;

	for (int i = 0; i < col_ord->size(); i++) {
		cur_crs = col_ord->at(i)->get_TreeNode();
		cur_neighbors = subG->get_neighbors(cur_crs);

		neighbor_to_set = new vector<Tree::TreeNode*>();
		for (int j = 0; j < cur_neighbors->size(); j++) {
			Tree::TreeNode* tmp = find_treenode(col_ord, cur_neighbors->at(j));
			neighbor_to_set->push_back(tmp);
		}

		col_ord->at(i)->set_neighbors(neighbor_to_set);
	}
}