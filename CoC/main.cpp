#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>

#include <math.h>
#include <glut.h>
#include <gl.h>
#include <glu.h>

#include "Student.h"
#include "Course.h"
#include "Graph.h"
#include "TimeSlot.h"
#include "Tree.h"

#define PI 3.1415926

using namespace std;

// graphical interface
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void menu(int value);
void mouse(int button, int state, int x, int y);

float rotate_angle = 0.0f;
bool r_pressed = false;
float threshold = 0.0f;
float thr_step = 0.05f;
int view = 1;
float mouse_x = 0;
float mouse_y = 0;
bool mouse_down = false;
bool ortho = false;

double angle_x = 0;
double angle_y = -30;
double angle_z = 0;

// variables
bool is_parsed = false;
bool is_ideal = true;
vector<Student*> student_list;
vector<Course*> course_list;
Graph* multi_graph;
Graph* simple_graph;
Graph* Copy_graph;
TimeSlot* greedy_time_slot;
TimeSlot* time_slot;
vector<Graph*>* subgraphs;
vector<Course*>* alone_list;
vector<Tree::TreeNode*>* color_order;
vector<Tree*>* all_trees;

// function prototypes
bool in_conversion(const char* path, vector<Student*>* s_list, vector<Course*>* c_list);
int check_track(char x, char y);
Graph* build_multi_graph(vector<Course*> course_list);
void compute_correlation(Graph* G, int index_i, int index_j, Course* cour_i, Course* cour_j);
Graph* build_simple_graph(Graph* multi_graph, vector<Course*> course_list);
bool is_connected(Graph* G, Course* u, Course* v);
void list_subgraphs(Graph* G, vector<Graph*>*sub_list);
vector<Course*>* bfs_connected_component(Graph* G, Course* root);
void graph_coloring(Graph* to_color, int color_limit);
void alone_coloring(vector<Course*>*, int color_limit);
void init_coloring(vector<Graph*>* to_init);
void printf_happiness(vector<Student*>*ptr, TimeSlot* T, vector<Course*>* ptr2, int happiness);
void draw_course(Tree::TreeNode* ptr, int _j);
Tree* build_tree(Graph* weighted_graph, vector<Tree::TreeNode*>* order_of_coloring);
vector<Course*>* max_sorting(Graph* G, vector<Course*>* crs_list, Course* cur, vector<Course*>* visited);
bool lets_color(Tree* T, vector<Tree::TreeNode*>* coloring_order);
vector<Graph*>* cut_subgraphs(Graph* G, float thres);
vector<Tree*>* main_coloring(Graph* G, vector<Graph*>* subgraphs, vector<Course*>* alone_list);

