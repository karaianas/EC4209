#include "Course.h"
#include "Student.h"
#include "RCourse.h"
#include <vector>
//#include "timeslot.h"

float one_happinese(vector<Student*>* ptr, int s_id)
{
	int one_happinese;
	int index = -1;
	for (int i = 0; i < ptr->size(); i++)
	{
		if (ptr->at(i)->get_id() == s_id)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		cout << "There is no such student" << endl;
		return -1;
	}
	int checking_course=0;
	vector<RCourse*> my_list = ptr->at(index)->get_course_list();
	int total_course = my_list.size();
	for (int i = 0; i < my_list.size() i++)
	{
		if ()
	}

	return one_happinese;
}
int set_happinese(int happinese)
{
	return happinese;
}

int num_of_student(vector<Student*>* ptr)
{
	int num_of_student = 0;
	
	for (int i = 0; i < ptr->size(); i++)
		if (one_happinese(ptr, i) >= set_happinese())
			num_of_student++;

	return num_of_student;
}

float average_happinese(vector<Student*>* ptr)
{
	float total_happinese= 0;
	float average_happinese;
	for (int i = 0; i < ptr->size(); i++)
		total_happinese+= one_happinese(ptr, i);
	average_happinese = sum / ptr->size();
	
	return average_happinese;
}