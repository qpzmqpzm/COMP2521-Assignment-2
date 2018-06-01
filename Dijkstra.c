#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INFINITY 999999


// ########## DijkstraHelperPrototypes ##########
static PredNode* newPredNode (int v);
static void freePredList (PredNode *list);


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
        // Set all distances to a 'infinity'
        paths.dist[i] = INFINITY;
        // Set all preds pointer to NULL
        paths.pred[i] = NULL;
    }

    // Set source distance to zero
    paths.dist[src] = 0;

    // Create new PQ
    PQ myPQ = newPQ();

    // Add src to PQ
    ItemPQ currItem;
    currItem.key = src;
    currItem.value = paths.dist[src];
    addPQ(myPQ, currItem);

    // Dijsktra's Algorithm Implementation w/ prepared Priority Queue
    while (!PQEmpty(myPQ)) {
        currItem = dequeuePQ(myPQ);

        // Get list of out edges from current vertex
        AdjList currAdjList = outIncident(g, currItem.key);

        // Inpect all edges.out
        while (currAdjList != NULL) {
            // Ignore any edge that goes back to currVertex
            // Prevents useless computations
            if (currAdjList->w != currItem.key) {
                // Distance to next vertex from currNode
                int alternateDist = currItem.value + currAdjList->weight;

                // Add next vertex if not already in PQ
                if (paths.dist[currAdjList->w] == INFINITY) {
                    ItemPQ temp;
                    temp.key = currAdjList->w;
                    temp.value = alternateDist;
                    addPQ(myPQ, temp);
                }

                // if equal, then found an alternate route to curr node w/ equal distance
                if (alternateDist == paths.dist[currAdjList->w]) {
                    // Cycle to end of list
                    PredNode *curr = paths.pred[currAdjList->w];
                    while (curr->next != NULL) {
                        curr = curr->next;
                    }

                    // Add new node to list
                    curr->next = newPredNode(currItem.key);
                // Found a better route to vertex
                } else if (alternateDist < paths.dist[currAdjList->w]) {
                    paths.dist[currAdjList->w] = alternateDist;

                    // replace predList
                    freePredList(paths.pred[currAdjList->w]);
                    paths.pred[currAdjList->w] = newPredNode(currItem.key);

                    // Update value on PQ
                    ItemPQ update;
                    update.key = currAdjList->w;
                    update.value = paths.dist[currAdjList->w];
                    // change priority of currAdjList
                    updatePQ(myPQ, update);
                }
            }

            currAdjList = currAdjList->next;
        }
    }

    // Set all values that were not reached to zero
    for (i = 0; i < nV; i++) {
        if (paths.dist[i] == INFINITY) {
            paths.dist[i] = 0;
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

static void freePredList (PredNode *list) {
    PredNode *temp;
    PredNode *curr = list;
    while (curr != NULL) {
        temp = curr->next;
        free(curr);
        curr = temp;
    }
}
