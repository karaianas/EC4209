#pragma once

#include <vector>
#include <iostream>
#include "Course.h"
#include "Graph.h"

using namespace std;

class Schedule{
public:
	// g should be a simple_graph
	Schedule(Graph* gptr, int _limit)
	{
		for (int i = 0; i < 12; i++)
		{
			vector<Course*>* temp = new vector<Course*>();
			all.push_back(temp);
		}

		size = all.size();

		g = gptr;
		limit = _limit;
	}

	int get_size(){ return size; }

	int get_freq(int t){ return all.at(t)->size(); }

	// get sum_t(correlations)
	float get_sum(int t, Course* c)
	{
		float sum = 0;

		for (int i = 0; i < all.at(t)->size(); i++)
			sum += g->get_correlation(c, all.at(t)->at(i));

		return sum;
	}

	// get avg(sum(correlations)) of all time slots
	float get_avg_sum(Course* c)
	{
		float sum = 0;
		for (int i = 0; i < 12; i++)
			sum += get_sum(i, c);

		if (sum == 0)
			return 0;
		return sum / 12;
	}

	// get course of Max(avg(sum(correlations)))
	Course* get_max_avg_sum(vector<Course*> list)
	{
		float prev = 0;
		int index = 0;

		for (int i = 0; i < list.size(); i++)
		{
			if (list.at(i)->get_time_slot() != -1)
				continue;

			float sum = get_avg_sum(list.at(i));
			if (sum > prev)
			{
				prev = sum;
				index = i;
			}
		}

		if (prev >= 0)
			return list.at(index);
		else
			return NULL;
	}

	// get index of Max(avg(sum(correlations)))
	int get_max_avg_sum_i(vector<Course*> list)
	{
		float prev = 0;
		int index = 0;

		for (int i = 0; i < list.size(); i++)
		{
			if (list.at(i)->get_time_slot() != -1)
				continue;

			float sum = get_avg_sum(list.at(i));
			if (sum > prev)
			{
				prev = sum;
				index = i;
			}
		}

		if (prev > 0)
			return index;
		else
			return 0;
	}

	// get time slot of min(sum_t(correlations))
	int get_min_slot(Course* c)
	{
		float prev = 100;
		int index = 0;

		for (int t = 0; t < 12; t++)
		{
			if (get_freq(t) <= limit)
			{
				float sum = get_sum(t, c);
				if (sum < prev)
				{
					prev = sum;
					index = t;
				}
			}
		}

		return index;
	}

	// print current time slot sum(correlations)
	void print_sum(int t, Course* c){ cout << get_sum(t, c) << endl; }

	//push courses into timeslot
	void set(int t, Course* c)
	{
		c->set_time_slot(t);
		all.at(t)->push_back(c);
	}

private:
	vector<vector<Course*> *> all;
	int size;
	Graph* g;
	int limit;
};