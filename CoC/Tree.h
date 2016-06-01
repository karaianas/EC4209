/* Tree.h
 *	the header of tree structure
 *	data structure used to store the state
 *	of graph coloring
 */
#pragma once

#include <vector>
#include <iostream>
#include <assert.h>

#include "Course.h"
#include "Graph.h"

using namespace std;

class Tree {
public:
	class TreeNode {
	public:
		TreeNode(Course* crs = NULL, \
			vector<TreeNode*>* children = NULL, \
			TreeNode* prnt = NULL) {

			if (!crs) {
				cout << "Attempting to make a NULL TreeNode" << endl;
				assert(false);
			}

			me = crs;
			parent = prnt;
			children_list = children;

		}
		Course* get_TreeNode() { return me; }
		vector<TreeNode*>* get_children() { return children_list; }
		int get_children_number() { return children_list->size(); }
		TreeNode* get_parent() { return parent; }
		void set_parent(TreeNode* par) { parent = par; }
		void set_children(vector<TreeNode*>* children) { children_list = children; }
		TreeNode* get_nth_child(int n) { return children_list->at(n); }
		void add_child(TreeNode* to_add) { children_list->push_back(to_add); }

	private:
		Course* me;
		TreeNode* parent;
		vector<TreeNode*>* children_list;
	};

	//Tree(Course* start = NULL, Graph* G = NULL) {
	//	// constructor
	//	assert(start);
	//	assert(G);
	//	
	//	vector<Course*>* crs_list = G->get_course_list();
	//	bool* spanned = new bool[crs_list->size()];
	//	for (int i = 0; crs_list->size(); i++)
	//		spanned[i] = false;

	//	root = start;

	//	vector<Course*>* neighbor_list = G->get_neighbors(start);
	//}
		
	Tree(TreeNode* rootPtr) {
		// constructor
		root = rootPtr;
	}
	
private:
	TreeNode* root;
};