int main(int argc, char** argv)
{
	// Add your own home directory
	int user_id;
	char* home_dir = " ";
	cout << "Giyeon[1] Sunwoo_lab[2] Kyubihn[3] Seoyoung_room[4] Seoyoung_lab[5] Seoyoung Debug[6] Sunwoo_room[7]" \
		<< endl << "Enter user number:";
	cin >> user_id;

	switch (user_id)
	{
	case 1:
		home_dir = "C:\\Users\\user\\Documents\\GitHub\\EC4209\\CoC\\";
		break;
	case 2:
		home_dir = "C:\\Users\\cvlab2243\\Source\\Repos\\EC4209\\CoC\\";
		break;
	case 3:
		home_dir = "C:\\Users\\규빈\\Documents\\GitHub\\EC4209\\CoC\\";
		break;
	case 4:
		home_dir = "C:\\Users\\USER\\Documents\\GitHub\\EC4209\\CoC\\";
		break;
	case 5:
		home_dir = "C:\\Users\\June\\Documents\\GitHub\\EC4209\\CoC\\";
		break;
	case 6:
		home_dir = "C:\\Users\\USER\\Desktop\\back_up\\CoC\\";
		break;
	case 7:
		home_dir = "C:\\Users\\Sunwoo\\Documents\\GitHub\\EC4209\\Coc\\";
		break;
	}

	is_parsed = in_conversion(home_dir, &student_list, &course_list);

	/* Build Multi Graph
	 *  builds a directed graph
	 *  with bidirectional correlation coefficients
	 */
	multi_graph = build_multi_graph(course_list);

	/* graph print test */
	multi_graph->file_print_graph(home_dir, "directed_graph.txt");

	/* Build Simple Graph
	 *  conversion from the bidirectional graph to a simple weighted graph
	 *  need specific models to merge the correlation coefficients
	 */
	simple_graph = build_simple_graph(multi_graph, course_list);

	simple_graph->file_print_graph(home_dir, "simple_graph.txt");
	
	// set correlation statistics
	vector<Course*>* cptr;
	cptr = &course_list;
	simple_graph->get_correlation_stats(cptr);

	// Timeslot Allocation -----------------------------------------------
	greedy_time_slot = new TimeSlot(12);
	time_slot = new TimeSlot(12);
	greedy_time_slot->Find_Greedy_Solution(multi_graph);
	printf_happiness(&student_list, greedy_time_slot, &course_list, 80);
	cout << multi_graph->get_size() << endl;

	/* make list of subgraphs from the simple_graph */
	subgraphs = new vector<Graph*>();

	cout << "coloring real graph" << endl;

	vector<Tree::TreeNode*>* color_order = new vector<Tree::TreeNode*>();

	Graph* simple_copy = new Graph(simple_graph);
	
	// 여기다 쓰면 됨
	all_trees = main_coloring(simple_copy, subgraphs, alone_list);

	// graphical interface 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Clash of Class");
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutCreateMenu(menu);
	glutAddMenuEntry("View Correlation", 1);
	glutAddMenuEntry("View Popularity", 2);
	glutAddMenuEntry("View Availability", 3);
	glutAddMenuEntry("View Distribution", 4);
	glutAddMenuEntry("View Timetable", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();

	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	// viewporzo	glViewport(0, 0, 1000, 1000);// left right corner and size

	if (ortho)
	{
		// orthogonal
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-100, 100, -100, 100, 1, 300.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else
	{
		// perspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1, 1, -1, 1, 1, 300.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	gluLookAt(0, 0, 100, 0.0, 0, 0.0, 0, 1.0, 0.0);// eye, at, up

	GLfloat size[2];
	GLfloat sizeL[2];
	GLfloat angle;
	glGetFloatv(GL_POINT_SIZE_RANGE, size);
	glGetFloatv(GL_LINE_WIDTH_RANGE, sizeL);
	glPointSize(size[0] * 6);

	if (view < 4)
	{
		if (r_pressed)
		{
			glRotatef(rotate_angle, 0.0, 0.0, 1.0);
			r_pressed = false;
		}

		int node_num = multi_graph->get_size();
		char* c = new char[node_num];

		int clicked = -1;
		int j = 0;
		for (float angle = 0.0; angle <= 2.0 * PI; angle += 2 * PI / node_num)
		{

			if (j < course_list.size())
			{
				char* name = new char[4];
				name = course_list[j]->get_course_name();
					
				switch (course_list[j]->get_track())
				{
				// gs
				case 1:
					glColor3f(0.4, 0.804, 0.667);// medium aquamarine
					break;
				// bi
				case 2:
					glColor3f(1, 0.843, 0);// gold
					break;
				// ch
				case 3:
					glColor3f(1, 0.498, 0.314);// coral
					break;
				// cs
				case 4:
					glColor3f(0, 0.749, 1);// deep sky blue
					break;
				// ev
				case 5:
					glColor3f(1, 0.753, 0.796);// pink
					break;
				// ma
				case 6:
					glColor3f(0.498039, 0.53, 1.0);// purple
					break;
				// me
				case 7:
					glColor3f(1.00, 0.11, 0.68);// hot pink
					break;
				// ph
				case 8:
					glColor3f(0.678, 1, 0.184);// green yellow
					break;
				}

				if (mouse_down)
				{
					if (sqrt(pow(mouse_x - 42.5 * cos(angle), 2) + pow(mouse_y - 42.5 * sin(angle), 2)) < 1)
					{
						glColor3f(1, 0, 0);
						clicked = j;
					}
				}

				glBegin(GL_POINTS);
				glVertex3f(80 * cos(angle), 80 * sin(angle), 0.0);
				glEnd();

				if (view == 2)
				{
					float weight = course_list[j]->get_popularity() / multi_graph->get_max_popularity();
					glColor3f(weight, weight, weight);
				}
				if (view == 3)
				{
					float weight = course_list[j]->get_availability() / multi_graph->get_max_availability();
					glColor3f(weight, weight, weight);
				}

				j++;

				glPushMatrix();
				glTranslatef(81 * cos(angle), 81 * sin(angle), 0.0);
				glRotatef(angle * 180 / PI, 0.0, 0.0, 1.0);
				glScalef(0.02f, 0.02f, 1.f);
				for (int i = 0; i < 4; i++)
				{
					glLineWidth(sizeL[0] * 0.6);
					glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, name[i]);
				}

				glPopMatrix();
			}
		}

		// draw edges
		if (mouse_down && clicked != -1)
		{
			for (int y = 0; y < course_list.size(); y++)
			{
				float weight = simple_graph->get_correlation(course_list[clicked], course_list[y]);

				if ((weight >= threshold) && (weight <= simple_graph->max))
				{
					float range = simple_graph->max - threshold;

					glColor3f(0, (weight - threshold) / range, (weight - threshold) / range);
					glLineWidth(sizeL[0] * 4.5 * (weight - threshold) / range);
					glBegin(GL_LINE_STRIP);
					glVertex3f(79 * cos(2 * PI / node_num * clicked), 79 * sin(2 * PI / node_num * clicked), 0);
					glVertex3f(79 * cos(2 * PI / node_num * y), 79 * sin(2 * PI / node_num * y), 0);
					glEnd();
				}
			}
			clicked = -1;
		}
		else
		{
			for (int x = 0; x < course_list.size(); x++)
			{
				for (int y = 0; y < x; y++)
				{
					float weight = simple_graph->get_correlation(course_list[x], course_list[y]);

					if ((weight >= threshold) && (weight <= simple_graph->max))
					{
						float range = simple_graph->max - threshold;

						glColor3f(0, (weight - threshold) / range, (weight - threshold) / range);
						glLineWidth(sizeL[0] * 4.5 * (weight - threshold) / range);
						glBegin(GL_LINE_STRIP);
						glVertex3f(79 * cos(2 * PI / node_num * x), 79 * sin(2 * PI / node_num * x), 0);
						glVertex3f(79 * cos(2 * PI / node_num * y), 79 * sin(2 * PI / node_num * y), 0);
						glEnd();
					}
				}
			}
		}
	}
	else if (view == 4)
	{
		float num = 2 / thr_step;
		float step = 180 / num;

		int freq[41] = { 0, };

		// frequency
		for (int x = 0; x < course_list.size(); x++)
		{
			for (int y = 0; y < x; y++)
			{
				float weight = simple_graph->get_correlation(course_list[x], course_list[y]);

				if (weight >= 0)
				{
					int bin = weight / thr_step;
					freq[bin] += 1;
				}
			}
		}

		// bars
		for (float x = 0; x <= num; x++)
		{
			glLineWidth(sizeL[1]);
			glColor3f(1, 1, 1);
			glBegin(GL_LINE_STRIP);
			glVertex3f(-90 + x * step, -90, 0);
			glVertex3f(-90 + x * step, -90 + freq[(int)x], 0);
			glEnd();
		}

		// x-axis grid
		for (float x = 0; x <= num; x++)
		{
			glLineWidth(sizeL[0]);
			glColor3f(1, 0.1, 0.3);

			glBegin(GL_LINE_STRIP);
			if ((int)x % 5 == 0)
			{
				glVertex3f(-90 + x * step, -90, 0);
				glVertex3f(-90 + x * step, -95, 0);
			}
			else
			{
				glVertex3f(-90 + x * step, -90, 0);
				glVertex3f(-90 + x * step, -93, 0);
			}
			glEnd();
		}

		// y-axis grid
		for (float x = 0; x <= num; x++)
		{
			glColor3f(0, 0.5, 1);

			glPushMatrix();
			glTranslatef(-92 + x * step, -88 + freq[(int)x], 0);
			glScalef(0.015f, 0.015f, 1.f);
			glLineWidth(sizeL[0]);
			
			char* number = new char[3];
			int n = freq[(int)x];
			int count = 0;

			do 
			{
				int digit = n % 10;
				putchar('0' + digit);
				n /= 10;
				number[count] = digit + '0';
				count++;
			} while (n > 0);

			for (int i = count - 1; i >= 0; i--)
				glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, number[i]);
			glPopMatrix();
		}

		glLineWidth(sizeL[1] * 0.2);
		// x-axis
		glColor3f(1, 0.1, 0.3);
		glBegin(GL_LINE_STRIP);
		glVertex3f(-90, -90, 0);
		glVertex3f(95, -90, 0);
		glEnd();

		// y-axis
		glColor3f(0, 0.5, 1);
		glBegin(GL_LINE_STRIP);
		glVertex3f(-90, -90, 0);
		glVertex3f(-90, 95, 0);
		glEnd();

	}
	// view timetable
	else
	{
		glPushMatrix();
		glScaled(0.6, 0.6, 0.6);
		glTranslated(0, -30, 0);
		glRotated(angle_x, 1.0f, 0.0f, 0.0f);
		glRotated(angle_y, 0.0f, 1.0f, 0.0f);
		glRotated(angle_z, 0.0f, 0.0f, 1.0f);

		glColor3f(0.7, 0.7, 0.7);

		/*
		// xy-plane
		for (int i = 0; i <= 120; i += 20)
		{
			glBegin(GL_LINES);
			glVertex3f(-60, 0 + i, -60);
			glVertex3f(60, 0 + i, -60);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(-60 + i, 120, -60);
			glVertex3f(-60 + i, 0, -60);
			glEnd();
		}
		
		// yz-plane
		for (int i = 0; i <= 120; i += 20)
		{
			glBegin(GL_LINES);
			glVertex3f(-60, 0 + i, 60);
			glVertex3f(-60, 0 + i, -60);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(-60, 0, -60 + i);
			glVertex3f(-60, 120, -60 + i);
			glEnd();
		}
		*/
		
		// zx-plane
		for (int i = 0; i <= 120; i += 20)
		{
			if (i == 60)
				i += 20;
			
			// zx
			glBegin(GL_LINES);
			glVertex3f(-60 + i, 0, 60);
			glVertex3f(-60 + i, 0, -60);
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(-60, 0, -60 + i);
			glVertex3f(60, 0, -60 + i);
			glEnd();
		}
		
		glColor3f(1, 0, 0);

		// x-axis
		glBegin(GL_LINES);
		glVertex3d(-60, 0.1, 0);
		glVertex3d(60, 0.1, 0);
		glEnd();

		// z-axis
		glBegin(GL_LINES);
		glVertex3d(0, 0.1, -60);
		glVertex3d(0, 0.1, 60);
		glEnd();

		/*int freq[12];
		for (int j = 0; j < 12; j++)
			freq[j] = 0;

		int k = 0;
		for (int i = 0; i < 50; i++)
		{
			Course* cptr = course_list[i];
			if (cptr->is_color_ok(k))
			{
				cptr->set_color(k);
				freq[k]++;
				draw_course(cptr, freq[k]);

				k++;

				if (k == 12)
					k = 0;
			}
		}*/

		glPopMatrix();
	}

	glutSwapBuffers();
}

void draw_course(Tree::TreeNode* ptr, int _j)
{
	int color = 1 + ptr->get_selected();
	
	int i, k, j;
	j = _j;

	// time slot
	if (color <= 6)
	{
		i = 1;
		k = color;
	}
	else
	{
		i = 2;
		k = color - 6;
	}

	switch (ptr->get_TreeNode()->get_track())
	{
		// gs
	case 1:
		glColor3f(0.4, 0.804, 0.667);// medium aquamarine
		break;
		// bi
	case 2:
		glColor3f(1, 0.843, 0);// gold
		break;
		// ch
	case 3:
		glColor3f(1, 0.498, 0.314);// coral
		break;
		// cs
	case 4:
		glColor3f(0, 0.749, 1);// deep sky blue
		break;
		// ev
	case 5:
		glColor3f(1, 0.753, 0.796);// pink
		break;
		// ma
	case 6:
		glColor3f(0.498039, 0.53, 1.0);// purple
		break;
		// me
	case 7:
		glColor3f(1.00, 0.11, 0.68);// hot pink
		break;
		// ph
	case 8:
		glColor3f(0.678, 1, 0.184);// green yellow
		break;
	}

	int delta_y = 10;

	glPushMatrix();
	glTranslated(-70 + 20 * i, -5 + (10 + delta_y) * j, -70 + 20 * k);
	glScaled(20, 10, 20);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslated(-30 + 20 * i, -5 + (10 + delta_y) * j, -70 + 20 * k);
	glScaled(20, 10, 20);
	glutSolidCube(1);
	glPopMatrix();

	glColor3f(0.4, 0.4, 0.4);
	glPushMatrix();
	glTranslated(-70 + 20 * i, -5 + (10 + delta_y) * j, -70 + 20 * k);
	glScaled(20, 10, 20);
	glutWireCube(1.01);
	glPopMatrix();


	glPushMatrix();
	glTranslated(-30 + 20 * i, -5 + (10 + delta_y) * j, -70 + 20 * k);
	glScaled(20, 10, 20);
	glutWireCube(1.01);
	glPopMatrix();
}

void reshape(int w, int h)
{
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		rotate_angle += 1.0f;
		r_pressed = true;
		break;
	case 'p':
		if (threshold <= 10)
		{ 
			threshold += thr_step;
			cout << "Current Threshold = " << threshold << endl;
			break;
		}
		break;
	case 'o':
		if (threshold >= 0)
		{
			threshold -= thr_step;
			cout << "Current Threshold = " << threshold << endl;
			break;
		}
		break;

	case 'a':
		angle_x += 30;
		break;
	case 's':
		angle_y += 30;
		break;
	case 'd':
		angle_z += 30;
		break;
	case 'q':
		angle_x -= 30;
		break;
	case 'w':
		angle_y -= 30;
		break;
	case 'e':
		angle_z -= 30;
		break;
	}

	glutPostRedisplay();
}

