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

			// color initialization
			for (int i = 0; i < NUM_COLORS; i++)
				colors[i] = 1;

			selected_color = -1;
		}

		Course* get_TreeNode() { return me; }
		vector<TreeNode*>* get_children() { return children_list; }
		int get_children_number() { return children_list->size(); }
		TreeNode* get_parent() { return parent; }
		void set_parent(TreeNode* par) { parent = par; }
		void set_children(vector<TreeNode*>* children) { children_list = children; }
		TreeNode* get_nth_child(int n) { return children_list->at(n); }
		void add_child(TreeNode* to_add) { children_list->push_back(to_add); }
		bool is_leaf() {
			if (children_list)
				return false;
			return true;
		}
		bool is_root() {
			if (parent)
				return false;
			return true;
		}

		// color related functions
		bool remove_color(int i)
		{
			int original_color = colors[i];

			int sum = 0;
			colors[i] = 0;

			for (int j = 0; j < NUM_COLORS; j++)
			{
				if (colors[i] > 0)
					sum++;
			}

			// removing the color makes an empty set
			if (sum == 0)
			{
				colors[i] = original_color;
				return false;
			}
			// it's ok to remove the color
			else
			{
				if (original_color == -1)
					colors[i] = -1;
				return true;
			}
		}

		bool is_color_ok(int i)
		{
			if (colors[i] > 0)
				return true;
			else
				return false;
		}

		void set_color(int i)
		{
			selected_color = i;
		}

		int get_selected()
		{
			return selected_color;
		}

		void set_flag(int i)
		{
			colors[i] = -1;
		}

		void reset()
		{
			for (int i = 0; i < NUM_COLORS; i++)
				colors[i] = 1;
			
			selected_color = -1;
		}

	private:
		Course* me;
		TreeNode* parent;
		vector<TreeNode*>* children_list;// make it a list of neighbors
		TreeNode* only_child;// make it the node to visit next i.e. child in DFS

		int selected_color;
		int colors[NUM_COLORS];
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

		// color frequency initialization
		for (int i = 0; i < NUM_COLORS; i++)
			color_frequency[i] = 0;
	}

	// get next TreeNode* to color
	TreeNode* get_next(TreeNode* cur)
	{
	}

	// return NULL if no problem; return parent otherwise
	TreeNode* color_parent(TreeNode* parent)
	{
		// set child
		TreeNode* child = NULL;

		// conflict flag
		bool conflict = false;

		// color parent
		for (int i = 0; i < NUM_COLORS; i++)
		{
			if (color_frequency[i] <= MAX_FREQ && parent->is_color_ok(i))
			{
				// forward checking children
				conflict = false;
				int num_children = parent->get_children_number();
				for (int j = 0; j < num_children; j++)
				{
					TreeNode* child = parent->get_nth_child(j);

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
					parent->set_color(i);
					color_frequency[i] += 1;
					return NULL;
				}
			}
		}

		return parent;
	}

	// return false if there's no soln; return true otherwise
	bool backtrack(TreeNode* child)
	{
		assert(child != NULL);

		// backtracking cannot solve this problem
		if (child == root)
			return false;

		// get parent color which should not be used next time
		TreeNode* bad_parent = child->get_parent();
		int bad_color = bad_parent->get_selected();

		// reset the whole tree
		reset(bad_parent, bad_color);

		return true;
	}

	// reset the tree
	void reset(TreeNode* parent, int color)
	{
		// traverse all treenodes
		TreeNode* cur;
		if (cur == parent)
		{
			cur->reset();
			cur->set_flag(color);
		}
		else
			cur->reset();
	}

	// color tree by DFS
	//bool color_tree()
	//{
	//	TreeNode* cur, next;

	//	// ...

	//	if (color_parent(cur) == NULL)
	//		
	//		// get next treenode to color
	//	else
	//	{
	//		if (backtrack(cur))
	//			// 
	//		else
	//			// backtracking does not work
	//	}
	//	// ...
	//}


	
private:
	TreeNode* root;
	int color_frequency[NUM_COLORS];
	Graph* gptr;
};