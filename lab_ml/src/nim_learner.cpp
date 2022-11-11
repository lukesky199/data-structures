/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "nim_learner.h"
#include <ctime>
#include <random>


/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true) {
  /* Your code goes here! */
  // Starting Vertex
  startingVertex_ = "p1-" + std::to_string(startingTokens);
  // Graph Helper Function
  auto createEdges = [this, &startingTokens](unsigned start, unsigned stepSize) {
    // Don't exceed the number of startingTokens
    if (start + stepSize > startingTokens)
      return;
    // Create Vertices
    if (!g_.vertexExists("p1-" + std::to_string(start + stepSize)))
      g_.insertVertex("p1-" + std::to_string(start + stepSize));
    if (!g_.vertexExists("p2-" + std::to_string(start + stepSize)))
      g_.insertVertex("p2-" + std::to_string(start + stepSize));
    // Create Edges
    g_.insertEdge("p2-" + std::to_string(start + stepSize), "p1-" + std::to_string(start));
    g_.setEdgeWeight("p2-" + std::to_string(start + stepSize), "p1-" + std::to_string(start), 0);
    g_.insertEdge("p1-" + std::to_string(start + stepSize), "p2-" + std::to_string(start));
    g_.setEdgeWeight("p1-" + std::to_string(start + stepSize), "p2-" + std::to_string(start), 0);
  };

  // Making nodes of end state
  g_.insertVertex("p1-0");
  g_.insertVertex("p2-0");
  for (unsigned i = 0; i < startingTokens; i++) {
    // Pick Up 1 Token
    createEdges(i, 1);
    createEdges(i, 2);
  }
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const {
  vector<Edge> path;
  /* Your code goes here! */
  std::vector<std::string> player{"p1-", "p2-"};
  int turn = 1;
  Vertex curr = startingVertex_;
  while (std::stoi(curr.substr(3)) > 0) {
    int step = rand() % 2 + 1;

    std::string next;
    if (std::stoi(curr.substr(3)) - step < 0)
      next = player[turn] + "0";
    else
      next = player[turn] + std::to_string(std::stoi(curr.substr(3)) - step);
    
    path.push_back(g_.getEdge(curr, next));
    curr = next;
    turn = (turn + 1) % 2;
  }
  return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) {
  /* Your code goes here! */
  std::string winner = path.back().source.substr(0, 2);
  // std::cout << "WINNER: " << winner << std::endl;
  for (Edge turn : path) {
    // std::cout << "SOURCE: " << turn.source << " DEST: " << turn.dest << std::endl;
    // std::cout << "WEIGHT: " << g_.getEdgeWeight(turn.source, turn.dest) << std::endl;
    if (turn.source.substr(0, 2) == winner) {
      g_.setEdgeWeight(turn.source, turn.dest, g_.getEdgeWeight(turn.source, turn.dest) + 1);
    } else {
      g_.setEdgeWeight(turn.source, turn.dest, g_.getEdgeWeight(turn.source, turn.dest) - 1);
    }
    // std::cout << "WEIGHT: " << g_.getEdgeWeight(turn.source, turn.dest) << std::endl;
  }
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex & v : g_.getVertices()) {
    for (const Vertex & w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);

      // Label all edges with positve weights as "WINPATH"
      if (weight > threshold)           { g_.setEdgeLabel(v, w, "WIN"); }
      else if (weight < -1 * threshold) { g_.setEdgeLabel(v, w, "LOSE"); }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}
