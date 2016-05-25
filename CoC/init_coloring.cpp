/* init_coloring.cpp
 *	본격 coloring을 시작하기 전에 앞서
 *	모든 Course의 selected_color를 -1로 initialize
 *	아무 것도 하지 않았는데 coloring이 되어 있는
 *	vertex들이 있어서 준비했음. 데헷-
 */
#pragma once

#include <iostream>
#include <vector>

#include "Graph.h"
#include "Course.h"

using namespace std;

void init_coloring(vector<Graph*>* to_init)
{
	for (int i = 0; i < to_init->size(); i++) {
		vector<Course*>* crs_list_i = to_init->at(i)->get_course_list();
		for (int j = 0; j < crs_list_i->size(); j++) {
			crs_list_i->at(j)->init_selected_color();
		}
	}
}