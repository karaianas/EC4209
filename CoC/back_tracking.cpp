//#pragma once
//
//#include <iostream>
//#include "Course.h"
//#include "Color.h"
//#include "Graph.h"
//#include <vector>
//// 모든 ptr들은 오류가 생긴 노드의 부모노드를 가리키는 포인터를 뜻합니다. 
//
//bool back_tracking(Course* ptr)
//{
//	if (ptr -> root)
//	{
//		cout << "This problem can not be solved by using backtracking algorithm" << endl;
//		return false;
//	}
//
//	make_flag(ptr);
//	recover_child(ptr);
//	if (ptr->is_empty())
//		back_tracking(ptr->get_parents());		//만약 ptr이 아직도 비어있다면 parents Node로 올라가서 다시 시작
//	else
//		return true;
//}
//
//void make_flag(Course* ptr)
//{
//	int select_color;
//	select_color = ptr->get_select_color();
//	ptr->set_flag(select_color);
//}
//
//void recover_child(Course* ptr)
//{
//	int select_color, len;
//	len = ptr->get_numOfchild();				// len은 ptr의 child 수
//	Course* list = ptr->get_child();			// list에 ptr의 child list가 저장되어있음
//	select_color = ptr->get_select_color();
//
//	for (int i = 0; i < len; i++)
//	{
//		list[i].recover_color(select_color);	//child list들에 모두 선택된 select_color를 다시 1로 복수
//	}
//}
//	