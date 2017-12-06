#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXV 100
#define ALICETURN 1
#define BOBTURN 0
#define INF 65534

//for graph
typedef struct node{
	struct node *next;
	int vertex_number;
} Node;
//for color set C
typedef struct p{
	int c[MAXV];
	int n_of_colors;
} Colors;
//proof number
typedef struct q{
	int pn;
	int dp;
} PNs;


Node* initialize_list();
void initialize_graph(Node* graph[],int n_of_vertices);
void initialize_matrix_graph(int matrix_graph[][MAXV]);
void insert_edge(Node* graph[],int begin_of_edge,int end_of_edge);
void print_matrix_graph(int matrix_graph[][MAXV],int n_of_vertices);
void read_matrix_graph(Node* graph[],int matrix_graph[][MAXV],FILE *fp,int n_of_vertices);
int is_edge(Node* graph[],int begin_of_edge,int end_of_edge);
void make_graph(Node* graph[],int matrix_graph[][MAXV],int n_of_vertices);
int search_kai_g(Node* graph[],int n_of_vertices,int turn,Colors colors);
int is_colored(Colors colors,int v);
int is_share_color(Node *graph[],Colors colors,int v,int color);
int finish_game(Colors colors,int n_of_vertices);
void print_colors(int turn,Colors colors,int n_of_vertices);
void print_list(Node *graph[],int n_of_vertices);
int count_choices(Node* graph[],int n_of_vertices,Colors colors);
int df_pn(Node* graph[],int n_of_vertices,int turn,Colors colors);
PNs df_pn_step(Node* graph[],int n_of_vertices,int turn,Colors colors,int th);

int main(int argc, char *argv[]){
	clock_t start,end;
	Node *graph[MAXV];
	int matrix_graph[MAXV][MAXV];
	int n_of_vertices=atoi(argv[2]);
	int i,j;
	Colors colors;
	start=clock();
	for(i=0;i<MAXV;i++)colors.c[i]=MAXV;
	colors.n_of_colors=atoi(argv[3]);
	initialize_graph(graph,n_of_vertices);
	initialize_matrix_graph(matrix_graph);
	
	FILE *fp=fopen(argv[1],"r");
	if(fp==NULL){fprintf(stderr, "error: the file is not exist.");return EXIT_FAILURE;}
	
	read_matrix_graph(graph,matrix_graph,fp,n_of_vertices);
	make_graph(graph,matrix_graph,n_of_vertices);
	print_list(graph,n_of_vertices);
	printf("%d colors\n",colors.n_of_colors);
	printf("------------------------------\n");
	printf("initialized colors\n");
	print_colors(MAXV,colors,n_of_vertices);
	printf("------------------------------\n");
	//if(search_kai_g(graph,n_of_vertices,ALICETURN,colors)==1)printf("ALICE WIN\n");else printf("BOB WIN\n");
	if(df_pn(graph,n_of_vertices,ALICETURN,colors)==1)printf("ALICE WIN\n");else printf("BOB WIN\n");
	end=clock();
	printf("経過時間：%d[ms]\n",end-start);
	return 0;
}



Node* initialize_list(){ //各リストを初期化する関数
	Node* n;
	n=(Node*) malloc(sizeof(Node)); 
	n->next = NULL;
	return n;
}

void initialize_graph(Node *graph[], int n_of_vertices){ //グラフを初期化する関数
	int i;
	for(i=0;i<n_of_vertices;i++){
		graph[i]=initialize_list();	
	}
}

void initialize_matrix_graph(int matrix_graph[][MAXV]){
	int i,j;
	for(i=0;i<MAXV;i++){
		for(j=0;j<MAXV;j++){
			matrix_graph[i][j]=0;
		}
	}
}

void print_matrix_graph(int matrix_graph[][MAXV],int n_of_vertices){
	int i,j;
	for(i=0;i<n_of_vertices;i++){
		for(j=0;j<n_of_vertices;j++){
			printf("%d ",matrix_graph[i][j]);
		}
		printf("\n");
	}
}

