
#include "node.h"
#include "graph.h"
#include <errno.h>
#include <string.h>

#define MAX_NODES   4096
#define MAX_LINE    1000

enum {NO_FILE_POS_VALUES = 2};

struct _Graph {
  Node * nodes[MAX_NODES];
  Bool connections[MAX_NODES][MAX_NODES];
  int num_nodes;
  int num_edges;
};

Status graph_readFromFile (FILE *fin, Graph *g);
int find_node_index(const Graph * g, int nId1);
int* graph_getConectionsIndex(const Graph * g, int index);


Graph * graph_ini()
  {
    Graph * g=NULL;
    int i;

    g=(Graph *)malloc(sizeof(Graph));
      if (!g)
        return NULL;
for(i=0;i<MAX_NODES;i++){
    g->nodes[i]=NULL;
  }
    g->num_nodes=0;
    g->num_edges=0;

    return g;
  }


void graph_destroy(Graph * g)
{
  int i;
  if (!g)
    return;

  for(i=0; i<g->num_nodes;i++){
    node_destroy(g->nodes[i]);
  }

    free(g);
}


Status graph_insertNode(Graph * g, const Node* n)
{
  int i;

  if (!g || !n) return ERROR;

  if(g->num_nodes > 0){
    for(i=0;i<g->num_nodes;i++){
      if(node_getId(n)==node_getId(g->nodes[i]))  return ERROR;
    }
  }
    g->nodes[g->num_nodes]=node_copy(n);
    if(!g->nodes[g->num_nodes]) return ERROR;
    g->num_nodes++;

    return OK;
}



Status graph_insertEdge(Graph *g, const int nId1, const int nId2){
	int a;
  int b;

	if (!g || nId1<0 || nId1<0) return ERROR;

	a = find_node_index(g, nId1);
	b = find_node_index(g, nId2);

	if (a == -1 || b == -1){
		fprintf(stderr, "Error al buscar el indice del nodo.\n");
		return ERROR;
	}

	if (g->connections[a][b] == TRUE){
		fprintf(stderr, "Error: Ya conectados.\n");
		return ERROR;
	}

	(g->num_edges)++;
	g->connections[a][b] = TRUE;

	b = node_getConnect(g->nodes[a]);
	if (b == -1){
		fprintf(stderr, "Error al obtener el numero de conexiones del nodo\n");
		(g->num_edges)--;
		return ERROR;
	}
	if(!node_setConnect(g->nodes[a], b+1)){
		fprintf(stderr, "Error al modificar el numero de conexiones del nodo\n");
		(g->num_edges)--;
		return ERROR;

	}

	return OK;
}



Node *graph_getNode (const Graph *g, int nId)
{
  Node * n=NULL;

  if (!g ||  nId<0) return NULL;

  n=node_copy(g->nodes[nId]);
    if(!n)
      return NULL;

  return n;
}

/* Actualiza el nodo del grafo que tiene el mismo id que el nodo n, con la
información de n */
Status graph_setNode (Graph *g, const Node *n)
{
  int id;

  if (!g || !n) return ERROR;

  id=node_getId(n);
  g->nodes[id]=(Node *) n;

  return OK;
}


int * graph_getNodesId (const Graph * g)
{
  int * nid;
  int i;

  if (!g)
    return NULL;

  nid=(int *)malloc(g->num_nodes*sizeof(int));
    if (!nid) return NULL;

  for(i=0;i<g->num_nodes;i++){
  nid[i] = node_getId(g->nodes[i]);
  }

  return nid;
}


int graph_getNumberOfNodes(const Graph * g)
{
  if (!g) return ERROR;

  return g->num_nodes;
}

int graph_getNumberOfEdges(const Graph * g)
{
  if (!g)
    return ERROR;

  return g->num_edges;
}

/* Determina si dos nodos están conectados*/
Bool graph_areConnected(const Graph * g, const int nId1, const int nId2)
{
  int a;
  int b;

    if (!g || nId1<0 || nId2 <0 )
    return FALSE;

      a = find_node_index(g, nId1);
      b = find_node_index(g, nId2);
      if(a<0 || b<0){
        fprintf(stderr, "Error al buscar el indice del nodo. \n");
        return FALSE;
      }

    return g->connections[a][b];
}


/* Devuelve el número de conexiones del nodo de id fromId */
int graph_getNumberOfConnectionsFrom(const Graph * g, const int fromId)
{
  int ncon;
  int i;

    if (!g || fromId<0)
      return ERROR;

    for (i=0, ncon=0;i<g->num_nodes;i++){
      if (graph_areConnected(g, fromId, i)==TRUE){
        ncon++;
      }
    }

    return ncon;
}

