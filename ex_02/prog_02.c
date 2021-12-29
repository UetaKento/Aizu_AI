/**
 * @file prog_02.c
 * @date 2016/10/04
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief DFS and BFS for traversing all vertices.
 * @details
 *  Artificial Intelligence, 2nd Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 *
 * Assignment:
 *  Fill in all blanks to complete the program.
 *  This program traverses all vertices of a given graph from a start vertex
 *  using depth first search (DFS) or breadth-first search (BFS).
 *  It outputs the vertex numbers in traversing order.
 *
 *  [N]  : The number of vertices.
 *  [E]  : The number of edges.
 *  [V_a]: One of the vertices that connects an edge.
 *  [V_b]: One of the vertices that connects an edge.
 *  [S]  : Starting vertex for search.
 *
 *  Input set:
 *      [N]
 *      [E]
 *      [V_a1] [V_b2]
 *          ...
 *      [V_aE] [V_bE]
 *      [S]
 */

#include "prog_02.h"
#include <stdbool.h>
#include <stdio.h>


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
    // In the following sentence, %zu is the format for reading a number with the type size_t.
    if (scanf("%zu", &g->vertex_count) != 1) {
        return false;
    }

    // Initialize the adjacency matrix.
    for (size_t i = 0; i < g->vertex_count; i++) {
        for (size_t j = 0; j < g->vertex_count; j++) {
            g->adjacent_matrix[i][j] = NOT_CONNECTED;
        }
    }

    // Load the number of edges.
    size_t num_edge;
    scanf("%zu", &num_edge);

    // Construct the graph (i.e. define the adjacency matrix).
    for (size_t i = 0; i < num_edge; i++) {
        size_t vtx_a, vtx_b;
        scanf("%zu %zu", &vtx_a, &vtx_b);
        g->adjacent_matrix[vtx_a][vtx_b] = CONNECTED;
        g->adjacent_matrix[vtx_b][vtx_a] = CONNECTED;
    }

    // Print adjacency matrix.
    printf("Adjacency matrix\n    ");
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf("%2zu ", i);
    }
    putchar('\n');
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf(" %2zu:", i);
        for (size_t j = 0; j < g->vertex_count; j++) {
            printf("%2u ", (g->adjacent_matrix[i][j] == CONNECTED) ? (1u) : (0u));
        }
        putchar('\n');
    }

    // Load the number of the starting vertex.
    scanf("%zu", &g->vertex_starting_search);
    printf("Start vertex: %zu\n", g->vertex_starting_search);

    return true;
}
