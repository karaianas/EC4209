/* init_coloring.cpp
 *	���� coloring�� �����ϱ� ���� �ռ�
 *	��� Course�� selected_color�� -1�� initialize
 *	�ƹ� �͵� ���� �ʾҴµ� coloring�� �Ǿ� �ִ�
 *	vertex���� �־ �غ�����. ����-
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