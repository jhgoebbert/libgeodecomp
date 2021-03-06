#include<libgeodecomp/config.h>
#ifdef LIBGEODECOMP_WITH_BOOST_SERIALIZATION
#ifndef LIBGEODECOMP_BOOSTSERIALIZATION_H
#define LIBGEODECOMP_BOOSTSERIALIZATION_H

#include <libgeodecomp/storage/arrayfilter.h>
#include <libgeodecomp/misc/chronometer.h>
#include <libgeodecomp/misc/color.h>
#include <libgeodecomp/geometry/coord.h>
#include <libgeodecomp/geometry/coord.h>
#include <libgeodecomp/geometry/coord.h>
#include <libgeodecomp/geometry/coordbox.h>
#include <libgeodecomp/storage/defaultarrayfilter.h>
#include <libgeodecomp/storage/defaultfilter.h>
#include <libgeodecomp/storage/filter.h>
#include <libgeodecomp/storage/filterbase.h>
#include <libgeodecomp/storage/fixedarray.h>
#include <libgeodecomp/geometry/floatcoord.h>
#include <libgeodecomp/geometry/floatcoord.h>
#include <libgeodecomp/geometry/floatcoord.h>
#include <libgeodecomp/io/initializer.h>
#include <libgeodecomp/loadbalancer/loadbalancer.h>
#include <libgeodecomp/misc/nonpodtestcell.h>
#include <libgeodecomp/loadbalancer/oozebalancer.h>
#include <libgeodecomp/misc/palette.h>
#include <libgeodecomp/io/parallelwriter.h>
#include <libgeodecomp/misc/quickpalette.h>
#include <libgeodecomp/geometry/region.h>
#include <libgeodecomp/io/serialbovwriter.h>
#include <libgeodecomp/storage/simplearrayfilter.h>
#include <libgeodecomp/io/simplecellplotter.h>
#include <libgeodecomp/io/simplecellplotter.h>
#include <libgeodecomp/storage/simplefilter.h>
#include <libgeodecomp/io/simpleinitializer.h>
#include <libgeodecomp/io/steerer.h>
#include <libgeodecomp/geometry/streak.h>
#include <libgeodecomp/io/writer.h>

namespace LibGeoDecomp {
class BoostSerialization
{
public:
    template<typename ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL, int ARITY>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::ArrayFilter<CELL, MEMBER, EXTERNAL, ARITY>& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::FilterBase<CELL > >(object);
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Chronometer& object, const unsigned /*version*/)
    {
        archive & object.totalTimes;
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Color& object, const unsigned /*version*/)
    {
        archive & object.rgb;
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Coord<1 >& object, const unsigned /*version*/)
    {
        archive & object.c;
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Coord<2 >& object, const unsigned /*version*/)
    {
        archive & object.c;
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Coord<3 >& object, const unsigned /*version*/)
    {
        archive & object.c;
    }

    template<typename ARCHIVE, int DIM>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::CoordBox<DIM>& object, const unsigned /*version*/)
    {
        archive & object.dimensions;
        archive & object.origin;
    }

    template<typename ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL, int ARITY>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::DefaultArrayFilter<CELL, MEMBER, EXTERNAL, ARITY>& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::ArrayFilter<CELL, MEMBER, EXTERNAL, ARITY > >(object);
    }

    template<typename ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::DefaultFilter<CELL, MEMBER, EXTERNAL>& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::Filter<CELL, MEMBER, EXTERNAL > >(object);
    }

    template<typename ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Filter<CELL, MEMBER, EXTERNAL>& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::FilterBase<CELL > >(object);
    }

    template<typename ARCHIVE, typename CELL>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::FilterBase<CELL>& object, const unsigned /*version*/)
    {
    }

    template<typename ARCHIVE, typename T, int SIZE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::FixedArray<T, SIZE>& object, const unsigned /*version*/)
    {
        archive & object.elements;
        archive & object.store;
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::FloatCoord<1 >& object, const unsigned /*version*/)
    {
        archive & object.c;
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::FloatCoord<2 >& object, const unsigned /*version*/)
    {
        archive & object.c;
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::FloatCoord<3 >& object, const unsigned /*version*/)
    {
        archive & object.c;
    }

    template<typename ARCHIVE, typename CELL>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Initializer<CELL>& object, const unsigned /*version*/)
    {
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::LoadBalancer& object, const unsigned /*version*/)
    {
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::NonPoDTestCell& object, const unsigned /*version*/)
    {
        archive & object.coord;
        archive & object.cycleCounter;
        archive & object.missingNeighbors;
        archive & object.seenNeighbors;
        archive & object.simSpace;
    }

    template<typename ARCHIVE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::OozeBalancer& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::LoadBalancer >(object);
        archive & object.newLoadWeight;
    }

    template<typename ARCHIVE, typename VALUE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Palette<VALUE>& object, const unsigned /*version*/)
    {
        archive & object.colors;
    }

    template<typename ARCHIVE, typename CELL_TYPE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::ParallelWriter<CELL_TYPE>& object, const unsigned /*version*/)
    {
        archive & object.period;
        archive & object.prefix;
        archive & object.region;
    }

    template<typename ARCHIVE, typename VALUE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::QuickPalette<VALUE>& object, const unsigned /*version*/)
    {
        archive & object.mark0;
        archive & object.mark1;
        archive & object.mark2;
        archive & object.mark3;
        archive & object.mark4;
        archive & object.mult;
    }

    template<typename ARCHIVE, int DIM>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Region<DIM>& object, const unsigned /*version*/)
    {
        archive & object.geometryCacheTainted;
        archive & object.indices;
        archive & object.myBoundingBox;
        archive & object.mySize;
    }

    template<typename ARCHIVE, typename CELL_TYPE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::SerialBOVWriter<CELL_TYPE>& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::Clonable<Writer<CELL_TYPE >, SerialBOVWriter<CELL_TYPE > > >(object);
        archive & object.brickletDim;
        archive & object.selector;
    }

    template<typename ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL, int ARITY>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleArrayFilter<CELL, MEMBER, EXTERNAL, ARITY>& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::ArrayFilter<CELL, MEMBER, EXTERNAL, ARITY > >(object);
    }

    template<typename ARCHIVE, typename CELL_TYPE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleCellPlotter<CELL_TYPE>& object, const unsigned /*version*/)
    {
        archive & object.cellToColorSelector;
    }

    template<typename ARCHIVE, typename CELL, typename MEMBER, typename PALETTE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleCellPlotterHelpers::CellToColor<CELL, MEMBER, PALETTE>& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::Filter<CELL, MEMBER, Color > >(object);
        archive & object.palette;
    }

    template<typename ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleFilter<CELL, MEMBER, EXTERNAL>& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::Filter<CELL, MEMBER, EXTERNAL > >(object);
    }

    template<typename ARCHIVE, typename CELL_TYPE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleInitializer<CELL_TYPE>& object, const unsigned /*version*/)
    {
        archive & boost::serialization::base_object<LibGeoDecomp::Initializer<CELL_TYPE > >(object);
        archive & object.dimensions;
        archive & object.steps;
    }

    template<typename ARCHIVE, typename CELL_TYPE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Steerer<CELL_TYPE>& object, const unsigned /*version*/)
    {
        archive & object.period;
        archive & object.region;
    }

    template<typename ARCHIVE, int DIM>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Streak<DIM>& object, const unsigned /*version*/)
    {
        archive & object.endX;
        archive & object.origin;
    }

    template<typename ARCHIVE, typename CELL_TYPE>
    inline
    static void serialize(ARCHIVE& archive, LibGeoDecomp::Writer<CELL_TYPE>& object, const unsigned /*version*/)
    {
        archive & object.period;
        archive & object.prefix;
    }

};
}