/* Devuelve la dirección de un array con los id de todos los nodos del grafo.
* Reserva memoria para el array. */
int* graph_getConnectionsFrom(const Graph * g, const int fromId)
{
  int *array = NULL, i, j=0, size;
  if (!g) return NULL;
  if (fromId < 0 || fromId >g->num_nodes) return NULL;

  size = node_getConnect (g->nodes[find_node_index(g,fromId)]);
  array = (int *) malloc(sizeof(int) * size);
  if (!array) {

  fprintf (stderr, "%s\n", strerror(errno));
  return NULL;
  }

  for(i = 0; i< g->num_nodes; i++) {
    if (g->connections[find_node_index(g,fromId)][i] == TRUE) {
      array[j] = node_getId(g->nodes[i]);
      j++;
    }
  }
return array;
}


/*
* Imprime en el flujo pf los datos de un grafo, devolviendo el número de
* caracteres impresos.
* Comprueba si ha habido errorres en el flujo de salida. Si es así imprime
* mensaje
* de error en stderror y devuelve el valor -1.
* El formato a seguir es: imprimir una linea por nodo con la información asociada
* al nodo y
* los id de sus conexiones. La salida para el grafo del ejercicio 2.3 de la parte
* 1 es:
* [1, a, 2] 2 3
* [2, b, 2] 1 3
* [3, c, 2]] 1 2
*/
int graph_print(FILE *pf, const Graph *g){
  int ret = 0;
  int i, j, a, node_id,n_id,n_connected;
  char *n_name = NULL;
  int *conn = NULL;

  if(!pf || !g)
    return -1;

  for(i = 0; i < g->num_nodes; i++){
    n_id = node_getId(g->nodes[i]);
    n_name = node_getName(g->nodes[i]);
    n_connected = node_getConnect(g->nodes[i]);

    ret += fprintf(pf, "[%d, %s, %d]",n_id,n_name,n_connected);

    a = find_node_index(g, node_getId(g->nodes[i]));
    conn = graph_getConectionsIndex(g, a);
    for(j=0;j < node_getConnect(g->nodes[i]) ; j++){
      node_id = node_getId(g->nodes[conn[j]]);
      ret += fprintf(pf, " %d", node_id);
    }
    fprintf(pf, "\n");

    free(conn);
  }

  if(ret<1){
    fprintf(stderr, "Error al imprimir\n");
    return -1;
  }

  return ret;
}

/* Lee de un flujo de entrada la información asociada a un grafo */
Status graph_readFromFile (FILE *fin, Graph *g)
{
  Node *n;
  char buff[MAX_LINE], name[MAX_LINE];
  int i, nnodes = 0, id1, id2;
  Status flag = ERROR;

  if ( fgets (buff, MAX_LINE, fin) != NULL)
  if ( sscanf(buff, "%d", &nnodes) != 1) return ERROR;

  n = node_ini();
  if (!n) return ERROR;

  for(i=0; i < nnodes; i++) {
    if ( fgets(buff, MAX_LINE, fin) != NULL)
    if (sscanf(buff, "%d %s", &id1, name) != NO_FILE_POS_VALUES) break;

    node_setName (n, name);
    node_setId (n, id1);

    if ( graph_insertNode (g, n) == ERROR) break;
  }

  if (i < nnodes) {
    node_destroy(n);
    return ERROR;
  }

  while ( fgets(buff, MAX_LINE, fin) ){
    if ( sscanf(buff, "%d %d", &id1,&id2) == NO_FILE_POS_VALUES )
      if (graph_insertEdge(g, id1,id2) == ERROR) break;
    }

    if (feof(fin)) flag = OK;

    node_destroy (n);
    return flag;
  }
int find_node_index(const Graph * g, int nId1)
{
  int i;
  if (!g) return -1;
  for(i=0; i < g->num_nodes; i++) {
    if (node_getId (g->nodes[i]) ==  nId1) return i;
  }


  return -1;
}

int* graph_getConectionsIndex(const Graph * g, int index)
{
  int *array = NULL, i, j=0, size;
  if (!g) return NULL;
  if (index < 0 || index >g->num_nodes) return NULL;

  size = node_getConnect (g->nodes[index]);
  array = (int *) malloc(sizeof(int) * size);
  if (!array) {

  fprintf (stderr, "%s\n", strerror(errno));
  return NULL;
  }

  for(i = 0; i< g->num_nodes; i++) {
    if (g->connections[index][i] == TRUE) {
      array[j] = i;
      j++;
    }
  }
return array;
}
