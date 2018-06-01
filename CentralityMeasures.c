// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


// ########## GlobalVariable ##########
int globalPathCounter;


// ########## ListHelperPrototypes ##########
static int countEdges (AdjList list);
static NodeValues createNodeValues (Graph g);
static double sumOfShortestPathDist (ShortestPaths paths);
static double numNodesReached (ShortestPaths paths);			// Assuming reachable includes src node
static double wassermanFaust (double noReachedNodes, int totalNodes, double sum);
static void recordVerticesInPath (ShortestPaths paths, double *record, int end);



// ########## CentralityMeasuresImplementation ##########
NodeValues outDegreeCentrality(Graph g){
	NodeValues nValues = createNodeValues(g);

	int i;
	AdjList tempList;
	for (i = 0; i < nValues.noNodes; i++) {
		tempList = outIncident(g, i);
		nValues.values[i] = countEdges(tempList);
	}

	return nValues;
}

NodeValues inDegreeCentrality(Graph g){
	NodeValues nValues = createNodeValues(g);

	int i;
	AdjList tempList;
	for (i = 0; i < nValues.noNodes; i++) {
		tempList = inIncident(g, i);
		nValues.values[i] = countEdges(tempList);
	}

	return nValues;
}

NodeValues degreeCentrality(Graph g) {
	NodeValues nValues = createNodeValues(g);

	int i;
	AdjList tempList;
	int degreeSum;
	for (i = 0; i < nValues.noNodes; i++) {
		// Count inEdges
		tempList = inIncident(g, i);
		degreeSum = countEdges(tempList);

		// Count outEdges
		tempList = outIncident(g, i);
		degreeSum += countEdges(tempList);

		// Add them together and assign value
		nValues.values[i] = degreeSum;
	}

	return nValues;
}

NodeValues closenessCentrality(Graph g) {
	NodeValues nValues = createNodeValues(g);

	int i;
	ShortestPaths tempPaths;
	for (i = 0; i < nValues.noNodes; i++) {
		tempPaths = dijkstra(g, i);

		// Calculate all components of Wasserman & Faust's formula
		double noReachedNodes = numNodesReached(tempPaths);
		double totalNodes = nValues.noNodes;
		double sum = sumOfShortestPathDist(tempPaths);

		// Assign output of wassermanFaust formula to corresponding vertex
		nValues.values[i] = wassermanFaust(noReachedNodes, totalNodes, sum);
	}

	return nValues;
}

NodeValues betweennessCentrality(Graph g) {
	// Prepare nValues (set all doubles to 0)
	NodeValues nValues = createNodeValues(g);
	nValues.values = memset(nValues.values, 0, nValues.noNodes * sizeof(double));

	double pathRecord[nValues.noNodes];
	ShortestPaths tempPaths;
	int v;
	for (v = 0; v < nValues.noNodes; v++) {
		// Find all shortest paths from v to all other nodes
		tempPaths = dijkstra(g, v);

		// Inspect all other nodes
		int w;
		for (w = 0; w < nValues.noNodes; w++) {
			// Reset pathRecord to O
			memset(pathRecord, 0, nValues.noNodes * sizeof(double));

			// from w to v, use pathRecord to count all vertices encountered
			// start from end (w) and go to v
			globalPathCounter = 1;
			recordVerticesInPath(tempPaths, pathRecord, w);

			// pathRecord now contains all the information -> insert into noValues
			int i;
			for (i = 0; i < nValues.noNodes; i++) {
				nValues.values[i] += pathRecord[i] / globalPathCounter;
			}

		}
	}

	return nValues;
}

NodeValues betweennessCentralityNormalised(Graph g) {
	NodeValues nValues = betweennessCentrality(g);

	double denominator = 1/((nValues.noNodes - 1)*(nValues.noNodes - 2));

	int i;
	for (i = 0; i < nValues.noNodes; i++) {
		nValues.values[i] *= denominator;
	}

	return nValues;
}

void showNodeValues(NodeValues values) {
	int i;
	for (i = 0; i < values.noNodes; i++) {
		printf("%d: %f\n", i, values.values[i]);
	}
}

void freeNodeValues(NodeValues values) {
	free(values.values);
}


// ########## HelperImplementation ##########
static int countEdges (AdjList list) {
	int count = 0;
	AdjList curr = list;
	while (curr != NULL) {
		count++;
		curr = curr->next;
	}

	return count;
}

static NodeValues createNodeValues (Graph g) {
	int nV = numVerticies(g);

	NodeValues nodeValues;
	nodeValues.noNodes = nV;
	nodeValues.values = malloc(nV * sizeof(double));

	return nodeValues;
}

static double sumOfShortestPathDist (ShortestPaths paths) {
	double sum = 0;
	int i;
	for (i = 0; i < paths.noNodes; i++) {
		// Note distance to src = 0
		// and distance to unreachables = 0
		// Both above don't affect sum
		sum += paths.dist[i];
	}

	return sum;
}

static double numNodesReached (ShortestPaths paths) {
	double count = 1;		// include source node
	int i;
	for (i = 0; i < paths.noNodes; i++) {
		if (paths.dist[i] > 0) {
			count++;
		}
	}

	return count;
}

static double wassermanFaust (double noReachedNodes, int totalNodes, double sum) {
	double returnVal;
	if (sum == 0) {
		returnVal = 0;
	} else {
		returnVal = (noReachedNodes - 1)/ sum;
	}

	double scale;
	if (totalNodes - 1 == 0) {
		scale = 0;
	} else {
		scale = (noReachedNodes - 1)/(totalNodes - 1);
	}

	return scale * returnVal;
}

static void recordVerticesInPath (ShortestPaths paths, double *record, int end) {
	// If reached final destination, stop the path searching
	if (paths.src != end) {
		// Get all pred nodes of v
		PredNode *curr = paths.pred[end];

		// for each predNode at v, branch a new wave of recursion
		while (curr != NULL) {
			// If there is another path, increment globalPathCounter
			if (curr->next != NULL) {
				globalPathCounter++;
			}

			// Record each vertex that comes into the path by incrementing their value by 1 in record
			// Don't count src node
			if (curr->v != paths.src)
				record[curr->v] += record[curr->v] + 1;

			// Recurse, capturing what the final noPaths is
			recordVerticesInPath(paths, record, curr->v);

			curr = curr->next;
		}
	}
}
