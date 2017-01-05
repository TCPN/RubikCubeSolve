#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define P(a) {printf("%s",#a);};
#define PV(a) {printf("%s=%d ",#a,a);};
#define PL(a) {printf("%s\n",#a);};
#define PLN {printf("L:%d ",__LINE__);};
#define PA(a,n) {int i; for(i=0;i<n;i++)printf("%d ",a[i]);};
#define PPV(cp,ep,ud) {char tt[8];valToPerm(8,cp,tt);printf("cp\n%d  %d\n %d%d\n %d%d\n%d  %d\n",tt[6],tt[4],tt[7],tt[5],tt[3],tt[1],tt[2],tt[0]);valToPerm(8,ep,tt);printf("ep\n    %2d\n    %2d\n%2d%2d  %2d%2d\n    %2d\n    %2d\n",tt[1]+4,tt[3]+4,tt[6]+4,tt[7]+4,tt[5]+4,tt[4]+4,tt[2]+4,tt[0]+4);valToPerm(4,ud,tt);printf("UD2\n%d%d\n%d%d\n",tt[3],tt[2],tt[1],tt[0]);};
#define PCPV(cp) {char tt[8];valToPerm(8,cp,tt);printf("cp\n%d  %d\n %d%d\n %d%d\n%d  %d\n",tt[6],tt[4],tt[7],tt[5],tt[3],tt[1],tt[2],tt[0]);};
#define PEPV(ep) {char tt[8];valToPerm(8,ep,tt);printf("ep\n    %2d\n    %2d\n%2d%2d  %2d%2d\n    %2d\n    %2d\n",tt[1]+4,tt[3]+4,tt[6]+4,tt[7]+4,tt[5]+4,tt[4]+4,tt[2]+4,tt[0]+4);};
#define PUDV(ud) {char tt[8];valToPerm(4,ud,tt);printf("UD2\n%d%d\n%d%d\n",tt[3],tt[2],tt[1],tt[0]);};
#define HOLD {char c;scanf("%c",&c);};
int SWITCH(int i){static s = 0; if(i==1)s=1;else if(i == -1)s = 0;return s;}
#define ON {SWITCH(1);};
#define OFF {SWITCH(1);};
#define IFON(a) {if(SWITCH(0)){a;}};

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

int colorToFace(Cube *cube, int color)
{
	int i;
	for(i = 0; i < 6; i ++)
	{
		if(cube->face[i].grid[1][1] == color)
			return i;
	}
	return -1;
}

// edge: FR, FL, BR, BL, UF, UB, DF, DB, UR, DR, UL, DL
// axis: U-D, R-L, F-B
		char faceGridOnEdge[6][12] = {
			{-1,-1,-1,-1, 7, 1,-1,-1, 5,-1, 3,-1},
			{ 5,-1, 3,-1,-1,-1,-1,-1, 7, 1,-1,-1},
			{ 7, 1,-1,-1, 5,-1, 3,-1,-1,-1,-1,-1},
			{-1,-1, 3, 5,-1, 1,-1, 7,-1,-1,-1,-1},
			{-1, 1,-1, 7,-1,-1,-1,-1,-1,-1, 3, 5},
			{-1,-1,-1,-1,-1,-1, 3, 5,-1, 1,-1, 7},
		};
		
int edgeBesides(int f1, int f2)
{
	int e;
	for(e = 0; e < 12; e ++)
	{
		if(faceGridOnEdge[f1][e] > 0 && faceGridOnEdge[f2][e] > 0)
			return e;
	}
	/*
	int edge = 0;
	int axisv[3] = {-1,-1,-1};
	int left = 3;
	int axis;
	axis = (face[0] < 3 ? face[0] : 5 - face[0]);
	axisv[axis] = !(face[0] < 3);
	left -= axis;
	axis = (face[1] < 3 ? face[1] : 5 - face[1]);
	axisv[axis] = !(face[0] < 3);
	left -= axis;
	
	edge |= left << 2;
	if(axisv[(left+1)%3])
		edge |= 1;
	if(axisv[(left+2)%3])
		edge |= 2;
	return edge;
	*/
}

int edgeByFaceGrid(Face *face, int grid)
{
	int a[2] = {};
	//a[0] = face->id;
	//a[1] = (grid < 4 ? 
//				(grid < 2 ? face->u->id : face->l->id) : 
//				(grid < 6 ? face->r->id : face->d->id)   );
	return edgeBesides(a[0], a[1]);
}

			
void gridsOfEdge(char edge, char grids[2][2])
{
	int f, count = 0;
	for(f = 0; f < 6; f ++)
	{
		if(faceGridOnEdge[f][edge] >= 0)
		{
			grids[count][1] = faceGridOnEdge[f][edge]; // grid
			grids[count][0] = f; // face
			count ++;
			if(count == 2)
				return;
		}
	}
}
	
int nextFaceAroundEdge(int face, int edge)
{
	if(face < 0 || (edge>>2 + 2)%3 == (face < 3 ? face : 5 - face))
		return (edge & 1 ? 5 - (edge>>2 + 1)%3 : (edge>>2 + 1)%3);
	if((edge>>2 + 1)%3 == (face < 3 ? face : 5 - face))
		return (edge & 2 ? 5 - (edge>>2 + 2)%3 : (edge>>2 + 2)%3);
	return -1;
}

