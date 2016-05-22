#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Student.h"
#include "Course.h"

int check_track(char x, char y);

bool in_conversion(const char* path, vector<Student*>* s_list, vector<Course*>* c_list)
{
	int id, year, unit;
	char* major; char* minor;
	char* prefix; int num;

	// creating output.txt
	ofstream output;
	string s = path;
	output.open(s + "output.txt");

	// read in test.txt
	string p = s + "test3.txt";
	FILE* file = fopen(p.c_str(), "r");
	int c;

	while ((c = fgetc(file)) != EOF)
	{
		// characters that are neither delims nor separators
		if (((char)c != ',') && ((char)c != '\t') && ((char)c != '\"') && ((char)c != ' '))
		{
			// unit
			if ((char)c == '(')
			{
				c = fgetc(file); c = fgetc(file); c = fgetc(file);
			}
			else
				output << (char)c;
		}
	}
	output.close();
	fclose(file);

	// read in output.txt
	string s2 = s + "output.txt";
	FILE* file2 = fopen(s2.c_str(), "r");
	char* code;
	size_t n = 0;
	int c2;

	// end if file is empty
	if (file2 == NULL)
		return false;

	// find and allocate file size
	fseek(file2, 0, SEEK_END);
	long f_size = ftell(file2);
	fseek(file2, 0, SEEK_SET);
	code = (char*)malloc(f_size);

	while ((c2 = fgetc(file2)) != EOF)
		code[n++] = (char)c2;

	code[n] = '\0';
	fclose(file2);

	int s_id = 1;
	bool first = true;

	int counter = 0;

	for (int i = 0; code[i] != '\0'; i++)
	{

		// year
		int year = 2000 + (code[i] - '0') * 10 + (code[i + 1] - '0');
		i += 2;

		// Major, minor
		int major = 0; int minor = 0;
		char x = code[i]; char y = code[i + 1];
		major = check_track(x, y);
		i += 2;
		x = code[i]; y = code[i + 1];
		minor = check_track(x, y);
		i += 2;

		Student* S = new Student(s_id, year, major, minor);

		// add courses with track and number

		while (code[i] != '\n')
		{
			int track, num;
			x = code[i]; y = code[i + 1];
			track = check_track(x, y);
			i += 2;
			x = code[i]; y = code[i + 1];
			num = (x - '0') * 10 + (y - '0');
			i += 2;

			if (first)
			{
				Course* C = new Course(track, num, counter);
				counter++;
				c_list->push_back(C);
				C->enroll_student(S);
				S->register_course(track, num, counter);
				first = false;
			}
			else
			{
				// find if the course is already in c_list
				bool exist = false;
				for (int j = 0; j < c_list->size(); j++)
				{
					if ((c_list->at(j)->get_num() == num) && (c_list->at(j)->get_track() == track))
					{
						c_list->at(j)->enroll_student(S);
						S->register_course(track, num, j);
						exist = true;
						break;
					}
				}

				if (!exist)
				{
					Course* C = new Course(track, num, counter);
					counter++;
					c_list->push_back(C);
					C->enroll_student(S);
					S->register_course(track, num, counter);
				}
			}
		}
		s_list->push_back(S);
		s_id++;
	}

	// read in list.txt
	string size_file_name = "list.txt";
	ifstream size_file(s + size_file_name);
	string line;

	while (getline(size_file, line))
	{
		istringstream iss(line);
		string crs, dummy;
		int num_cls, cls_size;

		iss >> crs >> num_cls >> cls_size >> dummy;

		// process pair
		// find course
		for (int i = 0; i < c_list->size(); i++)
		{
			char* name = c_list->at(i)->get_course_name();
			string crs_name(c_list->at(i)->get_course_name());

			crs_name.resize(4);

			if (crs_name == crs)
			{
				c_list->at(i)->set_class_size(cls_size);
				c_list->at(i)->set_num_classes(num_cls);
				c_list->at(i)->set_availability();
				c_list->at(i)->set_popularity();
				break;
			}
		}
	}
	size_file.close();

	return true;
}