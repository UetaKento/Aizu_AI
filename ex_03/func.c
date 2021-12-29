/**
 * @file func.c
 * @date 2016/10/04
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief DFS and BFS implementations.
 * @details
 *  Artificial Intelligence, 3rd Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved.
 */

#include "prog_03.h"
#include <stdbool.h>
#include <stdio.h>


static float dfs_visit(Graph const*, VertexState*, size_t, float);


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
    float cost = dfs_visit(g, vertex_states, g->vertex_starting_search, 0);

    printf("\nCost: %.2f\n", cost);
}


/**
 * @brief Find the vertices on the path recursively
 * @param g The pointer to the given graph.
 * @param vertex_states The pointer to the array which is used to check the state of a vertex (visit or not).
 * @param current_vertex The current vertex in the search process.
 * @param accumulate_cost Cost up to now.
 * @return Total cost.
 */
static float dfs_visit(Graph const* g, VertexState* vertex_states, size_t current_vertex, float accumulate_cost)
{
    // Print the ID of the current vertex and its cost.
    printf("%c(%5.2f) ", GET_ID_BY_INDEX(current_vertex), accumulate_cost);

    // State of the vertex is changed to VISITED because it is visited.
    vertex_states[current_vertex] = VISITED;

    // TODO: 2. Write the terminating condition for the recursive search.
    if (current_vertex == g->vertex_end_search) {
        return accumulate_cost;
    }

    // Loop for child vertices.
    for (size_t i = 0; i < g->vertex_count; i++) {
        // Check if the i-th child unvisited AND is connected to current vertex.
        if ((vertex_states[i] == UNVISITED) && (g->adjacent_matrix[current_vertex][i] == CONNECTED)) {
            // TODO: 3. Call dfs_visit recursively.
            // Note: Cost of the child = cost of the edge + accumulate cost.
            //       Also, pay attention to the return value.
            accumulate_cost = dfs_visit(g, vertex_states, i, accumulate_cost + g->edge_costs[current_vertex][i]);
        }

        // If the goal vertex is visited, finish looping.
        if (vertex_states[g->vertex_end_search] == VISITED) {
            break;
        }
    }

    return accumulate_cost;
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
    float accumulate_costs[MAX_VERTEX_SIZE];
    for (size_t i = 0; i < g->vertex_count; i++) {
        vertex_states[i]    = UNVISITED;
        accumulate_costs[i] = 0.0f;
    }

    // Initialize the queue used for tracking the search process.
    s_queue_init(NULL);

    // Set start vertex to begin searching.
    s_queue_enqueue(g->vertex_starting_search);

    // The starting vertex is in the queue and it state should be visited.
    vertex_states[g->vertex_starting_search] = VISITED;

    while (s_queue_is_empty() == false) {
        size_t current_vertex = s_queue_dequeue();

        // Print character of current vertex and cost.
        printf("%c(%5.2f) ", GET_ID_BY_INDEX(current_vertex), accumulate_costs[current_vertex]);

        // TODO: 4. Write the terminating condition for the search.
        if (g->vertex_end_search == current_vertex) {
            break;
        }

        // Loop for child vertices.
        for (size_t i = 0; i < g->vertex_count; i++) {
            // Check if the i-th child is unvisited AND it is connected to the current vertex.
            if ((vertex_states[i] == UNVISITED) && (g->adjacent_matrix[current_vertex][i] == CONNECTED)) {
                vertex_states[i] = VISITED;
                s_queue_enqueue(i);

                // TODO: 5. Calculate the accumulate cost.
                // Set accumulate cost from the starting vertex to the i-th child vertex.
                accumulate_costs[i] = accumulate_costs[current_vertex] + g->edge_costs[current_vertex][i];
            }
        }
    }

    printf("\nCost: %.2f\n", accumulate_costs[g->vertex_end_search]);
}