void edgeCubiePerms(Cube *cube, char *perms, char *oriens)
{
	int edgePos, axis, i, face_i;
	int color[2]; // generalized color
	for(edgePos = 0; edgePos < 12; edgePos++)
	{
		axis = edgePos >> 2;
		for(i = 1; i <= 2; i ++)
		{
			face_i = (edgePos&i ? 5-(axis+i)%3 : (axis+i)%3);
			color[i-1] = cube->face[face_i].grid[0][faceGridOnEdge[ face_i ][ edgePos ]];
		}
		
		i = (axis == 2 ? 0 : 1);
		if(color[i] == 0 || color[i] == 5 || 
			((color[i] == 2 || color[i] == 3) && color[1-i] != 0 && color[1-i] != 5))
		{
			oriens[edgePos] = 0;
		}
		else
		{
			oriens[edgePos] = 1;
		}
		perms[edgePos] = edgeBesides(color[0], color[1]);
	}
}


//corner: URF, FRD, UFL, LFD, UBR, RBD, ULB, BLD
// axis: U-D, R-L, F-B

		char faceGridOnCorner[6][8] = {
			{ 8,-1, 6,-1, 2,-1, 0,-1},
			{ 8, 2,-1,-1, 6, 0,-1,-1},
			{ 8, 6, 2, 0,-1,-1,-1,-1},
			{-1,-1,-1,-1, 0, 6, 2, 8},
			{-1,-1, 0, 2,-1,-1, 6, 8},
			{-1, 0,-1, 6,-1, 2,-1, 8},
		};
	

int isEvenSumCorner(int corner) // even: 0,3,5,6; odd: 1,2,4,7
{
	return (((corner&1) + ((corner>>1)&1) + ((corner>>2)&1) )%2 == 0);
}

int cornerBesides(int f1, int f2, int f3)
{
	/*
	int c;
	for(c = 0; c < 8; c ++)
	{
		if(faceGridOnCorner[f1][c] > 0 && faceGridOnCorner[f2][c] > 0 && faceGridOnCorner[f3][c] > 0)
			return c;
	}
	*/
	int corner = 0;
	if(f1 >= 3)
		corner |= (1 << 5 - f1); // corner.bit[axis] = 1
	if(f2 >= 3)
		corner |= (1 << 5 - f2); // corner.bit[axis] = 1
	if(f3 >= 3)
		corner |= (1 << 5 - f3); // corner.bit[axis] = 1
	return corner;
}

int cornerByFaceGrid(Face *face, int grid)
{
	int a[3] = {};
	//a[0] = face->id;
	//a[1] = (grid % 6 ? face->r->id : face->l->id);
	//a[2] = (grid / 6 ? face->d->id : face->u->id);
	return cornerBesides(a[0], a[1], a[2]);
}

void gridsOfCorner(char corner, char grids[3][2])
{
	int f, count = 0;
	for(f = 0; f < 6; f ++)
	{
		if(faceGridOnCorner[f][corner] >= 0)
		{
			grids[count][1] = faceGridOnCorner[f][corner]; // grid
			grids[count][0] = f;// face
			count ++;
			if(count == 3)
				return;
		}
	}
}
int nextFaceAroundCorner(int face, int corner)
{
	if(face < 0)
		return (corner & 1 ? 5 : 0);
	int axis = (face < 3 ? face : 5 - face);
	int nextAxis;
	if(((corner >> axis) & 1) == (face >= 3))
	{
		nextAxis = (axis + (isEvenSumCorner(corner) ? 1 : 2)) % 3; /* 0,3,5,6 +1(cw) 1,2,4,7 +2(ccw)*/
		return ((corner & (1<<nextAxis)) ? 5 - nextAxis : nextAxis);
	}
	else
	{
		return -1;
	}
}

int findCornerCubie(Cube *cube, int cubie)
{
	int color[3]; // generalized color
	color[0] = (cubie & 1 ? faceNameToIndex('D') : faceNameToIndex('U'));
	color[1] = (cubie & 2 ? faceNameToIndex('L') : faceNameToIndex('R'));
	color[2] = (cubie & 4 ? faceNameToIndex('B') : faceNameToIndex('F'));
	if( !isEvenSumCorner(cubie)) // 1,2,4,7
	{
		int tmp = color[1];
		color[1] = color[2];
		color[2] = tmp;
	}
	
	
}

void cornerCubiePerms(Cube *cube, char *perms, char *oriens)
{
	int corner, axis, face_i;
	int color[3]; // generalized color
	for(corner = 0; corner < 8; corner++)
	{
		for(axis = 0; axis < 3; axis ++)
		{
			face_i = (corner&(1<<axis) ? 5-axis : axis);
			color[axis] = cube->face[face_i].grid[0][faceGridOnCorner[ face_i ][ corner ]];
			if(color[axis] == 0 || color[axis] == 5)
			{
				oriens[corner] = (axis == 0 ? 0 : (isEvenSumCorner(corner) ? axis : 3-axis) );
			}
		}
		perms[corner] = cornerBesides(color[0], color[1], color[2]);
	}
}


