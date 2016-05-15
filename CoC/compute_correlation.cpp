#include <vector>

#include "Student.h"
#include "Course.h"
#include "Graph.h"

void compute_correlation(Graph* G, int index_i, int index_j, Course* cour_i, Course* cour_j)
{
	float correlation = 0;

	if (index_i == index_j)
	{
		G->set_correlation(cour_i, cour_j, correlation);
		return;
	}

	vector<Student*>* stud_i;
	vector<Student*>* stud_j;

	stud_i = cour_i->get_ptr_student_list();
	stud_j = cour_j->get_ptr_student_list();

	int intersection = 0;

	for (int s1 = 0; s1 < stud_i->size(); s1++)
		for (int s2 = 0; s2 < stud_j->size(); s2++)
			if (stud_i->at(s1)->get_id() \
				== stud_j->at(s2)->get_id())
				intersection++;

	if (!intersection)
	{
		G->set_correlation(cour_i, cour_j, 0);
		G->set_correlation(cour_j, cour_i, 0);
		return;
	}

	float i_to_j = (float)intersection / stud_i->size();
	float j_to_i = (float)intersection / stud_j->size();

	G->set_correlation(cour_i, cour_j, i_to_j);
	G->set_correlation(cour_j, cour_i, j_to_i);
}