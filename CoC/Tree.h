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

#define NUM_COLORS 12 // 12 time slots
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

			if (children == NULL)
				children_list = new vector<TreeNode*>();
			else
				children_list = children;
				
			parent = prnt;
			
			me = crs;
			
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
				if (colors[j] > 0)
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

		void set_neighbors(vector<TreeNode*>* to_set) {
			assert(to_set);
			neighbor_list = to_set;
		}

		vector<TreeNode*>* get_neighbors() {
			assert(neighbor_list);
			return neighbor_list;
		}

		int get_neighbors_num()
		{
			return neighbor_list->size();
		}

		TreeNode* get_nth_neighbor(int n) 
		{ 
			return neighbor_list->at(n); 
		}

	private:
		Course* me;
		TreeNode* parent;
		vector<TreeNode*>* children_list;// make it a list of neighbors
		TreeNode* only_child;// make it the node to visit next i.e. child in DFS
		vector<TreeNode*>* neighbor_list;

		int selected_color;
		int colors[NUM_COLORS];
	};
		
	Tree(TreeNode* rootPtr) {
		// constructor
		root = rootPtr;

		// color frequency initialization
		for (int i = 0; i < NUM_COLORS; i++)
			color_frequency[i] = 0;
	}

	// return NULL if no problem; return parent otherwise
	TreeNode* color_parent(TreeNode* parent)
	{
		// set child
		TreeNode* neighbor = NULL;

		// conflict flag
		bool conflict = false;

		// color parent
		for (int i = 0; i < NUM_COLORS; i++)
		{
			if (color_frequency[i] <= MAX_FREQ && parent->is_color_ok(i))
			{
				// forward checking children
				conflict = false;
				int num = parent->get_neighbors_num();
				for (int j = 0; j < num; j++)
				{
					neighbor = parent->get_nth_neighbor(j);

					// the color cannot be removed from the neighbor's color set
					if (!neighbor->remove_color(i))
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

		if (parent == root)
		{
			cout << parent << endl;
			cout << root << endl;
			return NULL;
		}
		return parent;
	}

	// return false if there's no soln; return true otherwise
	bool backtrack(TreeNode* child, vector<TreeNode*>* order)
	{
		assert(child != NULL);

		cout << " back " << endl;

		// backtracking cannot solve this problem
		if (child == root)
			return false;

		// get parent color which should not be used next time
		TreeNode* bad_parent = child->get_parent();
		int bad_color = bad_parent->get_selected();

		// reset the whole tree
		reset(bad_parent, bad_color, order);

		return true;
	}

	// reset the tree
	void reset(TreeNode* parent, int color, vector<TreeNode*>* order)
	{
		// traverse all treenodes
		for (int i = 0; i < order->size(); i++)
		{
			TreeNode* cur = order->at(i);
			if (cur == parent)
			{
				cur->reset();
				cur->set_flag(color);
			}
			else
				cur->reset();
		}
	}

	TreeNode* get_root()
	{
		return root;
	}

	void set_order(vector<TreeNode*>* _order)
	{
		order = new vector<TreeNode*>();
		order = _order;
	}

	vector<TreeNode*>* get_order()
	{
		return order;
	}

	int get_size()
	{
		if (order == NULL)
			return 1;
		else
			return order->size();
	}


private:
	TreeNode* root;
	int color_frequency[NUM_COLORS];
	Graph* gptr;
	vector<TreeNode*>* order;
};