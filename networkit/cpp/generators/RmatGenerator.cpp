/*
 * RmatGenerator.cpp
 *
 *  Created on: 18.03.2014
 *      Author: Henning
 */

#include "RmatGenerator.h"
#include "../auxiliary/Random.h"
#include "../auxiliary/Log.h"

namespace NetworKit {

RmatGenerator::RmatGenerator(count scale, count edgeFactor, double a, double b, double c, double d):
	scale(scale), edgeFactor(edgeFactor), a(a), b(b), c(c), d(d)
{
	defaultEdgeWeight = 1.0;
}

Graph RmatGenerator::generate() {
	count n = (1 << scale);
	count numEdges = n * edgeFactor;
	Graph G(n, true);
	double ab = a+b;
	double abc = ab+c;

	auto quadrant([&]() {
		double r = Aux::Random::probability();
		TRACE("r: ", r);

		if (r <= a) {
			return 0;
		}
		else if (r <= ab) {
			return 1;
		}
		else if (r <= abc) {
			return 2;
		}
		else return 3;
	});

	auto drawEdge([&]() {
		node u = 0;
		node v = 0;
		for (index i = 0; i < scale; ++i) {
			count q = quadrant();
//			TRACE("q: ", q);
			u = u << 1;
			v = v << 1;
			u = u | (q >> 1);
			v = v | (q & 1);
		}

		return std::make_pair(u, v);
	});

	for (index e = 0; e < numEdges; ++e) {
		std::pair<node, node> drawnEdge = drawEdge();
//		TRACE("edge drawn: ", drawnEdge.first, " - ", drawnEdge.second);
		G.increaseWeight(drawnEdge.first, drawnEdge.second, defaultEdgeWeight);
	}

	G.shrinkToFit();
	return G;
}

} /* namespace NetworKit */