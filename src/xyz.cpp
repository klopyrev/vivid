#include "vivid/conversion.h"
#include "vivid/functions.h"
#include "vivid/profiles.h"

namespace vivid::xyz {


////////////////////////////////////////////////////////////////////////////////
xyz_t fromLab( const lab_t& lab )
{
    xyz_t xyz;
    xyz.y = ( lab.x + 16.f ) / 116.f;
    xyz.x = xyz.y + lab.y / 500.0f;
    xyz.z = xyz.y - lab.z / 200.0f;

    auto lab2xyz = []( const float x ) -> float {
        return ( x > 0.206893034f ) ?
            ( x * x * x ) :
            ( ( x - 16.f / 116.f ) / 7.787037f );
    };

    xyz.x = lab2xyz( xyz.x );
    xyz.y = lab2xyz( xyz.y );
    xyz.z = lab2xyz( xyz.z );

    return static_cast<xyz_t>( xyz * profiles::tri_d65 );
}


////////////////////////////////////////////////////////////////////////////////
xyz_t fromSrgb( const srgb_t& srgb ) {
    return static_cast<xyz_t>(
        matrices::rgb_to_xyz * lrgb::fromSrgb( srgb )
    );
}


////////////////////////////////////////////////////////////////////////////////
xyz_t fromLch( const lch_t& lch ) {
    return xyz::fromLab( lab::fromLch( lch ) );
}


////////////////////////////////////////////////////////////////////////////////
xyz_t fromAdobe( const adobe_t& adobe ) {
    return static_cast<xyz_t>(
        matrices::adobe_to_xyz * rgb::gamma( adobe, 1.f / profiles::gamma_adobe )
    );
}



}   //  ::vivid::xyz
