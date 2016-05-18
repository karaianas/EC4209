#include <vector>
#include <iostream>

#include "Student.h"
#include "Course.h"
#include "Rcourse.h"

using namespace std;

void generate_virtual_students(vector<Student*>* orig_student, vector<Course*>* orig_course)
{
	int orig_student_size = orig_student->size();
	

	for (int i = 0; i < 175; i++)// orig_student_size; i++)
	{
		int id = orig_student->at(i)->get_id() + orig_student_size;
		int year = orig_student->at(i)->get_year();
		int major = orig_student->at(i)->get_maj();
		int minor = orig_student->at(i)->get_minor();

		Student* S = new Student(id, year, major, minor);
		vector<RCourse*> course_list = orig_student->at(i)->get_course_list();
		vector<int> courses = orig_student->at(i)->get_courses();

		int num = orig_student->at(i)->get_courses().size();
		for (int j = 0; j < num; j++)
		{
			//cout << orig_course->at(courses[j])->get_id() << endl;
			//orig_course->at(courses[j])->print_student_list();
			orig_course->at(courses[j])->enroll_student(S);
			//orig_course->at(courses[j])->print_student_list();
			S->register_course(course_list[j]->get_track(), course_list[j]->get_num(), courses[j]);
		}

		orig_student->push_back(S);
	}

	cout << "Total number of students after copy: " << orig_student->size() << endl;
}

void enroll_virtual_students(vector<Student*>* orig_student, vector<Course*>* orig_course)
{
	
}