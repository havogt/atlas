/*
 * (C) Copyright 1996-2014 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <sstream>
#include <algorithm>
 
#include "atlas/atlas_config.h"
#include "atlas/meshgen/RGG.hpp"
#include "atlas/meshgen/EqualAreaPartitioner.hpp"
#include "atlas/io/Gmsh.hpp"
#include "atlas/mesh/Mesh.hpp"
#include "atlas/mesh/FunctionSpace.hpp"
#include "atlas/mesh/Metadata.hpp"
#include "atlas/actions/BuildEdges.hpp"
#include "atlas/actions/BuildDualMesh.hpp"
#include "atlas/actions/BuildPeriodicBoundaries.hpp"
#include "atlas/testing/UnitTest.hpp"

using namespace atlas;
using namespace atlas::meshgen;

#define DISABLE if(0)
#define ENABLE if(1)

namespace atlas {
namespace test {
  
class DebugMesh:   public RGG { public: DebugMesh(); };
DebugMesh::DebugMesh()
{
  int nlat=5;
  int lon[] = {
    6,
    10,
    18,
    22,
    22,
  };
  /*
  First prediction of colatitudes
  */
  std::vector<double> colat(nlat);
  double z;
  for( int i=0; i<nlat; ++i )
  {
    z = (4.*(i+1.)-1.)*M_PI/(4.*2.*nlat+2.);
    colat[i] = z+1./(tan(z)*(8.*(2.*nlat)*(2.*nlat)));
  }
  /*
  Fill in final structures
  */
  lat_.resize(2*nlat);
  lon_.resize(2*nlat);
  std::copy( lon, lon+nlat, lon_.begin() );
  std::reverse_copy( lon, lon+nlat, lon_.begin()+nlat );
  std::copy( colat.begin(), colat.begin()+nlat, lat_.begin() );
  std::reverse_copy( colat.begin(), colat.begin()+nlat, lat_.begin()+nlat );
  for (int i=0; i<nlat; ++i)
    lat_[i]=M_PI/2.-lat_[i];
  for (int i=nlat; i<2*nlat; ++i)
    lat_[i]=-M_PI/2.+lat_[i];
} 

  
class MinimalMesh:   public RGG { public: MinimalMesh(int nlat, int lon[]); };
MinimalMesh::MinimalMesh(int nlat, int lon[])
{
  /*
  First prediction of colatitudes
  */
  std::vector<double> colat(nlat);
  double z;
  for( int i=0; i<nlat; ++i )
  {
    z = (4.*(i+1.)-1.)*M_PI/(4.*2.*nlat+2.);
    colat[i] = z+1./(tan(z)*(8.*(2.*nlat)*(2.*nlat)));
  }
  /*
  Fill in final structures
  */
  lat_.resize(2*nlat);
  lon_.resize(2*nlat);
  std::copy( lon, lon+nlat, lon_.begin() );
  std::reverse_copy( lon, lon+nlat, lon_.begin()+nlat );
  std::copy( colat.begin(), colat.begin()+nlat, lat_.begin() );
  std::reverse_copy( colat.begin(), colat.begin()+nlat, lat_.begin()+nlat );
  for (int i=0; i<nlat; ++i)
    lat_[i]=M_PI/2.-lat_[i];
  for (int i=nlat; i<2*nlat; ++i)
    lat_[i]=-M_PI/2.+lat_[i];
} 

} // end namespace test
} // end namespace atlas

class TestMeshGen: public UnitTest
{
public:
  TestMeshGen(int argc, char *argv[]) : UnitTest(argc,argv) {}
  
  virtual void run_tests()
  {
    // test_eq_caps();
    // test_partitioner();
    test_rgg_meshgen_one_part();
    // test_rgg_meshgen_many_parts();
  }
  
  void test_eq_caps()
  {
    std::vector<int>    n_regions;
    std::vector<double> s_cap;

    eq_caps(6, n_regions, s_cap);
    ATLAS_CHECK_EQUAL( n_regions.size(), 3 );
    ATLAS_CHECK_EQUAL( n_regions[0], 1 );
    ATLAS_CHECK_EQUAL( n_regions[1], 4 );
    ATLAS_CHECK_EQUAL( n_regions[2], 1 );

    eq_caps(10, n_regions, s_cap);
    ATLAS_CHECK_EQUAL( n_regions.size(), 4 );
    ATLAS_CHECK_EQUAL( n_regions[0], 1 );
    ATLAS_CHECK_EQUAL( n_regions[1], 4 );
    ATLAS_CHECK_EQUAL( n_regions[2], 4 );
    ATLAS_CHECK_EQUAL( n_regions[3], 1 );
  }
  
