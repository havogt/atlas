// TL159

#include "atlas/grids/rgg/rgg.h"

namespace atlas {
namespace grids {
namespace rgg {

eckit::ConcreteBuilderT1<Grid,N80> builder_N80 (N80::grid_type_str());

void N80::construct()
{
  int N=80;
  int lon[] = {
     18,
     25,
     36,
     40,
     45,
     54,
     60,
     64,
     72,
     72,
     80,
     90,
     96,
    100,
    108,
    120,
    120,
    128,
    135,
    144,
    144,
    150,
    160,
    160,
    180,
    180,
    180,
    192,
    192,
    200,
    200,
    216,
    216,
    216,
    225,
    225,
    240,
    240,
    240,
    256,
    256,
    256,
    256,
    288,
    288,
    288,
    288,
    288,
    288,
    288,
    288,
    288,
    300,
    300,
    300,
    300,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320,
    320
  };
  std::vector<double> lats(N);
  gaussian_latitudes_npole_equator(N,lats.data());
  setup_lat_hemisphere(N,lats.data(),lon,DEG);
}

} // namespace rgg
} // namespace grids
} // namespace atlas
