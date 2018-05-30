#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INFINITY 999999


// ########## DijkstraHelperPrototypes ##########
static PredNode* recordVertex (PredNode *list, int v);
static PredNode* newPredNode (int v);
static bool isAlreadyVisited (PredNode *list, int v);
static void freePredList (PredNode *list);
static void showPredList (PredNode *list);


// ########## DijkstraImplementation ##########
ShortestPaths dijkstra(Graph g, Vertex src) {
    int nV = numVerticies(g);

    // Preparing ShortestPaths
    ShortestPaths paths;
    paths.noNodes = nV;
    paths.src = src;
    paths.dist = malloc(nV * sizeof(int));
    paths.pred = malloc(nV * sizeof(PredNode*));

    int i;
    for (i = 0; i < nV; i++) {
        // Set all distances to a 'infinite'
        paths.dist[i] = INFINITY;
        // Set all preds pointer to NULL
        paths.pred[i] = NULL;
    }

    // Set source distance to zero
    paths.dist[src] = 0;

    // Create new PQ
    PQ myPQ = newPQ();
    ItemPQ temp;

    printf("Source Node: %d\n", src);

    // Add src to PQ
    ItemPQ currItem;
    currItem.key = src;
    currItem.value = paths.dist[src];
    addPQ(myPQ, currItem);
    
    // Dijsktra's Algorithm Implementation w/ prepared Priority Queue
    while (!PQEmpty(myPQ)) {
        currItem = dequeuePQ(myPQ);
        printf("Just dequeued from myPQ\n");
        printf("Item.key = %d, Item.value = %d\n", currItem.key, currItem.value);

        // Get list of out edges from current vertex
        AdjList currAdjList = outIncident(g, currItem.key);
        printf("Just gathered outIncident from dequeued item\n");
        
        // Inpect the whole list
        int j = 0;
        while (currAdjList != NULL) {
            printf("Inspecting the adjList - %d\n", j++);
        
            // Look into the current Vertex's past and check if w isn't in there
            if (!isAlreadyVisited(paths.pred[currItem.key], currAdjList->w)) {
                printf("Checked and entered isAlreadyVisited\n");
                if (currItem.value + currAdjList->weight < paths.dist[currAdjList->w]) {
                    printf("Checked if newWeight is less than what is recorded in dist array\n");
                    // Update dist array record
                    paths.dist[currAdjList->w] = currItem.value + currAdjList->weight;
                    printf("Just updated paths.dist[currAdjList->w] == %d\n", paths.dist[currAdjList->w]);

                    // Update predNode list of w
                    paths.pred[currAdjList->w] = recordVertex(paths.pred[currItem.key], currItem.key);
                    printf("Just updated the pred node path\n");
                    showPredList(paths.pred[currAdjList->w]);
                }

                // Add vertex to priority Queue
                temp.key = currAdjList->w;
                temp.value = paths.dist[temp.key];
                printf("Item to add to Queue - temp.key = %d, temp.value = %d\n", temp.key, temp.value);
                
                addPQ(myPQ, temp);
            }

        currAdjList = currAdjList->next;
        }
    }

    return paths;
}

void  showShortestPaths(ShortestPaths paths) {
    printf("noNodes: %d\n", paths.noNodes);
    printf("src: %d\n", paths.src);
    int i;
    for (i = 0; i < paths.noNodes; i++) {
        printf("(Vertex: %d, Dist: %d) ", i, paths.dist[i]);
    }
    
    PredNode *curr;
    for (i = 0; i < paths.noNodes; i++) {
        curr = paths.pred[i];
        printf("PredNode for %d: ", i);
        while (curr != NULL) {
            printf("%d <- ", curr->v);
        }
        printf("\n");
    }
}

void  freeShortestPaths(ShortestPaths paths) {
    int i;
    for (i = 0; i < paths.noNodes; i++) {
        freePredList(paths.pred[i]);
    }
}


// ########## DijsktraHelperImplementation ##########
static PredNode* newPredNode (int v) {
    PredNode *new = malloc(sizeof(PredNode));
    new->v = v;
    new->next = NULL;

    return new;
}

static PredNode* recordVertex (PredNode *list, int v) {
    PredNode *new = newPredNode(v);
    new->next = list;

    return new;
}

static bool isAlreadyVisited (PredNode *list, int v) {
    if (list == NULL) {
        return false;
    }
    if (list->v == v) {
        return true;
    }
    
    return isAlreadyVisited(list->next, v);
}

static void freePredList (PredNode *list) {
    if (list != NULL) {
        freePredList(list->next);
        free(list);
    }
}

static void showPredList (PredNode *list) {
    printf("Printing PredList => ");
    while (list != NULL) {
        printf("%d -> ", list->v);
        list = list->next;
    }
    printf("\n");
}