namespace boost {
namespace serialization {

using namespace LibGeoDecomp;

template<class ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL, int ARITY>
void serialize(ARCHIVE& archive, LibGeoDecomp::ArrayFilter<CELL, MEMBER, EXTERNAL, ARITY>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::Chronometer& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::Color& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::Coord<1 >& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::Coord<2 >& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::Coord<3 >& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, int DIM>
void serialize(ARCHIVE& archive, LibGeoDecomp::CoordBox<DIM>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL, int ARITY>
void serialize(ARCHIVE& archive, LibGeoDecomp::DefaultArrayFilter<CELL, MEMBER, EXTERNAL, ARITY>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL>
void serialize(ARCHIVE& archive, LibGeoDecomp::DefaultFilter<CELL, MEMBER, EXTERNAL>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL>
void serialize(ARCHIVE& archive, LibGeoDecomp::Filter<CELL, MEMBER, EXTERNAL>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL>
void serialize(ARCHIVE& archive, LibGeoDecomp::FilterBase<CELL>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename T, int SIZE>
void serialize(ARCHIVE& archive, LibGeoDecomp::FixedArray<T, SIZE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::FloatCoord<1 >& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::FloatCoord<2 >& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::FloatCoord<3 >& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL>
void serialize(ARCHIVE& archive, LibGeoDecomp::Initializer<CELL>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::LoadBalancer& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::NonPoDTestCell& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::OozeBalancer& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename VALUE>
void serialize(ARCHIVE& archive, LibGeoDecomp::Palette<VALUE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL_TYPE>
void serialize(ARCHIVE& archive, LibGeoDecomp::ParallelWriter<CELL_TYPE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename VALUE>
void serialize(ARCHIVE& archive, LibGeoDecomp::QuickPalette<VALUE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, int DIM>
void serialize(ARCHIVE& archive, LibGeoDecomp::Region<DIM>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL_TYPE>
void serialize(ARCHIVE& archive, LibGeoDecomp::SerialBOVWriter<CELL_TYPE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL, int ARITY>
void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleArrayFilter<CELL, MEMBER, EXTERNAL, ARITY>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL_TYPE>
void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleCellPlotter<CELL_TYPE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL, typename MEMBER, typename PALETTE>
void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleCellPlotterHelpers::CellToColor<CELL, MEMBER, PALETTE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL, typename MEMBER, typename EXTERNAL>
void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleFilter<CELL, MEMBER, EXTERNAL>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL_TYPE>
void serialize(ARCHIVE& archive, LibGeoDecomp::SimpleInitializer<CELL_TYPE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL_TYPE>
void serialize(ARCHIVE& archive, LibGeoDecomp::Steerer<CELL_TYPE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, int DIM>
void serialize(ARCHIVE& archive, LibGeoDecomp::Streak<DIM>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}

template<class ARCHIVE, typename CELL_TYPE>
void serialize(ARCHIVE& archive, LibGeoDecomp::Writer<CELL_TYPE>& object, const unsigned version)
{
    BoostSerialization::serialize(archive, object, version);
}


}
}

#endif

#endif
