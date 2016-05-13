#include <windows.h>
#include <iostream>
#include <fstream>
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

#define PI 3.1415926

using namespace std;

// graphical interface
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void menu(int value);

float rotate_angle = 0.0f;
bool r_pressed = false;
float threshold = 0.0f;
float thr_step = 0.05f;
bool view = true;

// variables
bool is_parsed = false;
vector<Student*> student_list;
vector<Course*> course_list;
Graph* multi_graph;
Graph* simple_graph;

// function prototypes
bool in_conversion(const char* path);
int check_track(char x, char y);
Graph* build_multi_graph(vector<Course*> course_list);
void compute_correlation(Graph* G, int index_i, int index_j, Course* cour_i, Course* cour_j);
Graph* build_simple_graph(Graph* multi_graph, vector<Course*> course_list);
bool is_connected(Graph* G, Course* u, Course* v);


int main(int argc, char** argv)
{
	// Add your own home directory
	int user_id;
	char* home_dir = " ";
	cout << "Giyeon[1] Sunwoo_lab[2] Kyubin[3] Seoyoung_room[4] Seoyoung_lab[5]" << endl << "Enter user number:";
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
		home_dir = "C:\\Users\\�Ժ�\\Documents\\EC4209-ClashOfClass\\EC4209-ClashOfClass\\CoC\\";
		break;
	case 4:
		home_dir = "C:\\Users\\USER\\Documents\\GitHub\\EC4209\\CoC\\";
		break;
	case 5:
		home_dir = "C:\\Users\\June\\Documents\\GitHub\\EC4209\\CoC\\";
		break;
	}

	is_parsed = in_conversion(home_dir);

	/*
	// result print out on console
	if (is_parsed)
	{
		for (int i = 0; i < student_list.size(); i++)
			student_list[i]->print_student_info();
		for (int j = 0; j < course_list.size(); j++)
			course_list[j]->print_student_list();
	}
	*/

	/* Toy graph example
	 * with simple toy course list
	 * used course list 0 to 4
	 */
	/*vector<Course*> toy_course_list;
	toy_course_list.push_back(course_list[0]);
	toy_course_list.push_back(course_list[1]);
	toy_course_list.push_back(course_list[3]);
	toy_course_list.push_back(course_list[29]);
	toy_course_list.push_back(course_list[69]);
	toy_course_list.push_back(course_list[39]);
	toy_course_list.push_back(course_list[98]);

	toy_course_list.push_back(course_list[100]);
	toy_course_list.push_back(course_list[110]);*/



	/* Build Multi Graph
	 *  builds a directed graph
	 *  with bidirectional correlation coefficients
	 */
	multi_graph = build_multi_graph(course_list);
	//multi_graph = build_multi_graph(toy_course_list);


	/* graph print test */
	multi_graph->file_print_graph(home_dir, "directed_graph.txt");

	/* Build Simple Graph
	 *  conversion from the bidirectional graph to a simple weighted graph
	 *  need specific models to merge the correlation coefficients
	 */
	simple_graph = build_simple_graph(multi_graph, course_list);
	//simple_graph = build_simple_graph(multi_graph, toy_course_list);	

	simple_graph->file_print_graph(home_dir, "simple_graph.txt");

	/*for (int p = 0; p < simple_graph->get_size(); p++)
	{
		for (int q = p + 1; q < simple_graph->get_size(); q++)
		{
			if (!is_connected(simple_graph, course_list[p], course_list[q]))
				printf("%d\t%d\n", p, q);
		}
	}*/

	// TEST01: get correlation statistics
	vector<Course*>* cptr;
	cptr = &course_list;
	multi_graph->get_correlation_stats(cptr);

	// TEST02: get course info
	course_list[0]->print_student_list();
	cout << course_list[0]->get_num_enrolled_students();

	// graphical interface
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Graph");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutCreateMenu(menu);
	glutAddMenuEntry("Wheel of Fortune", 1);
	glutAddMenuEntry("Distribution", 2);
	//glutAddMenuEntry("Spot Light", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();

	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat size[2];
	GLfloat sizeL[2];
	GLfloat angle;
	glGetFloatv(GL_POINT_SIZE_RANGE, size);
	glGetFloatv(GL_LINE_WIDTH_RANGE, sizeL);
	glPointSize(size[0] * 6);

	if (view)
	{
		if (r_pressed)
		{
			glRotatef(rotate_angle, 0.0, 0.0, 1.0);
			r_pressed = false;
		}

		int node_num = multi_graph->get_size();
		char* c = new char[node_num];

		int j = 0;
		for (float angle = 0.0; angle <= 2.0 * PI; angle += 2 * PI / node_num)
		{

			if (j < course_list.size())
			{
				char* name = new char[4];
				name = course_list[j]->get_course_name();
				switch (course_list[j]->get_track())
				{
				case 1:
					glColor3f(1.0, 0.498039, 0.0);
					break;
				case 2:
					glColor3f(0.137255, 0.556863, 0.137255);
					break;
				case 3:
					glColor3f(0.52, 0.39, 0.39);
					break;
				case 4:
					glColor3f(0.196078, 0.8, 0.196078);
					break;
				case 5:
					glColor3f(0.85, 0.85, 0.10);
					break;
				case 6:
					glColor3f(0.498039, 0.53, 1.0);
					break;
				case 7:
					glColor3f(1.00, 0.11, 0.68);
					break;
				case 8:
					glColor3f(0.196078, 0.6, 0.8);
					break;
				}
				j++;

				glBegin(GL_POINTS);
				glVertex3f(80 * cos(angle), 80 * sin(angle), 0.0);
				glEnd();

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

		for (int x = 0; x < course_list.size(); x++)
		{
			for (int y = 0; y < x; y++)
			{
				float weight = multi_graph->get_correlation_addition(course_list[x], course_list[y]);

				if ((weight >= threshold) && (weight <= multi_graph->max))
				{
					float range = multi_graph->max - threshold;

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
	else
	{
		float num = 2 / thr_step;
		float step = 180 / num;

		int freq[41] = { 0, };

		// frequency
		for (int x = 0; x < course_list.size(); x++)
		{
			for (int y = 0; y < x; y++)
			{
				float weight = multi_graph->get_correlation_addition(course_list[x], course_list[y]);

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



	glutSwapBuffers(); // this is only used for double buffers
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
	}

	glutPostRedisplay();
}

void menu(int value)
{

	switch (value)
	{
	case 1:
		view = true;
		break;
	case 2:
		view = false;
		break;
	}



	glutPostRedisplay();
}

bool in_conversion(const char* path)
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
				Course* C = new Course(track, num);
				course_list.push_back(C);
				C->enroll_student(S);
				S->register_course(track, num);
				first = false;
			}
			else
			{
				// find if the course is already in course_list
				bool exist = false;
				for (int j = 0; j < course_list.size(); j++)
				{
					if ((course_list[j]->get_num() == num) && (course_list[j]->get_track() == track))
					{
						course_list[j]->enroll_student(S);
						S->register_course(track, num);
						exist = true;
						break;
					}
				}

				if (!exist)
				{
					Course* C = new Course(track, num);
					course_list.push_back(C);
					C->enroll_student(S);
					S->register_course(track, num);
				}
			}
		}
		student_list.push_back(S);
		s_id++;
	}

	return true;
}