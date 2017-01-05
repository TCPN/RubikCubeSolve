#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct face{
	char grid[3][3];
	struct face *u, *r, *d, *l;
} Face;

typedef struct cube{
	Face face[6]; // U R F B L D
} Cube;

void initialCube(Cube* cube)
{
	cube->face[0].u = &(cube->face[3]);
	cube->face[0].r = &(cube->face[1]);
	cube->face[0].d = &(cube->face[2]);
	cube->face[0].l = &(cube->face[4]);

	cube->face[1].u = &(cube->face[5]);
	cube->face[1].r = &(cube->face[2]);
	cube->face[1].d = &(cube->face[0]);
	cube->face[1].l = &(cube->face[3]);

	cube->face[2].u = &(cube->face[4]);
	cube->face[2].r = &(cube->face[0]);
	cube->face[2].d = &(cube->face[1]);
	cube->face[2].l = &(cube->face[5]);

	cube->face[3].u = &(cube->face[0]);
	cube->face[3].r = &(cube->face[4]);
	cube->face[3].d = &(cube->face[5]);
	cube->face[3].l = &(cube->face[1]);

	cube->face[4].u = &(cube->face[2]);
	cube->face[4].r = &(cube->face[5]);
	cube->face[4].d = &(cube->face[3]);
	cube->face[4].l = &(cube->face[0]);

	cube->face[5].u = &(cube->face[1]);
	cube->face[5].r = &(cube->face[3]);
	cube->face[5].d = &(cube->face[4]);
	cube->face[5].l = &(cube->face[2]);
	
}

void get5chain(Face *face, char *chain[5][4])
{
	chain[0][0] = &(face->grid[0][0]); // 0
	chain[0][1] = &(face->grid[0][2]); // 2
	chain[0][2] = &(face->grid[2][2]); // 8
	chain[0][3] = &(face->grid[2][0]); // 6
	
	chain[1][0] = &(face->grid[0][1]); // 1
	chain[1][1] = &(face->grid[1][2]); // 5
	chain[1][2] = &(face->grid[2][1]); // 7
	chain[1][3] = &(face->grid[1][0]); // 3
	
	chain[2][0] = &(face->u->grid[0][2]); // 2
	chain[2][1] = &(face->r->grid[2][0]); // 6
	chain[2][2] = &(face->d->grid[2][2]); // 8
	chain[2][3] = &(face->l->grid[0][0]); // 0
	
	chain[3][0] = &(face->u->grid[0][1]); // 1
	chain[3][1] = &(face->r->grid[2][1]); // 7
	chain[3][2] = &(face->d->grid[1][2]); // 5
	chain[3][3] = &(face->l->grid[1][0]); // 3
	
	chain[4][0] = &(face->u->grid[0][0]); // 0
	chain[4][1] = &(face->r->grid[2][2]); // 8
	chain[4][2] = &(face->d->grid[0][2]); // 2
	chain[4][3] = &(face->l->grid[2][0]); // 6
	
	return;
}

const char faceName[6] = "URFBLD";

int faceNameToIndex(char face)
{
	switch(face)
	{
		case 'U': return 0;
		case 'R': return 1;
		case 'F': return 2;
		case 'B': return 3;
		case 'L': return 4;
		case 'D': return 5;
	}
	return -1;
}


void rotate(Cube *cube, char face, char times)
{
	char *chains[5][4];
	get5chain(&(cube->face[faceNameToIndex(face)]), chains);
	int i, j;
	char tmp[4];
	for(i = 0; i < 5; i ++)
	{
		for(j = 0; j < 4; j ++)
		{
			tmp[j] = *chains[i][j];
		}
		for(j = 0; j < 4; j ++)
		{
			*chains[i][(j + times) % 4] = tmp[j];
		}
	}
	
	return;
}

