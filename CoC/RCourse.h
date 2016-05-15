/*
	This class is different from class Course.
	RCourse stands for Registered Course.
	Each student will have a list of RCourses.
*/
#pragma once
#include <vector>



using namespace std;

class RCourse{
public:
	RCourse(int _track, int _num)
	{
		track = _track;
		num = _num;
	}

	int get_track(){ return track; }
	int get_num(){ return num; }

private:
	int track;
	int num;
};