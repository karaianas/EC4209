#include "Course.h"
#include "Student.h"
#include "RCourse.h"
#include "Graph.h"
#include <vector>
//#include "timeslot.h"

float one_happiness(vector<Student*>* ptr, int s_id, TimeSlot* T, vector<Course*>* ptr2)
{

	int one_happiness;
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
	vector<int> my_list = ptr->at(index)->get_courses();
	
	int total_course = my_list.size();
	vector <vector <int> > temp = T->get_Array();
	int sum = 0;
	for (int i = 0; i < temp.size(); i++)
	{
		int checking_course = 0;
		int len = my_list.size();
		if (len == 0)
			break; 
		for (int j = 0; j < temp.at(i).size(); j++)
		{
			
			for (int k = 0; k < my_list.size(); k++)
			{
				if (temp.at(i).at(j) == my_list.at(k))
				{
					my_list.erase(k);
					checking_course++;
				}
				if (checking_course > 0)
					sum++;
			}
		}
	}
	one_happiness = (sum / total_course) * 100;
	return one_happiness;
}

int num_of_student(vector<Student*>* ptr, float happiness, TImeSlot* T, vector<Course*>* ptr2)
{
	int num_of_student = 0;
	
	for (int i = 0; i < ptr->size(); i++)
		if (one_happiness(ptr, i, T, ptr2) >= happiness)
			num_of_student++;

	return num_of_student;
}

float average_happiness(vector<Student*>* ptr, TimeSlot* T, vector<Course*>* ptr2)
{
	float total_happiness= 0;
	float average_happiness;
	for (int i = 0; i < ptr->size(); i++)
		total_happiness+= one_happiness(ptr, i, T, ptr2);
	average_happiness = total_happiness/ ptr->size();
	
	return average_happiness;
}

void printf_happiness(vector<Student*>*ptr, TimeSlot* T, vector<Course*>* ptr2, int happiness)
{
	for (int i = 0; i < ptr->size(); i++)
		cout << i << "th happiness : " << one_happiness(ptr, i, T, ptr2) <<"%"<< endl;
	
	cout << "The number of happy student :" << num_of_student(ptr, happiness, T, ptr2) << endl;
	cout << "The average happiness of all student" << average_happiness(ptr, T, ptr2) << "%"<<endl;
}