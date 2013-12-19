#ifndef LIBGEODECOMP_MISC_NONPODTESTCELL_H
#define LIBGEODECOMP_MISC_NONPODTESTCELL_H

#include <cxxtest/TestSuite.h>

#include <libgeodecomp/io/simpleinitializer.h>
#include <libgeodecomp/misc/apitraits.h>
#include <libgeodecomp/misc/stdcontaineroverloads.h>

namespace LibGeoDecomp {

/**
 * This class is a test vehicle for checking whether a Simulator
 * handles cells which are not plain old datatypes (PoDs) correctly.
 * Basically all cells which include pointers or heap allocated
 * containes are not PoDs.
 *
 * It collects coordinates in some containers and pulls values from
 * neighbors to allow test code to check whether its own sets and the
 * neighboring cells are still valid.
 */
class NonPoDTestCell
{
public:
    friend class NonPoDTestCellTest;

    class API :
        public APITraits::HasBoostSerialization,
        public APITraits::HasFixedCoordsOnlyUpdate
    {};

    class Initializer : public SimpleInitializer<NonPoDTestCell>
    {
    public:
        Initializer() : SimpleInitializer(Coord<2>(15, 10), 20)
        {}

        virtual void grid(GridBase<NonPoDTestCell, 2> *target)
        {
            CoordBox<2> simSpace(Coord<2>(), gridDimensions());
            CoordBox<2> box(target->boundingBox());
            for (CoordBox<2>::Iterator i = box.begin(); i != box.end(); ++i) {
                target->set(*i, NonPoDTestCell(*i, simSpace));
            }
        }
    };

    NonPoDTestCell(const Coord<2>& coord = Coord<2>(), const CoordBox<2>& simSpace = CoordBox<2>()) :
        coord(coord),
        simSpace(simSpace),
        cycleCounter(0)
    {
        seenNeighbors << coord;

        for (CoordBox<2>::Iterator i = simSpace.begin(); i != simSpace.end(); ++i) {
            missingNeighbors << *i;
        }
    }

    template<typename NEIGHBORHOOD>
    void update(const NEIGHBORHOOD& neighborhood, int nanoStep)
    {
#define HOOD(X, Y) neighborhood[FixedCoord<X, Y, 0>()]

        coord = HOOD( 0,  0).coord;
        simSpace = HOOD( 0,  0).simSpace;
        cycleCounter = HOOD( 0,  0).cycleCounter + 1;

        // ultimately seenNeighbors should contain all coordinates (as
        // missingNeighbors did initiallty)...
        seenNeighbors |= HOOD(-1, -1).seenNeighbors;
        seenNeighbors |= HOOD( 0, -1).seenNeighbors;
        seenNeighbors |= HOOD( 1, -1).seenNeighbors;

        seenNeighbors |= HOOD(-1,  0).seenNeighbors;
        seenNeighbors |= HOOD( 0,  0).seenNeighbors;
        seenNeighbors |= HOOD( 1,  0).seenNeighbors;

        seenNeighbors |= HOOD(-1,  1).seenNeighbors;
        seenNeighbors |= HOOD( 0,  1).seenNeighbors;
        seenNeighbors |= HOOD( 1,  1).seenNeighbors;

        // ...and missingNeighbors should become empty
        missingNeighbors = HOOD(0, 0).missingNeighbors - seenNeighbors;
#undef HOOD

        check();
    }

private:
    Coord<2> coord;
    CoordBox<2> simSpace;
    int cycleCounter;
    std::set<Coord<2> > seenNeighbors;
    std::set<Coord<2> > missingNeighbors;

    void check()
    {
        Coord<2> distance = Coord<2>(1, 1) * cycleCounter;
        CoordBox<2> seenBox(coord - distance, distance * 2 + Coord<2>(1, 1));
        Coord<2> origin = Coord<2>(0, 0).max(seenBox.origin);
        Coord<2> oppositeCorner1 = seenBox.origin + seenBox.dimensions;
        Coord<2> oppositeCorner2 = simSpace.origin + simSpace.dimensions;
        seenBox.origin = origin;
        seenBox.dimensions = oppositeCorner1.min(oppositeCorner2) - origin;

        for (CoordBox<2>::Iterator i = seenBox.begin(); i != seenBox.end(); ++i) {
            TS_ASSERT_EQUALS(seenNeighbors.count(*i), 1);
            TS_ASSERT_EQUALS(missingNeighbors.count(*i), 0);
        }
    }
};

}

#endif