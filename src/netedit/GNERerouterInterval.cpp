/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2017 German Aerospace Center (DLR) and others.
/****************************************************************************/
//
//   This program and the accompanying materials
//   are made available under the terms of the Eclipse Public License v2.0
//   which accompanies this distribution, and is available at
//   http://www.eclipse.org/legal/epl-v20.html
//
/****************************************************************************/
/// @file    GNERerouterInterval.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Jan 2017
/// @version $Id$
///
//
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <utils/common/ToString.h>
#include <utils/common/MsgHandler.h>

#include "GNERerouterInterval.h"
#include "GNEEdge.h"
#include "GNELane.h"
#include "GNEClosingLaneReroute.h"
#include "GNEClosingReroute.h"
#include "GNEDestProbReroute.h"
#include "GNERouteProbReroute.h"
#include "GNERerouter.h"
#include "GNEUndoList.h"
#include "GNEChange_Attribute.h"
#include "GNEViewNet.h"
#include "GNENet.h"

// ===========================================================================
// member method definitions
// ===========================================================================

GNERerouterInterval::GNERerouterInterval(GNERerouter* rerouterParent, double begin, double end) :
    GNEAttributeCarrier(SUMO_TAG_INTERVAL, ICON_EMPTY),
    myRerouterParent(rerouterParent),
    myBegin(begin),
    myEnd(end) {
    assert(begin <= end);
}


GNERerouterInterval::~GNERerouterInterval() {
}


GNERerouter*
GNERerouterInterval::getRerouterParent() const {
    return myRerouterParent;
}


std::string 
GNERerouterInterval::getAttribute(SumoXMLAttr key) const {
    switch (key) {
    case SUMO_ATTR_BEGIN:
        return toString(myBegin);
    case SUMO_ATTR_END:
        return toString(myEnd);
    default:
        throw InvalidArgument(toString(getTag()) + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


void 
GNERerouterInterval::setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) {
    if (value == getAttribute(key)) {
        return; //avoid needless changes, later logic relies on the fact that attributes have changed
    }
    switch (key) {
    case SUMO_ATTR_BEGIN:
    case SUMO_ATTR_END:
        undoList->p_add(new GNEChange_Attribute(this, key, value));
        break;
    default:
        throw InvalidArgument(toString(getTag()) + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


bool 
GNERerouterInterval::isValid(SumoXMLAttr key, const std::string& value) {
    switch (key) {
    case SUMO_ATTR_BEGIN:
        return canParse<double>(value) && parse<double>(value) <= myEnd;
    case SUMO_ATTR_END:
        return canParse<double>(value) && parse<double>(value) >= myBegin;
    default:
        throw InvalidArgument(toString(getTag()) + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


const std::vector<GNEClosingLaneReroute*>&
GNERerouterInterval::getClosingLaneReroutes() const {
    return myClosingLaneReroutes;
}


const std::vector<GNEClosingReroute*>&
GNERerouterInterval::getClosingReroutes() const {
    return myClosingReroutes;
}


const std::vector<GNEDestProbReroute*>&
GNERerouterInterval::getDestProbReroutes() const {
    return myDestProbReroutes;
}


const std::vector<GNERouteProbReroute*>&
GNERerouterInterval::getRouteProbReroutes() const {
    return myRouteProbReroutes;
}


void 
GNERerouterInterval::addClosingLaneReroutes(GNEClosingLaneReroute* closingLaneReroute) {
    auto it = std::find(myClosingLaneReroutes.begin(), myClosingLaneReroutes.end(), closingLaneReroute);
    if(it == myClosingLaneReroutes.end()) {
        myClosingLaneReroutes.push_back(closingLaneReroute);
    } else {
        throw ProcessError("Closing lane Reroute already exist");
    }
}


void 
GNERerouterInterval::removeClosingLaneReroute(GNEClosingLaneReroute* closingLaneReroute) {
    auto it = std::find(myClosingLaneReroutes.begin(), myClosingLaneReroutes.end(), closingLaneReroute);
    if(it != myClosingLaneReroutes.end()) {
        myClosingLaneReroutes.erase(it);
    } else {
        throw ProcessError("Closing lane Reroute doesn't exist");
    }
}


void 
GNERerouterInterval::addClosingReroute(GNEClosingReroute* closingReroute) {
    auto it = std::find(myClosingReroutes.begin(), myClosingReroutes.end(), closingReroute);
    if(it != myClosingReroutes.end()) {
        myClosingReroutes.push_back(closingReroute);
    } else {
        throw ProcessError("Closing Reroute already exist");
    }
}


void 
GNERerouterInterval::removeClosingReroute(GNEClosingReroute* closingReroute) {
    auto it = std::find(myClosingReroutes.begin(), myClosingReroutes.end(), closingReroute);
    if(it != myClosingReroutes.end()) {
        myClosingReroutes.erase(it);
    } else {
        throw ProcessError("Closing Reroute doesn't exist");
    }
}


void 
GNERerouterInterval::addDestProbReroute(GNEDestProbReroute* destProbReroute) {
    auto it = std::find(myDestProbReroutes.begin(), myDestProbReroutes.end(), destProbReroute);
    if(it == myDestProbReroutes.end()) {
        myDestProbReroutes.push_back(destProbReroute);
    } else {
        throw ProcessError("Destiny Probability Reroute already exist");
    }
}


void 
GNERerouterInterval::removeDestProbReroute(GNEDestProbReroute* destProbReroute) {
    auto it = std::find(myDestProbReroutes.begin(), myDestProbReroutes.end(), destProbReroute);
    if(it != myDestProbReroutes.end()) {
        myDestProbReroutes.erase(it);
    } else {
        throw ProcessError("Destiny Probability Reroute doesn't exist");
    }
}


void 
GNERerouterInterval::addRouteProbReroute(GNERouteProbReroute* routeProbabilityReroute) {
    auto it = std::find(myRouteProbReroutes.begin(), myRouteProbReroutes.end(), routeProbabilityReroute);
    if(it == myRouteProbReroutes.end()) {
        myRouteProbReroutes.push_back(routeProbabilityReroute);
    } else {
        throw ProcessError("Route Probability Reroute already exist");
    }
}


void 
GNERerouterInterval::removeRouteProbReroute(GNERouteProbReroute* routeProbabilityReroute) {
    auto it = std::find(myRouteProbReroutes.begin(), myRouteProbReroutes.end(), routeProbabilityReroute);
    if(it != myRouteProbReroutes.end()) {
        myRouteProbReroutes.erase(it);
    } else {
        throw ProcessError("Route Probability Reroute doesn't exist");
    }
}

// ===========================================================================
// private
// ===========================================================================

void 
GNERerouterInterval::setAttribute(SumoXMLAttr key, const std::string& value) {
    switch (key) {
    case SUMO_ATTR_BEGIN: {
        myBegin = parse<double>(value);
        break;
    }
    case SUMO_ATTR_END: {
        myEnd = parse<double>(value);
        break;
    }
    default:
        throw InvalidArgument(toString(getTag()) + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}

/****************************************************************************/
