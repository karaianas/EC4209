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

#define NUM_COLORS 12 // 12 times slots
#define MAX_FREQ 10 // Max classes per slot

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

			// color frequency initialization
			for (int i = 0; i < NUM_COLORS; i++)
				color_frequency[i] = 0;

		}
		Course* get_TreeNode() { return me; }
		vector<TreeNode*>* get_children() { return children_list; }
		int get_children_number() { return children_list->size(); }
		TreeNode* get_parent() { return parent; }
		void set_parent(TreeNode* par) { parent = par; }
		void set_children(vector<TreeNode*>* children) { children_list = children; }
		TreeNode* get_nth_child(int n) { return children_list->at(n); }
		void add_child(TreeNode* to_add) { children_list->push_back(to_add); }


		// return NULL if no problem; return parent otherwise
		TreeNode* color_children(TreeNode* parent)
		{
			// get parent
			Course* p = parent->get_TreeNode();

			// set child
			TreeNode* child = NULL;

			// conflict flag
			bool conflict = false;
			
			// color parent
			for (int i = 0; i < NUM_COLORS; i++)
			{
				if (color_frequency[i] <= MAX_FREQ && p->is_color_ok(i))
				{
					// forward checking children
					conflict = false;
					int num_children = parent->get_children_number();
					for (int j = 0; j < num_children; j++)
					{
						Course* child = parent->get_nth_child(j)->get_TreeNode();

						// the color cannot be removed from the child's color set
						if (!child->remove_color(i))
						{
							conflict = true;
							break;
						}
					}

					// set parent color
					if (!conflict)
					{
						p->set_color(i);
						color_frequency[i] += 1;
						return NULL;
					}
				}
			}

			return parent;
		}

	private:
		Course* me;
		TreeNode* parent;
		vector<TreeNode*>* children_list;

		int color_frequency[NUM_COLORS];
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