void printCube(Cube *cube)
{
	printf("%d%d%d\n",cube->face[0].grid[0][0],cube->face[0].grid[0][1],cube->face[0].grid[0][2]);
	printf("%d%d%d\n",cube->face[0].grid[1][0],cube->face[0].grid[1][1],cube->face[0].grid[1][2]);
	printf("%d%d%d\n",cube->face[0].grid[2][0],cube->face[0].grid[2][1],cube->face[0].grid[2][2]);
	
	printf("%d%d%d %d%d%d %d%d%d %d%d%d\n",
	cube->face[2].grid[0][2],cube->face[2].grid[1][2],cube->face[2].grid[2][2],
	cube->face[1].grid[2][2],cube->face[1].grid[2][1],cube->face[1].grid[2][0],
	cube->face[3].grid[0][0],cube->face[3].grid[0][1],cube->face[3].grid[0][2],
	cube->face[4].grid[2][0],cube->face[4].grid[1][0],cube->face[4].grid[0][0]
	);
	printf("%d%d%d %d%d%d %d%d%d %d%d%d\n",
	cube->face[2].grid[0][1],cube->face[2].grid[1][1],cube->face[2].grid[2][1],
	cube->face[1].grid[1][2],cube->face[1].grid[1][1],cube->face[1].grid[1][0],
	cube->face[3].grid[1][0],cube->face[3].grid[1][1],cube->face[3].grid[1][2],
	cube->face[4].grid[2][1],cube->face[4].grid[1][1],cube->face[4].grid[0][1]
	);
	printf("%d%d%d %d%d%d %d%d%d %d%d%d\n",
	cube->face[2].grid[0][0],cube->face[2].grid[1][0],cube->face[2].grid[2][0],
	cube->face[1].grid[0][2],cube->face[1].grid[0][1],cube->face[1].grid[0][0],
	cube->face[3].grid[2][0],cube->face[3].grid[2][1],cube->face[3].grid[2][2],
	cube->face[4].grid[2][2],cube->face[4].grid[1][2],cube->face[4].grid[0][2]
	);
	
	printf("%d%d%d\n",cube->face[5].grid[2][0],cube->face[5].grid[1][0],cube->face[5].grid[0][0]);
	printf("%d%d%d\n",cube->face[5].grid[2][1],cube->face[5].grid[1][1],cube->face[5].grid[0][1]);
	printf("%d%d%d\n",cube->face[5].grid[2][2],cube->face[5].grid[1][2],cube->face[5].grid[0][2]);
	printf("\n");
}

void printCubeAsInput(Cube *cube)
{
	int i, j, k;
	for(i = 0; i < 6; i ++)
	{
		for(j = 0; j < 3; j ++)
		{
			for(k = 0; k < 3; k ++)
			{
				printf("%d ", cube->face[i].grid[j][k]);
			}
		}
		printf("\n");
	}	
	printf("\n");	
}

int isSolved(Cube *cube)
{
	int i, j, k;
	for(i = 0; i < 6; i ++)
		for(j = 0; j < 3; j ++)
			for(k = 0; k < 3; k ++)
				if(cube->face[i].grid[j][k] != cube->face[i].grid[1][1])
					return 0;
	return 1;
}

int findSolution(Cube *cube, int stepLimit, int *step, int lastRotatedFace)
{
	if(isSolved(cube))
		return 1;
	if(stepLimit <= 0)
		return 0;
	int i, j;
	for(i = 0; i < 6; i ++)
	{
		if(i == lastRotatedFace)
			continue;
		for(j = 1; j <= 3; j ++)
		{
			rotate(cube, faceName[i], j);
			if(findSolution(cube, stepLimit-1, step+1, i))
			{
				*step = i*4+j;
				return 1;
			}
			rotate(cube, faceName[i], 4-j);
		}
	}
	return 0;
}

