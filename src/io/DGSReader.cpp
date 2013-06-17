/*
 * DGSReader.cpp
 *
 *  Created on: 01.06.2013
 *      Author: cls
 */

#include "DGSReader.h"

namespace NetworKit {

DGSReader::DGSReader() {
	// TODO Auto-generated constructor stub
}

DGSReader::~DGSReader() {
	// TODO Auto-generated destructor stub
}

void DGSReader::read(std::string path, GraphEventProxy& Gproxy) {

	std::ifstream dgsFile(path);

	std::string line;

	// handle first line
	std::getline(dgsFile, line);
	if (line == "DGS004") {
		DEBUG("found magic cookie: DGS004");
	} else {
		throw std::runtime_error("This does not seem to be a valid DGS file. Expected magic cookie 'DGS004' in first line");
	}

	// handle second line: optional name of file, number of clock ticks, total number of events
	std::getline(dgsFile, line);
	std::vector<std::string> split = Aux::StringTools::split(line);

	// TODO: complete implementation
    std::unordered_map<std::string, node> nodeNames;

	while (std::getline(dgsFile, line)) {
		std::vector<std::string> split = Aux::StringTools::split(line);
		std::string tag = split[0];

	    //std::unordered_map<std::string, node> edgeNames;

		if (tag.compare("st") == 0 && split.size() == 2) { // clock
			Gproxy.timeStep();

		} else if (tag.compare("an") == 0 && split.size() == 2) { // add node
			// Get the node name from the input
			std::string nodeName = split[1];
			// Add a node to a graph, mapping it to the node name inside the nodeNames map
			nodeNames[nodeName] = Gproxy.addNode();

		} else if (tag.compare("ae") == 0 && split.size() >= 4) { // add edge
			std::string edge_from = split[2];
			std::string edge_to = split[3];
			std::string edge_name = split[1];
			Gproxy.addEdge(nodeNames[edge_from], nodeNames[edge_to], 1.0);

		} else if (tag.compare("ce") == 0 && split.size() == 3) { // update edge. Only the "weight" attribute is supported so far

			std::string from_to_edges = split[1];
			std::vector<std::string> edgesSplit = Aux::StringTools::split(from_to_edges, '-');
			std::string edge_from = edgesSplit[0];
			std::string edge_to = edgesSplit[1];

			std::string weight = split[2];
			std::vector<std::string> weightSplit = Aux::StringTools::split(weight, '=');
			double weightValue = atoi(weightSplit[1].c_str() );

			Gproxy.setWeight(nodeNames[edge_from], nodeNames[edge_to], weightValue);

		} else if (tag.compare("dn") == 0 && split.size() == 2) {
			std::string nodeName = split[1];
			node deleteNode = nodeNames[nodeName];
			// Delete the nodes only if there are no edges connected to it
			if (Gproxy.G->degree(deleteNode) == 0) {
				Gproxy.removeNode(deleteNode);
			} else {
				throw std::runtime_error("The node was not deleted, since there are edges attached to it.");
			}

		} else if (tag.compare("de") == 0 && split.size() == 2) {
			std::string from_to_edges = split[1];
			std::vector<std::string> edgesSplit = Aux::StringTools::split(from_to_edges, '-');
			std::string edge_from = edgesSplit[0];
			std::string edge_to = edgesSplit[1];
			node u = nodeNames[edge_from];
			node v = nodeNames[edge_to];
//			TRACE("u: " << edge_from << " " << u);
//			TRACE("v: " << edge_to << " " << v);

			Gproxy.removeEdge(u, v);
		}





	}

	std::cout << "nodeNames length " << nodeNames.size();
				std::map<std::string, node> ordered(nodeNames.begin(), nodeNames.end());
				for(auto it = ordered.begin(); it != ordered.end(); ++it)
					std::cout << " contents " << it->second << std::endl;


}

} /* namespace NetworKit */
