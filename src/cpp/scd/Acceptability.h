/*
 * Acceptability.h
 *
 * Created on: 16.06.2013
 * Author: cls, Yassine Marrakchi
 */

#ifndef ACCEPTABILITY_H_
#define ACCEPTABILITY_H_

#include "../graph/Graph.h"
#include <unordered_set>

namespace NetworKit {

/**
 * Acceptability measures quantify how likely a node from the community shell
 * is to improve the community when it is included.
 */
class Acceptability {
public:
	const Graph* G;							//!< pointer to current graph
	std::unordered_set<node>* community;	//!< pointer to current community
	std::unordered_set<node>* shell;		//!< pointer to current shell
public:

	/**
	 * @param[in]	G			pointer to current graph
	 * @param[in]	community	pointer to current community
	 * @param[in]	shell		pointer to current shell
	 */
	Acceptability(const Graph& G, std::unordered_set<node>& community, std::unordered_set<node>& shell);

	virtual ~Acceptability();

	/**
	 * Get the acceptability value for a node. Higher values are better.
	 */
	virtual double getValue(node v) = 0;
};

/**
 * Return the same value for every node.
 *
 * This class is used for variants without acceptability
 */
class DummySimilarity: public Acceptability {

public:

	DummySimilarity(const Graph& G, std::unordered_set<node>& community, std::unordered_set<node>& shell);

	virtual ~DummySimilarity();

	virtual double getValue(node v);
};

/**
 * Get the node cluster similarity value for a node.
 *
 * 	$\frac{|N(C) \cap N(v) |}{|N(C) \cup N(v)|}$
 */
class NodeClusterSimilarity: public Acceptability {

public:

	NodeClusterSimilarity(const Graph& G, std::unordered_set<node>& community, std::unordered_set<node>& shell);

	virtual ~NodeClusterSimilarity();

	virtual double getValue(node v);
};
} /* namespace NetworKit */
#endif /* ACCEPTABILITY_H_ */