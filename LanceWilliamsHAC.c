#include "LanceWilliamsHAC.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define EMPTY -1
#define UNIMPORTANT -1

#define alphai 0.5
#define alphaj 0.5
#define beta 0
#define gamma 0.5

#define SINGLE 0
#define COMPLETE 1


// ########## HelperPrototypes ##########
static Dendrogram mergeDeno (int vertex, Dendrogram left, Dendrogram right);
static double LanceWilliamsFormula (int method, double distIK, double distJK);


// ########## LanceWilliamsHACImplementation ##########
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    int nV = numVerticies(g);

    // Variables to hold the information relating to the closest clusters
    Vertex ci;
    Vertex cj;
    double minDistance = INFINITY;

    // Create a distance Matrix
    // Matrix will hold the distances between clusters
    // At the beginning, every vertex is its only cluster
    double distMatrix[nV][nV];

    // Hence,
    // Get direct distances between all Edges
    AdjList currAdjList;
    int i;
    for (i = 0; i < nV; i++) {
        // Set all direct edge i -> j as INFINITY
        int j;
        for (j = 0; j < nV; j++) {
            distMatrix[i][j] = INFINITY;
        }

        // Set the weights of existing direct edges with i -> j
        currAdjList = outIncident(g, i);
        while (currAdjList != NULL) {
            // distance = 1/wt where wt is the weight between i and currAdjList->w
            double weight = currAdjList->weight;
            double distance = 1 / weight;

            // if INFINITY, nothing has been added there - add edge weight
            if (distMatrix[i][currAdjList->w] == INFINITY) {
                distMatrix[i][currAdjList->w] = distance;
            // 1/larger < 1/smaller
            // so if distance < what is stored in distance matrix, then store distance (larger edge weight)
            } else if (distance < distMatrix[i][currAdjList->w]) {
                distMatrix[i][currAdjList->w] = distance;
            }

            // Is this the closest cluster distance?
            // if so, stored vertices for later use
            if (distance < minDistance) {
                minDistance = distance;
                ci = i;
                cj = j;
            }

            currAdjList = currAdjList->next;
        }
    }


    // At this point, the distance Matrix is complete
    // ci and cj also stored the vertices to the closest clusters

    // Array to contain Dendrograms
    Dendrogram dendA[nV];
    for (i = 0; i < nV; i++) {
        // Create new dendrograms for each vertex
        dendA[i] = malloc(sizeof(DNode));
        dendA[i]->vertex = i;
        dendA[i]->left = NULL;
        dendA[i]->right = NULL;
    }


    // LanceWilliamsHAC Algorithm must go through nV iterations until one dendogram is created
    // At the end of this loop, only 1 denogram should remain
    int iterations;
    for (iterations = 0; iterations < nV; iterations++) {
        // make ci the min and cj the max
        if (cj < ci) {
            int temp = cj;
            cj = ci;
            ci = temp;
        }



        // We have to 'remove' the two denogram clusters in dendA
        // Free the denogram and fill cj with a NULL space
        // input the new denogram into ci space
        Dendrogram new = mergeDeno(UNIMPORTANT, dendA[ci], dendA[cj]);


        // Input new denogram in ci spot
        dendA[ci] = new;
        // Fill cj spot with NULL - Should never be accessed until end of iteration loop
        dendA[cj] = NULL;

        // Update distMatrix with new distances
        // Need: distance between ci and k and distance between cj and k
        // Need: distance between ci and cj
        // Note: After loop completes, all cj distMatrix values will be assigned EMPTY
        // ci represents the dist between all other nodes and the new cluster
        int k;
        for (k = 0; k < nV; k++) {
            // Ignore all 'Empty' distMatrix values
            // We know ci, or cj currently is not empty
            // distMatrix[k][ci] is empty, then all k rows have been assigned empty (hence ignore)
            if (distMatrix[k][ci] != EMPTY) {
                // Assign new cluster distance
                double distIK = distMatrix[ci][k];
                double distJK = distMatrix[cj][k];

                distMatrix[k][ci] = LanceWilliamsFormula(method, distIK, distJK);
            }
        }

        // At this point, we have updated the new cluster distance with all other non-empty nodes
        // make all of cj empty



        for (k = 0; k < nV; k++) {

            distMatrix[k][cj] = EMPTY;


        }

        // Find new closest clusters
        minDistance = INFINITY;
        int x = 0;                       // search through half of the array
        for (i = 0; i < nV; i++) {
            int j;
            for (j = x; j < nV; j++, x++) {
                if (distMatrix[i][j] != EMPTY) {
                    if (distMatrix[i][j] < minDistance) {
                        minDistance = distMatrix[i][j];
                        ci = i;
                        cj = j;
                    }
                }
            }
        }

        // We have a new min distance between to closest clusters
        // we've merged two clusters
        // We've updated distances with LanceWilliamsFormula
        // iterate loop again
    }

    // At this point, we should only have one denogram left in dendA
    for (i = 0; i < nV; i++) {
        if (dendA[i] != NULL) {
            return dendA[i];
        }
    }

    // return NULL if failure
    return NULL;
}

void freeDendrogram(Dendrogram d) {
    if (d != NULL) {
        freeDendrogram(d->right);
        freeDendrogram(d->left);
        free(d);
    }

}


// ########## HelperImplementation ##########
static Dendrogram mergeDeno (int vertex, Dendrogram left, Dendrogram right) {
    Dendrogram new = malloc(sizeof(DNode));
    new->vertex = vertex;
    new->left = left;
    new->right = right;

    return new;
}

static double LanceWilliamsFormula (int method, double distIK, double distJK) {
    // Check if valid method given
    assert(method == SINGLE || method == COMPLETE);

    // Apply LanceWilliamsFormula according to the wanted method
    double newDistance = 0;
    if (method == SINGLE) {
        if (distIK < distJK) {
            newDistance = distIK;
        } else {
            newDistance = distJK;
        }
    } else if (method == COMPLETE) {
        if (distIK > distJK) {
            newDistance = distIK;
        } else {
            newDistance = distJK;
        }
    }

    return newDistance;
}
