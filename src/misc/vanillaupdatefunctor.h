#ifndef _libgeodecomp_misc_vanillaupdatefunctor_h_
#define _libgeodecomp_misc_vanillaupdatefunctor_h_

namespace LibGeoDecomp {

/**
 * updates a Streak of cells using the "vanilla" API (i.e.
 * LibGeoDecomp's classic cell interface which calls update() once per
 * cell and facilitates access to neighboring cells via a proxy object.
 */
template<typename CELL>
class VanillaUpdateFunctor
{
public:
    static const int DIM = CELL::Topology::DIMENSIONS;

    template<typename GRID1, typename GRID2>
    void operator()(
        const Streak<DIM>& streak,
        const Coord<DIM>& targetOrigin,
        const GRID1& gridOld,
        GRID2 *gridNew,
        unsigned nanoStep) 
    {
        Coord<DIM> sourceCoord = streak.origin;
        Coord<DIM> targetCoord = targetOrigin;

        for (; sourceCoord.x() < streak.endX; ++sourceCoord.x()) {
            (*gridNew)[targetCoord].update(gridOld.getNeighborhood(sourceCoord), nanoStep);
            ++targetCoord.x();
        }
    }
};

}

#endif
