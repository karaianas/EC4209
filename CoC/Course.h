#pragma once

#include <vector>
#include <iostream>
#include "Student.h"

using namespace std;

class Course{
public:
	Course(int _track, int _num, int _id)
	{
		track = _track;
		num = _num;
		course_id = _id;

		// by default
		popularity = 0;
		availability = 0;
		class_size = 0;
		num_classes = 0;
	}

	// get course track
	int get_track(){ return track; }

	// get course num
	int get_num(){ return num; }

	int get_id(){ return course_id; }

	// get pointer to the student list
	vector<Student*>* get_ptr_student_list()
	{
		return &student_list;
	}

	// enroll a student
	void enroll_student(Student* S)
	{
		student_list.push_back(S);
	}

	// print the enrolled students by their ID
	void print_student_list()
	{
		print_course_info();
		for (int i = 0; i < (int)student_list.size(); i++)
			cout << student_list[i]->get_id() << " ";
		cout << endl << endl;
	}

	// print the course name = track + numb
	void print_course_info()
	{
		switch (track)
		{
		case 1:
			printf("gs%02i ", num);
			break;
		case 2:
			printf("bi%02i ", num);
			break;
		case 3:
			printf("ch%02i ", num);
			break;
		case 4:
			printf("cs%02i ", num);
			break;
		case 5:
			printf("ev%02i ", num);
			break;
		case 6:
			printf("ma%02i ", num);
			break;
		case 7:
			printf("me%02i ", num);
			break;
		case 8:
			printf("ph%02i ", num);
			break;
		}
	}

	// get course name in array[4] e.g. gs24
	char* get_course_name()
	{
		char* name = new char[4];
		switch (track)
		{
		case 1:
			name[0] = 'g';
			name[1] = 's';
			break;
		case 2:
			name[0] = 'b';
			name[1] = 'i';
			break;
		case 3:
			name[0] = 'c';
			name[1] = 'h';
			break;
		case 4:
			name[0] = 'c';
			name[1] = 's';
			break;
		case 5:
			name[0] = 'e';
			name[1] = 'v';
			break;
		case 6:
			name[0] = 'm';
			name[1] = 'a';
			break;
		case 7:
			name[0] = 'm';
			name[1] = 'e';
			break;
		case 8:
			name[0] = 'p';
			name[1] = 'h';
			break;
		}

		name[2] = (num / 10) + '0';
		name[3] = num % 10 + '0';

		return name;
	}

	// get the number of enrolled students
	int get_num_enrolled_students()
	{
		return student_list.size();
	}

	// set course size, number of classes
	void set_course_size(int _size, int _num, int num_total_students)
	{
		class_size = _size;
		num_classes = _num;

		float num_students = student_list.size();

		// in an ideal case
		if (_num == -1)
		{
			num_classes = ceil(num_students / class_size);
			availability = 1;
		}
		else// number of classes are already set
		{
			num_classes = _num;
			if ((int)num_students <= class_size * num_classes)
				availability = 1;
			else
				availability = float(class_size * num_classes) / num_students;
		}

		popularity = num_students / float(num_total_students);

		//if (_num == -1)
		//	cout << "[Ideal Case]" << endl;
		//else
		//	cout << "[Not an Ideal Case]" << endl;
		//cout << "The number of registered students: " << num_students << endl;
		//cout << "The size of the class: " << class_size << endl;
		//cout << "The number of classes: " << num_classes << endl;
		//cout << "The popularity of the course: " << popularity << endl;
		//cout << "The availability of the course: " << availability << endl;
	}

	float get_popularity()
	{
		return popularity;
	}

	float get_availability()
	{
		return availability;
	}

	void set_class_size(int size) {
		class_size = size;
	}
	void set_num_classes(int num_cls) {
		num_classes = num_cls;
	}

	int get_class_size() {
		return class_size;
	}

	int get_num_classes() {
		return num_classes;
	}

private:
	// track: gs == 1 bi == 2 ch == 3 cs == 4 ... ph == 8
	int track;
	int num;

	int course_id;

	float popularity;
	float availability;
	int class_size;// this is the size of one single class of the course
	int num_classes;// this is the number of classes of the course

	// list of enrolled students
	vector<Student*> student_list;
};