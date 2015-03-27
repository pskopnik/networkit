/*
 * UnconnectedNodesFinder.cpp
 *
 *  Created on: 20.03.2015
 *      Author: Kolja Esders (kolja.esders@student.kit.edu)
 */

#include "UnconnectedNodesFinder.h"

#include <algorithm>

namespace NetworKit {

UnconnectedNodesFinder::UnconnectedNodesFinder(const Graph& G) : G(G) {
}

/*std::vector<std::pair<node, node>> UnconnectedNodesFinder::findAll(count k) {
  std::vector<std::pair<node, node>> missingLinks;
  G.forNodes([&](node u) {
    std::vector<std::pair<node, node>> missingAtU = findFromNode(u, k);
    missingLinks.insert(missingLinks.end(), missingAtU.begin(), missingAtU.end());
  });
  missingLinks.erase(std::remove_if(std::begin(missingLinks), std::end(missingLinks),
          [&](std::pair<node, node> p) { return p.first > p.second; }), std::end(missingLinks));
  return missingLinks;
}*/

std::vector<std::pair<node, node>> UnconnectedNodesFinder::findAll(count k) {
  std::vector<std::pair<node, node>> missingLinks;
  std::vector<node> nodes = G.nodes();
  #pragma omp parallel
  {
    std::vector<std::pair<node, node>> missingLinksPrivate;
    #pragma omp for nowait
    for (index i = 0; i < nodes.size(); ++i) {
      std::vector<std::pair<node, node>> missingAtU = findFromNode(nodes[i], k);
      // Discard all node-pairs of the form u > v. This removes all duplicates that result from undirected edges
      missingAtU.erase(std::remove_if(std::begin(missingAtU), std::end(missingAtU),
          [&](std::pair<node, node> p) { return p.first > p.second; }), std::end(missingAtU));
      missingLinksPrivate.insert(missingLinksPrivate.end(), missingAtU.begin(), missingAtU.end());
    }
    #pragma omp critical
    missingLinks.insert(missingLinks.end(), missingLinksPrivate.begin(), missingLinksPrivate.end());
  }
  return missingLinks;
}

std::vector<std::pair<node, node>> UnconnectedNodesFinder::findFromNode(node u, count k) {
  std::vector<std::pair<node, node>> missingLinks;
  std::vector<bool> visited;
  visited.resize(G.upperNodeIdBound(), false);
  std::queue<node> q;
  q.push(u);
  visited[u] = true;
  for (count i = 1; i <= k; ++i) {
    std::queue<node> newFound;
    while (!q.empty()) {
      node u = q.front();
      q.pop();
      G.forNeighborsOf(u, [&](node v) {
        if (!visited[v]) {
          newFound.push(v);
          visited[v] = true;
        }
      });
    }
    q = newFound;
  }
  while (!q.empty()) {
    missingLinks.push_back(std::make_pair(u, q.front()));
    q.pop();
  }
  return missingLinks;
}


} // namespace NetworKit