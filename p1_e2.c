#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "graph.h"

int main(int argc, char const *argv[])
{


	Node *node1 = NULL, *node2 = NULL;
	Graph * g1 = NULL;


 	node1 = node_ini();
  	if (!node1) {
    	fprintf(stderr, "Error al reservar memoria para el nodo 1\n");
    return -1;
  	}

	node2 = node_ini();
	if (!node2) {
	fprintf(stderr, "Error al reservar memoria para el nodo 2\n");
	node_destroy(node1);
	return -1;
	}

	node_setId(node1, 111);
	node_setId(node2, 222);
	node_setName(node1, "first");
	node_setName(node2, "second");


	g1 = graph_ini();
	if(!g1){
		fprintf(stderr, "Error al reservar memoria para el grafo\n" );
		return -1;
	}


	fprintf(stderr, "Insertando nodo 1...resultado...:%d\n", graph_insertNode(g1,node1));

	fprintf(stderr, "Insertando nodo 2...resultado...:%d\n", graph_insertNode(g1,node2));

  fprintf(stderr, "\n");


	fprintf(stderr, "Insertando edge: nodo 2 ---> nodo 1\n");
	if(!graph_insertEdge(g1,222,111)){
		fprintf(stderr, "No se ha podido realizar la conexi�n entre nodo 1 y nodo 2\n");
		return -1;
	}
  fprintf(stderr, "\n");



	fprintf(stderr, "Conectados nodo 1 y nodo 2? ");
	if(!graph_areConnected(g1,111,222)){
		fprintf(stderr, "No\n");
	}
	else
		fprintf(stderr, "Si\n");


	fprintf(stderr, "Conectados nodo 2 y nodo 1? ");
	if(!graph_areConnected(g1,222,111)){
		fprintf(stderr, "No\n");
	}
	else
		fprintf(stderr, "Si\n");
    fprintf(stderr, "\n");

	fprintf(stderr, "Insertando nodo 2...resultado...:%d\n", graph_insertNode(g1,node2));
  fprintf(stderr, "\n");

	fprintf(stderr, "Grafo\n");

	graph_print(stdout,g1);
	node_destroy(node1);
	node_destroy(node2);
	graph_destroy(g1);



	return 0;
}
