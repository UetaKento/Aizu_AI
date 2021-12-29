/**
 * @file prog_03.h
 * @date 2016/10/06
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Main header file.
 * @details
 *  Artificial Intelligence, 3rd Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */

#ifndef PROG_03_H
#define PROG_03_H


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
 * @struct graph
 * @brief This struct defines a graph.
 */
struct graph {
    EdgeState adjacent_matrix[MAX_VERTEX_SIZE][MAX_VERTEX_SIZE]; //!< The adjacent matrix.
    float edge_costs[MAX_VERTEX_SIZE][MAX_VERTEX_SIZE];          //!< The edge costs.
    size_t vertex_count;                                         //!< The number of vertices.
    size_t vertex_starting_search;                               //!< The starting vertex for search.
    size_t vertex_end_search;                                    //!< The end vertex for search.
};
typedef struct graph Graph;


/**
 * @def GET_INDEX_BY_ID(c)
 * @brief It converts charactor \a c to index of array.
 */
#define GET_INDEX_BY_ID(c) ((size_t)((c)-0x41))

/**
 * @def GET_ID_BY_INDEX(i)
 * @brief It converts index \a i to charactor.
 */
#define GET_ID_BY_INDEX(i) ((char)((i) + 0x41))


extern void dfs(Graph const*);
extern void bfs(Graph const*);


#endif
