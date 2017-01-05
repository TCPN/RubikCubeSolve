#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
typedef struct face{
	char grid[3][3];
	struct face *u, *r, *d, *l;
} Face;

typedef struct cube{
	Face face[6]; // U R F B L D
} Cube;

typedef struct record{
	Cube cube;
	int stepN;
	char steps[20];
} CubeRecord;

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


void rotate(Cube *cube, int cmd)
{
	char *chains[5][4];
	int faceIndex = cmd/3;
	int times = (cmd%3)+1;
	get5chain(&(cube->face[faceIndex]), chains);
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

int findRecorded(Cube *cube, long long recordN, CubeRecord *records)
{
	int r, i, j, k, check;
	for(r = 0; r < recordN; r ++)
	{
		check = 1;
		for(i = 0; check && i < 6; i ++)
			for(j = 0; check && j < 3; j ++)
				for(k = 0; check && k < 3; k ++)
					if(cube->face[i].grid[j][k] != records[r].cube.face[i].grid[j][k])
						check = 0;
		if(check == 1)
			return r;
	}
	return -1;
}

long long BackSearchMore(long long recordN, CubeRecord *records, CubeRecord **newRecAddr)
{
	long long newN = recordN * (recordN == 1 ? 18 : 15);
	CubeRecord *newRecs = (CubeRecord *)malloc(sizeof(CubeRecord) * newN);
	int r, m;
	long long recCount = 0;
	Cube *cube, *newCube;
	int stepN;
	char *steps;
	for(r = 0; r < recordN; r ++)
	{
		cube = &(records[r].cube);
		stepN = records[r].stepN;
		steps = records[r].steps;
		for(m = 0; m < 18; m ++)
		{
			if(stepN > 0 && m/3 == steps[stepN-1]/3) // skip 1/6
			{
				m += 2; 
				continue;
			}
			if(stepN > 1 && m/3 == (steps[stepN-2]/3) && ((steps[stepN-1]/3) + m/3) == 5) // skip 1/36
			{
				m += 2; 
				continue;
			}
				
			memcpy(&newRecs[recCount], &records[r], sizeof(CubeRecord));
			newRecs[recCount].steps[stepN] = m;
			newRecs[recCount].stepN ++;
			newCube = &(newRecs[recCount].cube);
			initialCube(newCube);
			rotate(newCube, m);
			recCount ++;
		}
	}
	*newRecAddr = newRecs;
	return recCount;
}

int findSolution(Cube *cube, int stepLimit, char *stepStack, long long recordN, CubeRecord *records)
{
	int i, j;
	int stackCount = 0;
	int popped = -1;
	long long matchRecord = -1;
	do
	{
		if(stackCount == stepLimit || stackCount == stepLimit - 1)
		{
			matchRecord = findRecorded(cube, recordN, records);
			if(matchRecord >= 0)
			{
				printf("A Record is Found(%d).\n", matchRecord);
				char *steps = stepStack;
				printf("from puzzle(%d): ",stackCount);
				for(i = 0; i < stackCount; i ++)
				{
					printf("%c%s ", faceName[steps[i]/3], (steps[i] % 3 == 1 ? "2" : (steps[i] % 3 == 2 ? "'" : "")));
				}
				steps = records[matchRecord].steps;
				printf("from goal(%d): ",records[matchRecord].stepN);
				for(i = 0; i < records[matchRecord].stepN; i ++)
				{
					printf("%c%s ", faceName[steps[i]/3], (steps[i] % 3 == 1 ? "2" : (steps[i] % 3 == 2 ? "'" : "")));
				}
				printf("\n");
				for(i = records[matchRecord].stepN-1; i >= 0; i --)
				{
					stepStack[stackCount] = (steps[i]/3)*3 + (2 - steps[i]%3);
					rotate(cube, stepStack[stackCount]);
					stackCount ++;
				}
				if(isSolved(cube))
					printf("SUCCESS!\n");
				return stackCount;
			}
		}
		if( stackCount < stepLimit)
		{
			stepStack[stackCount] = 0;
			// avoid rotate the same face as last step
			while(	stackCount > 0 && stepStack[stackCount]/3 == stepStack[stackCount-1]/3 || 
					(stackCount > 1
						&& stepStack[stackCount]/3 == stepStack[stackCount-2]/3
						&& (stepStack[stackCount]/3 + stepStack[stackCount-1]/3 )== 5))
			{
				stepStack[stackCount] += 3;
			}
			rotate(cube, stepStack[stackCount]);
			stackCount ++;
		}
		else
		{
			stackCount --;
			while(stackCount >= 0)
			{
				rotate(cube, (stepStack[stackCount] / 3)*3+(2-stepStack[stackCount] % 3));
				stepStack[stackCount] ++;
				// avoid rotate the same face as last step
				while(	stackCount > 0 && stepStack[stackCount]/3 == stepStack[stackCount-1]/3 || 
						(stackCount > 1
							&& stepStack[stackCount]/3 == stepStack[stackCount-2]/3
							&& (stepStack[stackCount]/3 + stepStack[stackCount-1]/3 )== 5))
				{
					stepStack[stackCount] += 3;
				}
				if(stepStack[stackCount] < 18)
				{
					rotate(cube, stepStack[stackCount]);
					stackCount++;
					break;
				}
				else
				{
					stackCount --;
				}
			}
		}
	}while(stackCount > 0);
	return -1;
}

int main(void)
{
	printf("Please input in the order \n\
	1. record the current color of the center grid of U=>0,R=>1,F=>2,B=>3,L=>4,D=>5\n\
	2. 'input current U, then up become left, then left become front' (repeat for 3 times)\n\
	3. up become front, then front become back, then do 2. again\n");
	
	Cube *cube = (Cube *)malloc(sizeof(Cube));
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
	
	// get the first record ready for back search
	int RecN = 1;
	CubeRecord *records = (CubeRecord*)malloc(sizeof(CubeRecord));
	CubeRecord *newRecords;
	memcpy(&(records[0].cube), cube, sizeof(Cube));
	records[0].stepN = 0;
	initialCube(&(records[0].cube));
	
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
	
	printCube(cube);
	
	printf("Solving...\n");
	
	time_t begin, end;
	time(&begin);
	
	char steps[24] = {0};
	int stepN = -1;
	for(i = 0; i < 25; i ++)
	{
		printf("trying %d steps...\n", i);
		if((stepN = findSolution(cube, i, steps, RecN, records)) >= 0)
			break;
		
		printf("One More Step Back Search...\n", i);
		RecN = BackSearchMore(RecN, records, &newRecords);
		free(records);
		records = newRecords;
		
	}
	if(isSolved(cube))
	{
		printf("solved in %d step:\n", stepN);
		for(i = 0; i < stepN; i ++)
		{
			printf("%c%s ", faceName[steps[i]/3], (steps[i] % 3 == 1 ? "2" : (steps[i] % 3 == 2 ? "'" : "")));
		}
	}
	else
	{
		printf("unsolved.");
	}
	printf("\n");
	
	time(&end);
	printf("spend time: %f sec\n", difftime(end, begin));
	
	return 0;
}