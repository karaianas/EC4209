#pragma once

#include <vector>
#include <iostream>
#include "Student.h"
//#include "Graph.h"

using namespace std;
#define COLOR_SIZE 12

class Course{
public:
	
	Course(int _track = 0, int _num = 0, int _id = 0)
	{
		track = _track;
		num = _num;
		course_id = _id;

		// by default
		popularity = 0;
		availability = 0;
		class_size = 0;
		num_classes = 0;

		for (int i = 0; i < COLOR_SIZE; i++)
			color[i] = true;
		selected_color = -1;
	}

	// enroll a student
	void enroll_student(Student* S)
	{
		student_list.push_back(S);
	}

	// print---------------------------------------------------------
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

	// get-----------------------------------------------------------
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

	float get_popularity()
	{
		return popularity;
	}

	float get_availability()
	{
		return availability;
	}

	int get_class_size() {
		return class_size;
	}

	int get_num_classes() {
		return num_classes;
	}

	// set-----------------------------------------------------------
	void set_popularity()
	{
		float num_students = student_list.size();

		popularity = (float)num_students / float(class_size * num_classes);
	}

	void set_availability()
	{
		float num_students = student_list.size();

		if ((int)num_students <= class_size * num_classes)
			availability = 1;
		else
			availability = float(class_size * num_classes) / (float)num_students;
	}

	void set_class_size(int size) {
		class_size = size;
	}

	void set_num_classes(int num_cls) {
		num_classes = num_cls;
	}
	
	// color---------------------------------------------------------
	bool is_color_ok(int i)
	{
		if (color[i])
			return true;
		else
			return false;
	}

	int color_possible_num()
	{
		int temp = 0;
		for (int i = 0; i < COLOR_SIZE; i++)
			temp = temp + is_color_ok(i);
		return temp;
	}

	// set color to i = (0, 11)
	void set_color(int i)
	{
		char* name;
		name = get_course_name();

		for (int j = 0; j < 4; j++)
			cout << name[j];

		if (color[i] > 0)
		{
			selected_color = i;
			cout << ": color set to " << i << endl;
		}
		else
			cout << ": color " << i << " cannot be used[unavailable]" << endl;
	}
	void set_flag(int i)
	{
		color[i] = -1;
	}

	// remove a color
	bool remove_color(int i)
	{
		char* name;
		name = get_course_name();

		int original_color = color[i];

		color[i] = 0;

		if (is_empty())
		{
			for (int j = 0; j < 4; j++)
				cout << name[j];
			cout << ": color " << i << " cannot be removed[becomes empty]" << endl;
			color[i] = original_color;
			return false;
		}
		else
		{
			if (original_color == -1)
				color[i] = -1;
			return true;
		}
	}

	// check if the color set is empty
	bool is_empty()
	{
		int purity = 0;
		for (int i = 0; i < COLOR_SIZE; i++)
			if (color[i] == 1)
			{
				purity++;
				break;
			}

		if (!purity)
			return 1;		//비었으면 return 1;
		else
			return 0;		//안 비어있으면 return 0;
	}

	int get_select_color()
	{
		return selected_color;
	}

	int *get_color()
	{
		return color;
	}

	void insert_color_set(int which_number)
	{
		get_color()[which_number] = 1;
	}
	
	bool whether_selected()
	{
		int sum = 0;
		for (int i = 0; i < COLOR_SIZE; i++)
			sum += get_color()[i];
		if (sum == 1)
			return 1;
		else
			return 0;	
	}

	void set_select_color()
	{
		if (whether_selected())
			for (int i = 0; i < COLOR_SIZE; i++)
				if (get_color()[i])
					selected_color = i;
		else
			cout << "we can not choose the selected color" << endl;
	}

	void init_selected_color()
	{
		selected_color = -1;
	}
	void recover_color(int i)
	{
		color[i] = 1;
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

	int color[COLOR_SIZE];
	int selected_color;

	// list of enrolled students
	vector<Student*> student_list;
};