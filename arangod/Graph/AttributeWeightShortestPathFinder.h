////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2016 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Michael Hackstein
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGODB_GRAPH_ATTRIBUTE_WEIGHT_SHORTEST_PATH_FINDER_H
#define ARANGODB_GRAPH_ATTRIBUTE_WEIGHT_SHORTEST_PATH_FINDER_H 1

#include "Basics/Mutex.h"
#include "Basics/MutexLocker.h"

#include "Graph/ShortestPathFinder.h"
#include "Graph/ShortestPathPriorityQueue.h"

#include <thread>


namespace arangodb {
namespace graph {

class AttributeWeightShortestPathFinder : public ShortestPathFinder {
 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Step, one position with a predecessor and the edge
  //////////////////////////////////////////////////////////////////////////////

  struct Step {
   private:
    double _weight;

   public:
    arangodb::velocypack::Slice _vertex;
    arangodb::velocypack::Slice _predecessor;
    arangodb::velocypack::Slice _edge;
    bool _done;

    Step() : _done(false) {}

    Step(arangodb::velocypack::Slice const& vert,
         arangodb::velocypack::Slice const& pred, double weig,
         arangodb::velocypack::Slice const& edge)
        : _weight(weig),
          _vertex(vert),
          _predecessor(pred),
          _edge(edge),
          _done(false) {}

    double weight() const { return _weight; }

    void setWeight(double w) { _weight = w; }

    arangodb::velocypack::Slice const& getKey() const { return _vertex; }
  };

  //////////////////////////////////////////////////////////////////////////////
  /// @brief edge direction
  //////////////////////////////////////////////////////////////////////////////

  typedef enum { FORWARD, BACKWARD } Direction;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief callback to find neighbors
  //////////////////////////////////////////////////////////////////////////////

  typedef std::function<void(arangodb::velocypack::Slice const&,
                             std::vector<Step*>&)>
      ExpanderFunction;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief our specialization of the priority queue
  //////////////////////////////////////////////////////////////////////////////

  typedef arangodb::graph::ShortestPathPriorityQueue<
      arangodb::velocypack::Slice, Step, double>
      PQueue;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief information for each thread
  //////////////////////////////////////////////////////////////////////////////

  struct ThreadInfo {
    PQueue _pq;
    arangodb::Mutex _mutex;
  };

  //////////////////////////////////////////////////////////////////////////////
  /// @brief a Dijkstra searcher for the multi-threaded search
  //////////////////////////////////////////////////////////////////////////////

  class SearcherTwoThreads {
    AttributeWeightShortestPathFinder* _pathFinder;
    ThreadInfo& _myInfo;
    ThreadInfo& _peerInfo;
    arangodb::velocypack::Slice _start;
    ExpanderFunction _expander;
    std::string _id;

   public:
    SearcherTwoThreads(AttributeWeightShortestPathFinder* pathFinder,
                       ThreadInfo& myInfo, ThreadInfo& peerInfo,
                       arangodb::velocypack::Slice const& start,
                       ExpanderFunction expander, std::string const& id);

   private:
    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Insert a neighbor to the todo list.
    ////////////////////////////////////////////////////////////////////////////////
    void insertNeighbor(Step* step, double newWeight);

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Lookup our current vertex in the data of our peer.
    ////////////////////////////////////////////////////////////////////////////////

    void lookupPeer(arangodb::velocypack::Slice& vertex, double weight);

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Search graph starting at Start following edges of the given
    /// direction only
    ////////////////////////////////////////////////////////////////////////////////

    void run();

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief start and join functions
    ////////////////////////////////////////////////////////////////////////////////

   public:
    void start();

    void join();

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief The thread object.
    ////////////////////////////////////////////////////////////////////////////////

   private:
    std::thread _thread;
  };

  //////////////////////////////////////////////////////////////////////////////
  /// @brief a Dijkstra searcher for the single-threaded search
  //////////////////////////////////////////////////////////////////////////////

  class Searcher {
    AttributeWeightShortestPathFinder* _pathFinder;
    ThreadInfo& _myInfo;
    ThreadInfo& _peerInfo;
    arangodb::velocypack::Slice _start;
    ExpanderFunction _expander;
    std::string _id;

   public:
    Searcher(AttributeWeightShortestPathFinder* pathFinder, ThreadInfo& myInfo,
             ThreadInfo& peerInfo, arangodb::velocypack::Slice const& start,
             ExpanderFunction expander, std::string const& id);

   private:
    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Insert a neighbor to the todo list.
    ////////////////////////////////////////////////////////////////////////////////

    void insertNeighbor(Step* step, double newWeight);

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Lookup our current vertex in the data of our peer.
    ////////////////////////////////////////////////////////////////////////////////

    void lookupPeer(arangodb::velocypack::Slice& vertex, double weight);

   public:
    ////////////////////////////////////////////////////////////////////////////////
    /// @brief Do one step only.
    ////////////////////////////////////////////////////////////////////////////////

    bool oneStep();
  };

  // -----------------------------------------------------------------------------

  AttributeWeightShortestPathFinder(AttributeWeightShortestPathFinder const&) =
      delete;
  AttributeWeightShortestPathFinder& operator=(
      AttributeWeightShortestPathFinder const&) = delete;
  AttributeWeightShortestPathFinder() = delete;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief create the PathFinder
  //////////////////////////////////////////////////////////////////////////////

  AttributeWeightShortestPathFinder(ExpanderFunction&& forwardExpander,
                                    ExpanderFunction&& backwardExpander,
                                    bool bidirectional = true);

  ~AttributeWeightShortestPathFinder(){};

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Find the shortest path between start and target.
  ///        Only edges having the given direction are followed.
  ///        nullptr indicates there is no path.
  //////////////////////////////////////////////////////////////////////////////

  // Caller has to free the result
  // If this returns true there is a path, if this returns false there is no
  // path
  bool shortestPath(arangodb::velocypack::Slice const& start,
                    arangodb::velocypack::Slice const& target,
                    arangodb::traverser::ShortestPath& result,
                    std::function<void()> const& callback) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief return the shortest path between the start and target vertex,
  /// multi-threaded version using SearcherTwoThreads.
  //////////////////////////////////////////////////////////////////////////////

  // Caller has to free the result
  // If this returns true there is a path, if this returns false there is no
  // path

  bool shortestPathTwoThreads(arangodb::velocypack::Slice& start,
                              arangodb::velocypack::Slice& target,
                              arangodb::traverser::ShortestPath& result);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief lowest total weight for a complete path found
  //////////////////////////////////////////////////////////////////////////////

  bool _highscoreSet;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief lowest total weight for a complete path found
  //////////////////////////////////////////////////////////////////////////////

  double _highscore;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief _bingo, flag that indicates termination
  //////////////////////////////////////////////////////////////////////////////

  std::atomic<bool> _bingo;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief result code. this is used to transport errors from sub-threads to
  /// the caller thread
  //////////////////////////////////////////////////////////////////////////////

  std::atomic<int> _resultCode;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief _resultMutex, this is used to protect access to the result data
  //////////////////////////////////////////////////////////////////////////////

  arangodb::Mutex _resultMutex;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief _intermediate, one vertex on the shortest path found, flag
  /// indicates
  /// whether or not it was set.
  //////////////////////////////////////////////////////////////////////////////

  bool _intermediateSet;
  arangodb::velocypack::Slice _intermediate;

 private:
  ExpanderFunction _forwardExpander;
  ExpanderFunction _backwardExpander;
  bool _bidirectional;
};

}  // namespace graph
}  // namespace arangodb

#endif
