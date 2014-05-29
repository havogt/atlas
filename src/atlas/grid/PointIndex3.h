/*
 * (C) Copyright 1996-2014 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef atlas_grid_PointIndex3_h
#define atlas_grid_PointIndex3_h

#include "eckit/container/KDTree.h"
#include "eckit/container/KDMapped.h"
#include "eckit/container/KDMemory.h"

#include "atlas/io/Gmsh.hpp"
#include "atlas/mesh/Mesh.hpp"
#include "atlas/mesh/FunctionSpace.hpp"
#include "atlas/mesh/Field.hpp"
#include "atlas/mesh/Parameters.hpp"

#include "eckit/geometry/Point3.h"

//------------------------------------------------------------------------------------------------------

namespace atlas {

//------------------------------------------------------------------------------------------------------

template<class Traits>
class PointKdTree : public eckit::KDTreeMemory<Traits> {
public:
    typedef eckit::KDTreeMemory<Traits> Tree;
    typedef typename Tree::Point Point;
    typedef typename Tree::Alloc    Alloc;

    typedef typename Tree::NodeList    NodeList;
    typedef typename Tree::NodeInfo    NodeInfo;
    typedef typename Tree::PayloadType Payload;
    typedef typename Tree::Value   Value;

    PointKdTree(): eckit::KDTreeMemory<Traits>() {}

    NodeInfo nearestNeighbour(const Point& p)
    {
        return Tree::nearestNeighbour(p);
    }

    NodeInfo nearestNeighbourBruteForce(const Point& p)
    {
        return Tree::nearestNeighbourBruteForce(p);
    }

    NodeList findInSphere(const Point& p,double radius)
    {
        return Tree::findInSphere(p, radius);
    }

    NodeList findInSphereBruteForce(const Point& p,double radius)
    {
        return Tree::findInSphereBruteForce(p,radius);
    }

    NodeList kNearestNeighbours(const Point& p,size_t k)
    {
        return Tree::kNearestNeighbours(p, k);
    }

    NodeList kNearestNeighboursBruteForce(const Point& p,size_t k)
    {
        return Tree::kNearestNeighboursBruteForce(p,k);
    }

    PointKdTree(Alloc& alloc): Tree(alloc) {}
};

//------------------------------------------------------------------------------------------------------

struct PointIndex3TreeTrait
{
    typedef eckit::geometry::Point3 Point;
    typedef size_t                  Payload;
};

//------------------------------------------------------------------------------------------------------

typedef PointKdTree<PointIndex3TreeTrait>  PointIndex3;

//------------------------------------------------------------------------------------------------------

PointIndex3* create_cell_centre_index( atlas::Mesh& mesh );

//---------------------------------------------------------------------------------------------------------

} // namespace atlas

#endif

