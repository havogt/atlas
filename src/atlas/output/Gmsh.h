/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef atlas_output_Gmsh_h
#define atlas_output_Gmsh_h

#include "atlas/output/Output.h"
#include "atlas/parallel/mpi/mpi.h"
#include "atlas/util/Config.h"

namespace atlas {
namespace util {
namespace io {
class Gmsh;
}
}
}

namespace atlas {
namespace output {

// -----------------------------------------------------------------------------

class GmshFileStream : public std::ofstream {
public:
  static std::string parallelPathName(const PathName& path,int part = parallel::mpi::comm().rank());
  GmshFileStream(const PathName& file_path, const char* mode, int part = parallel::mpi::comm().rank());

};

// -----------------------------------------------------------------------------

class Gmsh : public Output {

public:

  typedef eckit::SharedPtr<Gmsh> Ptr;

public:

    Gmsh(Stream&);
    Gmsh(Stream&, const eckit::Parametrisation&);

    Gmsh(const PathName&, const std::string& mode);
    Gmsh(const PathName&, const std::string& mode, const eckit::Parametrisation&);

    Gmsh(const PathName&);
    Gmsh(const PathName&, const eckit::Parametrisation&);

    virtual ~Gmsh();

    /// Write mesh file
    virtual void write(
        const mesh::Mesh&,
        const eckit::Parametrisation& = util::NoConfig() ) const;

    /// Write field to file
    virtual void write(
        const field::Field&,
        const eckit::Parametrisation& = util::NoConfig() ) const;

    /// Write fieldset to file using FunctionSpace
    virtual void write(
        const field::FieldSet&,
        const eckit::Parametrisation& = util::NoConfig() ) const;

    /// Write field to file using Functionspace
    virtual void write(
        const field::Field&,
        const functionspace::FunctionSpace&,
        const eckit::Parametrisation& = util::NoConfig() ) const;

    /// Write fieldset to file using FunctionSpace
    virtual void write(
        const field::FieldSet&,
        const functionspace::FunctionSpace&,
        const eckit::Parametrisation& = util::NoConfig() ) const;

public:

  struct Configuration {
    bool binary;
    bool edges;
    bool elements;
    bool gather;
    bool ghost;
    bool info;
    std::vector<long> levels;
    std::string nodes;
    std::string file;
    std::string openmode;
    std::string coordinates;
  };

  static void setGmshConfiguration(util::io::Gmsh&, const Configuration& );

private:


  mutable Configuration config_;

  void defaults();
};

// -----------------------------------------------------------------------------

#define Parametrisation eckit::Parametrisation

extern "C" {
  
Gmsh* atlas__output__Gmsh__create_pathname_mode(const char* pathname, const char* mode);
Gmsh* atlas__output__Gmsh__create_pathname_mode_config(const char* pathname, const char* mode, const Parametrisation* params);

}

#undef Parametrisation

// -----------------------------------------------------------------------------

} // namespace output
} // namespace atlas

#endif