int check_track(char x, char y)
{
	if ((x == 'N') || (x == 'g'))
		return 1;
	else if (x == 'b')
		return 2;
	else if (x == 'c')
	{
		if (y == 'h')
			return 3;
		else
			return 4;
	}
	else if (x == 'e')
		return 5;
	else if (x == 'm')
	{
		if (y == 'a')
			return 6;
		else
			return 7;
	}
	else
		return 8;
}