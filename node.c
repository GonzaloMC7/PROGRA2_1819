/*FILE: node.c
  Author: Gonzalo Martínez
  Description: functions for the node Tad are implemented in this file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"


struct _Node{
    char name[MAX];
    int id;
    int nConnect;
};

/*--------------------------------------------------------------------------------*/

Node * node_ini(){
    Node * n=NULL;
    
    n=(Node*)malloc(sizeof(Node));
    if(!n) return NULL;
    
    n->name[MAX]='\0';
    n->id=0;
    n->nConnect=0;
    
    return n;
}

/*--------------------------------------------------------------------------------*/

void node_destroy(Node * n){
    if(!n) return;
    
    free(n);
}

/*--------------------------------------------------------------------------------*/

int node_getId(const Node * n){
    if(!n) return -1;
    
    return n->id;
}

/*--------------------------------------------------------------------------------*/

char* node_getName(const Node * n){
    if(!n) return NULL;
    
    return (char*)n->name;
}

/*--------------------------------------------------------------------------------*/

int node_getConnect(const Node * n){
    if(!n) return -1;
    
    return n->nConnect;
}

/*--------------------------------------------------------------------------------*/

Node * node_setId(Node * n, const int id){
    if(!n) return NULL;
    
    n->id=id;
    
    return n;
}

/*--------------------------------------------------------------------------------*/

Node * node_setName(Node * n, const char* name){
    if(!n) return NULL;
    
    strcpy(n->name,name);
    
    return n;
}

/*--------------------------------------------------------------------------------*/

Node * node_setConnect(Node * n, const int cn){
    if(!n) return NULL;
    
    n->nConnect=cn;
    
    return n;
}

/*--------------------------------------------------------------------------------*/

int node_cmp(const Node * n1, const Node * n2){
    if(!n1) return -1;
    if(!n2) return -1;
    
    if(n1->id==n2->id){
        return 0;
    }else if(n1->id<n2->id){
        return -2;
    }else return 1;
    
}

/*--------------------------------------------------------------------------------*/

Node * node_copy(const Node * src){
    Node * aux = NULL;
    
    if(!src) return NULL;
    
    aux=node_ini();
    if(!aux) return NULL;
    
    
    /*CONTROL DE ERRORES??*/
    node_setName(aux,src->name);
    node_setId(aux,src->id);
    node_setConnect(aux,src->nConnect);
    
    /*strcpy(aux->name,src->name);
    aux->id=src->id;
    aux->nConnect=src->nConnect;*/
    
    return aux;
}

/*--------------------------------------------------------------------------------*/

int node_print(FILE *pf, const Node * n){
    int ncar=0;
    
    if(!n) return -1;
    
    ncar+=fprintf(pf,"[Id:%d, Name:%s, NConnect: %d]\n",node_getId(n),node_getName(n),node_getConnect(n));
    if(ncar<=0){
        fprintf(stderr,"Error al imprimir\n");
    }
    
    return ncar;
}






