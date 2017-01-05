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

int  cOrienToVal(char orien[8])
{
    int i, m = 1, v = 0;

    for(i = 0; i < 7; i ++)
	{
		v += m * orien[i];
		m *= 3;
    }
    return v;
}

int eOrienToVal(char orien[12])
{
    int i, m = 1, v = 0;

    for(i = 0; i < 11; i ++)
	{
		v += m * orien[i];
		m *= 2;
    }
    return v ;
}

void valToCOrien(int v, char orien[8])
{
    int i, sum = 0;

    for(i = 0; i < 7; i ++)
	{
		orien[i] = v % 3;
		v /= 3;
		sum += orien[i];
    }
	orien[7] = ((sum%3 == 0) ? 0 : (3-sum%3));
    return;
}

void valToEOrien(int v, char orien[12])
{
    int i, sum = 0;

    for(i = 0; i < 11; i ++)
	{
		orien[i] = v % 2;
		v /= 2;
		sum += orien[i];
    }
	orien[11] = ((sum%2 == 0) ? 0 : 1);
    return;
}


void valToPerm(int n, int value, char permuted[n])
{
    int i, index, m = value;
    char elems[n];

    for(i = 0; i < n; i ++)
		elems[i] = i;

    for(i = 0; i < n; i ++)
	{
		index = m % (n - i);
		m = m / (n - i);
		permuted[i] = elems[index];
		elems[index] = elems[n - i - 1];
    }
    return;
}

int permToVal(int n, char perm[n])
{
    int i, k = 0, m = 1;
	char pos[n];
    char elems[n];

    for(i = 0; i < n; i ++)
	{
		pos[i]=i;
		elems[i]=i;
	}

    for( i = 0; i < n-1; i ++)
    {
            k += m * pos[perm[i]];
            m = m * (n - i);
            pos[elems[n-i-1]] = pos[perm[i]];
            elems[pos[perm[i]]] = elems[n-i-1];
    }

    return k;
}

void valToUDslicePerm(int value, char occupied[12])
{
	int n = 11, k = 3, sum = value, comb;//printf("value %d\n",value);
	while(n >= 0)
	{
		if(k >= 0)
		{
			comb = combination(n,k);//printf("comb(%d,%d) = %d, sum = %d\n",n,k,comb, sum);
			if(sum >= comb) // not here (11-n)
			{//printf("NO\n");
				occupied[11-n] = 0;
				sum -= comb;
			}
			else
			{//printf("YES\n"); // here (11-n) it is
				occupied[11-n] = 1;
				k --;
			}
		}
		else
		{
			occupied[11-n] = 0;
		}
		n --;
	}
	return;
}

int UDslicePermToVal(char occupied[12])
{
	int n = 11, k = 3, sum = 0;;
	while(n >= 0 && k >= 0)
	{
		if(occupied[11-n]) // check from 0 to 11
			k --;
		else
		{
			sum += combination(n,k); 
		}
		n --;
	}
	return sum;
}

int combination(int n, int k)
{
	if(n < 0)
		return 1;
	if(k == 0)
		return 1;
	return (combination(n-1,k-1)*n/k);
}