void read_matrix_graph(Node* graph[],int matrix_graph[][MAXV],FILE *fp,int n_of_vertices){
	int i,j;
	for(i=0;i<n_of_vertices;i++){
		for(j=0;j<n_of_vertices;j++){
			fscanf(fp,"%d",&matrix_graph[i][j]);
		}
	}
	printf("loaded graph\n");
	print_matrix_graph(matrix_graph,n_of_vertices);
	printf("------------------------------\n");
}
				
void insert_edge(Node* graph[],int begin_of_edge,int end_of_edge){
	Node *n1,*n2;

	if(is_edge(graph,begin_of_edge,end_of_edge)==1) return;
	
	
	n1=(Node*) malloc(sizeof(Node));
	n2=(Node*) malloc(sizeof(Node));
	
	n1->vertex_number = begin_of_edge;
	n2->vertex_number = end_of_edge;
	
	n2->next = graph[begin_of_edge]->next;
	graph[begin_of_edge]->next = n2;

	n1->next = graph[end_of_edge]->next;
	graph[end_of_edge]->next = n1;
}

int is_edge(Node* graph[],int begin_of_edge,int end_of_edge){
	Node* tmp=graph[begin_of_edge];
	if(tmp->next==NULL)return 0;
	else tmp=tmp->next;

	while(tmp!=NULL){
		if(tmp->vertex_number == end_of_edge) return 1;	
		tmp = tmp->next;
	}
	return 0;
}

void make_graph(Node* graph[],int matrix_graph[][MAXV],int n_of_vertices){
	int i,j;
	for(i=0;i<n_of_vertices;i++){
		for(j=0;j<n_of_vertices;j++){
			if(matrix_graph[i][j]!=0)insert_edge(graph,i,j);
		}
	}
	return;
}

int search_kai_g(Node* graph[],int n_of_vertices,int turn,Colors colors){
	int i,j,k,ctmp;
	print_colors(turn,colors,n_of_vertices);
	if(finish_game(colors,n_of_vertices))return 1;
	if((turn%2)==ALICETURN){
		k=0;
		for(i=0;i<colors.n_of_colors;i++){
			for(j=0;j<n_of_vertices;j++){
				if(is_colored(colors,j) || is_share_color(graph,colors,j,i))continue;
				ctmp=colors.c[j];
				colors.c[j]=i;
				if(search_kai_g(graph,n_of_vertices,turn+1,colors)==1)k=1;
				colors.c[j]=ctmp;
			}
			if(turn=1)break;
		}
	}
	if((turn%2)==BOBTURN){
		k=1;
		for(i=0;i<colors.n_of_colors;i++){
			for(j=0;j<n_of_vertices;j++){
				if(is_colored(colors,j) || is_share_color(graph,colors,j,i))continue;
				ctmp=colors.c[j];
				colors.c[j]=i;
				if(search_kai_g(graph,n_of_vertices,turn+1,colors)==0)k=0;
				colors.c[j]=ctmp;
			}
		}
	}

	if(k==1)return 1;
	else return 0;
}

int df_pn(Node* graph[],int n_of_vertices,int turn,Colors colors){
	PNs pns;
	int th=0;
	while(pns.pn!=0 && pns.pn!=INF){
		printf("th=%d\n",th);//debugcode
		pns=df_pn_step(graph,n_of_vertices,turn,colors,th);
		th++;
		/////////////////////this has to be checked////////////////
		//if(th==(n_of_vertices*colors.n_of_colors))break;//debugcode
		///////////////////////////////////////////////////////////
	}
	if(pns.pn==0)return 1;
	else return 0;
}

