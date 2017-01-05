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

char uf[6] = {3,5,4,0,2,1};
char rf[6] = {1,2,0,4,5,3};
char df[6] = {2,0,1,5,3,4};
char lf[6] = {4,3,5,1,0,2};
char *nbf[4] = {uf,rf,df,lf};

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

void cOrienToVal(char orien[8])
{
    int i, m = 1, v = 0;

    for(i = 0; i < 7; i ++)
	{
		v += m * orien[i];
		m *= 3;
    }
    return;
}

void eOrienToVal(char orien[12])
{
    int i, m = 1, v = 0;

    for(i = 0; i < 11; i ++)
	{
		v += m * orien[i];
		m *= 2;
    }
    return;
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
	orien[7] = (sum%3 ? 0 : (3-sum%3));
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
	orien[11] = (sum%2 ? 0 : 1);
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


char reverseStep(char step)
{
	return ((step/3)*3 + (2 - step%3));
}
char mirrorStep(char *type, char step)
{
	int face = step/3;
	int rad = step%3;
	if(strncmp(type, "RL", 2) == 0)
	{
		if(face == 4)
			face = 1;
		else if(face == 1)
			face = 4;
	}
	else if(strncmp(type, "UD", 2) == 0)
	{
		if(face == 0)
			face = 5;
		else if(face == 5)
			face = 0;
	}
	else if(strncmp(type, "FB", 2) == 0)
	{
		if(face == 2)
			face = 3;
		else if(face == 3)
			face = 2;
	}
	return (face*3 + (2 - rad));
}
char rotateStep(char type, char rotateTimes, char step)
{
	int face = step/3;
	int rad = step%3;
	int (*transT)[6];
	int transX[6] = {3,1,0,5,4,2};
	int transY[6] = {0,2,4,1,3,5};
	int transZ[6] = {1,5,2,3,0,4};
	if(type == 'x') // R face
		transT = &transX;
	else if(type == 'y') // U face
		transT = &transY;
	else if(type == 'z') // F face
		transT = &transZ;
	int i;
	for(i = 0; i < rotateTimes; i ++)
		face = (*transT)[face];
	
	return (face*3 + rad);
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

void printCube3(Cube *cube)
{
		printf("%d      %d      %d\n",cube->face[0].grid[0][0],cube->face[0].grid[0][1],cube->face[0].grid[0][2]);
		
	    printf(" | =%d==%d==%d= |\n",cube->face[3].grid[0][2],cube->face[3].grid[0][1],cube->face[3].grid[0][0]);
	   printf(" |%d =%d=%d=%d= %d| \n",cube->face[4].grid[2][0],cube->face[3].grid[1][2],cube->face[3].grid[1][1],cube->face[3].grid[1][0],cube->face[1].grid[2][0]);
	   printf(" | %d =%d%d%d= %d | \n",cube->face[4].grid[2][1],cube->face[3].grid[2][2],cube->face[3].grid[2][1],cube->face[3].grid[2][0],cube->face[1].grid[1][0]);
	
	   printf(" |  %d %d%d%d %d  | \n",
	cube->face[4].grid[2][2],
	cube->face[5].grid[2][2],cube->face[5].grid[1][2],cube->face[5].grid[0][2],
	cube->face[1].grid[0][0]
	);
	printf("%d|%d%d%d %d%d%d %d%d%d|%d\n",cube->face[0].grid[1][0],
	cube->face[4].grid[1][0],cube->face[4].grid[1][1],cube->face[4].grid[1][2],
	cube->face[5].grid[2][1],cube->face[5].grid[1][1],cube->face[5].grid[0][1],
	cube->face[1].grid[0][1],cube->face[1].grid[1][1],cube->face[1].grid[2][1]
	,cube->face[0].grid[1][2]);
	  printf(" |  %d %d%d%d %d  | \n",
	cube->face[4].grid[0][2],
	cube->face[5].grid[2][0],cube->face[5].grid[1][0],cube->face[5].grid[0][0],
	cube->face[1].grid[0][2]
	);
	
	   printf(" | %d =%d%d%d= %d | \n",cube->face[4].grid[0][1],cube->face[2].grid[0][0],cube->face[2].grid[1][0],cube->face[2].grid[2][0],cube->face[1].grid[1][2]);
	   printf(" |%d =%d=%d=%d= %d| \n",cube->face[4].grid[0][0],cube->face[2].grid[0][1],cube->face[2].grid[1][1],cube->face[2].grid[2][1],cube->face[1].grid[2][2]);
	    printf(" | =%d==%d==%d= | \n",cube->face[2].grid[0][2],cube->face[2].grid[1][2],cube->face[2].grid[2][2]);
		
		printf("%d      %d      %d\n",cube->face[0].grid[2][0],cube->face[0].grid[2][1],cube->face[0].grid[2][2]);
	printf("\n");
}
void printCube2(Cube *cube)
{
		printf("....<U%d%d%d......\n",cube->face[0].grid[0][0],cube->face[0].grid[0][1],cube->face[0].grid[0][2]);
		
		printf(".     %d%d%d<B   .\n",cube->face[3].grid[0][2],cube->face[3].grid[0][1],cube->face[3].grid[0][0]);
		printf(".     %d%d%d R   .\n",cube->face[3].grid[1][2],cube->face[3].grid[1][1],cube->face[3].grid[1][0]);
		printf(".     %d%d%d v   .\n",cube->face[3].grid[2][2],cube->face[3].grid[2][1],cube->face[3].grid[2][0]);
	
	printf("%d %d%d%d %d%d%d %d%d%d %d\n",cube->face[0].grid[0][0],
	cube->face[4].grid[2][0],cube->face[4].grid[2][1],cube->face[4].grid[2][2],
	cube->face[5].grid[2][2],cube->face[5].grid[1][2],cube->face[5].grid[0][2],
	cube->face[1].grid[0][0],cube->face[1].grid[1][0],cube->face[1].grid[2][0]
	,cube->face[0].grid[0][2]);
	printf("%d %d%d%d %d%d%d^%d%d%d %d\n",cube->face[0].grid[1][0],
	cube->face[4].grid[1][0],cube->face[4].grid[1][1],cube->face[4].grid[1][2],
	cube->face[5].grid[2][1],cube->face[5].grid[1][1],cube->face[5].grid[0][1],
	cube->face[1].grid[0][1],cube->face[1].grid[1][1],cube->face[1].grid[2][1]
	,cube->face[0].grid[1][2]);
	printf("%d %d%d%d %d%d%dD%d%d%d %d\n",cube->face[0].grid[2][0],
	cube->face[4].grid[0][0],cube->face[4].grid[0][1],cube->face[4].grid[0][2],
	cube->face[5].grid[2][0],cube->face[5].grid[1][0],cube->face[5].grid[0][0],
	cube->face[1].grid[0][2],cube->face[1].grid[1][2],cube->face[1].grid[2][2]
	,cube->face[0].grid[2][2]);
	
		printf(". L> F%d%d%d     .\n",cube->face[2].grid[0][0],cube->face[2].grid[1][0],cube->face[2].grid[2][0]);
		printf(".    v%d%d%d     .\n",cube->face[2].grid[0][1],cube->face[2].grid[1][1],cube->face[2].grid[2][1]);
		printf(".     %d%d%d     .\n",cube->face[2].grid[0][2],cube->face[2].grid[1][2],cube->face[2].grid[2][2]);
		
		printf("''''''%d%d%d''''''\n",cube->face[0].grid[2][0],cube->face[0].grid[2][1],cube->face[0].grid[2][2]);
	printf("\n");
}
void printSteps(int stepN, int *steps)
{
	int i;
	for(i = 0; i < stepN; i ++)
	{
		if(steps[i] < 0)
		{
			continue;
		}
		printf("%c%s ", faceName[steps[i]/3], (steps[i] % 3 == 1 ? "2" : (steps[i] % 3 == 2 ? "'" : "")));
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

int LayerSolved(Cube *cube, int layer)
{
	int i, j, k;
	//Face *bottom = cube->face;
	int bottom = 0;
	if(layer == 1 || layer == 3)
	{
		if(layer == 3) bottom = 5;
			//bottom = bottom->u->d;
		
		for(j = 0; j < 3; j ++)
			for(k = 0; k < 3; k ++)
				if(cube->face[bottom].grid[j][k] != cube->face[bottom].grid[1][1])
					return 0;

		for(k = 0; k < 3; k ++)
			if(cube->face[bottom].u->grid[0][k] != cube->face[bottom].u->grid[1][1])
				return 0;
		for(k = 0; k < 3; k ++)
			if(cube->face[bottom].r->grid[2][k] != cube->face[bottom].r->grid[1][1])
				return 0;
		for(k = 0; k < 3; k ++)
			if(cube->face[bottom].d->grid[k][2] != cube->face[bottom].d->grid[1][1])
				return 0;
		for(k = 0; k < 3; k ++)
			if(cube->face[bottom].l->grid[k][0] != cube->face[bottom].l->grid[1][1])
				return 0;
		return 1;
	}
	else if(layer == 2)
	{
		for(k = 0; k < 3; k += 2)
			if(cube->face[bottom].u->grid[1][k] != cube->face[bottom].u->grid[1][1])
				return 0;
		for(k = 0; k < 3; k += 2)
			if(cube->face[bottom].r->grid[1][k] != cube->face[bottom].r->grid[1][1])
				return 0;
		for(k = 0; k < 3; k += 2)
			if(cube->face[bottom].d->grid[k][1] != cube->face[bottom].d->grid[1][1])
				return 0;
		for(k = 0; k < 3; k += 2)
			if(cube->face[bottom].l->grid[k][1] != cube->face[bottom].l->grid[1][1])
				return 0;
		return 1;
	}
	return 0;
}

int findSolution(Cube *cube, int stepLimit, int *step)
{
	if(isSolved(cube))
		return 1;
	if(stepLimit <= 0)
		return 0;
	int i, j;
	for(i = 0; i < 6; i ++)
	{
		for(j = 0; j < 3; j ++)
		{
			rotate(cube, i*3+j);
			if(findSolution(cube, stepLimit-1, step+1))
			{
				*step = i*3+j;
				return 1;
			}
			rotate(cube, i*3+(2-j));
		}
	}
	return 0;
}

char lateralFace[4] = {1,2,4,3};
char leftLateralFace[5] = {0,2,4,1,3};

int moveL1Edge(Cube *cube, int *steps, char top, char gs[2][2], char destLateralFace)
{
	int sc = 0;
	if(gs[top][0] == 0)
	{printf("A\n");
		steps[sc] = gs[1-top][0]*3+1; // flip 180
		rotate(cube, steps[sc]);
		sc ++;
		gs[top][0] = 5;
		gs[top][1] = (gs[top][1] > 4 ? gs[top][1] - 4 : gs[top][1] + 4);
		gs[1-top][1] = 8 - gs[1-top][1];
		//return sc + moveL1Edge(cube, steps+1, top, gs, destLateralFace);
	}printf("Ae\n");printCube3(cube);printSteps(1,steps+sc-1);
	// assert gs[top][0] != 0
	if(gs[top][0] == 5)
	{printf("B\n");
		char diff = (memchr(lateralFace, destLateralFace, 5) - memchr(lateralFace, gs[1-top][0], 5) + 4)%4;
		if(diff != 0)
		{printf("B2\n");
			// assert gs[top][0] == 5
			steps[sc] = /*gs[top][0]*/5*3+ 3-diff/*!!*/; // reduce diff
			rotate(cube, steps[sc]);
			sc ++;
			gs[1-top][0] = destLateralFace;
			// ignore other grid data
		}printf("B2e\n");printCube3(cube);printSteps(1,steps+sc-1);
		steps[sc] = gs[1-top][0]*3+1; // flip 180
		rotate(cube, steps[sc]);
		sc ++;printf("Be\n");printCube3(cube);printSteps(1,steps+sc-1);
		return sc; // done
	}
	else
	{printf("C\n");
		char diff;
		int recoverStep[2] = {-1,-1};
		//assert gs[top][0] != 0 && gs[top][0] != 5
		if(gs[1-top][0] == 5 || gs[1-top][0] == 0)
		{printf("C2\n");
			steps[sc] = gs[top][0]*3 + (gs[1-top][0] == 5 ? 0 : 2); // rotate to left lateral
			
				// RECOVER: 
				if((gs[1-top][0] == 5 )
					&& (cube->face[0].grid[0][(gs[1-top][1]+4)%8] == 0 )
					&& (cube->face[gs[top][0]].grid[0][8-gs[top][1]] == cube->face[gs[top][0]].grid[1][1]))
				{
					recoverStep[0] = reverseStep(steps[sc]);printf("for recover: ");printSteps(1,recoverStep);
				}
			
			rotate(cube, steps[sc]);
			sc ++;
			
			
			gridsOfEdge(edgeBesides(gs[top][0], leftLateralFace[gs[top][0]]), gs);
			if(cube->face[gs[top][0]].grid[0][gs[top][1]] != 0)
				top = 1-top; // change top
		}printf("C2e\n");printCube3(cube);printSteps(1,steps+sc-1);
		//assert gs[1-top][0] != 5 && gs[1-top][0] != 0
		
		if(gs[1-top][0] == cube->face[gs[1-top][0]].grid[0][gs[1-top][1]])
		{printf("C3\n");// turn white to top
			steps[sc] = gs[1-top][0]*3 + ((gs[1-top][0] == leftLateralFace[gs[top][0]]) ? 2 : 0);
			rotate(cube, steps[sc]);
			sc ++;printf("C3e\n");printCube3(cube);printSteps(1,steps+sc-1);
			
			if(recoverStep[0] >= 0)
			{printf("C recover 1\n");
				steps[sc] = recoverStep[0];
				rotate(cube, steps[sc]);
				sc ++;printCube3(cube);printSteps(1,steps+sc-1);
			}printf("C rec 1 e\n");printCube3(cube);printSteps(1,steps+sc-1);
		}
		else
		{
			// turn white to bottom
			steps[sc] = gs[1-top][0]*3 + ((gs[1-top][0] == leftLateralFace[gs[top][0]]) ? 0 : 2);
			rotate(cube, steps[sc]);
			sc ++;printf("C4e\n");printCube3(cube);printSteps(1,steps+sc-1);
			
			//RECOVER
			if(cube->face[gs[top][0]].grid[0][gs[top][1]] == 0 && 
					cube->face[gs[1-top][0]].grid[0][gs[1-top][1]] == cube->face[gs[1-top][0]].grid[1][1])
			{
				recoverStep[1] = reverseStep(steps[sc-1]);printf("for recover 2: ");printSteps(1,recoverStep+1);
			}
			
			
			//assert gs[top][0] == 5
			gs[top][0] = 5;
			
			diff = (memchr(lateralFace, destLateralFace, 5) - memchr(lateralFace, gs[1-top][0], 5) + 4)%4;
			if(diff != 0)
			{printf("C5\n");
				steps[sc] = /*gs[top][0]*/5*3+3-diff; // reduce diff
				rotate(cube, steps[sc]);
				sc ++;
				// ignore other grid data
			}printf("C5e\n");printCube3(cube);printSteps(1,steps+sc-1);
			
			if(recoverStep[1] >= 0)
			{printf("C recover 2\n");
				steps[sc] = recoverStep[1];
				rotate(cube, steps[sc]);
				sc ++;printCube3(cube);printSteps(1,steps+sc-1);
			}printf("C rec 2 e\n");
			if(recoverStep[0] >= 0)
			{printf("C recover 1\n");
				steps[sc] = recoverStep[0];
				rotate(cube, steps[sc]);
				sc ++;printCube3(cube);printSteps(1,steps+sc-1);
			}printf("C rec 1 e\n");
			
			steps[sc] = destLateralFace*3+1; // flip 180
			rotate(cube, steps[sc]);
			sc ++;printCube3(cube);printSteps(1,steps+sc-1);
		}
		return sc; // done
	}
	return 0;
}

char cornerGridSeq[4] = {0,2,8,6};
int moveL1Corner(Cube *cube, int *steps, char gs[3][2], char destCorner)
{
	int sc = 0;
	int diff;
	char testgs[3][2];printf("MC\n");
	if(gs[0][0] == 0)
	{printf("A\n");
		gridsOfCorner(destCorner, testgs);
		//assert testgs[0][0] == 0
		diff = (((char *)memchr(cornerGridSeq, testgs[0][1], 4) - (char *)memchr(cornerGridSeq, gs[0][1], 4)) + 4)%4;
		switch(diff)
		{printf("A1\n");
			case 0:printf("A10\n");
				return 0;
			case 1:printf("A11\n");
				steps[sc] = gs[1][0]*3+2; // X': turn white down and facing to left
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = 5*3+2; // D': turn white left
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = gs[1][0]*3; // X: go back
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = (5-gs[1][0])*3; // opX
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = 5*3; // D
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = (5-gs[1][0])*3+2; // opX'
				rotate(cube, steps[sc]);
				sc ++;printf("A110\n");
				return sc;
			case 2:printf("A12\n");
				steps[sc] = gs[2][0]*3; // X: down and face right
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = (5-gs[2][0])*3; // opX:
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = 5*3+1; // D2: 
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = (5-gs[2][0])*3+2; // opX':
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = gs[2][0]*3+2; // X':
				rotate(cube, steps[sc]);
				sc ++;printf("A120\n");
				return sc;
			case 3:printf("A13\n");
				steps[sc] = gs[2][0]*3; // X: turn white down and facing to right
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = 5*3; // D: turn white right
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = gs[2][0]*3+2; // X':
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = (5-gs[2][0])*3+2; // opX':
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = 5*3+2; // D':
				rotate(cube, steps[sc]);
				sc ++;
				steps[sc] = (5-gs[2][0])*3; // opX:
				rotate(cube, steps[sc]);
				sc ++;printf("A130\n");
				return sc;
		}
	}
	else if(gs[0][0] == 5)
	{printf("B\n");
		gridsOfCorner(destCorner | 1/*correspond corner on face 5*/, testgs);
		//assert testgs[0][0] == 0
		diff = (((char *)memchr(cornerGridSeq, testgs[2][1], 4) - (char *)memchr(cornerGridSeq, gs[0][1], 4)) + 4)%4;
		if(diff != 0)
		{printf("B1\n");
			steps[sc] = 5*3+ diff-1/*!!*/; // reduce diff
			rotate(cube, steps[sc]);
			sc ++;
			// ignore other grid data
		}
		char f = nextFaceAroundCorner(5, destCorner | 1);
		printf("B2\n");
		steps[sc] = f*3; // X
		rotate(cube, steps[sc]);
		sc ++;
		steps[sc] = 5*3+2; // D'
		rotate(cube, steps[sc]);
		sc ++;
		steps[sc] = f*3+2; // X'
		rotate(cube, steps[sc]);
		sc ++;
		f = nextFaceAroundCorner(f, destCorner | 1);
		steps[sc] = f*3+2; // rightX'
		rotate(cube, steps[sc]);
		sc ++;
		steps[sc] = 5*3+1; // D2
		rotate(cube, steps[sc]);
		sc ++;
		steps[sc] = f*3; // rightX
		rotate(cube, steps[sc]);
		sc ++;printf("B3\n");
		return sc; // done
	}
	else
	{printf("C\n");
		if(gs[1][0] == 0 || gs[2][0] == 0)
		{printf("C10\n");
			steps[sc] = gs[0][0]*3+((gs[2][0] == 0)?2:0); 
			rotate(cube, steps[sc]);
			sc ++;
			steps[sc] = 5*3+1; // D2
			rotate(cube, steps[sc]);
			sc ++;
			steps[sc] = gs[0][0]*3+((gs[2][0] == 0)?0:2); 
			rotate(cube, steps[sc]);
			sc ++;printf("C21\n");
			
			gridsOfCorner(7-cornerBesides(gs[0][0],gs[1][0], gs[2][0]), testgs);
			//assert testgs[2][0] == 5
			if(gs[1][0] == 0)
			{
				gs[2][0] = 5;
				gs[2][1] = testgs[2][1];
			}
			else 
			{
				gs[1][0] = 5;
				gs[1][1] = testgs[2][1];
			}
		}
		
		//assert gs[1][0] != 0 && gs[2][0] != 0
		
		printf("C3\n");
		gridsOfCorner(destCorner | 1, testgs);
		//assert testgs[2][0] == 5
		diff = (((char *)memchr(cornerGridSeq, testgs[2][1], 4) - (char *)memchr(cornerGridSeq, (gs[1][0]==5?gs[1][1]:gs[2][1]), 4)) + 4)%4;
		if(diff != 0)
		{printf("C31\n");
			steps[sc] = 5*3+diff-1; // reduce diff
			rotate(cube, steps[sc]);
			sc ++;
			// ignore other grid data
		}printf("C32\n");
		char f;printf("C331 gs[1][0]=%d\n", gs[1][0]);
		f = ((gs[1][0]==5) ? 
				nextFaceAroundCorner(0, destCorner) : 
				nextFaceAroundCorner(5, destCorner | 1));
		steps[sc] = f*3+((gs[1][0]==5) ? 2 : 0); // X'
		rotate(cube, steps[sc]);
		sc ++;
		steps[sc] = 5*3+((gs[1][0]==5) ? 2 : 0); // D'
		rotate(cube, steps[sc]);
		sc ++;
		steps[sc] = f*3+((gs[1][0]==5) ? 0 : 2); // X
		rotate(cube, steps[sc]);
		sc ++;printf("C4\n");
		
		return sc; // done
	}
	return 0;
}
int SolveLayer1(Cube *cube, int *steps)
{
	int i,j,k;
	int sc = 0;
	
	// solve top edges
	char ep[12], eo[12];
	char solvingE, eCurPos, gs[3][2];
	for(i = 0; i < 4; i ++)
	{
		printf("try to solve edge between top and face:%d\n",lateralFace[i]);
		edgeCubiePerms(cube, ep, eo);
		solvingE = edgeBesides(0, lateralFace[i]);
		eCurPos = ((char *)memchr(ep, solvingE, 12)) - ep;
		gridsOfEdge(eCurPos, gs);
		for(j = 0; j < 2; j ++)
		{
			if(cube->face[gs[j][0]].grid[0][gs[j][1]] == 0)
			{
				sc += moveL1Edge(cube, steps+sc, j, gs, lateralFace[i]);
				break;
			}
		}printCube3(cube);printSteps(sc, steps);//char dump[1024];fgets(dump,1024,stdin);
	}
	printCube3(cube);printSteps(sc, steps);/*scanf("%*s");*/
	
	
	// solve top corners
	char cp[8], co[8];
	char solvingC, cCurPos;
	for(i = 0; i < 4; i ++)
	{
		printf("try to solve corner between top and face:%d,%d\n",lateralFace[i], leftLateralFace[lateralFace[i]]);
		cornerCubiePerms(cube, cp, co);
		solvingC = cornerBesides(0, lateralFace[i], leftLateralFace[lateralFace[i]]);
		cCurPos = ((char *)memchr(cp, solvingC, 8)) - cp;
		gridsOfCorner(cCurPos, gs);
		for(j = 0; j < 3; j ++)
		{
			if(cube->face[gs[j][0]].grid[0][gs[j][1]] == 0)
			{
				break;
			}
		}printf("cCurPos=%d j=%d  %d,%d %d,%d %d,%d\n",cCurPos, j, gs[0][0],gs[0][1],gs[1][0],gs[1][1],gs[2][0],gs[2][1]);
		char sortedGrids[3][2];
		sortedGrids[0][0] = gs[j][0];
		sortedGrids[0][1] = gs[j][1];
		if(gs[(j+1)%3][0] == nextFaceAroundCorner(gs[j][0], cCurPos))
		{
			sortedGrids[1][0] = gs[(j+1)%3][0];
			sortedGrids[1][1] = gs[(j+1)%3][1];
			sortedGrids[2][0] = gs[(j+2)%3][0];
			sortedGrids[2][1] = gs[(j+2)%3][1];
		}
		else
		{
			sortedGrids[1][0] = gs[(j+2)%3][0];
			sortedGrids[1][1] = gs[(j+2)%3][1];
			sortedGrids[2][0] = gs[(j+1)%3][0];
			sortedGrids[2][1] = gs[(j+1)%3][1];
		}printf("%d,%d %d,%d %d,%d\n",sortedGrids[0][0],sortedGrids[0][1],sortedGrids[1][0],sortedGrids[1][1],sortedGrids[2][0],sortedGrids[2][1]);
		sc += moveL1Corner(cube, steps+sc, sortedGrids, solvingC);printCube3(cube);printSteps(sc, steps);/*scanf("%*s");*/
	}
	
	printCube3(cube);printSteps(sc, steps);
	{char dump[32];/*scanf("%s", dump);*/}
	
	//findAnEdgeWith(cube, cube->face[0].grid[1][1]);
	
	//findAVertexWith(cube, cube->face[0].grid[1][1]);
	
	return sc;
}

char edgeGridSeq[4] = {1,5,7,3};

int getL2EdgeDown(Cube *cube, int *steps, char gs[2][2])
{
	if(gs[1][0] != 5)
		return -1;
	int sc = 0;
	int diff;
	
	int primaryColor = cube->face[5].grid[0][gs[1][1]];
	int secondColor = cube->face[gs[0][0]].grid[0][gs[0][1]];
	int direction = 1;
	
	if(primaryColor != leftLateralFace[secondColor])
		direction = -1;
	
	int readyFace = 5-primaryColor; // opposite from primary
	int currentFace = gs[0][0];
	
	diff = (((char *)memchr(lateralFace, readyFace, 4) - (char *)memchr(lateralFace, currentFace, 4)) + 4)%4;
	printf("primaryColor=%d\n",primaryColor);printf("secondColor=%d\n",secondColor);printf("direction=%d\n",direction);printf("readyFace=%d\n",readyFace);printf("currentFace=%d\n",currentFace);
	if(diff != 0)
	{
		steps[sc] = 5*3+(3-diff); // reduce diff
		rotate(cube, steps[sc]);
		sc ++;
	}
	printf("reduce diff to ready\n");printCube3(cube);
	// consider              X
	//             primary        ready
	//                     second (see from bottom)
	// for maneuver
	int maneuver[7] = {primaryColor*3, 5*3, primaryColor*3+2, 5*3+2, secondColor*3+2, 5*3+2, secondColor*3};
	// P' D' P D S D S'
	printSteps(7,maneuver);
	int i;
	for(i = 0; i < 7; i ++)
	{
		if(direction > 0)
		{
			steps[sc] = maneuver[i];
		}
		else
		{ // reverse for mirro image state!!  this happens to be so
			steps[sc] = reverseStep(maneuver[i]);
		}
		rotate(cube, steps[sc]);printSteps(1,&steps[sc]);
		sc ++;
	}
	return sc; // done
}


int swapL2Edge(Cube *cube, int *steps, char dest, char pos, char gs[2][2])
{
	if(gs[0][0] == 0 || gs[1][0] == 5)
		return -1;
	int sc = 0;
	int diff = ((dest > 1 ? 5-dest : dest) - (pos > 1 ? 5-pos : pos) + 4) % 4;
	int orienSame =	((gs[0][0] == leftLateralFace[gs[1][0]]) ==
		(cube->face[gs[0][0]].grid[0][gs[0][1]] == leftLateralFace[cube->face[gs[1][0]].grid[0][gs[1][1]]]));
	int i, j;
	if(diff == 0)
	{
		if(orienSame)
			return 0; // already done
		else
		{ // R2 D2 F' R2 F D2 R D' R
			int maneuver[9] = {1*3+1, 5*3+1, 2*3+2, 1*3+1, 2*3, 5*3+1, 1*3, 5*3+2, 1*3};
			for(i = 0; i < 9; i ++)
			{
				switch(pos)
				{
					case 0: steps[sc] = maneuver[i]; break;
					case 1: steps[sc] = rotateStep('y', 1, maneuver[i]); break;
					case 2: steps[sc] = rotateStep('y', 3, maneuver[i]); break;
					case 3: steps[sc] = rotateStep('y', 2, maneuver[i]); break;
					default:steps[sc] = maneuver[i]; break;
				}
				rotate(cube, steps[sc]);
				sc ++;
			}
			return sc;
		}
	}
	else if(diff == 2)
	{
		if(orienSame)
		{//R2 U' D' B2 U D R2
			int maneuver[7] = {1*3+1, 0*3+2, 5*3+2, 3*3+1, 0*3, 5*3, 1*3+1};
			for(i = 0; i < 7; i ++)
			{
				switch(pos)
				{
					case 0: steps[sc] = maneuver[i]; break;
					case 1: steps[sc] = rotateStep('y', 1, maneuver[i]); break;
					case 2: steps[sc] = rotateStep('y', 3, maneuver[i]); break;
					case 3: steps[sc] = rotateStep('y', 2, maneuver[i]); break;
					default:steps[sc] = maneuver[i]; break;
				}
				rotate(cube, steps[sc]);
				sc ++;
			}
			return sc; // done
		}
		else
		{ // R2 B R D' B' R B D B' 
			int maneuver[9] = {1*3+1, 3*3, 1*3, 5*3+2, 3*3+2, 1*3, 3*3, 5*3, 3*3+2};
			for(i = 0; i < 9; i ++)
			{
				switch(pos)
				{
					case 0: steps[sc] = maneuver[i]; break;
					case 1: steps[sc] = rotateStep('y', 1, maneuver[i]); break;
					case 2: steps[sc] = rotateStep('y', 3, maneuver[i]); break;
					case 3: steps[sc] = rotateStep('y', 2, maneuver[i]); break;
					default:steps[sc] = maneuver[i]; break;
				}
				rotate(cube, steps[sc]);
				sc ++;
			}
			return sc; // done
		}
	}
	else // diff == 1,3
	{
		if(orienSame)
		{//F D R F' R' F' 
			int maneuver[6] = {2*3, 5*3, 1*3, 2*3+2, 1*3+2, 2*3+2};
			for(i = 0; i < 6; i ++)
			{
				if(diff == 3)
				{
					maneuver[i] = rotateStep('y',1,mirrorStep("FB", maneuver[i]));
				}
				switch(pos)
				{
					case 0: steps[sc] = maneuver[i]; break;
					case 1: steps[sc] = rotateStep('y', 1, maneuver[i]); break;
					case 2: steps[sc] = rotateStep('y', 3, maneuver[i]); break;
					case 3: steps[sc] = rotateStep('y', 2, maneuver[i]); break;
					default:steps[sc] = maneuver[i]; break;
				}
				rotate(cube, steps[sc]);
				sc ++;
			}
			return sc; // done
		}
		else
		{ // 2*3+1 5*3+1 2*3+1 5*3+1 2*3+1
			int maneuver[5] = {2*3+1, 5*3+1, 2*3+1, 5*3+1, 2*3+1};
			for(i = 0; i < 5; i ++)
			{
				if(diff == 3)
				{
					maneuver[i] = rotateStep('y',1,mirrorStep("FB", maneuver[i]));
				}
				switch(pos)
				{
					case 0: steps[sc] = maneuver[i]; break;
					case 1: steps[sc] = rotateStep('y', 1, maneuver[i]); break;
					case 2: steps[sc] = rotateStep('y', 3, maneuver[i]); break;
					case 3: steps[sc] = rotateStep('y', 2, maneuver[i]); break;
					default:steps[sc] = maneuver[i]; break;
				}
				rotate(cube, steps[sc]);
				sc ++;
			}
			return sc; // done
		}
		
	}
	return sc; // done
}

int SolveLayer2(Cube *cube, int *steps)
{
	//if(!LayerSolved(cube, 1))
	//	return -1;
	int i,j,k;
	int sc = 0;
	
	// solve lateral edges 0,1,2,3
	char ep[12], eo[12];
	char solvedBIT = 0, eCurPos[4], gs[3][2], testgs[2][2];
	char getL2EdgeDownUsed = 0;
	while(solvedBIT < 15)
	{
		getL2EdgeDownUsed = 0;
		edgeCubiePerms(cube, ep, eo);printCube3(cube);printf("solve status: %d%d%d%d\n",(solvedBIT>>3)&1,(solvedBIT>>2)&1,(solvedBIT>>1)&1,(solvedBIT>>0)&1);
		for(i = 0; i < 4; i ++)
		{
			if((solvedBIT >> i) & 1)
				continue;
			printf("check for edge %d, between and face:%d,%d\n",edgeBesides(i+1, leftLateralFace[i+1]),i+1,leftLateralFace[i+1]);
			eCurPos[i] = ((char *)memchr(ep, i, 12)) - ep;
			printf("current position: %d\n", eCurPos[i]);
			// for test
			gridsOfEdge(eCurPos[i], gs);printf(" %d,%d  %d,%d\n", gs[0][0],gs[0][1],gs[1][0],gs[1][1]);
			// assert if eCurPos[i] is on face 5, then gs[1][0] == 5
			if(gs[1][0] == 5)//eCurPos[i] == 4,5,8,10
			{printf("try to solve with getL2EdgeDown....\n");
				sc += getL2EdgeDown(cube, steps+sc, gs);
				solvedBIT |= (1 << i);printCube3(cube);printf("%d steps: ", sc);printSteps(sc, steps);/*scanf("%*s");*/
				getL2EdgeDownUsed = 1;
				break;// re-fetch states for checking
			}
		}
		if(!getL2EdgeDownUsed)
		{
			printf("No place to use getL2EdgeDown.\n");
			printf("Now use other maneuvers fto swap L2 cubies...\n");
			
			for(i = 0; i < 4; i ++)
			{
				if((solvedBIT >> i) & 1)
					continue;
				printf("try to solve edge %d, between and face:%d,%d with swap....\n",edgeBesides(i+1, leftLateralFace[i+1]),i+1,leftLateralFace[i+1]);
				printf("current position: %d\n", eCurPos[i]);
				// for test
				gridsOfEdge(eCurPos[i], gs);printf(" %d,%d  %d,%d\n", gs[0][0],gs[0][1],gs[1][0],gs[1][1]);
				if(eCurPos[i] > 3) // not in the slice between UD
					continue;
				 // assert eCurPos[i] == 0,1,2,3
				sc += swapL2Edge(cube, steps+sc, i, eCurPos[i], gs);
				solvedBIT |= (1 << i);printCube3(cube);printf("%d steps: ", sc);printSteps(sc, steps);/*scanf("%*s");*/
				break;// re-fetch states for checking
			}
		}
	}
	printCube3(cube);printf("%d steps: ", sc);printSteps(sc, steps);/*scanf("%*s");*/
		
	return sc;
}

int solveL2Edge(Cube *cube, int *steps, char gs[2][2])
{
	if(gs[1][0] != 5)
		return -1;
	int sc = 0;
	int diff;
	
	int primaryColor = cube->face[5].grid[0][gs[1][1]];
	int secondColor = cube->face[gs[0][0]].grid[0][gs[0][1]];
	int direction = 1;
	
	if(primaryColor != leftLateralFace[secondColor])
		direction = -1;
	
	int readyFace = 5-primaryColor; // opposite from primary
	int currentFace = gs[0][0];
	
	diff = (((char *)memchr(lateralFace, readyFace, 4) - (char *)memchr(lateralFace, currentFace, 4)) + 4)%4;
	
	if(diff != 0)
	{
		steps[sc] = 5*3+(3-diff); // reduce diff
		rotate(cube, steps[sc]);
		sc ++;
	}
	
	// consider              X
	//             primary        ready
	//                     second
	// for maneuver
	int maneuver[7] = {primaryColor*3, 5*3, primaryColor*3+2, 5*3+2, secondColor*3+2, 5*3+2, secondColor*3};
	// P' D' P D S D S'
	
	int i;
	for(i = 0; i < 7; i ++)
	{
		if(direction > 0)
		{
			steps[sc] = maneuver[i];
		}
		else
		{ // reverse for mirro image state!!  this happens to be so
			steps[sc] = reverseStep(maneuver[6-i]);
		}
		rotate(cube, steps[sc]);
		sc ++;
	}
	return sc; // done
}
int bottomEdges[4] = {9,6,11,7};
int SolveLayer3(Cube *cube, int *steps)
{
	//if(!LayerSolved(cube, 1))
	//	return -1;
	int i,j,k;
	int sc = 0;
	
	//solve bottom edges
	char ep[12], eo[12];
	char solvedBIT = 0, eCurPos[4], testgs[2][2];
	char getL2EdgeDownUsed = 0;
	
	char eAtPos[4];
	char iOrien[4];
	char gs[4][2][2] = {{{5,1},{1,1}},{{5,3},{2,3}},{{5,7},{4,5}},{{5,5},{3,7}}};
	
	edgeCubiePerms(cube, ep, eo);
	int flipCount = 0;
	int first = 0, last = 0;
	int distFromLast = 0;
	int totalDist = 0;
	int maxDistPoint = -1;
	for(i = 0; i < 4; i ++)
	{
		eAtPos[i] = ep[bottomEdges[i]];
		for(j = 0; j < 4; j++)
		{
			if(bottomEdges[j] == eAtPos[i])
			{
				if(i == 0)
					first = j;
				else
				{
					distFromLast = (j+4 - last)%4;
					totalDist += distFromLast;
					if(distFromLast == 3)
						maxDistPoint = i-1; // OR i ???
				}
				last = j;
				break;
			}
		}
		if(eo[bottomEdges[i]] == 1)
		{
			iOrien[flipCount] = i;
			flipCount++;
		}
	}
	
	distFromLast = (first+4 - last)%4;
	totalDist += distFromLast;
	if(distFromLast == 3)
		maxDistPoint = 3; // OR i (= 0) ???
	
	printf("pattern: \n");
	if(totalDist == 4)
	{printf("  NO SWAP\n");
		if(flipCount == 0)
		{printf("  NO FLIPS\n");
			// almost done
		}
		else if(flipCount == 4)
		{printf("  4 FLIPS\n");
			//  L' F' D R' D R F L D' B D' B'
			int maneuver[12] = {4*3+2, 2*3+2, 5*3, 1*3+2, 5*3, 1*3, 2*3, 4*3, 5*3+2, 3*3, 5*3+2, 3*3+2};
			for(i = 0; i < 12; i ++)
			{
				steps[sc] = maneuver[i];
				rotate(cube, steps[sc]);
				sc ++;
			}
		}
		else if(flipCount == 2)
		{printf("  2 FLIPS\n");
			if((iOrien[1] - iOrien[0]) == 2)
			{printf("    FLIPS: ORTHO\n");
				// R F D F' D R' D' R D' R'
				int maneuver[10] = {1*3, 2*3, 5*3, 2*3+2, 5*3, 1*3+2, 5*3+2, 1*3, 5*3+2, 1*3+2};printf("rotate %d/4\n", iOrien[0] == 0);
				for(i = 0; i < 10; i ++)
				{
					steps[sc] = (iOrien[0] == 0 ? maneuver[i] : rotateStep('y', 1, maneuver[i]));
					rotate(cube, steps[sc]);
					sc ++;
				}
			}
			else // ((iOrien[1] - iOrien[0]) == 1,3)
			{printf("    FLIPS: PARA\n");
				// R F2 U F U2 R U R2 F
				int anchor = (((iOrien[1] - iOrien[0]) == 3) ? iOrien[1] : iOrien[0]);printf("rotate %d/4\n", anchor);
				int maneuver[9] = {1*3, 2*3+1, 0*3, 2*3, 0*3+1, 1*3, 0*3, 1*3+1, 2*3};
				for(i = 0; i < 9; i ++)
				{
					steps[sc] = rotateStep('y', anchor, maneuver[i]);
					rotate(cube, steps[sc]);
					sc ++;
				}
			}
		}
	}
	else if(totalDist == 12)
	{printf("  ORTHO SWAP\n");
		if(flipCount == 0)
		{printf("  NO FLIPS\n");
			// F' R F' U2 B L' B' U2 F2 R' 
			int maneuver[10] = {2*3+2, 1*3, 2*3+2, 0*3+1, 3*3, 4*3+2, 3*3+2, 0*3+1, 2*3+1, 1*3+2};
			for(i = 0; i < 10; i ++)
			{
				steps[sc] = maneuver[i];
				rotate(cube, steps[sc]);
				sc ++;
			}
		}
		else if(flipCount == 4)
		{printf("  4 FLIPS\n");
			// R D2 R' B' L' D F' D2 F L B 
			int maneuver[11] = {1*3, 5*3+1, 1*3+2, 3*3+2, 4*3+2, 5*3, 2*3+2, 5*3+1, 2*3, 4*3, 3*3};
			for(i = 0; i < 11; i ++)
			{
				steps[sc] = maneuver[i];
				rotate(cube, steps[sc]);
				sc ++;
			}
		}
		else if(flipCount == 2)
		{printf("  2 FLIPS\n");
			if((iOrien[1] - iOrien[0]) == 2)
			{printf("    FLIPS: ORTHO\n");
				// B R2 U R U' R B' R F D F' R' 
				int maneuver[12] = {3*3, 1*3+1, 0*3, 1*3, 0*3+2, 1*3, 3*3+2, 1*3, 2*3, 5*3, 2*3+2, 1*3+2};printf("rotate %d/4\n", iOrien[0] == 0);
				for(i = 0; i < 12; i ++)
				{
					steps[sc] = (iOrien[0] == 0 ? maneuver[i] : rotateStep('y', 1, maneuver[i]));
					rotate(cube, steps[sc]);
					sc ++;
				}
			}
			else // ((iOrien[1] - iOrien[0]) == 1,3)
			{printf("    FLIPS: PARA\n");
				// R' B' D B R F D' F' D 
				int anchor = (((iOrien[1] - iOrien[0]) == 3) ? iOrien[1] : iOrien[0]);
				int maneuver[9] = {1*3+2, 3*3+2, 5*3, 3*3, 1*3, 2*3, 5*3+2, 2*3+2, 5*3};printf("rotate %d/4\n", anchor);
				for(i = 0; i < 9; i ++)
				{
					steps[sc] = rotateStep('y', anchor, maneuver[i]);
					rotate(cube, steps[sc]);
					sc ++;
				}
			}
		}
	}
	else if(totalDist == 8)
	{printf("  PARA SWAP\n");
		if(flipCount == 0)
		{printf("  NO FLIPS\n");
			// R' D2 R D R' D R D
			int maneuver[8] = {1*3+2, 5*3+1, 1*3, 5*3, 1*3+2, 5*3, 1*3, 5*3};printf("rotate %d/4\n", maxDistPoint);
			for(i = 0; i < 8; i ++)
			{
				steps[sc] = rotateStep('y', maxDistPoint, maneuver[i]);
				rotate(cube, steps[sc]);
				sc ++;
			}
		}
		else if(flipCount == 4)
		{printf("  4 FLIPS\n");
			// R F D2 F2 D' R' D R F D2 R' D
			int maneuver[12] = {1*3, 2*3, 5*3+1, 2*3+1, 5*3+2, 1*3+2, 5*3, 1*3, 2*3, 5*3+1, 1*3+2, 5*3};printf("rotate %d/4\n", maxDistPoint);
			for(i = 0; i < 12; i ++)
			{
				steps[sc] = rotateStep('y', maxDistPoint, maneuver[i]);
				rotate(cube, steps[sc]);
				sc ++;
			}
		}
		else if(flipCount == 2)
		{printf("  2 FLIPS\n");
			if((iOrien[1] - iOrien[0]) == 2)
			{printf("    FLIPS: ORTHO\n");
				// D F L D L' D' F'  
				int maneuver[7] = {5*3, 2*3, 4*3, 5*3, 4*3+2, 5*3+2, 2*3+2};
				int needMirror = (iOrien[0] != maxDistPoint && iOrien[1] != maxDistPoint);printf("flip = %d\n", needMirror);printf("rotate %d/4\n", (needMirror ? (maxDistPoint + 1)%4 : maxDistPoint));
				for(i = 0; i < 7; i ++)
				{
					if(needMirror)
						maneuver[i] = mirrorStep("FB", maneuver[i]);
					steps[sc] = rotateStep('y', (needMirror ? (maxDistPoint + 1)%4 : maxDistPoint), maneuver[i]);
					rotate(cube, steps[sc]);
					sc ++;
				}
			}
			else // ((iOrien[1] - iOrien[0]) == 1,3)
			{ printf("    FLIPS: PARA\n");
				if(eo[bottomEdges[maxDistPoint]] != eo[bottomEdges[(maxDistPoint+1)%4]]) // only one of the swap ones flips
				{printf("    1 OF SWAP\n");
					// R' D' B' D B R D 
					int maneuver[7] = {1*3+2, 5*3+2, 3*3+2, 5*3, 3*3, 1*3, 5*3};
					int needMirror = (iOrien[0] != maxDistPoint && iOrien[1] != maxDistPoint);printf("flip = %d\n", needMirror);printf("rotate %d/4\n", (needMirror ? (maxDistPoint + 1)%4 : maxDistPoint));
					for(i = 0; i < 7; i ++)
					{
						if(needMirror)
							maneuver[i] = mirrorStep("FB", maneuver[i]);
						steps[sc] = rotateStep('y', (needMirror ? (maxDistPoint + 1)%4 : maxDistPoint), maneuver[i]);
						rotate(cube, steps[sc]);
						sc ++;
					}
				}
				else if(eo[bottomEdges[maxDistPoint]] == 1) // the swap two both flip
				{printf("    2 OF SWAP\n");
					// R U2 L F L' U2 R B' R B R
					int maneuver[11] = {1*3, 0*3+1, 4*3, 2*3, 4*3+2, 0*3+1, 1*3, 3*3+2, 1*3, 3*3, 1*3};printf("rotate %d/4\n", maxDistPoint);
					for(i = 0; i < 11; i ++)
					{
						steps[sc] = rotateStep('y', maxDistPoint, maneuver[i]);
						rotate(cube, steps[sc]);
						sc ++;
					}
				}
				else // none of the swap ones flips
				{printf("    0 OF SWAP\n");
					// R' B' L B' L' B2 R D
					int maneuver[8] = {1*3+2, 3*3+2, 4*3, 3*3+2, 4*3+2, 3*3+1, 1*3, 5*3};printf("rotate %d/4\n", maxDistPoint);
					for(i = 0; i < 8; i ++)
					{
						steps[sc] = rotateStep('y', maxDistPoint, maneuver[i]);
						rotate(cube, steps[sc]);
						sc ++;
					}
				}
			}
		}
	}
	
	printCube3(cube);printf("%d steps: ", sc);printSteps(sc, steps);
	
	int diff;
	diff = (cube->face[1].grid[0][1] - 1);
	if(diff > 1)
		diff = 5-diff; // for cube->face[1].grid[0][1]== 4 & 3 
	
	if(diff != 0)
	{
		steps[sc] = 5*3+3-diff;
		rotate(cube, steps[sc]);
		sc ++;
	}
	
	printCube3(cube);printf("%d steps: ", sc);printSteps(sc, steps);/*scanf("%*s");*/
	
	
	printf("solve bottom corner....\n");
	
	// solve bottom corners
	char cp[8], co[8];
	char btmCorner[4] = {1,3,7,5};
	
	cornerCubiePerms(cube, cp, co);
	
	printf("bottom orientations %d %d %d %d\n", co[1],co[3],co[7],co[5]);
		
	int patternList[3][3][3] = //give category*10 + position shift
		{	{{0,	120,	210}	//00
			,{1020,	1110,	121}	//01
			,{1022,	211,	2220}	//02
			}                       
		,	{{213,	1113,	1021}	//10
			,{1112,	1111,	11220}	//11
			,{122,	12120,	11221}	//12
			}                       
		,	{{123,	1023,	2223}	//20
			,{212,	11223,	12121}	//21
			,{2222,	11222,	2221}	//22
			}// 0     1       2
		};
		
	int pattern = patternList[co[btmCorner[0]]][co[btmCorner[1]]][co[btmCorner[2]]];
	int shift = pattern % 10;
	pattern = pattern / 10;
	
	printf("pattern = %d   shift = %d\n", pattern, shift);
	
	int maneuvers[7][16]={
		//  12:  R2 B R' F2 R B' R' F2 R' (9f*)
		{1*3+1, 3*3, 1*3+2, 2*3+1, 1*3, 3*3+2, 1*3+2, 2*3+1, 1*3+2},
		//  21:  R F R' B R F' R' B'  (8f*)
		{1*3, 2*3, 1*3+2, 3*3, 1*3, 2*3+2, 1*3+2, 3*3+2},   
		// 111:  D' R' D L D' R D L'  (8f*)
		{5*3+2, 1*3+2, 5*3, 4*3, 5*3+2, 1*3, 5*3, 4*3+2},   
		// 222:  L D' R' D L' D' R D  reverse of 111
		{4*3, 5*3+2, 1*3+2, 5*3, 4*3+2, 5*3+2, 1*3, 5*3},   
		// 102:  F' R' B' R F R' B R  (8f*)
		{2*3+2, 1*3+2, 3*3+2, 1*3, 2*3, 1*3+2, 3*3, 1*3},   
		//1122:  R U' B2 D R2 F2 D' B2 U L2 F2 R'  (12f*)
		{1*3, 0*3+2, 3*3+1, 5*3, 1*3+1, 2*3+1, 5*3+2, 3*3+1, 0*3, 4*3+1, 2*3+1, 1*3+2},   
		//1212:  R' B R B' D2 R2 F L' F L F2 R2 D2  (13f*)
		{1*3+2, 3*3, 1*3, 3*3+2, 5*3+1, 1*3+1, 2*3, 4*3+2, 2*3, 4*3, 2*3+1, 1*3+1, 5*3+1},
	};
	int mi = -1, msn = 0;
	switch(pattern)
	{
		case 12:	mi = 0; msn =  9; break;
		case 21:	mi = 1; msn =  8; break;
		case 111:	mi = 2; msn =  8; break;
		case 222:	mi = 3; msn =  8; break;
		case 102:	mi = 4; msn =  8; break;
		case 1122:	mi = 5; msn = 12; break;
		case 1212:	mi = 6; msn = 13; break;
	}
	
	for(i = 0; i < msn; i ++)
	{
		steps[sc] = rotateStep('y', (4-shift)%4, maneuvers[mi][i]);
		rotate(cube, steps[sc]);
		sc ++;
	}
	printCube3(cube);printf("%d steps: ", sc);printSteps(sc, steps);///*scanf("%*s");*/
	
	cornerCubiePerms(cube, cp, co);
	
	if(cp[7] == 1 && cp[1] == 7)
	{printf("'X' exchange pattern\n");
		//F2 R2 L2 B2 U F2 R2 L2 B2 D  (10f*)
		int maneuver[10] = {2*3+1, 1*3+1, 4*3+1, 3*3+1, 0*3, 2*3+1, 1*3+1, 4*3+1, 3*3+1, 5*3};
		for(i = 0; i < 10; i ++)
		{
			steps[sc] = maneuver[i];
			rotate(cube, steps[sc]);
			sc ++;
		}printCube3(cube);printf("%d steps: ", sc);printSteps(sc, steps);///*scanf("%*s");*/
	}
	else if((cp[1] == 5 && cp[5] == 1) || (cp[1] == 3 && cp[3] == 1))
	{printf("'=' exchange pattern\n");
		// R2 U F2 U' B2 D R2 F2 D' B2 U L2 U' R2  (14f*)
		int maneuver[14] = {1*3+1, 0*3, 2*3+1, 0*3+2, 3*3+1, 5*3, 1*3+1, 2*3+1, 5*3+2, 3*3+1, 0*3, 4*3+1, 0*3+2, 1*3+1};
		for(i = 0; i < 14; i ++)
		{
			steps[sc] = ((cp[1] == 5) ? maneuver[i] : rotateStep('y', 1, maneuver[i]));
			rotate(cube, steps[sc]);
			sc ++;
		}printCube3(cube);printf("%d steps: ", sc);printSteps(sc, steps);///*scanf("%*s");*/
	}
	else
	{printf("triangle exchange pattern\n");
		int fix, i, fixi;
		for(i = 0; i < 4; i ++)
		{
			if(cp[btmCorner[i]] == btmCorner[i])
			{
				fix = btmCorner[i];
				fixi = i;
				break;
			}
		}
		int rev;
		if(cp[btmCorner[(fixi+1)%4]] == btmCorner[(fixi+3)%4])
			rev = 0;
		else if(cp[btmCorner[(fixi+3)%4]] == btmCorner[(fixi+1)%4])
			rev = 1;
		else if(cp[btmCorner[(fixi+1)%4]] == btmCorner[(fixi+1)%4])
			//done
			return sc;
		printf("rev = %d\n",rev);
		//L' F L' B2 L F' L' B2 L2  (9f*)
		int maneuver[9] = {4*3+2, 2*3, 4*3+2, 3*3+1, 4*3, 2*3+2, 4*3+2, 3*3+1, 4*3+1};
		for(i = 0; i < 9; i ++)
		{
			steps[sc] = rotateStep('y', fixi, (rev ? reverseStep(maneuver[8-i]) : maneuver[i]));
			rotate(cube, steps[sc]);
			sc ++;
		}printCube3(cube);printf("%d steps: ", sc);printSteps(sc, steps);///*scanf("%*s");*/
	}
	
	return sc;
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
	printf("Please input in the order \n\
	1. record the current color of the center grid of U=>0,R=>1,F=>2,B=>3,L=>4,D=>5\n\
	2. 'input current U, then up become left, then left become front' (repeat for 3 times)\n\
	3. up become front, then front become back, then do 2. again\n");
	
	Cube *cube = (Cube*)malloc(sizeof(Cube));
	initialCube(cube);
	
	int i,j,k;
	/*
	printf("TEST:\n");
	for(i = 0; i < 18; i ++)
		printf("rev %c%s %d => %c%s %d\n", faceName[i/3], (i % 3 == 1 ? "2" : (i % 3 == 2 ? "'" : "")), i, faceName[reverseStep(i)/3], (reverseStep(i) % 3 == 1 ? "2" : (reverseStep(i) % 3 == 2 ? "'" : "")), reverseStep(i));
	for(i = 0; i < 18; i ++)
		printf("RL %c%s %d => %c%s %d\n", faceName[i/3], (i % 3 == 1 ? "2" : (i % 3 == 2 ? "'" : "")), i, faceName[mirrorStep("RL",i)/3], (mirrorStep("RL",i) % 3 == 1 ? "2" : (mirrorStep("RL",i) % 3 == 2 ? "'" : "")), mirrorStep("RL",i));
	for(i = 0; i < 18; i ++)
		printf("UD %c%s %d => %c%s %d\n", faceName[i/3], (i % 3 == 1 ? "2" : (i % 3 == 2 ? "'" : "")), i, faceName[mirrorStep("UD",i)/3], (mirrorStep("UD",i) % 3 == 1 ? "2" : (mirrorStep("UD",i) % 3 == 2 ? "'" : "")), mirrorStep("UD",i));
	for(i = 0; i < 18; i ++)
		printf("FB %c%s %d => %c%s %d\n", faceName[i/3], (i % 3 == 1 ? "2" : (i % 3 == 2 ? "'" : "")), i, faceName[mirrorStep("FB",i)/3], (mirrorStep("FB",i) % 3 == 1 ? "2" : (mirrorStep("FB",i) % 3 == 2 ? "'" : "")), mirrorStep("FB",i));
	for(i = 0; i < 18; i ++)
		for(j = 1; j <= 3; j ++)
			printf("x%d %c%s %d => %c%s %d\n", j,faceName[i/3], (i % 3 == 1 ? "2" : (i % 3 == 2 ? "'" : "")), i, faceName[rotateStep("x", j, i)/3], (rotateStep("x", j, i) % 3 == 1 ? "2" : (rotateStep("x", j, i) % 3 == 2 ? "'" : "")), rotateStep("x", j, i));
	for(i = 0; i < 18; i ++)
		for(j = 1; j <= 3; j ++)
			printf("y%d %c%s %d => %c%s %d\n", j,faceName[i/3], (i % 3 == 1 ? "2" : (i % 3 == 2 ? "'" : "")), i, faceName[rotateStep("y", j, i)/3], (rotateStep("y", j, i) % 3 == 1 ? "2" : (rotateStep("y", j, i) % 3 == 2 ? "'" : "")), rotateStep("y", j, i));
	for(i = 0; i < 18; i ++)
		for(j = 1; j <= 3; j ++)
			printf("z%d %c%s %d => %c%s %d\n", j,faceName[i/3], (i % 3 == 1 ? "2" : (i % 3 == 2 ? "'" : "")), i, faceName[rotateStep("z", j, i)/3], (rotateStep("z", j, i) % 3 == 1 ? "2" : (rotateStep("z", j, i) % 3 == 2 ? "'" : "")), rotateStep("z", j, i));
	*/
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
	printCube3(cube);
	
	printf("Solving...\n");
	
	time_t begin, end;
	time(&begin);
	
	int steps[100] = {};
	int stepN = 0;
	int stepPartN = 0;
	printf("solve Layer 1...\n");
	stepPartN = SolveLayer1(cube, steps);
	printf("Layer 1 solved in %d step.\n", stepPartN);
	stepN += stepPartN;
	
	printf("solve Layer 2...\n");
	stepPartN = SolveLayer2(cube, steps+stepN);
	stepN += stepPartN;
	printf("Layer 2 solved in %d step.\n", stepPartN);
	
	printf("solve Layer 3...\n");
	stepPartN = SolveLayer3(cube, steps+stepN);
	stepN += stepPartN;
	printf("Layer 3 solved in %d step.\n", stepPartN);
	
	if(isSolved(cube))
	{
		printf("solved in %d step:\n", stepN);
		printSteps(stepN, steps);
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