#pragma once

#include <iostream>
#include <vector>
#include <math.h>

#include "Graph.h"
#include "Course.h"
#define epsilon 0.0002

Graph* find_minimum_threshold(Graph* G)
{
	float low = 0.0000;
	float high = 2.0000;
	float mid = (low + high) / 2;

	float minimum = 2.0;
	Graph *ans;

	while (abs(high - low) >= epsilon)
	{
		Graph *temp = new Graph(G);
		temp->remove_less_threshold(mid);
		/*
		if(temp->isPossibleColoring())
		{
			if(mid < minumum)
			{
				minimum = mid;
				ans = temp;
			}
			high = mid;
		}
		else
			low = mid;
		mid = (high+low) / 2;
		*/
	}
	return ans;
}