PNs df_pn_step(Node* graph[],int n_of_vertices,int turn,Colors colors,int th){
	int i,j,k,l=0,ctmp;
	Colors tmpcolors;
	PNs pns,tmppns;
	//printf("turnend-----------\n");//debugcode
	if(finish_game(colors,n_of_vertices)){
		pns.pn=0;
		pns.dp=INF;
		return pns;
	}
	if(turn%2==ALICETURN){
		//printf("%d Alice turn-------\n",turn);//debugcode
		pns.pn=INF+1;
		pns.dp=0;
		for(i=0;i<colors.n_of_colors;i++){
			for(j=0;j<n_of_vertices;j++){
				if(is_colored(colors,j) || is_share_color(graph,colors,j,i))continue;
				l++;
				tmpcolors=colors;
				tmpcolors.c[j]=i;
				k=count_choices(graph,n_of_vertices,tmpcolors);

				//printf("k:%d\n",k);//debugcode
				if(k<th){
					//printf("hogehoge\n");//debugcode
					ctmp=colors.c[j];
					colors.c[j]=i;
					tmppns=df_pn_step(graph,n_of_vertices,turn+1,colors,th);
					//printf("return:%d pn:%d dp:%d pnspn:%d\n",turn,tmppns.pn,tmppns.dp,pns.pn);//debugcode
					if(pns.pn>tmppns.pn)pns.pn=tmppns.pn;
					pns.dp+=tmppns.dp;
					if(pns.dp>INF)pns.dp=INF;
					colors.c[j]=ctmp;
					//printf("pn=%d,dp=%d\n",pns.pn,pns.dp);//debugcode
				}
			}
			if(turn==1)break;
		}	
		if(l==0){
				//printf("last leaf\n");//debugcode
				pns.pn=INF+1;
				pns.dp=0;
				//printf("pn:%d,dp:%d\n",pns.pn,pns.dp);//debugcode
				return pns;
		}

	}
	if(turn%2==BOBTURN){
		//printf("%d Bob turn-----------\n",turn);//debugcode
		pns.pn=0;
		pns.dp=INF;
		for(i=0;i<colors.n_of_colors;i++){
			for(j=0;j<n_of_vertices;j++){
				if(is_colored(colors,j) || is_share_color(graph,colors,j,i))continue;
				l++;
				tmpcolors=colors;
				tmpcolors.c[j]=i;
				k=count_choices(graph,n_of_vertices,tmpcolors);
				//printf("k:%d\n",k);//debugcode
				if(k<th){
					//printf("fugafuga\n");//debugcode
					ctmp=colors.c[j];
					colors.c[j]=i;
					tmppns=df_pn_step(graph,n_of_vertices,turn+1,colors,th);
					//printf("return:%d pn:%d dp:%d\n",turn,tmppns.pn,tmppns.dp);//debugcode
					if(pns.dp>tmppns.dp)pns.dp=tmppns.dp;
					pns.pn+=tmppns.pn;
					if(pns.pn>INF)pns.pn=INF;
					colors.c[j]=ctmp;
					//printf("pn=%d,dp=%d\n",pns.pn,pns.dp);//debugcode

				}
			}	
		}	
		if(l==0){
			//printf("last leaf\n");//debugcode
			pns.pn=INF;
			pns.dp=0;
			//printf("pn:%d,dp:%d\n",pns.pn,pns.dp);//debugcode
			return pns;
		}

	}
	return pns;
}

int count_choices(Node* graph[],int n_of_vertices,Colors colors){
	int i,j,c=0;
	for(i=0;i<colors.n_of_colors;i++){
		for(j=0;j<n_of_vertices;j++){
			if(is_colored(colors,j) || is_share_color(graph,colors,j,i))continue;
			c++;
		}
	}
	return c;
}

int finish_game(Colors colors,int n_of_vertices){
	int i;
	for(i=0;i<n_of_vertices;i++){
		if(colors.c[i]==MAXV)return 0;
	}
	return 1;
}

int is_share_color(Node *graph[],Colors colors,int v,int color){
	Node *tmp;
	for(tmp=graph[v]->next;tmp!=NULL;tmp=tmp->next){
		if(colors.c[tmp->vertex_number]!=MAXV && color==colors.c[tmp->vertex_number])return 1;
	}
	return 0;
}

int is_colored(Colors colors,int v){
	if(colors.c[v]!=MAXV)return 1;
	return 0;
}

void print_colors(int turn,Colors colors,int n_of_vertices){
	int i;
	printf("turn: %d\n",turn);
	for(i=0;i<n_of_vertices;i++){
		printf("%d: %d\n",i,colors.c[i]);
	}
	printf("\n");
}

void print_list(Node *graph[],int n_of_vertices){
	Node *tmp;int i;
		printf("loaded linked list graph -----------------\n");
	for(i=0;i<n_of_vertices;i++){
		printf("%d: ",i);
		for(tmp=graph[i]->next;tmp!=NULL;tmp=tmp->next){
			printf("%d ",tmp->vertex_number);
		}
		printf("\n");
	}
		printf("------------------------------\n");

}