  void test_partitioner()
  {    
    // 12 partitions
    {
      EqualAreaPartitioner partitioner(12);
      ATLAS_CHECK_EQUAL( partitioner.nb_bands(),    4 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(0), 1 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(1), 5 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(2), 5 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(3), 1 );
    }
    
    // 24 partitions
    {
      EqualAreaPartitioner partitioner(24);
      ATLAS_CHECK_EQUAL( partitioner.nb_bands(),     5 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(0),  1 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(1),  6 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(2), 10 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(3),  6 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(4),  1 );
    }
    
    // 48 partitions
    {
      EqualAreaPartitioner partitioner(48);
      ATLAS_CHECK_EQUAL( partitioner.nb_bands(),     7 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(0),  1 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(1),  6 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(2), 11 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(3), 12 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(4), 11 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(5),  6 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(6),  1 );
    }
    
    // 96 partitions
    {
      EqualAreaPartitioner partitioner(96);
      ATLAS_CHECK_EQUAL( partitioner.nb_bands(),    10 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(0),  1 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(1),  6 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(2), 11 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(3), 14 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(4), 16 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(5), 16 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(6), 14 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(7), 11 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(8),  6 );
      ATLAS_CHECK_EQUAL( partitioner.nb_regions(9),  1 );
    }
  }
  