int factorial(int n)
{
	return factorial(n-1)*n;
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

char distance[2187][495][1024];
//char *distance;
short coMove[2187][18];
short eoMove[2048][18];
short UDSliceMove[495][18];

unsigned int *queue;
int queueSize = 4096;
int queueLen = 0;
int queueHead = 0;



int initQueue(void)
{
	queue = (unsigned int *)malloc(sizeof(unsigned int) * queueSize);
	if(queue != NULL)
		return 1;
	printf("Failed to allocate queue buffer.\n");
	return 0;
}
int expandQueue(void)
{
	/*
	unsigned int *newbuf = realloc(queue, queueSize*2);
	if(newbuf != NULL)
	{
		memcpy((&queue[queueHead]) + queueSize, &(queue[queueHead]), queueSize - queueHead);
		queueHead = queueHead + queueSize;
		queueSize *= 2;
		printf("Expanded queue size to %d * 4!!\n", queueSize);
		return 1;
	}
	else
	{
		printf("queueSize = %d. Failed to reallocate queue buffer.\n",queueSize);
		return 0;
	}*/
		
}
void enQueue(unsigned int v)
{
	if(queueLen == queueSize)
	{
		int e = expandQueue();
		if(!e)
			return;
	}
	
	queue[(queueHead+queueLen)%queueSize] = v;
	queueLen++;
		
}
unsigned int deQueue()
{
	if(queueLen == 0)
		return -1;
	unsigned int ret = queue[queueHead];
	queueLen--;
	queueHead = (queueHead+1)%queueSize;
}


unsigned int cmprPhase1Crd(short co, short eo, short UD)
{
	return (((unsigned int)co * 2048 + (unsigned int)eo ) * 495 + (unsigned int)UD);
}
void extrPhase1Crd(unsigned int dq, short *co, short *eo, short *UD)
{
	*UD = dq % 495;
	dq /= 495;
	*eo = dq % 2048;
	dq /= 2048;
	*co = dq;
	return;
}

char readDistance(short co, short eo, short UD)
{//printf("readDistance %d %d %d\n",co,eo,UD);printf("%d,",(((co * 495 + UD) * 1024) + eo/2));printf(" -> %d",distance[ (((co * 495 + UD) * 1024) + eo/2)]);
	char d;
	//char d = distance[ (((co * 495 + UD) * 1024) + eo/2)];
	if(eo % 2 == 1)
	{
		d = ((distance[co][UD][eo/2]>>4) & 15);
	}
	else
	{
		d = ((distance[co][UD][eo/2]) & 15);
	}
	if(d == 15)
		return -1;
	else
		return d;
}
void setDistance(short co, short eo, short UD, char value)
{
	//assume value <= 12
	if(eo % 2 == 1)
	{
		distance[co][UD][eo/2] = ((value & 15 )<<4) | (distance[co][UD][eo/2] & 15);
		//distance[ (((co * 495 + UD) * 1024) + eo/2)] = ((value & 15 )<<4) | (distance[ (((co * 495 + UD) * 1024) + eo/2)] & 15);
	}
	else
	{
		distance[co][UD][eo/2] = (distance[co][UD][eo/2] & (15<<4)) | (value & 15);
		//distance[ (((co * 495 + UD) * 1024) + eo/2)] = (distance[ (((co * 495 + UD) * 1024) + eo/2)] & (15<<4)) | (value & 15);
	}
}

int generatePruneTable(void)
{printf("generate Prune table....\n");

	//distance = (char *) malloc(sizeof(char)*2187*495*1024);

	memset(distance, -1, sizeof(char)*2187*495*1024);
	
	short coV = 0, eoV = 0, UDslV = 0;
	short ncoV = 0, neoV = 0, nUDslV = 0;
	char myDist;
	setDistance(coV, eoV, UDslV, 0);
	
	int m;
	int depthLimit = 0;
	int coStack[21];
	int eoStack[21];
	int UDStack[21];
	char stepStack[21];
	int stackH = 0;
	
	int ndist;
	
	long long foundNewNode = 0;
	
	for(depthLimit = 0; depthLimit < 9; depthLimit ++)
	{
		foundNewNode = 0;
		//DFID
		coStack[0] = 0;
		eoStack[0] = 0;
		UDStack[0] = 0;
		stepStack[0] = -1;
		stackH = 0;
		printf("depth %d iteration....\n",depthLimit);
		time_t begin, end;
		time(&begin);
		
		while(stackH >= 0)
		{
			coV = coStack[ stackH ];
			eoV = eoStack[ stackH ];
			UDslV = UDStack[ stackH ];
			m = stepStack[ stackH ];
			
			myDist = readDistance(coV, eoV, UDslV);
			
			//printf("Being node (%d, %d, %d) with depth %d.\n",coV ,eoV, UDslV,myDist);
			//printf("last move: %d.\n",m);
			
			if(depthLimit == stackH)
			{//printf("reach depth limit. Back to upper\n");
				stackH --;
			}
			else
			{
				m ++;
				if(stackH >= 1 && (stepStack[ stackH - 1] ) / 3 == m/3)
					m += 3;
				if(stackH >= 2 && (stepStack[ stackH - 2] ) / 3 == m/3 && (5 - ((stepStack[ stackH - 1] ) / 3)) == (m/3))
					m += 3;
				stepStack[ stackH ] = m;
				
				if(m < 18)
				{//printf("take a move: %d.\n",m);
					ncoV = coMove[coV][m];
					neoV = eoMove[eoV][m];
					nUDslV = UDSliceMove[UDslV][m];
					
					ndist = readDistance(ncoV ,neoV, nUDslV);
					//printf("lead to a node (%d, %d, %d) with depth %d.\n",ncoV ,neoV, nUDslV, ndist);
					
					if(( ndist != -1 && ndist <= myDist )|| (ncoV == coV && neoV == eoV && nUDslV == UDslV))
					{
						continue;
					}
					else
					{
					
						if((myDist == depthLimit - 1 )&& ndist != -1)
						{
							//continue;
						}
						if(ndist == -1)
						{//printf("It's a new node\n");
							setDistance(ncoV ,neoV, nUDslV, myDist+1);
							foundNewNode++;
							if((foundNewNode & 4194303) == 0)
							{
								if((foundNewNode & 16777215) == 0)
									printf("%d nodes are found so far.\n",foundNewNode );
								else
									printf(".");
							}
						}
						stackH ++;
						coStack[ stackH ] = ncoV;
						eoStack[ stackH ] = neoV;
						UDStack[ stackH ] = nUDslV;
						stepStack[ stackH ] = -1;
						//printf("Go Down\n");
					}
				}
				else
				{//printf("no more move. Back to upper\n");
					stackH --;
				}
			}
		}
		
		
		time(&end);
		printf("depth %d iteration finish. spend time: %f sec\n",depthLimit, difftime(end, begin));//char ac; scanf("%c", &ac);
		printf("%d nodes are found.\n",foundNewNode );
		if(depthLimit > 0 && (foundNewNode == 0))
		{
			printf("all nodes are found. \n Prune table done. (deepest=%d)\n", depthLimit-1);
			break;
		}
	}
	
	// for nodes whose depth >= 9
	printf("turn for backward search....\n");
	
	for(depthLimit = 8; depthLimit <= 12; depthLimit ++)
	{ // do this only for 5 times
		// backward search
		printf("search for depth %d nodes....\n", depthLimit);
		foundNewNode = 0;
		time_t begin, end;
		time(&begin);
		
		for(coV = 0; coV < 2187; coV ++)
		{
			
			for(eoV = 0; eoV < 2048; eoV ++)
			{
			
				for(UDslV = 0; UDslV < 495; UDslV ++)
				{
					if(readDistance(coV ,eoV, UDslV) == -1)
					{
						for(m = 0; m < 18; m ++)
						{
							ncoV = coMove[coV][m];
							neoV = eoMove[eoV][m];
							nUDslV = UDSliceMove[UDslV][m];
							
							ndist = readDistance(ncoV ,neoV, nUDslV);
							if(ndist == depthLimit)
							{
								setDistance(coV ,eoV, UDslV, depthLimit+1);
								foundNewNode++;
								if((foundNewNode & 4194303) == 0)
								{
									if((foundNewNode & 16777215) == 0)
										printf("%d nodes are found so far.\n",foundNewNode );
									else
										printf(".");
								}
							}
						}
					}
				}
			}
		}
		
		time(&end);
		printf("Finish searching for depth %d nodes. spend time: %f sec\n",depthLimit, difftime(end, begin));//char ac; scanf("%c", &ac);
		printf("%d nodes are found.\n",foundNewNode );
		if(foundNewNode == 0)
		{
			printf("all nodes are found. \n Prune table done. (deepest=%d)\n", depthLimit);
			break;
		}
	}
	/*
	unsigned int dq;
	initQueue();	
	enQueue(cmprPhase1Crd(coV, eoV, UDslV));
	
	while(queueLen > 0)
	{//printf("A");
		dq = deQueue();
		extrPhase1Crd(dq, &coV, &eoV, &UDslV);//printf("A2");
		
		myDist = readDistance(coV, eoV, UDslV);
		//printf("B");
		for(m = 0; m < 18; m ++)
		{
			ncoV = coMove[coV][m];
			neoV = eoMove[eoV][m];
			nUDslV = UDSliceMove[UDslV][m];
			//printf("C");
			if(readDistance(ncoV ,neoV, nUDslV) == -1)
			{//printf("E");
				setDistance(ncoV ,neoV, nUDslV, myDist+1);//printf("F");
				enQueue(cmprPhase1Crd(ncoV, neoV, nUDslV));
			}//printf("D");
		}
	}*/
	printf("Prune table is done.\n");
	return 0;
	
}

int generateMoveTable(void)
{printf("generate Corner Orientations Move table....\n");
	int i, j, k;
	int tmp;
	char cubeCo[8];
	for(i = 0; i < 2187; i ++)
	{//printf("i=%d\n",i);
		valToCOrien(i, cubeCo);//int t; for(t = 0; t < 8; t ++){printf("%d ", cubeCo[t]);}printf("\n",i);
		for(j = 0; j < 6; j ++)
		{
			for(k = 0; k <= 3; k ++)
			{
				switch(j)
				{
					case 0:tmp=cubeCo[0];cubeCo[0]=cubeCo[4];  cubeCo[4]=cubeCo[6];  cubeCo[6]=cubeCo[2];  cubeCo[2]=tmp;  break;
					case 1:tmp=cubeCo[4];cubeCo[4]=(cubeCo[0]+1)%3;cubeCo[0]=(cubeCo[1]+2)%3;cubeCo[1]=(cubeCo[5]+1)%3;cubeCo[5]=(tmp+2)%3;break;
					case 2:tmp=cubeCo[0];cubeCo[0]=(cubeCo[2]+1)%3;cubeCo[2]=(cubeCo[3]+2)%3;cubeCo[3]=(cubeCo[1]+1)%3;cubeCo[1]=(tmp+2)%3;break;
					case 3:tmp=cubeCo[6];cubeCo[6]=(cubeCo[4]+1)%3;cubeCo[4]=(cubeCo[5]+2)%3;cubeCo[5]=(cubeCo[7]+1)%3;cubeCo[7]=(tmp+2)%3;break;
					case 4:tmp=cubeCo[2];cubeCo[2]=(cubeCo[6]+1)%3;cubeCo[6]=(cubeCo[7]+2)%3;cubeCo[7]=(cubeCo[3]+1)%3;cubeCo[3]=(tmp+2)%3;break;
					case 5:tmp=cubeCo[1];cubeCo[1]=cubeCo[3];  cubeCo[3]=cubeCo[7];  cubeCo[7]=cubeCo[5];  cubeCo[5]=tmp;  break;
				}//printf("move=%d*3+%d  =>  ",j,k);int t; for(t = 0; t < 8; t ++){printf("%d ", cubeCo[t]);}printf("\n",i);
				if(k != 3)
				{
					coMove[i][j*3+k] = cOrienToVal(cubeCo);
					//printf("coMove[i][j*3+k] = %d\n",coMove[i][j*3+k]);
				}
			}
		}//scanf("%[\n]");
	}
printf("generate Edge Orientations Move table....\n");	
	char cubeEo[12];
	for(i = 0; i < 2048; i ++)
	{
		valToEOrien(i, cubeEo);
		for(j = 0; j < 6; j ++)
		{
			for(k = 0; k <= 3; k ++)
			{
				switch(j)
				{
					case 0:tmp=cubeEo[5]  ;cubeEo[5]=cubeEo[10] ;cubeEo[10]=cubeEo[4] ;cubeEo[4]=cubeEo[8]  ;cubeEo[8]=tmp  ;break;
					case 1:tmp=cubeEo[8]  ;cubeEo[8]=cubeEo[0]  ;cubeEo[0]=cubeEo[9]  ;cubeEo[9]=cubeEo[2]  ;cubeEo[2]=tmp  ;break;
					case 2:tmp=cubeEo[4]  ;cubeEo[4]=(cubeEo[1]+1)%2;cubeEo[1]=(cubeEo[6]+1)%2;cubeEo[6]=(cubeEo[0]+1)%2;cubeEo[0]=(tmp+1)%2;break;
					case 3:tmp=cubeEo[5]  ;cubeEo[5]=(cubeEo[2]+1)%2;cubeEo[2]=(cubeEo[7]+1)%2;cubeEo[7]=(cubeEo[3]+1)%2;cubeEo[3]=(tmp+1)%2;break;
					case 4:tmp=cubeEo[10] ;cubeEo[10]=cubeEo[3] ;cubeEo[3]=cubeEo[11] ;cubeEo[11]=cubeEo[1] ;cubeEo[1]=tmp  ;break;
					case 5:tmp=cubeEo[6]  ;cubeEo[6]=cubeEo[11] ;cubeEo[11]=cubeEo[7] ;cubeEo[7]=cubeEo[9]  ;cubeEo[9]=tmp  ;break;
				}
				if(k != 3)
					eoMove[i][j*3+k] = eOrienToVal(cubeEo);
			}
		}
	}
printf("generate UD slices position Move table....\n");
	char cubeUDslice[12];
	for(i = 0; i < 495; i ++)
	{
		valToUDslicePerm(i, cubeUDslice);
		for(j = 0; j < 6; j ++)
		{
			for(k = 0; k <= 3; k ++)
			{
				switch(j)
				{
					case 0:tmp=cubeUDslice[5]  ;cubeUDslice[5]=cubeUDslice[10] ;cubeUDslice[10]=cubeUDslice[4] ;cubeUDslice[4]=cubeUDslice[8]  ;cubeUDslice[8]=tmp  ;break;
					case 1:tmp=cubeUDslice[8]  ;cubeUDslice[8]=cubeUDslice[0]  ;cubeUDslice[0]=cubeUDslice[9]  ;cubeUDslice[9]=cubeUDslice[2]  ;cubeUDslice[2]=tmp  ;break;
					case 2:tmp=cubeUDslice[4]  ;cubeUDslice[4]=cubeUDslice[1]  ;cubeUDslice[1]=cubeUDslice[6]  ;cubeUDslice[6]=cubeUDslice[0]  ;cubeUDslice[0]=tmp  ;break;
					case 3:tmp=cubeUDslice[5]  ;cubeUDslice[5]=cubeUDslice[2]  ;cubeUDslice[2]=cubeUDslice[7]  ;cubeUDslice[7]=cubeUDslice[3]  ;cubeUDslice[3]=tmp  ;break;
					case 4:tmp=cubeUDslice[10] ;cubeUDslice[10]=cubeUDslice[3] ;cubeUDslice[3]=cubeUDslice[11] ;cubeUDslice[11]=cubeUDslice[1] ;cubeUDslice[1]=tmp  ;break;
					case 5:tmp=cubeUDslice[6]  ;cubeUDslice[6]=cubeUDslice[11] ;cubeUDslice[11]=cubeUDslice[7] ;cubeUDslice[7]=cubeUDslice[9]  ;cubeUDslice[9]=tmp  ;break;
				}
				if(k != 3)
					UDSliceMove[i][j*3+k] = UDslicePermToVal(cubeUDslice);
			}
		}
	}
	
	
	return 0;
	
}

int findSolution(Cube *cube, int stepLimit, char *stepStack, int recordN, CubeRecord *records)
{
	int i;
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
	printf("10~20 MIN IS NEEDED MAYBE.");
	
	generateMoveTable();
	
	generatePruneTable();
	FILE *fp = fopen("p1_prune_table", "wb");
	fwrite(distance, sizeof(char), 2187 * 495 * 1024, fp);
	fclose(fp);
	/*
	printf("%d %d %d  %d %d %d %d %d\n",coMove[0][0],coMove[0][1],coMove[0][2]
		,coMove[0][3],coMove[0][6],coMove[0][9],coMove[0][12],coMove[0][15]	);
	printf("%d %d %d  %d %d %d %d %d\n",eoMove[0][0],eoMove[0][1],eoMove[0][2]
		,eoMove[0][3],eoMove[0][6],eoMove[0][9],eoMove[0][12],eoMove[0][15]	);
	printf("%d %d %d  %d %d %d %d %d\n",UDSliceMove[0][0],UDSliceMove[0][1],UDSliceMove[0][2]
		,UDSliceMove[0][3],UDSliceMove[0][6],UDSliceMove[0][9],UDSliceMove[0][12],UDSliceMove[0][15]	);
	*/
	char inputbuf[12];
	char buf[12];
	
	int v, m, c,e,p;
	char cmd[32];
	while(scanf("%s", cmd) != EOF)
	{
		if(cmd[0] == 'c')
		{
			for(i = 0; i < 8; i ++)
			{
				scanf("%d", &inputbuf[i]);
			}
			c = v = cOrienToVal(inputbuf);
			printf("val=%d\n", v);
			for(i = 0; i < 18; i ++)
			{
				m = coMove[v][i];
				printf("move %d => %d [ ", i, m);
				valToCOrien(m, buf);
				for(j = 0; j < 8; j ++)
				{
					printf("%d ", buf[j]);
				}
				printf("]\n");
			}
		}
		else if(cmd[0] == 'e')
		{
			for(i = 0; i < 12; i ++)
			{
				scanf("%d", &inputbuf[i]);
			}
			e = v = eOrienToVal(inputbuf);
			printf("val=%d\n", v);
			for(i = 0; i < 18; i ++)
			{
				m = eoMove[v][i];
				printf("move %d => %d [ ", i, m);
				valToEOrien(m, buf);
				for(j = 0; j < 12; j ++)
				{
					printf("%d ", buf[j]);
				}
				printf("]\n");
			}
		}
		else if(cmd[0] == 'p')
		{
			for(i = 0; i < 12; i ++)
			{
				scanf("%d", &inputbuf[i]);
			}
			p = v = UDslicePermToVal(inputbuf);
			printf("val=%d\n", v);
			for(i = 0; i < 18; i ++)
			{
				m = UDSliceMove[v][i];
				printf("move %d => %d [ ", i, m);
				valToUDslicePerm(m, buf);
				for(j = 0; j < 12; j ++)
				{
					printf("%d ", buf[j]);
				}
				printf("]\n");
			}
		}
		else if(cmd[0] == 'd')
		{
			printf("c=%d e=%d p=%d\n",c,e,p);
			printf("Distance=%d\n",readDistance(c ,e, p));
		}
	}
		
	return 0;
}