int main(void)
{
	
	printf("Please input in the order \n\
	1. record the current color of the center grid of U=>0,R=>1,F=>2,B=>3,L=>4,D=>5\n\
	2. 'input current U, then up become left, then left become front' (repeat for 3 times)\n\
	3. up become front, then front become back, then do 2. again\n");
	
	Cube *cube = (Cube*)malloc(sizeof(Cube));
	initialCube(cube);
	
	int i,j,k;
	for(i = 0; i < 6; i ++)
	{
		for(j = 0; j < 3; j ++)
		{
			for(k = 0; k < 3; k ++)
			{
				cube->face[i].grid[j][k] = i+1;
			}
		}
	}

	printCube(cube);
	
	char cmd[32];
	while(scanf("%s", &cmd) >= 0)
	{
		if(strncmp(cmd, "random", 7) == 0)
		{
			cmd[6] = 0;
			int s = 0;
			scanf("%d", &s);
			srand(time(NULL));
			for(; s > 0; s --)
			{
				rotate(cube, faceName[rand()%6], rand()%3+1);
			}
		}
		else if(strncmp(cmd, "reset", 6) == 0)
		{
			int i,j,k;
			for(i = 0; i < 6; i ++)
			{
				for(j = 0; j < 3; j ++)
				{
					for(k = 0; k < 3; k ++)
					{
						cube->face[i].grid[j][k] = i+1;
					}
				}
			}
		}
		else if(strncmp(cmd, "solve", 6) == 0)
		{
			printf("Solving...\n");
			
			time_t begin, end;
			time(&begin);
			
			int steps[25] = {};
			for(i = 0; i < 25; i ++)
			{
				printf("trying %d steps...\n", i);
				if(findSolution(cube, i, steps, -1))
					break;
			}
			if(isSolved(cube))
			{
				printf("solved:\n");
				for(i = 0; i < 25; i ++)
				{
					if(steps[i] == 0)
						break;
					printf("%c%s ", faceName[steps[i]/4], (steps[i] % 4 == 2 ? "2" : (steps[i] % 4 == 3 ? "'" : "")));
				}
			}
			else
			{
				printf("unsolved.");
			}
			printf("\n");
			
			time(&end);
			printf("spend time: %f sec\n", difftime(end, begin));
	
		}
		else if(cmd[0] == 'p')
		{
			printCubeAsInput(cube);
		}
		else if(cmd[0] == 'i')
		{
			int i,j,k;
			for(i = 0; i < 6; i ++)
			{
				for(j = 0; j < 3; j ++)
				{
					for(k = 0; k < 3; k ++)
					{
						scanf("%d", &(cube->face[i].grid[j][k]));
					}
				}
			}
		}
		else if(cmd[0] == 'c')
		{
			char line[1024] = {};
			fgets(line, 1024, stdin);
			char *p = line;
			char outputbuf[2048] = {};
			char *output = outputbuf;
			int f, w;
			while(*p != 0)
			{
				switch(*p)
				{
					case 'U':f = 0; break;
					case 'R':f = 1; break;
					case 'F':f = 2; break;
					case 'B':f = 3; break;
					case 'L':f = 4; break;
					case 'D':f = 5; break;
					case ' ': p++; continue;
					default: f = -1; break;
				}
				p ++;
				if(f == -1)
				{
					output += sprintf(output, "%c ", *(p-1));
					continue;
				}
				switch(*p)
				{
					case '\'': case '3':
						w = 2; p ++; break;
					case '2':
						w = 1; p ++; break;
					case '1': case ' ':
							   p ++; 
					default:
						w = 0; break;
				}
				if(cmd[1] == '1' || cmd[1] == 0)
				{
					output += sprintf(output, "%d ", f*3+w);
				}
				else if(cmd[1] == '2')
				{
					output += sprintf(output, "%d+%d ", f*3,w);
				}
				else if(cmd[1] == '3')
				{
					output += sprintf(output, "%d*3+%d ", f,w);
				}
				else if(cmd[1] == '4')
				{
					output += sprintf(output, "%d*3%s, ", f,(w==0 ? "" :(w==1 ? "+1" : "+2")));
				}
			}
			printf("%s\n", outputbuf);
		}
		else if(cmd[0] == 'q' || cmd[0] == 'e')
		{
			break;
		}
		else if(strchr("URLDFBurldfb", cmd[0]) != NULL)
		{
			int w = (cmd[1] == '\0' || cmd[1] == '1' ? 1 : (cmd[1] == '2' ? 2 : (cmd[1] == '3' || cmd[1] == '\'' ? 3 : 0)));
			rotate(cube, toupper(cmd[0]), w);
		}
		printCube(cube);
	}
	
	
	return 0;
}