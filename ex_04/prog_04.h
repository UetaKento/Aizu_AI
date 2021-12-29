/**
 * @file prog_04.h
 * @date 2016/10/06
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Main header file.
 * @details
 *  Artificial Intelligence, 4th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved
 */

#ifndef PROG_04_H
#define PROG_04_H


#include "s_queue.h"
#include <stddef.h>


/**
 * @brief This enum has numerical constants.
 */
enum {
    MAX_VERTEX_SIZE = MAX_QUEUE_SIZE, //!< Define the maximum number of vertices.
};

/**
 * @enum vertex_state
 * @brief This enum defines the state of a vertex.
 */
enum vertex_state {
    VISITED,   //!< Take this value if the vertex is visited.
    UNVISITED, //!< Take this value if the vertex is unvisited.
    IN_QUEUE,  //!< Take this value if the vertex is in a queue.
};
typedef enum vertex_state VertexState;

/**
 * @enum edge_state
 * @brief This enum defines the existence of an edge.
 */
enum edge_state {
    NOT_CONNECTED = 0, //!< Take this value if the edge does not exist.
    CONNECTED,         //!< Take this value if the edge exists.
};
typedef enum edge_state EdgeState;

/**
 * @struct struct graph
 * @brief This struct defines a graph.
 */
struct graph {
    EdgeState adjacent_matrix[MAX_VERTEX_SIZE][MAX_VERTEX_SIZE]; //!< Adjacent matrix.
    float edge_costs[MAX_VERTEX_SIZE][MAX_VERTEX_SIZE];          //!< Edge costs.
    float heuristic_values[MAX_VERTEX_SIZE];                     //!< Heuristic values.
    size_t vertex_count;                                         //!< The number of vertices in the graph.
    size_t vertex_starting_search;                               //!< Start vertex for searching.
    size_t vertex_end_search;                                    //!< End vertex for searching.
};
typedef struct graph Graph;


/**
 * @def GET_ID_BY_CHAR(c)
 * @brief It converts charactor \a c to index of array.
 */
#define GET_ID_BY_CHAR(c) ((size_t)((c)-0x41))

/**
 * @def GET_CHAR_BY_ID(i)
 * @brief It converts index \a i to charactor.
 */
#define GET_CHAR_BY_ID(i) ((char)((i) + 0x41))


extern void dfs(Graph const*);
extern void bfs(Graph const*);
extern void uniform_cost_search(Graph const*);
extern void best_first_search(Graph const*);
extern void a_star(Graph const*);


#endif