void rotateFace(Cube *cube, Face *face, char times)
{
	char *chains[5][4];
	get5chain(face, chains);
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


void rotate(Cube *cube, char step)
{
	rotateFace(cube, &(cube->face[step/3]), step%3+1);
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

void getStepName(int step, char name[2])
{
	name[0] = faceName[step/3];
	name[1] = (step % 3 == 1 ? '2' : (step % 3 == 2 ? '\'' : 0));
}

void printSteps(int stepN, int *steps)
{
	int i;
	for(i = 0; i < stepN; i ++)
	{
		if(steps[i] < 0)
		{
			printf("%d(?) ",steps[i]);
		}
		else
		{
			char snamebuf[3] = {0,0,0};
			getStepName(steps[i], snamebuf);
			printf("%s ", snamebuf);
		}
	}
	printf("\n");
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

void valToPerm(int n, int value, char perm[n])
{
    int i, j;
	int v = value;
	int count = 0;
	char chosen[n];
    for( i = 0; i < n ; i ++)
		chosen[i] = 0;
    for( i = n; i > 0 ; i --)
    {//printf("## i=%d ",i);
		count = v % i;//printf("count=%d ",count);
		v = v / i;//printf("v->%d ",v);
		
		for(j = 0; j < n; j ++)
		{//printf("j=%d count=%d ",j,count);
			if(chosen[j])
			{//printf("%d is chosen ", j);
				continue;
			}
			else if(count != 0)
			{//printf("count down ");
				count --;
			}
			else
			{
				chosen[j] = 1;
				perm[n-i] = j;//printf("set perm[%d]=%d \n",i-1,j);
				break;
			}
		}
    }
    return;
}

int permToVal(int n, char perm[n])
{
    int i, j, m = n;
	int v = 0;
	int count = 0;
    for( i = n-2; i >= 0 ; i --)
    {
		count = 0;
		for(j = i + 1; j < n; j ++)
		{
			if(perm[j] < perm[i])
				count ++;
		}
		v = v * (n-i) + count;
    }
    return v;
}
void valToPerm2(int n, int value, char permuted[n])
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

int permToVal2(int n, char perm[n])
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

//char distanceP1[2187][495][1024];
char *distanceP1;
short coMove[2187][18];
short eoMove[2048][18];
short UDSliceMove[495][18];



unsigned short cpMove[40320][18];
unsigned short epMove[40320][18];
char UD2Move[24][18];


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

long long cmprPhase2Crd(int cp, int ep, int UD2)
{
	//IFON( PLN PV(cp) PV(ep) PV(UD2)
	//PV((((long long)cp * 40320 + (long long)ep ) * 24 + (long long)UD2)) )
	return (((long long)cp * 40320 + (long long)ep ) * 24 + (long long)UD2);
}
void extrPhase2Crd(long long v, int *cp, int *ep, int *UD2)
{
	*UD2 = v % 24;
	v /= 24;
	*ep = v % 40320;
	v /= 40320;
	*cp = v;
	return;
}

char readDistanceP1(short co, short eo, short UD)
{//printf("readDistanceP1 %d %d %d\n",co,eo,UD);printf("%d,",(((co * 495 + UD) * 1024) + eo/2));printf(" -> %d",distanceP1[ (((co * 495 + UD) * 1024) + eo/2)]);
	char d;
	//char d = distanceP1[ (((co * 495 + UD) * 1024) + eo/2)];
	if(eo % 2 == 1)
	{
		//d = ((distanceP1[co][UD][eo/2]>>4) & 15);
		d = (((distanceP1[(co*495 + UD)*1024 + eo/2])>>4) & 15);
	}
	else
	{
		//d = ((distanceP1[co][UD][eo/2]) & 15);
		d = ((distanceP1[(co*495 + UD)*1024 + eo/2]) & 15);
	}
	if(d == 15)
	{
		return -1;
	}
	else
	{
		return d;
	}
}
void setDistanceP1(short co, short eo, short UD, char value)
{
	//assume value <= 12
	char *p = distanceP1  + ((co*495 + UD)*1024 + eo/2);
	if(eo % 2 == 1)
	{
		*p = ((value & 15 )<<4) | ((*p) & 15);
		//distanceP1[ (((co * 495 + UD) * 1024) + eo/2)] = ((value & 15 )<<4) | (distanceP1[ (((co * 495 + UD) * 1024) + eo/2)] & 15);
	}
	else
	{
		*p = ((*p) & (15<<4)) | (value & 15);
		//distanceP1[ (((co * 495 + UD) * 1024) + eo/2)] = (distanceP1[ (((co * 495 + UD) * 1024) + eo/2)] & (15<<4)) | (value & 15);
	}
}

int generateP1MoveTable(void)
{printf("Generate Corner Orientations Move table....");
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
	}printf("done\n");
printf("Generate Edge Orientations Move table....");	
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
	}printf("done\n");
printf("Generate UD slices position Move table....");
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
	}printf("done\n");
	
	
	return 0;
	
}

