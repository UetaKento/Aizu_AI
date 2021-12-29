/**
 * @file func.c
 * @date 2016/10/04
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief DFS and BFS implementations.
 * @details
 *  Artificial Intelligence, 2nd Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */

#include "prog_02.h"
#include <stdbool.h>
#include <stdio.h>


static void dfs_visit(Graph const*, VertexState*, size_t);


/**
 * @brief Function for depth first search.
 * @param g The pointer to the given graph.
 */
void dfs(Graph const* g)
{
    printf("DFS: ");

    // Initialize all vertices by reseting their states to UNVISITED.
    VertexState vertex_states[MAX_VERTEX_SIZE];
    for (size_t i = 0; i < g->vertex_count; i++) {
        vertex_states[i] = UNVISITED;
    }

    // TODO: 1. Set the arguments in order to start DFS.
    dfs_visit(g, vertex_states, g->vertex_starting_search);
    putchar('\n');
}


/**
 * @brief Function for recursive call of DFS.
 * @param g The pointer to the given graph.
 * @param vertex_states The pointer to the state array of the vertices.
 * @param current_vertex The current vertex under consideration.
 */
static void dfs_visit(Graph const* g, VertexState* vertex_states, size_t current_vertex)
{
    // Print current vertex number.
    printf("%zu ", current_vertex);

    // TODO: 2. Change the current vertex state.
    vertex_states[current_vertex] = VISITED;

    // Loop for child vertices.
    for (size_t i = 0; i < g->vertex_count; i++) {
        // TODO: 3. If the i-th vertex is not visited yet and the current vertex is connected to it, visit the i-th vertex recursively.
        if (vertex_states[i] == UNVISITED && g->adjacent_matrix[current_vertex][i] == CONNECTED) {
            dfs_visit(g, vertex_states, i);
        }
    }
}


/**
 * @brief Function for breadth first search.
 * @param g The pointer to the given graph.
 */
void bfs(Graph const* g)
{
    printf("BFS: ");

    // Initialize all vertices by reseting their states to UNVISITED.
    VertexState vertex_states[MAX_VERTEX_SIZE];
    for (size_t i = 0; i < g->vertex_count; i++) {
        vertex_states[i] = UNVISITED;
    }

    // Initialize the queue used for tracking the search process.
    s_queue_init(NULL);

    // TODO: 4. Set the argument to start BFS.
    // Set start vertex to begin searching.
    s_queue_enqueue(g->vertex_starting_search);

    // TODO: 5. Set the state of the starting vertex to begin search.
    // The starting vertex is in the queue and it state should be visited.
    vertex_states[g->vertex_starting_search] = VISITED;

    while (s_queue_is_empty() == false) {
        size_t current_vertex = s_queue_dequeue();
        printf("%zu ", current_vertex);

        // Loop for child vertices.
        for (size_t i = 0; i < g->vertex_count; i++) {
            // TODO: 6. If the i-th vertex is not visited yet and the current vertex is connected to it, append it to the queue.
            if (vertex_states[i] == UNVISITED && g->adjacent_matrix[current_vertex][i] == CONNECTED) {
                vertex_states[i] = VISITED;
                s_queue_enqueue(i);
            }
        }
    }

    putchar('\n');
}
