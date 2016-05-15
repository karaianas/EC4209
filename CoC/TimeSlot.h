#pragma once
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <utility>

#include "Course.h"
#include "Student.h"
#include "Graph.h"

using namespace std;

class TimeSlot {
public:
	TimeSlot(int n)
	{
		assert(n >= 0);

		timeslotnum = n;

		for (int i = 0; i < n; i++)
		{
			vector< int > elem;
			Base_Solution_Array.push_back(elem);
		}
	}

	~TimeSlot() {
	}

	//		float happiness(Course *a, int time);
	//when a would be in timeslot, calculate the expected happinessvalue
	void find_basic_solution(Graph *G);

	//	bool cmp(pair< pair<int, int>, float> a, pair< pair<int, int>, float> b);
	void course_sort();

	void put_graph_info(Graph *G);

	void print_timeslot(Graph *G);
	void print_graph_info();

	vector< vector < int > > get_Array(void);
	vector< pair< pair<int, int>, float> > get_graph_info(void);

private:
	vector< vector < int > > Base_Solution_Array;

	//		Course course_list;
	int timeslotnum;
	int CourseNum;

	vector< pair< pair<int, int>, float> > graph_info;
};

void TimeSlot::print_timeslot(Graph *G)
{
	for (int i = 0; i < Base_Solution_Array.size(); i++)
	{
		cout << "timeslot " << i << " : ";
		for (int j = 0; j < Base_Solution_Array.at(i).size(); j++)
		{
			Course* temp = G->get_course(Base_Solution_Array.at(i).at(j));
			temp->print_course_info();
			cout << " ";
		}
		cout << endl;
	}
}

void TimeSlot::print_graph_info()
{
	for (int i = 0; i < graph_info.size(); i++)
	{
		if (graph_info.at(i).second > 0)
			cout << graph_info.at(i).first.first << " " << graph_info.at(i).first.second << " " << graph_info.at(i).second << endl;
	}
}



vector< vector<int> > TimeSlot::get_Array()
{
	return Base_Solution_Array;
}

vector< pair< pair<int, int>, float> > TimeSlot::get_graph_info()
{
	return graph_info;
}

void TimeSlot::put_graph_info(Graph *G)
{
	for (int i = 0; i<G->get_size(); i++)
	{
		Course* temp_course = G->get_course(i);

		for (int j = 0; j<G->get_size(); j++)
		{
			Course* temp_course_2 = G->get_course(j);
			float temp_correlation_value = G->get_correlation(temp_course, temp_course_2);

			pair< pair<int, int>, float> L_tempNode;
			pair< int, int > S_tempNode;

			S_tempNode.first = i;
			S_tempNode.second = j;

			L_tempNode.first = S_tempNode;
			L_tempNode.second = temp_correlation_value;

			graph_info.push_back(L_tempNode);
		}
	}

	CourseNum = G->get_size();
}

/*
bool TimeSlot::cmp(pair< pair<int, int>, float> a, pair< pair<int, int>, float> b)
{
return a.second > b.second;
}
*/

void TimeSlot::course_sort()
{
	sort(graph_info.begin(), graph_info.end(), [](const pair< pair<int, int>, float>& lhs, const pair< pair<int, int>, float >& rhs) {return lhs.second > rhs.second; });
}

void TimeSlot::find_basic_solution(Graph *G){

	bool *visited = new bool[CourseNum];

	for (int i = 0; i < CourseNum; i++)
		visited[i] = 0;

	int num;

	for (int i = 0; i < graph_info.size(); i++)
	{
		num = 0;

		int num1 = graph_info.at(i).first.first;
		int num2 = graph_info.at(i).first.second;
		Course* Node1 = G->get_course(num1);
		Course* Node2 = G->get_course(num2);

		float min_correlation_sum = 99999999.0;
		// checking for first node of graph_info vector
		if (visited[num1] == 0) {
			for (int j = 0; j < Base_Solution_Array.size(); j++)
			{
				float temp_correlation_sum = 0.0;
				for (int k = 0; k < Base_Solution_Array.at(j).size(); k++)
				{
					int tempnum = Base_Solution_Array.at(j).at(k);
					Course* tempNode = G->get_course(tempnum);
					temp_correlation_sum += G->get_correlation(Node1, tempNode);
				}

				if (temp_correlation_sum < min_correlation_sum)
				{
					num = j;
					min_correlation_sum = temp_correlation_sum;
				}
			}

			Base_Solution_Array.at(num).push_back(num1);
			visited[num1] = 1;
		}

		min_correlation_sum = 99999999.0;
		//checking for second node of graph_info vector
		if (visited[num2] == 0) {
			for (int j = 0; j < Base_Solution_Array.size(); j++)
			{
				float temp_correlation_sum = 0.0;

				for (int k = 0; k < Base_Solution_Array.at(j).size(); k++)
				{
					int tempnum = Base_Solution_Array.at(j).at(k);
					Course* tempNode = G->get_course(tempnum);
					temp_correlation_sum += G->get_correlation(Node2, tempNode);
				}

				if (temp_correlation_sum < min_correlation_sum)
				{
					num = j;
					min_correlation_sum = temp_correlation_sum;
				}
			}
			//int newid = G->get_course(num2)->get_id();
			Base_Solution_Array.at(num).push_back(num2);
			visited[num2] = 1;
		}
	}

}
