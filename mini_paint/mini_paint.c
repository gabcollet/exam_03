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
	float x;
	float y;
	float r;
	char c;
}				t_draw;

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
	float sqr;
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
					result = fscanf(fd, "\n%c %f %f %f %c", &el.t, &el.x, &el.y, &el.r, &el.c);
					if (result == -1)
						return (err_msg(fd, 0));
					else if (result != 5 || el.r <= 0 || (el.t != 'c' && el.t != 'C'))
						break ;  
					int line = 0;
					while (line < H)
					{
						int col = 0;
						while (col < W)
						{
							sqr = sqrtf((col - el.x) * (col - el.x) + (line -el.y) * (line - el.y));
							if (sqr <= el.r)
							{
								if (el.t == 'c' && sqr + 1 > el.r)
									TAB[line][col] = el.c;
								else if (el.t == 'C')
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