//
// Copyright (C) 2015-present  Aleksandar Zlateski <zlateski@mit.edu>
// ----------------------------------------------------------
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef ZNN_CORE_FILTER_HPP_INCLUDED
#define ZNN_CORE_FILTER_HPP_INCLUDED

#include "types.hpp"
#include "volume_operators.hpp"

namespace zi {
namespace znn {

class filter
{
private:
    vol<double>   W_;

    // weight update stuff
    double        eta_          ;
    double        momentum_     ;
    vol<double>   mom_volume_   ;
    double        weight_decay_ ;

public:
    const vol<double>& W() const
    {
        return W_;
    }

    void update(const vol<double>& dEdW, double patch_size = 0 )
    {
        double delta = ( patch_size != 0 ) ? -eta_/patch_size : -eta_;

        if ( momentum_ == 0 )
        {
            // W' = W - eta*dEdW/ps - eta*wd*W
            //    = W(1 - eta*wd)  - eta*dEdW/ps;

            if ( weight_decay_ != 0 )
            {
                W_ *= static_cast<double>(1) - eta_ * weight_decay_;
            }

            mad_to( delta, dEdW, W_ );
        }
        else
        {
            mom_volume_ *= momentum_;
            mad_to( delta, dEdW, mom_volume_ );

            if ( weight_decay_ != 0 )
            {
                mad_to( -eta_ * weight_decay_, W_, mom_volume_ );
            }

            W_ += mom_volume_;
        }
    }
};

}} // namespace zi::znn

#endif // ZNN_CORE_FILTER_HPP_INCLUDED
