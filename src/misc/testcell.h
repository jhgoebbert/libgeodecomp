#ifndef LIBGEODECOMP_MISC_TESTCELL_H
#define LIBGEODECOMP_MISC_TESTCELL_H

#include <libgeodecomp/config.h>
#include <libgeodecomp/misc/apitraits.h>
#include <libgeodecomp/geometry/coord.h>
#include <libgeodecomp/geometry/coordbox.h>
#include <libgeodecomp/geometry/stencils.h>
#include <libgeodecomp/storage/coordmap.h>
#include <libgeodecomp/storage/fixedneighborhood.h>

#include <iostream>
#include <libflatarray/flat_array.hpp>

namespace LibGeoDecomp {

namespace TestCellHelpers {

/**
 * We'll use the TestCell with different API specs. This is one of
 * them...
 */
class EmptyAPI
{};

/**
 * ...and this another.
 */
class SoAAPI :
        public APITraits::HasSoA,
        public APITraits::HasFixedCoordsOnlyUpdate,
        public APITraits::HasUpdateLineX
{};

/**
 * Various topologies are to be tested, too.
 */
template<int DIM>
class TopologyType
{
public:
    typedef typename Topologies::Cube<DIM>::Topology Topology;
};

/**
 * Make the 3D TestCell use a torus topology for a change...
 */
template<>
class TopologyType<3>
{
public:
    typedef Topologies::Torus<3>::Topology Topology;
};

/**
 * We'll use this class to enble debug output on host code and disable
 * it on CUDA devices (where std::cout isn't available).
 */
class StdOutput
{
public:
    template<typename T>
    const StdOutput& operator<<(T output) const
    {
        std::cout << output;
        return *this;
    }
};

/**
 * see above
 */
class NoOutput
{
public:
    template<typename T>
    const NoOutput& operator<<(T /*unused*/) const
    {
        return *this;
    }
};

/**
 * Helps with iterating through a stencil shape
 */
template<class STENCIL, int INDEX>
class CheckNeighbor
{
public:
    typedef typename STENCIL::template Coords<INDEX> RelCoord;

    template<class TESTCELL, class NEIGHBORHOOD>
    __host__ __device__
    void operator()(bool *isValid, TESTCELL *cell, const NEIGHBORHOOD& neighborhood)
    {
        (*isValid) &= cell->checkNeighbor(neighborhood[RelCoord()], RelCoord());
    }
};

}

/**
 * Useful for verifying the various parallelizations in LibGeoDecomp
 */
template<
    int DIM,
    typename STENCIL = Stencils::Moore<DIM, 1>,
    typename TOPOLOGY = typename TestCellHelpers::TopologyType<DIM>::Topology,
    typename ADDITIONAL_API = TestCellHelpers::EmptyAPI,
    typename OUTPUT = TestCellHelpers::StdOutput>
class TestCell
{
public:
    friend class Typemaps;
    friend class TestCellTest;

    static const int DIMENSIONS = DIM;
    static const unsigned NANO_STEPS = 27;

    class API :
        public ADDITIONAL_API,
        public APITraits::HasTopology<TOPOLOGY>,
        public APITraits::HasNanoSteps<NANO_STEPS>,
        public APITraits::HasStencil<STENCIL>
#ifdef LIBGEODECOMP_WITH_MPI
        , public APITraits::HasAutoGeneratedMPIDataType<Typemaps>
#endif
    {};

    Coord<DIM> pos;
    CoordBox<DIM> dimensions;
    unsigned cycleCounter;
    bool isEdgeCell;
    bool isValid;
    double testValue;

    static double defaultValue()
    {
        return 666;
    }

    TestCell() :
        cycleCounter(0),
        isEdgeCell(false),
        isValid(false),
        testValue(defaultValue())
    {}

    TestCell(
        const Coord<DIM>& pos,
        const Coord<DIM>& gridDim,
        const unsigned& cycleCounter = 0,
        const double& testValue = defaultValue()) :
        pos(pos),
        dimensions(Coord<DIM>(), gridDim),
        cycleCounter(cycleCounter),
        isValid(true),
        testValue(testValue)
    {
        isEdgeCell = !inBounds(pos);
    }

    const bool& valid() const
    {
        return isValid;
    }

    __host__ __device__
    bool inBounds(const Coord<DIM>& c) const
    {
        return !TOPOLOGY::isOutOfBounds(c, dimensions.dimensions);
    }

    bool operator==(const TestCell& other) const
    {
        return (pos == other.pos)
            && (dimensions == other.dimensions)
            && (cycleCounter == other.cycleCounter)
            && (isEdgeCell == other.isEdgeCell)
            && (isValid == other.isValid)
            && (testValue == other.testValue);
    }

    bool operator!=(const TestCell& other) const
    {
        return !((*this) == other);
    }

