/**
 * @file func.c
 * @date 2016/10/06
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief DFS, BFS, uniform cost search, best first search and A* algorithm implementations.
 * @details
 *  Artificial Intelligence, 4th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved
 */

#include <stdio.h>
#include <stdbool.h>
#include "prog_04.h"

/**
 * @enum search_type
 * @brief This enum defines the signature of a search type.
 */
enum search_type {
    SEARCH_TYPE_BFS,
    SEARCH_TYPE_UNIFORM_COST,
    SEARCH_TYPE_BEST_FIRST,
    SEARCH_TYPE_ASTAR,
};
typedef enum search_type SearchType;


static float dfs_visit(Graph const*, VertexState*, size_t, float);
static void base_bfs(Graph const*, SearchType);


/**
 * @brief Function for A* algorithm.
 * @param g The pointer to the given graph.
 */
void a_star(Graph const* g)
{
    base_bfs(g, SEARCH_TYPE_ASTAR);
}


/**
 * @brief Function for uniform cost search.
 * @param g The pointer to the given graph.
 */
void uniform_cost_search(Graph const* g)
{
    base_bfs(g, SEARCH_TYPE_UNIFORM_COST);
}


/**
 * @brief Function for best first search.
 * @param g The pointer to the given graph.
 */
void best_first_search(Graph const* g)
{
    base_bfs(g, SEARCH_TYPE_BEST_FIRST);
}


/**
 * @brief Function for breadth first search.
 * @param g The pointer to the given graph.
 */
void bfs(Graph const* g)
{
    base_bfs(g, SEARCH_TYPE_BFS);
}


/**
 * @brief This BFS function is extended to execute many search algorithms such as Uniform cost search, best first search and A* algorithm is based on BFS.
 *        So, we can implement them by changing some parts of this function (E.g., sorting queue, calculating some values).
 * @param g The pointer to the given graph.
 * @param type It determine what type of search is executed.
 */