void menu(int value)
{
	view = value;

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	int mouseButton = button;
	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			mouse_x = (float)x / 10 - 50;
			mouse_y = 50 - (float)y / 10;
			
			mouse_down = true;
		}
		break;

	};

	glutPostRedisplay();
}

float one_happiness(vector<Student*>* ptr, int s_id, TimeSlot* T, vector<Course*>* ptr2)
{

	float _one_happiness;
	int index = -1;
	for (int i = 0; i < ptr->size(); i++)
	{
		if (ptr->at(i)->get_id() == s_id + 1)
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

	float sum = 0;
	float possibility = 0;
	int purity = my_list.size();
	for (int i = 0; i < temp.size(); i++)
	{
		bool checked = 0;
		for (int j = 0; j < my_list.size(); j++)
		{
			for (int k = 0; k < temp.at(i).size(); k++)
			{
				if ( multi_graph->get_course(temp.at(i).at(k))->get_id() == my_list.at(j))
				{
					possibility = multi_graph->get_course(temp.at(i).at(k))->get_availability();
					if (!checked)
					{
						checked = 1;
						sum += possibility;
					}
					else
						purity--;
				}
			}
		}
	}
	_one_happiness = ((float)purity / (float)total_course) * 100;	//분반이 없을때 
//	_one_happiness = ((float)sum / (float)total_course) * 100;		//확률적으로 들을 수 있을 떄 

	return _one_happiness;
}

int num_of_student(vector<Student*>* ptr, float happiness, TimeSlot* T, vector<Course*>* ptr2)
{
	int num_of_student = 0;

	for (int i = 0; i < ptr->size(); i++)
		if (one_happiness(ptr, i, T, ptr2) >= happiness)
			num_of_student++;

	return num_of_student;
}

float average_happiness(vector<Student*>* ptr, TimeSlot* T, vector<Course*>* ptr2)
{
	float total_happiness = 0;
	float average_happiness;
	for (int i = 0; i < ptr->size(); i++)
		total_happiness += one_happiness(ptr, i, T, ptr2);
	average_happiness = total_happiness / ptr->size();

	return average_happiness;
}

void printf_happiness(vector<Student*>*ptr, TimeSlot* T, vector<Course*>* ptr2, int happiness)
{
	cout << "The number of happy student :" << num_of_student(ptr, happiness, T, ptr2) << endl;
	cout << "The average happiness of all student : " << average_happiness(ptr, T, ptr2) << "%" << endl;
}