  void test_rgg_meshgen_one_part()
  {
    Mesh* m;
    RGGMeshGenerator generate;
    generate.options.set("nb_parts",1);
    generate.options.set("part",    0);

    ENABLE {
      generate.options.set("three_dimensional",true);
      generate.options.set("include_pole",false);
      m = generate( atlas::test::DebugMesh() );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).extents()[0], 156 );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).extents()[0], 134 );
      ATLAS_CHECK_EQUAL( m->function_space("triags").extents()[0],  32 );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).metadata().get<int>("max_glb_idx"), 156 );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).metadata().get<int>("nb_owned"),    156 );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).metadata().get<int>("max_glb_idx"), 166 );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).metadata().get<int>("nb_owned"),    134 );
      ATLAS_CHECK_EQUAL( m->function_space("triags").metadata().get<int>("max_glb_idx"), 166 );
      ATLAS_CHECK_EQUAL( m->function_space("triags").metadata().get<int>("nb_owned"),     32 );
      delete m;
    }
    
    ENABLE {
      generate.options.set("three_dimensional",false);
      generate.options.set("include_pole",false);
      m = generate( atlas::test::DebugMesh() );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).extents()[0], 166 );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).extents()[0], 134 );
      ATLAS_CHECK_EQUAL( m->function_space("triags").extents()[0],  32 );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).metadata().get<int>("max_glb_idx"), 166 );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).metadata().get<int>("nb_owned"),    166 );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).metadata().get<int>("max_glb_idx"), 166 );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).metadata().get<int>("nb_owned"),    134 );
      ATLAS_CHECK_EQUAL( m->function_space("triags").metadata().get<int>("max_glb_idx"), 166 );
      ATLAS_CHECK_EQUAL( m->function_space("triags").metadata().get<int>("nb_owned"),     32 );
      delete m;
    }

    ENABLE {
      generate.options.set("three_dimensional",true);
      generate.options.set("include_pole",true);
      m = generate( atlas::test::DebugMesh() );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).extents()[0], 158 );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).extents()[0], 134 );
      ATLAS_CHECK_EQUAL( m->function_space("triags").extents()[0],  44 );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).metadata().get<int>("max_glb_idx"), 158 );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).metadata().get<int>("nb_owned"),    158 );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).metadata().get<int>("max_glb_idx"), 178 );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).metadata().get<int>("nb_owned"),    134 );
      ATLAS_CHECK_EQUAL( m->function_space("triags").metadata().get<int>("max_glb_idx"), 178 );
      ATLAS_CHECK_EQUAL( m->function_space("triags").metadata().get<int>("nb_owned"),     44 );
      delete m;
    }

    Mesh* mesh;    

    ENABLE {
      generate.options.set("three_dimensional",false);
      generate.options.set("include_pole",false);
      int nlat=2;
      int lon[] = { 4, 6 };
      mesh = generate( test::MinimalMesh(nlat,lon) );
      ATLAS_CHECK_EQUAL( mesh->function_space("nodes" ).extents()[0], 24 );
      ATLAS_CHECK_EQUAL( mesh->function_space("quads" ).extents()[0], 14 );
      ATLAS_CHECK_EQUAL( mesh->function_space("triags").extents()[0],  4 );
    
      build_periodic_boundaries(*mesh);
      build_edges(*mesh);
      build_dual_mesh(*mesh);
      Gmsh::write(*mesh,"minimal2.msh");
      delete mesh;
    }
    // 3 latitudes
    ENABLE {
      int nlat=3;
      int lon[] = { 4, 6, 8 };
      mesh = generate( test::MinimalMesh(nlat,lon) );
      ATLAS_CHECK_EQUAL( mesh->function_space("nodes" ).extents()[0], 42 );
      ATLAS_CHECK_EQUAL( mesh->function_space("quads" ).extents()[0], 28 );
      ATLAS_CHECK_EQUAL( mesh->function_space("triags").extents()[0],  8 );
    
      build_periodic_boundaries(*mesh);
      build_edges(*mesh);
      build_dual_mesh(*mesh);
      Gmsh::write(*mesh,"minimal3.msh");
      delete mesh;
    }
    // 4 latitudes
    ENABLE {
      int nlat=4;
      int lon[] = { 4, 6, 8, 10 };
      mesh = generate( test::MinimalMesh(nlat,lon) );
      ATLAS_CHECK_EQUAL( mesh->function_space("nodes" ).extents()[0], 64 );
      ATLAS_CHECK_EQUAL( mesh->function_space("quads" ).extents()[0], 46 );
      ATLAS_CHECK_EQUAL( mesh->function_space("triags").extents()[0], 12 );
    
      build_periodic_boundaries(*mesh);
      build_edges(*mesh);
      build_dual_mesh(*mesh);
      Gmsh::write(*mesh,"minimal4.msh");
      delete mesh;
    }
    // 5 latitudes WIP
    ENABLE {
      int nlat=5;
      int lon[] = { 6, 10, 18, 22, 22 };
      mesh = generate( test::MinimalMesh(nlat,lon) );
      // ATLAS_CHECK_EQUAL( mesh->function_space("nodes" ).extents()[0], 42 );
      // ATLAS_CHECK_EQUAL( mesh->function_space("quads" ).extents()[0], 28 );
      // ATLAS_CHECK_EQUAL( mesh->function_space("triags").extents()[0],  8 );
    
      build_periodic_boundaries(*mesh);
      build_edges(*mesh);
      build_dual_mesh(*mesh);
      Gmsh::write(*mesh,"minimal5.msh");
      delete mesh;
    }
    // 10 latitudes WIP
    ENABLE {
      int nlat=6;
      int lon[] = {
        36-25,
        40-25,
        45-25,
        50-25,
        60-25,
        64-25,
        72-25,
        75-25 };
      // int lon[] = { 4, 6, 8, 10, 12, 14, 16, 18, 20, 22 };
      mesh = generate( test::MinimalMesh(nlat,lon) );
      // ATLAS_CHECK_EQUAL( mesh->function_space("nodes" ).extents()[0], 42 );
      // ATLAS_CHECK_EQUAL( mesh->function_space("quads" ).extents()[0], 28 );
      // ATLAS_CHECK_EQUAL( mesh->function_space("triags").extents()[0],  8 );
      Gmsh::write(*mesh,"before.msh");
    
      build_periodic_boundaries(*mesh);
      build_edges(*mesh);
      // build_dual_mesh(*mesh);
      Gmsh::write(*mesh,"after.msh");
      delete mesh;
    }
    // T63 WIP
    ENABLE {
      mesh = generate( T63() );
      // ATLAS_CHECK_EQUAL( mesh->function_space("nodes" ).extents()[0], 42 );
      // ATLAS_CHECK_EQUAL( mesh->function_space("quads" ).extents()[0], 28 );
      // ATLAS_CHECK_EQUAL( mesh->function_space("triags").extents()[0],  8 );
    
      build_periodic_boundaries(*mesh);
      build_edges(*mesh);
      build_dual_mesh(*mesh);
      Gmsh::write(*mesh,"t63.msh");
      delete mesh;
    }
  }
  
  void test_rgg_meshgen_many_parts()
  {
    RGGMeshGenerator generate;
    generate.options.set("nb_parts",20);
    generate.options.set("include_pole",false);
    generate.options.set("three_dimensional",false);

    int nodes[]  = {312,317,333,338,335,352,350,359,360,361,358,360,359,370,337,334,338,335,332,314};
    int quads[]  = {242,277,291,294,292,307,312,320,321,322,319,321,320,331,293,291,294,293,290,244};
    int triags[] = {42, 12, 13, 12, 12, 15, 0,  1,  0,  0,  2,  0,  1,  0,  14, 12, 13, 11, 14, 42 };
    for( int p=0; p<generate.options.get<int>("nb_parts"); ++p)
    {
      generate.options.set("part",p);
      Mesh* m = generate( T63() );
      ATLAS_CHECK_EQUAL( m->function_space("nodes" ).extents()[0], nodes[p]  );
      ATLAS_CHECK_EQUAL( m->function_space("quads" ).extents()[0], quads[p]  );
      ATLAS_CHECK_EQUAL( m->function_space("triags").extents()[0], triags[p] );
      std::stringstream filename; filename << "d" << p << ".msh";
      Gmsh::write(*m,filename.str());
      delete m;
    }
  }
};


int main(int argc, char *argv[])
{
  TestMeshGen tests(argc,argv);
  tests.start();
  return tests.exit_status();
}