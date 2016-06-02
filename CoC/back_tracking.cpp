//#pragma once
//
//#include <iostream>
//#include "Course.h"
//#include "Color.h"
//#include "Graph.h"
//#include <vector>
//// ��� ptr���� ������ ���� ����� �θ��带 ����Ű�� �����͸� ���մϴ�. 
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
//		back_tracking(ptr->get_parents());		//���� ptr�� ������ ����ִٸ� parents Node�� �ö󰡼� �ٽ� ����
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
//	len = ptr->get_numOfchild();				// len�� ptr�� child ��
//	Course* list = ptr->get_child();			// list�� ptr�� child list�� ����Ǿ�����
//	select_color = ptr->get_select_color();
//
//	for (int i = 0; i < len; i++)
//	{
//		list[i].recover_color(select_color);	//child list�鿡 ��� ���õ� select_color�� �ٽ� 1�� ����
//	}
//}
//	