#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>
#include<limits.h>
#define NV 265214
#define NE 420045
#define TRUE 1
#define FALSE 0
#define MAXINT 10000000
typedef struct edgenode {
  int y;
  int weight;
  struct edgenode *next;
} Edgenode;
 
typedef struct {
  Edgenode *edges[NV+1];
  int degree[NV+1];
  int nvertices;
  int nedges;
  int directed;
} Graph;
Graph g;
struct mHNode 
{ 
	int v; 
	int dist; 
};
struct mH 
{ 
	int size;	  
	int capacity; 
	int *pos;	 
	struct mHNode **array; 
}; 
struct mH* mH; 
struct mHNode* newMHN(int v, int dist) 
{ 
	struct mHNode* mHNode = 
		(struct mHNode*) malloc(sizeof(struct mHNode)); 
	mHNode->v = v; 
	mHNode->dist = dist; 
	return mHNode; 
} 

struct mH* createmH(int capacity) 
{ 
	struct mH* mH = 
		(struct mH*) malloc(sizeof(struct mH)); 
	mH->pos = (int *)malloc(capacity * sizeof(int)); 
	mH->size = 0; 
	mH->capacity = capacity; 
	mH->array = 
		(struct mHNode**) malloc(capacity * sizeof(struct mHNode*)); 
	return mH; 
} 

void swapmHNode(struct mHNode** a, struct mHNode** b) 
{ 
	struct mHNode* t = *a; 
	*a = *b; 
	*b = t; 
} 
void mHify(struct mH* mH, int idx) 
{ 
	int small, left, right; 
	small = idx; 
	left = 2 * idx + 1; 
	right = 2 * idx + 2; 

	if (left < mH->size && 
		mH->array[left]->dist < mH->array[small]->dist ) 
	small = left; 

	if (right < mH->size && 
		mH->array[right]->dist < mH->array[small]->dist ) 
	small = right; 
	if (small != idx) 
	{ 
		struct mHNode *smallNode = mH->array[small]; 
		struct mHNode *idxNode = mH->array[idx]; 

		mH->pos[smallNode->v] = idx; 
		mH->pos[idxNode->v] = small; 

		swapmHNode(&mH->array[small], &mH->array[idx]); 

		mHify(mH, small); 
	} 
} 

int isEmpty(struct mH* mH) 
{ 
	return mH->size == 0; 
} 

struct mHNode* extractMin(struct mH* mH) 
{ 
	if (isEmpty(mH)) 
		return NULL; 

	struct mHNode* root = mH->array[0]; 
	struct mHNode* lastNode = mH->array[mH->size - 1]; 
    mH->pos[lastNode->v] = 0; 
        
    swapmHNode(&mH->array[0], &mH->array[mH->size-1]); 

	mH->pos[root->v] = mH->size-1; 
	mH->pos[lastNode->v] = 0; 
	--mH->size; 
	mHify(mH, 0); 

	return root; 
} 
void decreaseKey(struct mH* mH, int v, int dist) 
{ 
	int i = mH->pos[v]; 
	mH->array[i]->dist = dist; 
	while (i && mH->array[i]->dist < mH->array[(i - 1) / 2]->dist) 
	{ 
		mH->pos[mH->array[i]->v] = (i-1)/2; 
		mH->pos[mH->array[(i-1)/2]->v] = i; 
		swapmHNode(&mH->array[i], &mH->array[(i - 1) / 2]); 
		i = (i - 1) / 2; 
	} 
} 

int isInmH(struct mH *mH, int v) 
{ 
    if (mH->pos[v] < mH->size) 
	    return 0; 
    return 1; 
} 

void printArr(int dist[]) 
{ 
	printf("Vertex Distance from Source\n"); 
	for (int i = 0; i < g.nvertices; ++i) 
		printf("%d \t\t %d\n", i, dist[i]); 
} 
void dijkstra(Graph *g, int start, int *distance){
  int i;
  Edgenode *p;
  int v, w;
  int weight;
  for (i=0; i< g->nvertices; i++) {
    distance[i] = MAXINT;
    mH->array[i]->v = i;
    mH->array[i]->dist = distance[i];
	  mH->pos[i] = i; 
  }
  distance[start] = 0;
  v = start;
  mH->array[start]->dist =  distance[start];
  decreaseKey(mH, start, distance[start]); 
  mH->size = g->nvertices; 
  while (!isEmpty(mH)) 
	{ 
		struct mHNode* mHNode = extractMin(mH); 
		int u = mHNode->v; 
        p = g->edges[u];    
        while (p != NULL) {
            w = p->y;
            weight = p->weight;
            if (isInmH(mH, w) == 0 && distance[u] != MAXINT && weight + distance[u] < distance[w]) 
			{ 
				distance[w] = distance[u] + weight; 
				decreaseKey(mH, w, distance[w]); 
			} 
			p = p->next; 
        } 
	} 
	printArr(distance); 
}
void dijkstra_all_src(){
   int i;
   int * dist = (int *)malloc(g.nvertices * sizeof(int));  
   /* intialize mH. */
   mH = createmH(g.nvertices); 
   /* initialize each array in mH. */
   for (i=0; i< g.nvertices; i++) mH->array[i] = newMHN(i, MAXINT); 
   for (i=0; i<g.nvertices; i++) dijkstra(&g, i, dist);
   for (i=0; i<g.nvertices; i++) free(mH->array[i]);
   free(mH->pos);
   free(mH);
   free(dist);
}
int main(int argc, char **argv){
  Edgenode *e;
  FILE *file;
  char *filename;
  int u,v,w;
  int i, j;
    
  filename = argv[1];
  file = fopen(filename,"r");
  file = fopen(filename,"r");
  fscanf(file, "%d %d \n", &g.nvertices, &g.nedges);
  for (i=0;i<g.nvertices;++i) g.edges[i] = NULL;
  g.directed = TRUE;
  Edgenode *prev = NULL;
  while ( fscanf(file, "%d %d %d \n", &u, &v, &w) != EOF ){
    Edgenode *new_node = (Edgenode *) malloc(sizeof(Edgenode));          
    if (g.edges[u] == NULL){            
      g.edges[u]          = new_node;
      g.edges[u]->y       = v;
      g.edges[u]-> weight = w;
      g.edges[u]->next    = NULL;
      prev = new_node;
    }
    else{                                
      prev->next          = new_node;
      new_node->y         = v;
      new_node->weight    = w;
      new_node->next      = NULL;
      prev                = new_node;
    }
  }
  fclose(file);
 double	start = omp_get_wtime();
 dijkstra_all_src();
 double stop = omp_get_wtime();
 printf("Time %.6f \n", stop - start);
 
} 
