#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int	W;
int H;
char BG;
char **TAB;

typedef struct draw{
	char t;
	float y;
	float y2;
	float x;
	float x2;
	float w;
	float h;
	char c;
}				t_draw;

int	is_in_rectangle(int col, int line, t_draw el)
{
	if ((col - el.x < 1.0f) || (el.x2 - col < 1.0f) || (line - el.y < 1.0f) || (el.y2 - line < 1.0f))
		return 1;
	return 0;
}

int err_msg(FILE *fd, int err)
{
	if (err == 2 && (err = 1))
		write (1, "Error: Operation file corrupted\n", 32);
	else if (err == 1)
		write (1, "Error: argument\n", 16);
	else
	{
		int i = 0;
		while (i < H)
		{
			write(1, TAB[i], W);
			write(1, "\n", 1);
			i++;
		}
	}
	if (fd)
		fclose(fd);
	return (err);
}

int main(int argc, char **argv)
{
	t_draw el;
	FILE *fd;
	int result;

	fd = NULL;
	if (argc != 2)
		return (err_msg(fd, 1));
	if ((fd = fopen(argv[1], "r")))
	{
		if ((result = fscanf(fd, "%d %d %c", &W, &H, &BG)) == 3)
		{
			if (W > 0 && W <= 300 && H > 0 && H <= 300)
			{
				TAB = malloc(sizeof(char *) * H);
				int i = 0;
				while(i < H)
				{
					TAB[i] = malloc(sizeof(char) * W);
					memset(TAB[i], BG, W);
					i++;
				}
				while (1)
				{
					result = fscanf(fd, "\n%c %f %f %f %f %c", &el.t, &el.x, &el.y, &el.w, &el.h, &el.c);
					if (result == -1)
						return (err_msg(fd, 0));
					else if (result != 6 || el.w <= 0 || el.h <= 0 || (el.t != 'r' && el.t != 'R'))
						break ;
					el.x2 = el.x + el.w;
					el.y2 = el.y + el.h;  
					int line = 0;
					while (line < H)
					{
						int col = 0;
						while (col < W)
						{
							if (line >= el.y && el.y2 >= line && col >= el.x && el.x2 >= col)
							{
								if ((el.t == 'R' )|| (el.t == 'r' && is_in_rectangle(col, line, el)))
									TAB[line][col] = el.c;
							}
							col++;
						}
						line++;
					}
				}
			}
		}
	}
	return (err_msg(fd, 2));
}