int generateP2MoveTable(void)
{printf("Generate Corner Permutation Move table....");
	int i, j, k;
	int tmp;
	char cubeCp[8];
	for(i = 0; i < 40320; i ++)
	{//printf("i=%d\n",i);
		//PL(king)PCPV(i)
		valToPerm(8, i, cubeCp);//int t; for(t = 0; t < 8; t ++){printf("%d ", cubeCp[t]);}printf("\n",i);
		for(j = 0; j < 6; j ++)
		{
			for(k = 0; k <= 3; k ++)
			{//PV(j)PV(k)PL()
				switch(j)
				{
					case 0:tmp=cubeCp[0];cubeCp[0]=cubeCp[4];cubeCp[4]=cubeCp[6];cubeCp[6]=cubeCp[2];cubeCp[2]=tmp;break;
					case 1:tmp=cubeCp[4];cubeCp[4]=cubeCp[0];cubeCp[0]=cubeCp[1];cubeCp[1]=cubeCp[5];cubeCp[5]=tmp;break;
					case 2:tmp=cubeCp[0];cubeCp[0]=cubeCp[2];cubeCp[2]=cubeCp[3];cubeCp[3]=cubeCp[1];cubeCp[1]=tmp;break;
					case 3:tmp=cubeCp[6];cubeCp[6]=cubeCp[4];cubeCp[4]=cubeCp[5];cubeCp[5]=cubeCp[7];cubeCp[7]=tmp;break;
					case 4:tmp=cubeCp[2];cubeCp[2]=cubeCp[6];cubeCp[6]=cubeCp[7];cubeCp[7]=cubeCp[3];cubeCp[3]=tmp;break;
					case 5:tmp=cubeCp[1];cubeCp[1]=cubeCp[3];cubeCp[3]=cubeCp[7];cubeCp[7]=cubeCp[5];cubeCp[5]=tmp;break;
				}//printf("move=%d*3+%d  =>  ",j,k);int t; for(t = 0; t < 8; t ++){printf("%d ", cubeCp[t]);}printf("\n",i);
				//P(cp)PA(cubeCp,8)PCPV(permToVal(8, cubeCp))HOLD
				if(k != 3)
				{
					cpMove[i][j*3+k] = permToVal(8, cubeCp);
					//printf("cpMove[i][j*3+k] = %d\n",cpMove[i][j*3+k]);
				}
			}
		}//scanf("%[\n]");
	}printf("done\n");
printf("Generate 8 Edge Permutation Move table....");	
	char cubeEp[8];
	for(i = 0; i < 40320; i ++)
	{
		valToPerm(8, i, cubeEp);
		for(j = 0; j < 6; j += 5)
		{
			for(k = 0; k <= 3; k ++)
			{
				switch(j)
				{
					case 0:tmp=cubeEp[1];cubeEp[1]=cubeEp[6];cubeEp[6]=cubeEp[0];cubeEp[0]=cubeEp[4];cubeEp[4]=tmp;break;
					case 5:tmp=cubeEp[2];cubeEp[2]=cubeEp[7];cubeEp[7]=cubeEp[3];cubeEp[3]=cubeEp[5];cubeEp[5]=tmp;break;
				}
				if(k != 3)
					epMove[i][j*3+k] = permToVal(8, cubeEp);
			}
		}
		for(j = 1; j <= 4; j ++)
		{
			for(k = 0; k < 2; k ++)
			{
				switch(j)
				{
					case 1:tmp=cubeEp[4];cubeEp[4]=cubeEp[5];cubeEp[5]=tmp;break;
					case 2:tmp=cubeEp[0];cubeEp[0]=cubeEp[2];cubeEp[2]=tmp;break;
					case 3:tmp=cubeEp[1];cubeEp[1]=cubeEp[3];cubeEp[3]=tmp;break;
					case 4:tmp=cubeEp[6];cubeEp[6]=cubeEp[7];cubeEp[7]=tmp;break;
				}
				if(k == 0)
					epMove[i][j*3+1] = permToVal(8, cubeEp);
			}
			epMove[i][j*3] = -1;
			epMove[i][j*3+2] = -1;
		}
	}printf("done\n");
printf("Generate UD 4 Edge Permutation Move table....");
	char cubeUD2p[4];
	for(i = 0; i < 24; i ++)
	{
		valToPerm(4, i, cubeUD2p);
		for(j = 0; j < 6; j ++)
		{
			for(k = 0; k < 2; k ++)
			{
				switch(j)
				{
					case 0:break;
					case 1:tmp=cubeUD2p[0];cubeUD2p[0]=cubeUD2p[2];cubeUD2p[2]=tmp;break;
					case 2:tmp=cubeUD2p[1];cubeUD2p[1]=cubeUD2p[0];cubeUD2p[0]=tmp;break;
					case 3:tmp=cubeUD2p[2];cubeUD2p[2]=cubeUD2p[3];cubeUD2p[3]=tmp;break;
					case 4:tmp=cubeUD2p[3];cubeUD2p[3]=cubeUD2p[1];cubeUD2p[1]=tmp;break;
					case 5:break;
				}
				if(k == 0)
					UD2Move[i][j*3+1] = permToVal(4, cubeUD2p);
			}
			if(j == 5 || j == 0)
			{
				UD2Move[i][j*3] = i;
				UD2Move[i][j*3+1] = i;
				UD2Move[i][j*3+2] = i;
			}
			else
			{
				UD2Move[i][j*3] = -1;
				UD2Move[i][j*3+2] = -1;
			}
		}
	}printf("done\n");
	
	
	return 0;
	
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

typedef struct bucketItem
{
	int cpep;
	short UD2;
	char depth;
	char isLast;
} BucketItem ;

BucketItem **buckets;
//int *bucketSize;
long long bucketN = 100000000;
long long countnode = 0;int test = 0;

int findRecorded(int cp,  int ep, int UD2)
{//IFON(PLN)
	long long v = cmprPhase2Crd(cp, ep, UD2);//if(test){printf("%d %d %d=>%lld ", cp,ep,UD2,v);PPV(cp,ep,UD2);scanf("%*s");}
	unsigned long long b = (unsigned long long)(v % (unsigned long long)bucketN);// IFON(PLN;PV(b);PV(v)PV(bucketN)PV((unsigned long long)(v % (unsigned long long)bucketN)))
	BucketItem *bu = buckets[b];//IFON(PLN)
	if(bu == NULL)
	{
		return -1;
	}
	int i;
	int cpep = cp * 40320 + ep;//IFON(PLN)
	for(i = 0; i < 1000000/*?*/; i ++)
	{//IFON(PLN)
		if(bu[i].cpep == cpep && bu[i].UD2 == UD2)
		{//IFON(PLN)
			return bu[i].depth;
		}//IFON(PLN)
		if(bu[i].isLast == 1)
		{//IFON(PLN)
			break;
		}
	}//IFON(PLN)
	return -1;
}
void record(int cp ,int ep, int UD2, char depth)
{//printf("%d %d %d ", cp,ep,UD2);scanf("%*s");
	long long v = cmprPhase2Crd(cp, ep, UD2);
	long long b = v % bucketN;
	BucketItem *bu = buckets[b];
	if(bu == NULL)
	{
		//bucketSize[b] = 4;
		bu = (BucketItem *)malloc(sizeof(BucketItem) * 4);
		if(bu == NULL)
		{
			printf("no memory, cannot init a bucket, (#nodes = %d)\n", countnode);
			scanf("%*s");
			exit(-1);
		}
		memset(bu, -1, sizeof(BucketItem) * 4);
		bu[3].isLast = 1;
		buckets[b] = bu;
	}
	int i;//int test2=0;if(cp == 33720 && ep == 26880 && UD2 == 16){printf("FOCUS!!!\n");test2=0;}
	for(i = 0; i < 1000000/*?*/; i ++)
	{
		if(bu[i].isLast == 1)
		{//if(1|test){printf("FULL!!!\n");}
			BucketItem *p = realloc(bu, sizeof(BucketItem) * (i+1)*2);
			if(p == NULL)
			{
				printf("no memory, cannot expand a bucket, (#nodes = %d)\n", countnode);
				scanf("%*s");
				i--;continue; //retry
			}
			memset((p+(i+1)), -1, sizeof(BucketItem) * (i+1));
			p[(i+1)*2-1].isLast = 1;
			bu[i].isLast = 0;
			buckets[b] = p;//if(test){printf("STILL!!!\n");}
		}
		if(bu[i].depth == -1)
		{//if(1|test){printf("RECORDED!!!\n");}
			bu[i].depth = depth;
			bu[i].cpep = cp * 40320 + ep;
			bu[i].UD2 = UD2;
			
	countnode ++ ;
			return;
		}
	}//if(1|test){printf("WHY!!!\n");test=0;}
	return;
}


void phase2SearchDepth8(void)
{
	
	printf("Backward Search nodes in 8 depth....\n");
	
	buckets = (BucketItem **)malloc(sizeof(BucketItem *) * bucketN);
	if(buckets == NULL)
	{
		printf("no memory, cannot init bucket chain, (#nodes = %d)\n", countnode);
		scanf("%*s");
		exit(-1);//retry
	}
	//bucketSize = (int *)malloc(sizeof(int) * bucketN);
	//memset(bucketSize, 0, sizeof(int) * bucketN);
	
	int cpV = 0, epV = 0, UD2V = 0;
	int ncpV = 0, nepV = 0, nUD2V = 0;
	char myDist;
	//setDistance(cpV, epV, UD2V, 0);
	record(cpV, epV, UD2V, 0);
	
	int m;
	int depthLimit = 0;
	int cpStack[10];
	int epStack[10];
	int UD2Stack[10];
	char stepStack[10];
	int stackH = 0;
	
	int nDist;
	
	// long long foundNewNode = 0; // ?
	
	for(depthLimit = 0; depthLimit < 9; depthLimit ++)
	{
		// foundNewNode = 0; // ?
		//DFID
		cpStack[0] = 0;
		epStack[0] = 0;
		UD2Stack[0] = 0;
		stepStack[0] = -1;
		stackH = 0;
		printf("depth %d iteration....",depthLimit);
		time_t begin, end;
		time(&begin);
		
		while(stackH >= 0)
		{
			cpV = cpStack[ stackH ];
			epV = epStack[ stackH ];
			UD2V = UD2Stack[ stackH ];
			m = stepStack[ stackH ];
			
			//myDist = findRecorded(cpV, epV, UD2V);
			myDist = stackH;
			
			//printf("Being node (%d, %d, %d) with depth %d.\n",cpV ,epV, UDslV,myDist);
			//printf("last move: %d.\n",m);
			
			if(depthLimit == stackH)
			{//printf("reach depth limit. Back to upper\n");
				stackH --;
			}
			else
			{
				do
				{
					m ++;
				}
				while(		(stackH >= 1 && (stepStack[ stackH - 1] ) / 3 == m/3)
						||	(stackH >= 2 && (stepStack[ stackH - 2] ) / 3 == m/3 
								&& (5 - ((stepStack[ stackH - 1] ) / 3)) == (m/3)	)
						||	(m / 3 != 0 && m/3 != 5 && m%3 != 1)
					);
				stepStack[ stackH ] = m;
				
				if(m < 18)
				{//printf("take a move: %d.\n",m);
					ncpV = cpMove[cpV][m];
					nepV = epMove[epV][m];
					nUD2V = UD2Move[UD2V][m];
					
					nDist = findRecorded(ncpV ,nepV, nUD2V);
					//ndist = readDistance(ncoV ,neoV, nUDslV);
					//printf("lead to a node (%d, %d, %d) with depth %d.\n",ncpV ,nepV, nUD2V, nDist);
					
					if(( nDist != -1 && nDist <= myDist ))
					{
						continue;
					}
					else
					{
					
						if((myDist == depthLimit - 1 )&& nDist != -1)
						{
							//continue;
						}
						if(nDist == -1)
						{//printf("It's a new node\n");
							// {char tt[8];
							// valToPerm(8,ncpV,tt);printf("cp\n%d  %d\n %d%d\n %d%d\n%d  %d\n",tt[6],tt[4],tt[7],tt[5],tt[3],tt[1],tt[2],tt[0]);
							// valToPerm(8,nepV,tt);printf("ep\n    %2d\n    %2d\n%2d%2d  %2d%2d\n    %2d\n    %2d\n",tt[1]+4,tt[3]+4,tt[6]+4,tt[7]+4,tt[5]+4,tt[4]+4,tt[2]+4,tt[0]+4);
							// valToPerm(4,nUD2V,tt);printf("UD2\n%d%d\n%d%d\n",tt[3],tt[2],tt[1],tt[0]);}
							//setDistance(coV ,eoV, UDslV, depthLimit+1);
							record(ncpV ,nepV, nUD2V, myDist+1); 
							// foundNewNode++;
							// if((foundNewNode & 4194303) == 0)
							// {
								// if((foundNewNode & 16777215) == 0)
									// printf("%d nodes are found so far.\n",foundNewNode );
								// else
									// printf(".");
							// }
						}
						stackH ++;
						cpStack[ stackH ] = ncpV;
						epStack[ stackH ] = nepV;
						UD2Stack[ stackH ] = nUD2V;
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
		printf("finish. spend time: %f sec\n",depthLimit, difftime(end, begin));//char ac; scanf("%c", &ac);
		//printf("Total: %d nodes are found so far.\n",countnode);
		// if(depthLimit > 0 && (foundNewNode == 0))
		// {
			// printf("all nodes are found. \n Prune table done. (deepest=%d)\n", depthLimit-1);
			// break;
		// }
	}
	/*
	char myDist = readDistanceP1(co, eo, UD);
	if(myDist <= 0)
		return myDist;
	
	short nco, neo, nUD, nDist;
	int i, j;
	for(i = 0; i < 6; i ++)
	{
		for(j = 0; j < 3; j ++)
		{
			
			nco = coMove[co][i*3+j];
			neo = eoMove[eo][i*3+j];
			nUD = UDSliceMove[UD][i*3+j];
			
			nDist = readDistanceP1(nco ,neo, nUD);
			
			if(nDist < myDist)
			{
				*step = i*3+j;
				int stepN = phase1(nco, neo, nUD, step + 1);
				if(stepN >= 0)
					return 1 + stepN;
			}
		}
	}
	return -2;
	if(isSolved(cube))
		return 0;
	if(stepLimit <= 0)
		return -1;
	int i, j;
	for(i = 0; i < 6; i ++)
	{
		if(i == (step[-1] / 3)) // skip 1/6
			continue;
		if(i == (step[-2] / 3) && ((step[-1] / 3) + i) == 5) // skip 1/36
			continue;
		for(j = 0; j < 3; j ++)
		{
			if(i != 5 && i != 0 && j != 1)
				continue;
			
			int curStep = i*3+j;
			rotate(cube, curStep);
			*step = curStep;
			
			int subSol = phase2FindSolution(cube, stepLimit-1, step+1);
			if(subSol >= 0)
			{
				return 1 + subSol;
			}
			// retreat
			rotate(cube, curStep);
			*step = -1;
		}
	}
	return -1;
	*/
}

int phase2DFSforPrune(int cp, int ep, int UD2, int stepLimit, int *step)
{//printf("next ");test=1;
	if(cp >= 40320 || ep >= 40320 || UD2 >= 24)
	{
		printf("Illegal states! cp %d ep %d UD2 %d\n", cp, ep, UD2);
		return -1;
	}
	int myDist = findRecorded(cp,ep,UD2);
	//printf("dist=%d,",myDist);
	if(myDist >= 0)
	{
		printf("A Node says %d Step Left!!\n",myDist);
		//PV(cp)PV(ep)PV(UD2)
		//PPV(cp,ep,UD2);
		return 0;
	}
	if(stepLimit <= 0)
	{//printf("prev");
		return -1;
	}
	int i, j;
	for(i = 0; i < 6; i ++)
	{
		if(i == (step[-1] / 3)) // skip 1/6
			continue;
		if(i == (step[-2] / 3) && ((step[-1] / 3) + i) == 5) // skip 1/36
			continue;
		for(j = 0; j < 3; j ++)
		{
			if(i != 5 && i != 0 && j != 1)
				continue;
			//char sn[3]={};getStepName(i*3+j,sn);printf("%s ",sn);scanf("%*s");
			int m = i*3+j;
			*step = m;
			
			int stepN = phase2DFSforPrune(cpMove[cp][m], epMove[ep][m], UD2Move[UD2][m], stepLimit-1, step+1);
			if(stepN >= 0)
			{
				return 1 + stepN;
			}
			// retreat
			*step = -1;
		}
	}//printf("prev ");
	return -1;
}
int phase2FollowPrune(int cp, int ep, int UD2, int *step)
{//PLN PV(cp )PV(ep )PV(UD2 )
	int myDist = findRecorded(cp, ep, UD2);//PLN PV(myDist )
	if(myDist <= 0)
	{//PLN PV(myDist )
		// if(myDist < 0)
		// {
			// printf("i'm beyond 9 steps\n");
		// }
		// else if(myDist==0)
		// {
			// printf("Reach the answer ! ! !\n");
			// PPV(cp,ep,UD2)
		// }
		return myDist;
	}
	//PLN
	int ncp, nep, nUD2, nDist;
	int i, j;
	for(i = 0; i < 6; i ++)
	{
		//if(i == (step[-1] / 3)) // skip 1/6
		//	continue;
		//if(i == (step[-2] / 3) && ((step[-1] / 3) + i) == 5) // skip 1/36
		//	continue;
		for(j = 0; j < 3; j ++)
		{
			if(i != 5 && i != 0 && j != 1)
				continue;
			
			ncp = (unsigned short)cpMove[cp][i*3+j];
			nep = (unsigned short)epMove[ep][i*3+j];
			nUD2 = (unsigned short)UD2Move[UD2][i*3+j];
			
			nDist = findRecorded(ncp, nep, nUD2);
			
			if(nDist < myDist)
			{//PLN PV(cp )PV(ep )PV(UD2 )
				*step = i*3+j;//PV(*step) PL(GO DOWN)
				int stepN = phase2FollowPrune(ncp, nep, nUD2, step + 1);//P(BACKED)PV(*step)
				if(stepN >= 0)
				{
					//PL(SUCCESS)
					return (1 + stepN);
				}
				*step = -1;//PLN
			}
		}
	}//printf("FAILED");
	return -2;
}
int phase2(Cube *cube, int *steps)
{
	generateP2MoveTable();
	
	phase2SearchDepth8();
	
	
	char cp[8], co[8];
	char epA[12], eo[12];
	char UD[12];
	
	cornerCubiePerms(cube, cp, co);
	edgeCubiePerms(cube, epA, eo);
	
	char ep[8], UD2[4];
	
	int i;
	for(i = 0; i < 12; i ++)
		if(i < 4)
			UD2[i] = epA[i];
		else
			ep[i - 4] = (epA[i] - 4);
	
	printf("cp [ ");  for(i = 0; i < 8; i ++){printf("%d ",cp[i]);}  printf("]\n");
	printf("ep [ ");  for(i = 0; i < 8; i ++){printf("%d ",ep[i]);}  printf("]\n");
	printf("UD2 [ ");  for(i = 0; i < 4; i ++){printf("%d ",UD2[i]);}  printf("]\n");
	
	int cpV = permToVal(8, cp);
	int epV = permToVal(8, ep);
	int UD2V = permToVal(4, UD2);
	
	printf("coord: co %d  eo %d  UD %d  \n", cpV, epV, UD2V);
	
	printf("Try to found nodes with known distance....\n");
	
	int depthLimit, stepN1;
	for(depthLimit = 0; depthLimit < 20; depthLimit ++)
	{//DFID
		printf("trying %d steps...", depthLimit);
		stepN1 = phase2DFSforPrune(cpV, epV, UD2V, depthLimit, steps);
		if(stepN1 >= 0)
		{
			printf("Found a node 9 step left, in 6 step!!\n", stepN1);
			break;
		}
		printf("finished.\n", depthLimit);
	}
	
	// printf("BEFORE:\n");
	// printCube(cube);
	// printf("AFTER: ");
	printf("steps by this stage: ");
	printSteps(stepN1, steps);
	for(i = 0; i < stepN1; i++)
	{
		rotate(cube, steps[i]);
	}
	
	// printCube(cube);
	
	// phase2-2
	
	cornerCubiePerms(cube, cp, co);
	edgeCubiePerms(cube, epA, eo);
	
	
	for(i = 0; i < 12; i ++)
		if(i < 4)
			UD2[i] = epA[i];
		else
			ep[i - 4] = (epA[i] - 4);
	
	printf("cp [ ");  for(i = 0; i < 8; i ++){printf("%d ",cp[i]);}  printf("]\n");
	printf("ep [ ");  for(i = 0; i < 8; i ++){printf("%d ",ep[i]);}  printf("]\n");
	printf("UD2 [ ");  for(i = 0; i < 4; i ++){printf("%d ",UD2[i]);}  printf("]\n");
	
	cpV = permToVal(8, cp);
	epV = permToVal(8, ep);
	UD2V = permToVal(4, UD2);
	
	//PPV(cpV,epV,UD2V)
	//PV(cpV)PV(epV)PV(UD2V)
	//PLN
	printf("follow the prune....");
	//PLN
	int stepN2;
	stepN2 = phase2FollowPrune(cpV, epV, UD2V, steps+stepN1);
	if(stepN2 >= 0)
	{
		printf("done!\n", stepN2);
	}
	// printf("BEFORE:\n");
	// printCube(cube);
	// printf("AFTER: ");
	printf("steps by this stage: ");
	printSteps(stepN2, steps+stepN1);
	for(i = 0; i < stepN2; i++)
	{
		rotate(cube, (steps+stepN1)[i]);
	}
	// printCube(cube);
	
	return stepN1+stepN2;
}

int phase1(short co, short eo, short UD, int *step)
{
	char myDist = readDistanceP1(co, eo, UD);
	if(myDist <= 0)
		return myDist;
	
	short nco, neo, nUD, nDist;
	int i, j;
	for(i = 0; i < 6; i ++)
	{
		for(j = 0; j < 3; j ++)
		{
			
			nco = coMove[co][i*3+j];
			neo = eoMove[eo][i*3+j];
			nUD = UDSliceMove[UD][i*3+j];
			
			nDist = readDistanceP1(nco ,neo, nUD);
			
			if(nDist < myDist)
			{
				*step = i*3+j;
				int stepN = phase1(nco, neo, nUD, step + 1);
				if(stepN >= 0)
					return 1 + stepN;
			}
		}
	}
	return -2;
}

int findSolution(Cube *cube, int stepLimit, int *step, int lastRotatedFace)
{/*
	if(isSolved(cube))
		return 1;
	if(stepLimit <= 0)
		return 0;
	int i, j;
	for(i = 0; i < 6; i ++)
	{
		if(i == lastRotatedFace) // skip 1/6
			continue;
		if(i == (step[-2] / 3) && ((step[-1] / 3) + i) == 5) // skip 1/36
			continue;
		for(j = 1; j <= 3; j ++)
		{
			rotate(cube, faceName[i], j);
			*step = i*4+j;
			if(findSolution(cube, stepLimit-1, step+1, i))
			{
				return 1;
			}
			rotate(cube, faceName[i], 4-j);
			*step = -1;
		}
	}*/
	return 0;
}

int facecolor[6] = {-1,-1,-1,-1,-1,-1};
void generalizeColor(Cube *cube)
{
	int i,j,k,c;
	for(i = 0; i < 6; i ++)
		facecolor[i] = cube->face[i].grid[1][1];
	
	for(i = 0; i < 6; i ++)
		for(j = 0; j < 3; j ++)
			for(k = 0; k < 3; k ++)
				for(c = 0; c < 6; c ++)
					if(cube->face[i].grid[j][k] == facecolor[c])
						cube->face[i].grid[j][k] = c;
}


int main(void)
{
	generateP1MoveTable();
	
	int check;
	
	distanceP1 = (char *)malloc(sizeof(char) * 2187 * 495 * 1024);
	
	printf("Read prune table....");
	
	FILE *fp = fopen("p1_prune_table", "rb");
	check = fread(distanceP1, sizeof(char), 2187 * 495 * 1024, fp);
	fclose(fp);
	if(!check)
	{
		printf("CANNOT find prune table, use table.exe to generate it first.\n");
		return -1;
	}
	
	printf("done.\n");
	
	
	printf("Please input in the order , OR USE THE magicCubeGenerate.exe \n\
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
				scanf("%d", &(cube->face[i].grid[j][k]));
			}
		}
	}
	
	generalizeColor(cube);
	
	printf("Recoloring....");	
	printCube(cube);
	
	printf("Solving...\n");
	
	time_t begin, end;
	time(&begin);
	
	int stepBuf[40] = {-3, -3};
	int *steps = stepBuf+2;
	int stepN = 0;
	int retStep;
	
	printf("Phase 1....\n");
	
	char cp[8], co[8];
	char ep[12], eo[12];
	char UD[12];
	
	cornerCubiePerms(cube, cp, co);
	edgeCubiePerms(cube, ep, eo);
	
	for(i = 0; i < 12; i ++)
		UD[i] = (ep[i] <= 3 ? 1 : 0);
	
	printf("co [ ");  for(i = 0; i < 8; i ++){printf("%d ",co[i]);}  printf("]\n");
	printf("eo [ ");  for(i = 0; i < 12; i ++){printf("%d ",eo[i]);}  printf("]\n");
	printf("UD [ ");  for(i = 0; i < 12; i ++){printf("%d ",UD[i]);}  printf("]\n");
	
	short coV = cOrienToVal(co);
	short eoV = eOrienToVal(eo);
	short UDV = UDslicePermToVal(UD);
	
	printf("coord: co %d  eo %d  UD %d  \n", coV, eoV, UDV);
	
	retStep = phase1(coV, eoV, UDV, steps);
	printf("Phase 1 done in %d steps: ", retStep);
	if(retStep < 0)
		printf("FAILED\n");
	
	stepN += retStep;
	if(stepN >= 0)
	{
		printSteps(stepN, steps);
	}
	
	for(i = 0; i < stepN; i ++)
	{
		rotate(cube, steps[i]);
	}
	printCube(cube);
	
	free(distanceP1);
	
	if(retStep < 0)
		exit(0);
	
	
	
	printf("Phase 2....\n");
	
	retStep = phase2(cube, steps+stepN);
	printf("Phase 2 done in %d steps: \n", retStep);
	if(retStep < 0)
		printf("FAILED\n");	
	else
		printSteps(retStep, steps+stepN);
	stepN += retStep;	
	
	
	if(isSolved(cube))
	{
		printCube(cube);
		printf("total solved in %d step:\n", stepN);
		printSteps(stepN, steps);
		
	}
	else
	{
		printf("unsolved.\n");
	}
	
	time(&end);
	printf("spend time: %f sec\n", difftime(end, begin));
	
	return 0;
}