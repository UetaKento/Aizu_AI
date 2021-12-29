/**
 * @file prog_03.c
 * @date 2016/10/04
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief DFS and BFS for finding a path between a starting point and a goal, and the cost of the path.
 * @details
 *  Artificial Intelligence, 3rd Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 *
 * Assignment:
 *  Fill in the all blanks for completing the program.
 *  This program is almost the same as ex_02.
 *  The differences are:
 *   - input of an edge is denoted by an alphabet and
 *   - each edge has a cost.
 *  The outputs of the program contains:
 *   - The numbers of the vertices on the path, and
 *   - the cost of each vertex.
 *
 *  [N]  : The number of vertices.
 *  [E]  : The number of edges.
 *  [V_a]: One of the vertices that connects an edge.
 *  [V_b]: One of the vertices that connects an edge.
 *  [C]  : The cost of an edge.
 *  [S]  : Starting verter for search.
 *  [G]  : Goal vertex for search.
 *
 *  Input format:
 *      [N]
 *      [E]
 *      [V_a1] [V_b2]
 *          ...
 *      [V_aE] [V_bE]
 *      [S]
 *      [G]
 */


#include <stdio.h>
#include <stdbool.h>
#include "prog_03.h"


static bool init_graph(Graph*);


int main(void)
{
    Graph graph;

    while (init_graph(&graph) == true) {
        dfs(&graph);
        bfs(&graph);
    }

    return 0;
}


/**
 * @brief Initializing the graph.
 * @param g The pointer to the given graph.
 * @return true if initialization was succeeded.
 */
static bool init_graph(Graph* g)
{
    // Load the number of vertices
    if (scanf("%zu", &g->vertex_count) != 1) {
        return false;
    }

    // Initialize the adjacency matrix and the edge costs.
    for (size_t i = 0; i < g->vertex_count; i++) {
        for (size_t j = 0; j < g->vertex_count; j++) {
            g->adjacent_matrix[i][j] = NOT_CONNECTED;
            g->edge_costs[i][j]      = 0.0f;
        }
    }

    // Load the number of edges.
    size_t num_edge;
    scanf("%zu%*c", &num_edge);

    // Construct the graph (i.e. define the adjacency matrix).
    for (size_t i = 0; i < num_edge; i++) {
        char c1, c2;
        float cost;
        scanf("%c %c %f%*c", &c1, &c2, &cost);
        size_t vtx_a                     = GET_INDEX_BY_ID(c1);
        size_t vtx_b                     = GET_INDEX_BY_ID(c2);
        g->adjacent_matrix[vtx_a][vtx_b] = CONNECTED;
        g->adjacent_matrix[vtx_b][vtx_a] = CONNECTED;
        g->edge_costs[vtx_a][vtx_b]      = cost;
        g->edge_costs[vtx_b][vtx_a]      = cost;
    }

    // Print adjacency matrix.
    printf("Adjacency matrix\n   ");
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf("%c ", GET_ID_BY_INDEX(i));
    }
    putchar('\n');
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf(" %c:", GET_ID_BY_INDEX(i));
        for (size_t j = 0; j < g->vertex_count; j++) {
            printf("%u ", (g->adjacent_matrix[i][j] == CONNECTED) ? (1u) : (0u));
        }
        putchar('\n');
    }

    // Print costs.
    printf("Costs\n   ");
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf("%4c ", GET_ID_BY_INDEX(i));
    }
    putchar('\n');
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf(" %c:", GET_ID_BY_INDEX(i));
        for (size_t j = 0; j < g->vertex_count; j++) {
            printf("%.2f ", g->edge_costs[i][j]);
        }
        putchar('\n');
    }

    // Load the character of the starting and end vertex.
    char cs, ce;
    scanf("%c %c", &cs, &ce);
    g->vertex_starting_search = GET_INDEX_BY_ID(cs);
    g->vertex_end_search      = GET_INDEX_BY_ID(ce);
    printf("Start vertex: %c\n", GET_ID_BY_INDEX(g->vertex_starting_search));
    printf("End vertex: %c\n", GET_ID_BY_INDEX(g->vertex_end_search));

    return true;
}