    template<typename COORD_MAP>
    __host__ __device__
    void update(const COORD_MAP& neighborhood, const unsigned& nanoStep)
    {
        // initialize Cell by copying from previous state
        *this = TestCell(neighborhood[FixedCoord<0, 0, 0>()]);

        if (isEdgeCell) {
#ifndef __CUDACC__
            OUTPUT() << "TestCell error: update called for edge cell\n";
#endif
            isValid = false;
            return;
        }

        Stencils::RepeatCuda<STENCIL::VOLUME,
                         TestCellHelpers::CheckNeighbor,
                         STENCIL>()(&isValid, this, neighborhood);

        if (nanoStep >= NANO_STEPS) {
#ifndef __CUDACC__
            OUTPUT() << "TestCell error: nanoStep too large: "
                     << nanoStep << "\n";
#endif
            isValid = false;
            return;
        }

        unsigned expectedNanoStep = cycleCounter % NANO_STEPS;
        if (nanoStep != expectedNanoStep) {
#ifndef __CUDACC__
            OUTPUT() << "TestCell error: nanoStep out of sync. got "
                     << nanoStep << " but expected "
                     << expectedNanoStep << "\n";
#endif
            isValid = false;
            return;
        }

        ++cycleCounter;
    }

    template<typename NEIGHBORHOOD>
    static void updateLineX(
        TestCell *targetLine,
        long *index,
        long indexEnd,
        const NEIGHBORHOOD& hood,
        unsigned nanoStep)
    {
        for (; *index < indexEnd; ++(*index)) {
            targetLine[*index].update(hood, nanoStep);
        }
    }

    template<typename ACCESSOR1, typename ACCESSOR2>
    static void updateLineX(
        ACCESSOR1 hoodOld, int indexEnd,
        ACCESSOR2 hoodNew, unsigned nanoStep)
    {
        for (; hoodOld.index() < indexEnd; ++hoodOld.index(), ++hoodNew.index) {
            TestCell cell;
            cell.update(hoodOld, nanoStep);
            hoodNew << cell;
        }
    }

    std::string toString() const
    {
        std::ostringstream ret;
        ret << "TestCell\n"
            << "  pos: " << pos << "\n"
            << "  dimensions: " << dimensions << "\n"
            << "  cycleCounter: " << cycleCounter << "\n"
            << "  isEdgeCell: " << (isEdgeCell ? "true" : "false") << "\n"
            << "  testValue: " << testValue << "\n"
            << "  isValid: " << (isValid ? "true" : "false") << "\n";
        return ret.str();
    }

    // returns true if valid neighbor is found (at the right place, in
    // the same cycle etc.)
    __host__ __device__
    bool checkNeighbor(
        const TestCell& other,
        const Coord<DIM>& relativeLoc) const
    {
        if (!other.isValid) {
#ifndef __CUDACC__
            OUTPUT() << "Update Error for " << toString() << ":\n"
                     << "Invalid Neighbor at " << relativeLoc << ":\n"
                     << other.toString()
                     << "--------------" << "\n";
#endif
            return false;
        }

        bool otherShouldBeEdge = !inBounds(pos + relativeLoc);
        if (other.isEdgeCell != otherShouldBeEdge) {
#ifndef __CUDACC__
            OUTPUT() << "TestCell error: bad edge cell (expected: "
                     << otherShouldBeEdge << ", is: "
                     << other.isEdgeCell << " at relative coord "
                     << relativeLoc << ")\n";
#endif
            return false;
        }

        if (!otherShouldBeEdge) {
            if (other.cycleCounter != cycleCounter) {
#ifndef __CUDACC__
                OUTPUT() << "Update Error for TestCell "
                         << toString() << ":\n"
                         << "cycle counter out of sync with neighbor "
                         << other.toString() << "\n";
#endif
                return false;
            }

            if (other.dimensions != dimensions) {
#ifndef __CUDACC__
                OUTPUT() << "TestCell error: grid dimensions differ. Expected: "
                         << dimensions << ", but got " << other.dimensions << "\n";
#endif
                return false;
            }

            Coord<DIM> rawPos = pos + relativeLoc;
            Coord<DIM> expectedPos = TOPOLOGY::normalize(rawPos, dimensions.dimensions);

            if (other.pos != expectedPos) {
#ifndef __CUDACC__
                OUTPUT() << "TestCell error: other position "
                         << other.pos
                         << " doesn't match expected "
                         << expectedPos << "\n";
#endif
                return false;
            }
        }

        return true;
    }

    template<int X, int Y, int Z>
    __host__ __device__
    bool checkNeighbor(
        const TestCell& other,
        FixedCoord<X, Y, Z> coord) const
    {
        return checkNeighbor(other, Coord<DIM>(coord));
    }
};

typedef TestCell<
    3,
    Stencils::Moore<3, 1>,
    Topologies::Cube<3>::Topology,
    TestCellHelpers::SoAAPI
    > TestCellSoA;

/**
 * The MPI typemap generator need to find out for which template
 * parameter values it should generate typemaps. It does so by
 * scanning all class members. Therefore this dummy class forces the
 * typemap generator to create MPI datatypes for TestCells with the
 * dimensions as specified below.
 */
class TestCellMPIDatatypeHelper
{
    friend class Typemaps;
    TestCell<1> a;
    TestCell<2> b;
    TestCell<3> c;
};

}

LIBFLATARRAY_REGISTER_SOA(LibGeoDecomp::TestCellSoA, ((LibGeoDecomp::Coord<3>)(pos))((LibGeoDecomp::CoordBox<3>)(dimensions))((unsigned)(cycleCounter))((bool)(isEdgeCell))((bool)(isValid))((double)(testValue)))

template<
    typename CharT,
    typename Traits,
    int Dim,
    typename Stencil,
    typename Topology,
    typename AdditionalAPI,
    typename Output>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const LibGeoDecomp::TestCell<Dim, Stencil, Topology, AdditionalAPI, Output>& cell)
{
    os << cell.toString();
    return os;
}

#endif
