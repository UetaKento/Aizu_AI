/**
 * @file prog_04.c
 * @date 2016/10/06
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief DFS, BFS, uniform cost search, best first search and A* algorithm for traversing path between start and goal and calculating traverse cost.
 * @details
 *  Artificial Intelligence, 4th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved
 *
 * Assignment:
 *  Fill in the all blanks for completing the program.
 *  This program is almost the same as ex_02 and ex_03.
 *  In this program, uniform cost search, best first search and A* algorithm are added.
 *
 *  [N]  : The number of vertices.
 *  [E]  : The number of edges.
 *  [V_a]: One of the vertices that connects an edge.
 *  [V_b]: One of the vertices that connects an edge.
 *  [C]  : A cost of an edge.
 *  [S]  : Search will begin from this vertex.
 *  [G]  : Search will finish at this vertex.
 *  [H]  : heuristic value.
 *
 *  Input set:
 *      [N]
 *      [H_0] [H_i] ... [H_N]
 *      [E]
 *      [V_a1] [V_b2] [C_1]
 *          ...
 *      [V_aE] [V_bE] [C_E]
 *      [S]
 *      [G]
 */

#include "prog_04.h"
#include <stdbool.h>
#include <stdio.h>


static bool init_graph(Graph*);


int main(void)
{
    Graph graph;

    while (init_graph(&graph) == true) {
        puts("\nResults:");

        dfs(&graph);
        bfs(&graph);
        uniform_cost_search(&graph);
        best_first_search(&graph);
        a_star(&graph);
    }

    return 0;
}


/**
 * @brief Initializing graph struct.
 * @param g Pointer to graph which will be initialized.
 * @return true if initialization was succeeded.
 */
static bool init_graph(Graph* g)
{
    if (scanf("%zu", &g->vertex_count) != 1) {
        return false;
    }

    // Load heuristic values.
    for (size_t i = 0; i < g->vertex_count; i++) {
        scanf("%f", &g->heuristic_values[i]);
    }

    // Init adjacency matrix and edge costs.
    for (size_t i = 0; i < g->vertex_count; i++) {
        for (size_t j = 0; j < g->vertex_count; j++) {
            g->adjacent_matrix[i][j] = NOT_CONNECTED;
            g->edge_costs[i][j]      = 0.0f;
        }
    }

    // Load the number of edge.
    size_t num_edge;
    scanf("%zu%*c", &num_edge);

    // Set edge connection.
    for (size_t i = 0; i < num_edge; i++) {
        char c1, c2;
        float cost;
        scanf("%c %c %f%*c", &c1, &c2, &cost);
        size_t vtx_a                     = GET_ID_BY_CHAR(c1);
        size_t vtx_b                     = GET_ID_BY_CHAR(c2);
        g->adjacent_matrix[vtx_a][vtx_b] = CONNECTED;
        g->adjacent_matrix[vtx_b][vtx_a] = CONNECTED;
        g->edge_costs[vtx_a][vtx_b]      = cost;
        g->edge_costs[vtx_b][vtx_a]      = cost;
    }

    puts("------------------------------");

    // Print adjacency matrix.
    printf("Adjacency matrix\n   ");
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf("%c ", GET_CHAR_BY_ID(i));
    }
    putchar('\n');
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf(" %c:", GET_CHAR_BY_ID(i));
        for (size_t j = 0; j < g->vertex_count; j++) {
            printf("%u ", (g->adjacent_matrix[i][j] == CONNECTED) ? (1u) : (0u));
        }
        putchar('\n');
    }

    // Print costs.
    printf("Costs\n   ");
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf("%4c ", GET_CHAR_BY_ID(i));
    }
    putchar('\n');
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf(" %c:", GET_CHAR_BY_ID(i));
        for (size_t j = 0; j < g->vertex_count; j++) {
            printf("%.2f ", g->edge_costs[i][j]);
        }
        putchar('\n');
    }

    // Print heuristic values.
    printf("Heuristic values\n");
    for (size_t i = 0; i < g->vertex_count; i++) {
        printf("%c(%.2f) ", GET_CHAR_BY_ID(i), g->heuristic_values[i]);
    }
    putchar('\n');

    // Load start vertex number.
    char cs, ce;
    scanf("%c %c", &cs, &ce);
    g->vertex_starting_search = GET_ID_BY_CHAR(cs);
    g->vertex_end_search      = GET_ID_BY_CHAR(ce);
    printf("Start vertex: %c\n", GET_CHAR_BY_ID(g->vertex_starting_search));
    printf("End vertex: %c\n", GET_CHAR_BY_ID(g->vertex_end_search));

    return true;
}
