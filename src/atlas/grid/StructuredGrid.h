/*
 * (C) Copyright 2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */

#pragma once

#include <functional>
#include <initializer_list>
#include <string>

#include "atlas/grid/Grid.h"
#include "atlas/grid/detail/grid/Structured.h"


namespace atlas {

//---------------------------------------------------------------------------------------------------------------------
class StructuredGrid;
class RegularGrid;
class GaussianGrid;
class ReducedGaussianGrid;
class RegularGaussianGrid;
class RegularLonLatGrid;
class ShiftedLonLatGrid;

/*
                                             Grid
                                               |
                                    +----------+----------+
                                    |                     |
                             StructuredGrid        UnstructuredGrid
                                    |
               +--------------------+-----------------------+
               |                    |                       |
          ReducedGrid          GaussianGrid            RegularGrid
               |                 |     |                 |     |
               +--------+--------+     +--------+--------+     +-----+
                        |                       |                    |
               ReducedGaussianGrid     RegularGaussianGrid    RegularLonLatGrid
*/

//---------------------------------------------------------------------------------------------------------------------

class StructuredGrid : public Grid {
public:
    using grid_t = grid::detail::grid::Structured;
    using XSpace = grid_t::XSpace;
    using YSpace = grid_t::YSpace;

public:
    StructuredGrid();
    StructuredGrid( const Grid& );
    StructuredGrid( const Grid::Implementation* );
    StructuredGrid( const std::string& name, const Domain& = Domain() );
    StructuredGrid( const Config& );
    StructuredGrid( const XSpace&, const YSpace&, const Projection& = Projection(), const Domain& = Domain() );
    StructuredGrid( const Grid&, const Domain& );

    operator bool() const { return valid(); }

    bool valid() const { return grid_; }

    inline idx_t ny() const { return grid_->ny(); }

    inline idx_t nx( idx_t j ) const { return grid_->nx( j ); }

    inline const std::vector<idx_t>& nx() const { return grid_->nx(); }

    inline idx_t nxmax() const { return grid_->nxmax(); }

    inline const std::vector<double>& y() const { return grid_->y(); }

    inline double x( idx_t i, idx_t j ) const { return grid_->x( i, j ); }

    inline double y( idx_t j ) const { return grid_->y( j ); }

    using Grid::xy;
    void xy( idx_t i, idx_t j, double xy[] ) const { grid_->xy( i, j, xy ); }

    using Grid::lonlat;
    void lonlat( idx_t i, idx_t j, double lonlat[] ) const { grid_->lonlat( i, j, lonlat ); }

    PointXY xy( idx_t i, idx_t j ) const { return PointXY( x( i, j ), y( j ) ); }

    PointLonLat lonlat( idx_t i, idx_t j ) const { return grid_->lonlat( i, j ); }

    inline bool reduced() const { return grid_->reduced(); }

    inline bool regular() const { return not reduced(); }

    bool periodic() const { return grid_->periodic(); }

    const XSpace& xspace() const { return grid_->xspace(); }

    const YSpace& yspace() const { return grid_->yspace(); }

private:
    const grid_t* grid_;
};

//---------------------------------------------------------------------------------------------------------------------

class ReducedGrid : public StructuredGrid {
public:
    using StructuredGrid::StructuredGrid;

    operator bool() const { return valid(); }

    bool valid() const { return StructuredGrid::valid() && reduced(); }
};

//---------------------------------------------------------------------------------------------------------------------

class RegularGrid : public StructuredGrid {
public:
    using StructuredGrid::StructuredGrid;
    using StructuredGrid::x;
    using StructuredGrid::xy;

    operator bool() const { return valid(); }

    bool valid() const { return StructuredGrid::valid() && regular(); }

    idx_t nx() const { return nxmax(); }

    inline double x( idx_t i ) const { return x( i, 0 ); }

    PointXY xy( idx_t i, idx_t j ) const { return PointXY( x( i ), y( j ) ); }
};

//---------------------------------------------------------------------------------------------------------------------

template <class Grid>
class Gaussian : public Grid {
public:
    using Grid::Grid;

    idx_t N() const { return Grid::ny() / 2; }

    inline double lon( idx_t i, idx_t j ) const { return Grid::x( i, j ); }

    inline double lat( idx_t j ) const { return Grid::y( j ); }

    PointLonLat lonlat( idx_t i, idx_t j ) const { return Grid::xy( i, j ); }

protected:
    bool gaussian() const {
        return Grid::domain().global() && not Grid::projection() && Grid::yspace().type() == "gaussian";
    }
};

//---------------------------------------------------------------------------------------------------------------------

class GaussianGrid : public Gaussian<StructuredGrid> {
    using grid_t = Gaussian<StructuredGrid>;

public:
    using grid_t::grid_t;

    operator bool() const { return valid(); }

    bool valid() const { return StructuredGrid::valid() && gaussian(); }
};

//---------------------------------------------------------------------------------------------------------------------

class ReducedGaussianGrid : public Gaussian<ReducedGrid> {
    using grid_t = Gaussian<ReducedGrid>;

public:
    using grid_t::grid_t;
    ReducedGaussianGrid( const std::initializer_list<idx_t>& pl );
    ReducedGaussianGrid( const std::vector<long>& pl, const Domain& = Domain() );
    ReducedGaussianGrid( const std::vector<int>& pl, const Domain& = Domain() );

    operator bool() const { return valid(); }

    bool valid() const { return ReducedGrid::valid() && gaussian(); }
};

//---------------------------------------------------------------------------------------------------------------------

class RegularGaussianGrid : public Gaussian<RegularGrid> {
    using grid_t = Gaussian<RegularGrid>;

public:
    using grid_t::grid_t;
    RegularGaussianGrid( int N, const Domain& = Domain() );

    inline double lon( idx_t i ) const { return x( i ); }

    inline double lat( idx_t j ) const { return y( j ); }

    PointLonLat lonlat( idx_t i, idx_t j ) const { return xy( i, j ); }

    operator bool() const { return valid(); }

    bool valid() const { return RegularGrid::valid() && gaussian(); }
};

//---------------------------------------------------------------------------------------------------------------------

class RegularLonLatGrid : public RegularGrid {
public:
    using RegularGrid::RegularGrid;

public:
    operator bool() const { return valid(); }

    bool valid() const { return RegularGrid::valid() && global_lonlat(); }

    inline double lon( idx_t i ) const { return x( i ); }

    inline double lat( idx_t j ) const { return y( j ); }

    PointLonLat lonlat( idx_t i, idx_t j ) const { return xy( i, j ); }

    bool standard() const { return standard_lon() && standard_lat(); }
    bool shifted() const { return shifted_lon() && shifted_lat(); }
    bool shiftedLon() const { return shifted_lon() && standard_lat(); }
    bool shiftedLat() const { return standard_lon() && shifted_lat(); }

protected:
    bool global_lonlat() const { return domain().global() && not projection() && yspace().type() == "linear"; }

    bool standard_lon() const { return x( 0 ) == 0.; }

    bool standard_lat() const { return y( 0 ) == 90. && ny() % 2 == 1; }

    bool shifted_lon() const { return x( 0 ) == 0.5 * 360. / nx(); }

    bool shifted_lat() const { return y( 0 ) == 90. - 0.5 * 180. / ny() && ny() % 2 == 0; }
};

//---------------------------------------------------------------------------------------------------------------------

}  // namespace atlas
