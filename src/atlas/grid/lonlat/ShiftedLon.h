/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#ifndef atlas_grid_lonlat_ShiftedLon_h
#define atlas_grid_lonlat_ShiftedLon_h

#include "atlas/grid/lonlat/LonLat.h"


namespace atlas {
namespace grid {
namespace lonlat {


class ShiftedLon : public LonLat {

  public:

    static std::string grid_type_str();

    static std::string className();

    std::string gridType() const;

    ShiftedLon(const eckit::Parametrisation&);

    ShiftedLon(const size_t N, const Domain& dom=Domain::makeGlobal());

    ShiftedLon(const size_t nlon, const size_t nlat, const Domain& dom=Domain::makeGlobal());

    virtual eckit::Properties spec() const;

    size_t nlon() const {
        return Structured::nlon(0);
    }

    double lon(const size_t jlon) const {
        return Structured::lon(0,jlon);
    }

  private:

    void setup(const eckit::Parametrisation&);

    std::string shortName() const;

  private:

    mutable std::string shortName_;

};


}  // namespace lonlat
}  // namespace grid
}  // namespace atlas


#endif