static void base_bfs(Graph const* g, SearchType type)
{
    // Print the name of search algorithm which will be executed.
    switch (type) {
        case SEARCH_TYPE_BFS:
            puts("BFS:");
            break;
        case SEARCH_TYPE_UNIFORM_COST:
            puts("Uniform cost search:");
            break;
        case SEARCH_TYPE_BEST_FIRST:
            puts("Best first search:");
            break;
        case SEARCH_TYPE_ASTAR:
            puts("A*:");
            break;
    }

    // Initialize vertex states, costs and eval values.
    VertexState vertex_states[MAX_VERTEX_SIZE];
    float accumulate_costs[MAX_VERTEX_SIZE];
    float eval_values[MAX_VERTEX_SIZE];
    for (size_t i = 0; i < g->vertex_count; i++) {
        vertex_states[i]    = UNVISITED;
        accumulate_costs[i] = 0.0f;
        eval_values[i]      = 0.0f;
    }

    // Set the evaluation value of starting vertex for A* algorithm.
    eval_values[g->vertex_starting_search] = g->heuristic_values[g->vertex_starting_search];

    // Initialize the queue used for tracking the search process.
    // Determine a criteria for sorting the elements in the queue based on the search algorithm.
    // TODO: 1. Based on each search type, set the arguments of `sortable_queue_init` functions.
    switch (type) {
        case SEARCH_TYPE_BFS:
            s_queue_init(NULL);
            break;
        case SEARCH_TYPE_UNIFORM_COST:
            s_queue_init(accumulate_costs);
            break;
        case SEARCH_TYPE_BEST_FIRST:
            s_queue_init(g->heuristic_values);
            break;
        case SEARCH_TYPE_ASTAR:
            s_queue_init(eval_values);
            break;
    }

    // Set start vertex to begin searching.
    s_queue_enqueue(g->vertex_starting_search);

    // This vertex is in the queue and it means the vertex is already visited.
    vertex_states[g->vertex_starting_search] = VISITED;

    while (s_queue_is_empty() == false) {
        size_t current_vertex = s_queue_dequeue();

        // Print character of current vertex and cost.
        printf("%c(%5.2f) ", GET_CHAR_BY_ID(current_vertex), accumulate_costs[current_vertex]);

        if (current_vertex == g->vertex_end_search) {
            // If it reaches the end vertex, finish looping.
            break;
        } else if (type == SEARCH_TYPE_ASTAR) {
            vertex_states[current_vertex] = VISITED;
        }

        // Loop for child vertices.
        for (size_t i = 0; i < g->vertex_count; i++) {
            if (g->adjacent_matrix[current_vertex][i] == NOT_CONNECTED) {
                // This child vertex is skipped because it is NOT connected to the current vertex.
                continue;
            }

            float child_accumulate_cost     = 0.0f;
            float child_eval_value          = 0.0f;
            VertexState* child_vertex_state = &vertex_states[i];

            // Switch procedures by the search algorithm.
            switch (type) {
                case SEARCH_TYPE_BFS:
                    if (*child_vertex_state == UNVISITED) {
                        *child_vertex_state = VISITED;
                        s_queue_enqueue(i);

                        // Set accumulate cost from start vertex to this child vertex.
                        accumulate_costs[i] = accumulate_costs[current_vertex] + g->edge_costs[current_vertex][i];
                    }
                    break;
                case SEARCH_TYPE_UNIFORM_COST:
                    if (*child_vertex_state == VISITED) {
                        // The cost until this child is already determined.
                        continue;
                    }

                    child_accumulate_cost = accumulate_costs[current_vertex] + g->edge_costs[current_vertex][i];

                    // TODO: 2. Write the condition when this child vertex will be enqueued.
                    if (*child_vertex_state == UNVISITED) {
                        *child_vertex_state = IN_QUEUE;
                        s_queue_enqueue(i);

                        // Set accumulate cost between start vertex and this child vertex.
                        accumulate_costs[i] = child_accumulate_cost;
                    } else if ((*child_vertex_state == IN_QUEUE) && (child_accumulate_cost < accumulate_costs[i])) {
                        // This child is in queue and new accumulate cost is smaller than old one
                        // Therefore, it should be updated.
                        accumulate_costs[i] = child_accumulate_cost;
                        *child_vertex_state = VISITED;
                    }

                    // Sorting the all elements in the queue based on the "costs" to get vertex that has "minimum cost" in the next loop.
                    s_queue_sort();
                    break;
                case SEARCH_TYPE_BEST_FIRST:
                    // TODO: 3. Write the condition when this child vertex will be enqueued.
                    if (*child_vertex_state == UNVISITED) {
                        *child_vertex_state = VISITED;
                        s_queue_enqueue(i);

                        // Set accumulate cost from start vertex to child vertex.
                        accumulate_costs[i] = accumulate_costs[current_vertex] + g->edge_costs[current_vertex][i];

                        // Sorting the all elements in the queue based on the "heuristic values" to get vertex that has "minimum heuristic values" in the next loop.
                        s_queue_sort();
                    }
                    break;
                case SEARCH_TYPE_ASTAR:
                    child_accumulate_cost = accumulate_costs[current_vertex] + g->edge_costs[current_vertex][i];

                    // TODO: 4. Calculate `child_eval_value`.
                    child_eval_value = g->heuristic_values[i] + child_accumulate_cost;

                    if (*child_vertex_state == UNVISITED) {
                        *child_vertex_state = IN_QUEUE;
                        s_queue_enqueue(i);

                        eval_values[i]      = child_eval_value;
                        accumulate_costs[i] = child_accumulate_cost;
                    } else if ((*child_vertex_state == IN_QUEUE) && (child_eval_value < eval_values[i])) {
                        // This child is in queue and new evaluation value and new accumulate cost is smaller than old one
                        // Therefore, it should be updated.
                        eval_values[i]      = child_eval_value;
                        accumulate_costs[i] = child_accumulate_cost;
                    } else if ((*child_vertex_state == VISITED) && (child_eval_value < eval_values[i])) {
                        // The cost until this child is already determined, but smaller cost is found.
                        // Therefore, it should be traversed again.
                        // TODO: 5. Make this child vertex traversable again
                        *child_vertex_state = IN_QUEUE;
                        s_queue_enqueue(i);

                        eval_values[i]      = child_eval_value;
                        accumulate_costs[i] = child_accumulate_cost;
                    }

                    // Sorting the all elements in the queue based on the "evaluation values" to get vertex that has "minumum evaluation values".
                    s_queue_sort();
                    break;
            }
        }
    }

    printf("\nCost: %5.2f\n", accumulate_costs[g->vertex_end_search]);
}


/**
 * @brief Function for depth first search.
 * @param g The pointer to the given graph.
 */
void dfs(Graph const* g)
{
    printf("DFS:\n");

    // Initialize vertex states.
    VertexState vertex_states[MAX_VERTEX_SIZE];
    for (size_t i = 0; i < g->vertex_count; i++) {
        vertex_states[i] = UNVISITED;
    }

    printf("\nCost: %5.2f\n", dfs_visit(g, vertex_states, g->vertex_starting_search, 0));
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
    printf("%c(%5.2f) ", GET_CHAR_BY_ID(current_vertex), accumulate_cost);

    // State of the vertex is changed to VISITED because it is visited.
    vertex_states[current_vertex] = VISITED;

    // If DFS reaches the goal vertex, stop and return the total cost.
    if (current_vertex == g->vertex_end_search) {
        return accumulate_cost;
    }

    // Loop for child vertices.
    for (size_t i = 0; i < g->vertex_count; i++) {
        // Check if the i-th child unvisited AND is connected to current vertex.
        if ((vertex_states[i] == UNVISITED) && (g->adjacent_matrix[current_vertex][i] == CONNECTED)) {
            accumulate_cost = dfs_visit(g, vertex_states, i, accumulate_cost + g->edge_costs[current_vertex][i]);
        }

        // If the goal vertex is visited, finish looping.
        if (vertex_states[g->vertex_end_search] == VISITED) {
            break;
        }
    }

    return accumulate_cost;
}
