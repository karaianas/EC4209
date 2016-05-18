/* Graph.h
 * Graph with courses as vertices,
 * correlation coefficients as edges (bidirectional)
 */
#pragma once

#include <iostream>
#include <assert.h>
#include <vector>
#include <limits>
#include <fstream>

#include "Course.h"

// negative infinity (type float)
// const float neg_inf = -numeric_limits<float>::infinity();

using namespace std;

class Graph{
public:
	Graph(Graph* to_copy = NULL, int n = 0) {
		assert(n >= 0);
		num_courses = n;
		
		if (to_copy)
		{
			num_courses = to_copy->get_size();
			index = to_copy->index;
			p = to_copy->p;
			/*for (int i = 0; i < num_courses; i++)
			{
				index.push_back(to_copy->in)
			}*/
		}
		
	}

	// return TRUE if nodes have an edge, FALSE if not
	bool is_edge(Course* i, Course* j) {
		float cor = get_correlation(i, j);
		if (cor > 0)
			return true;
		else
			return false;
	}

	// get the correlation of two courses,
	// correlation of i -> j
	float get_correlation(Course* i, Course* j) {
		int index_i = get_index(i);
		int index_j = get_index(j);

		return p.at(index_i).at(index_j);
	}

	// set the correlation of two courses
	// course ordering from i to j
	// !! different from modifying correlation
	void set_correlation(Course* i, Course* j, float correlation) {
		/* set correlation coefficient */
		int index_i = get_index(i);
		int index_j = get_index(j);
		
		// if index out of range, push_back
		vector<float> to_push(index_j + 1, -1);

		// i(column to modify) greater than the size of matrix
		if (index_i >= p.size() || index_i == 0)
			p.push_back(to_push);

		// j(row to modify) greater than the size of vector
		if (index_j >= p.at(index_i).size())
			p[index_i].push_back(correlation);

		modify_correlation(i, j, correlation);
	}

	void modify_correlation(Course* i, Course* j, float correlation) {
		int index_i = get_index(i);
		int index_j = get_index(j);
		//vector<float> tmp_vec = p[index_i];

		// sanity check
		assert(index_i < p.size() || \
			index_j < p.at(index_i).size());

		p.at(index_i).at(index_j) = correlation;
		//cout << "in modify func! with corr, " << correlation << \
			", p_val: " << p.at(index_i).at(index_j) << endl;
	}
	
	int get_index(Course* c) {
		int i;
		for (i = 0; i < num_courses; i++)
			if (index[i]->get_num()== c->get_num() && \
				index[i]->get_track() == c->get_track())
				return i;

		assert(i < num_courses);
	}

	Course* get_course(int ind)
	{
		return index[ind];
	}

	void add_index(Course* c) {
		for (int i = 0; i < index.size(); i++)
			if (index[i]->get_num() == c->get_num() && \
				index[i]->get_track() == c->get_track())
				return;
		index.push_back(c);
		num_courses++;
		//cout << index.size() << endl;
	}

	int get_size() { return num_courses; }

	void print_graph() {
		//printf("size_i: %d\nsize_j: %d\n", p.size(), p.at(0).size());
		for (int i = 0; i < p.size(); i++) {
			for (int j = 0; j < p.at(i).size(); j++)
				printf("%.3f\t", p.at(i).at(j));
			cout << endl;
		}
		cout << endl;
		int x;
		//cin >> x;
	}

	void file_print_graph(const char* path, const char* file_name) {
		// creating output.txt
		ofstream output;
		string s = path;
		string name = file_name;
		output.open(s + name);

		for (int i = 0; i < p.size(); i++) {
			for (int j = 0; j < p.at(i).size(); j++)
				output << p.at(i).at(j) << "\t";
			output << endl;
		}
		output << endl;
	}

	// returns the degree of a vertex
	int get_vertex_degree(Course* v)
	{
		assert(v != NULL);
		int degree = 0;
		int index_v = get_index(v);

		for (int i = 0; i < p.at(index_v).size(); i++)
		{
			if (i == index_v)
				continue;
			if (p.at(index_v).at(i) > 0)
				degree++;
		}

		return degree;
	}

	float get_vertex_weight(Course* v)
	{
		return get_correlation(v, v);
	}

	void set_vertex_weight(Course* v, float correlation)
	{
		set_correlation(v, v, correlation);
	}

	// get linear addition of the two correlations
	float get_correlation_addition(Course* i, Course* j)
	{
		float i_j = get_correlation(i, j);
		float j_i = get_correlation(j, i);

		return i_j + j_i;
	}

	// get std, avg, min, and max of correlations
	void get_correlation_stats(vector<Course*>* cptr)
	{
		float sum = 0, _min = 10, _max = 0, std = 0;
		int num = 0;

		for (int x = 0; x < num_courses; x++)
		{
			for (int y = 0; y < x; y++)
			{
				float weight = get_correlation(cptr->at(x), cptr->at(y));
				//float weight = get_correlation_addition(cptr->at(x), cptr->at(y));

				// don't count the disconncted edges
				if (weight >= 0)
				{
					sum += weight;
					num++;
					if (weight < _min)
						_min = weight;
					if (weight > _max)
						_max = weight;
				}
				else
					num++;
			}
		}

		avg = sum / num;
		min = _min;
		max = _max;

		cout << "Correlation Statistics" << endl << "avg: " << avg << " min: " << min << " Max: " << max << endl;
	}
	
	// get maximum popularity
	float get_max_popularity()
	{
		float max = 0;

		for (int i = 0; i < num_courses; i++)
		{
			float _pop = get_course(i)->get_popularity();
			if (max < _pop)
				max = _pop;
		}

		return max;
	}

	// get maximum availability
	float get_max_availability()
	{
		float max = 0;

		for (int i = 0; i < num_courses; i++)
		{
			float _ava = get_course(i)->get_availability();
			if (max < _ava)
				max = _ava;
		}

		return max;
	}

	// get neighbors of Course i
	vector<Course*>* get_neighbors(Course* i)
	{
		vector<Course*>* list = new vector<Course*>;

		for (int j = 0; j < num_courses; j++)
		{
			Course* ptr = get_course(j);
			if (is_edge(i, ptr))
				list->push_back(get_course(j));
		}

		return list;
	}

	void print_course_availablilty()
	{
		for (int i = 0; i < num_courses; i++)
		{
			index[i]->print_course_info();
			cout << index[i]->get_availability() << endl;
		}
	}

	// correlation statistics
	float avg, min, max;

private:
	int num_courses;
	vector<Course*> index;
	vector<vector<float>> p;
};