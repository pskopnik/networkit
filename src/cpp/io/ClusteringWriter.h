/*
 * ClusteringWriter.h
 *
 *  Created on: 22.01.2013
 *      Author: Christian Staudt (christian.staudt@kit.edu)
 */

#ifndef CLUSTERINGWRITER_H_
#define CLUSTERINGWRITER_H_

#include <fstream>

#include "../structures/Partition.h"

namespace NetworKit {

/**
 * Write a clustering to a file.
 */
class ClusteringWriter {

public:

	ClusteringWriter();

	virtual ~ClusteringWriter();

	virtual void write(Partition& zeta, std::string path) const;
};

} /* namespace NetworKit */
#endif /* CLUSTERINGWRITER_H_ */