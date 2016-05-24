#pragma once

#include <iostream>
#include <vector>

#include "Color.h"
#include "Graph.h"
#include "Course.h"

using namespace std;

void graph_coloring_bfs(Graph* to_color, int color_limit)
{
	// Color* color_obj = new Color(to_color, color_limit);
	Color* color_obj = new Color(to_color);

	color_obj->start_coloring(to_color->get_course_list(), color_limit);
}