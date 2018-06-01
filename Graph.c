// Implementation of Graph ADT
// Inspiration taken from COMP2521 Lecture (Graph Structures)
// ListHelperFunctions taken and modified from COMP2521 Lecture Code ('list.c')
// Lance Young (z5184991)

#include "Graph.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

// *************** COST ANALYSIS OF GRAPH REP VARIATION *****************
// GraphRep will contain an array of listHeads, with the position in the array corresponding to the current vertex in question
// A listHead is simple struct of two AdjList pointers, inList and outList
// The inList is a linked list of all incoming edges to the vertex in question
// The outList is a linked list of all outgoing edges to the vertex in question

// Cost Analysis:
//      time analysis: Adding a node to another list => costs O(1)
//                     Deleting an edge from graph now is double, needs to be done twice => costs O(2 * E)
//                     inIncident() was O(V * V), now benefit => O(1)
//      storage Analysis: Doubles the amount of edges being stored
// With no real restrictions on storage, and with a huge benefit to time with inIncident and outIncident being
//      called upon a lot, decided it is worth implementing
// *************** COST ANALYSIS OF GRAPH REP VARIATION *****************


// ########## Structs ##########
typedef struct listHead {
    AdjList in;
    AdjList out;
} listHead;

typedef struct GraphRep {
    listHead *edges;
    int nV;     // #vertices
    int nE;     // #edges that exist (not stored for convenience)
} GraphRep;


// ########## ListHelperPrototypes ##########
static AdjList makeAdjList (int v);                                     // create AdjList(w), return
static AdjList insertAdjListNode (AdjList L, int v, int weight);        // insert @ head, return head
static AdjList deleteAdjListNode (AdjList L, int v);                    // delete node(v), return head
static bool inAdjList (AdjList L, int v);                               // true | false if in AdjList
static void showAdjList (AdjList L);
static void freeAdjList (AdjList L);


// ########## GraphHelperImplementation ##########
// check if vertex is valid in a graph
static bool validV(Graph g, Vertex v) {
   return (g != NULL && v >= 0 && v < g->nV);
}


// ########## GraphImplementation ##########
Graph newGraph(int nV) {
    assert(nV >= 0);

    Graph g = malloc(sizeof(GraphRep));
    assert(g != NULL);
    g->nV = nV;
    g->nE = 0;

    // Allocate memory for array of listHeads
    g->edges = malloc(nV * sizeof(listHead));
    assert(g->edges != NULL);

    // All listHead pointers set to NULL
    int i;
    for (i = 0; i < nV; i++) {
        g->edges[i].in = NULL;
        g->edges[i].out = NULL;
    }

   return g;
}

void  insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
   assert(g != NULL && validV(g, src) && validV(g, dest));

   // Does not check if already include -> can be more than one edge between nodes
   g->edges[src].out = insertAdjListNode(g->edges[src].out, dest, weight);

   g->edges[dest].in = insertAdjListNode(g->edges[dest].in, src, weight);

   g->nE++;

}

void  removeEdge(Graph g, Vertex src, Vertex dest) {
    assert(g != NULL && validV(g, src) && validV(g, dest));

    g->edges[src].out = deleteAdjListNode(g->edges[src].out, dest);

    // remove record of edge coming in @ dest node
    g->edges[dest].in = deleteAdjListNode(g->edges[dest].in, src);

    g->nE--;
}

bool adjacent(Graph g, Vertex v, Vertex w) {
   assert(g != NULL && validV(g,v) && validV(g,w));

   return inAdjList(g->edges[v].out, w);
}

int numVerticies(Graph g) {
    return g->nV;
}

AdjList outIncident(Graph g, Vertex v) {
    return g->edges[v].out;
}

AdjList inIncident(Graph g, Vertex v) {
    return g->edges[v].in;
}

void showGraph(Graph g) {
    assert(g != NULL);
    int i;

    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    printf("Existing Edges out: \n");
    for (i = 0; i < g->nV; i++) {
       printf("%d - ", i);
       showAdjList(g->edges[i].out);
    }

    printf("\nRecorded edges.in: \n");
    for (i = 0; i < g->nV; i++) {
        printf("%d - ", i);
        showAdjList(g->edges[i].in);
    }
}

void freeGraph(Graph g) {
    assert(g != NULL);
    int i;
    for (i = 0; i < g->nV; i++) {
        freeAdjList(g->edges[i].in);
        freeAdjList(g->edges[i].out);
    }

    free(g);
}

// ########## ListHelperImplementation ##########
static bool inAdjList (AdjList L, int v) {
    if (L == NULL) {
        return false;
    }
    if (L->w == v) {
        return true;
    }

    return inAdjList(L->next, v);
}

static AdjList insertAdjListNode (AdjList L, int v, int weight) {
    if (inAdjList(L, v))
        return L;

    // add new node at the beginning
    AdjList new = makeAdjList(v);
    new->weight = weight;
    new->next = L;
    return new;
}

static AdjList makeAdjList (int v) {
    AdjList new = malloc(sizeof(adjListNode));
    new->w = v;
    new->weight = 0;
    new->next = NULL;

    return new;
}

static AdjList deleteAdjListNode (AdjList L, int v) {
   if (L == NULL)
      return L;
   if (L->w == v)
      return L->next;

   L->next = deleteAdjListNode(L->next, v);
   return L;
}

static void showAdjList (AdjList L) {
   if (L == NULL)
      printf("\n");
   else {
      printf("(Node:%d, Weight: %d) ", L->w, L->weight);
      showAdjList(L->next);
   }
}

static void freeAdjList (AdjList L) {
   if (L != NULL) {
      freeAdjList(L->next);
      free(L);
   }
}
