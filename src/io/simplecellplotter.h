#ifndef LIBGEODECOMP_IO_SIMPLECELLPLOTTER_H
#define LIBGEODECOMP_IO_SIMPLECELLPLOTTER_H

#include <libgeodecomp/io/initializer.h>
#include <libgeodecomp/misc/palette.h>
#include <libgeodecomp/misc/quickpalette.h>
#include <libgeodecomp/storage/filter.h>
#include <libgeodecomp/storage/selector.h>

#include <boost/shared_ptr.hpp>
#include <stdexcept>

namespace LibGeoDecomp {

class PPMWriterTest;
class HPXSerialization;

namespace SimpleCellPlotterHelpers {

template<typename CELL, typename MEMBER, typename PALETTE>
class CellToColor : public Filter<CELL, MEMBER, Color>
{
public:
#ifdef LIBGEODECOMP_WITH_HPX
    HPX_SERIALIZATION_POLYMORPHIC_TEMPLATE_SEMIINTRUSIVE(CellToColor);
#endif

    friend class PolymorphicSerialization;
    friend class BoostSerialization;
    friend class HPXSerialization;
    friend class LibGeoDecomp::HPXSerialization;
    friend class LibGeoDecomp::PPMWriterTest;

    explicit
    CellToColor(const PALETTE& palette = PALETTE()) :
        palette(palette)
    {}

    void copyStreakInImpl(
        const Color *source,
        MemoryLocation::Location sourceLocation,
        MEMBER *target,
        MemoryLocation::Location targetLocation,
        const std::size_t num,
        const std::size_t stride)
    {
        throw std::logic_error(
            "undefined behavior: can only convert members to colors, not the other way around");
    }

    void copyStreakOutImpl(
        const MEMBER *source,
        MemoryLocation::Location sourceLocation,
        Color *target,
        MemoryLocation::Location targetLocation,
        const std::size_t num,
        const std::size_t stride)
    {
        for (std::size_t i = 0; i < num; ++i) {
            target[i] = palette[source[i]];
        }
    }

    void copyMemberInImpl(
        const Color *source,
        MemoryLocation::Location sourceLocation,
        CELL *target,
        MemoryLocation::Location targetLocation,
        std::size_t num,
        MEMBER CELL:: *memberPointer)
    {

        throw std::logic_error("undefined behavior: can only convert cells to colors, not the other way around");
    }

    void copyMemberOutImpl(
        const CELL *source,
        MemoryLocation::Location sourceLocation,
        Color *target,
        MemoryLocation::Location targetLocation,
        std::size_t num,
        MEMBER CELL:: *memberPointer)
    {
        for (std::size_t i = 0; i < num; ++i) {
            target[i] = palette[source[i].*memberPointer];
        }
    }

private:
    PALETTE palette;
};

}

/**
 * This is a convenience class which uses a Palette to map a single
 * value of a cell to a color range.
 */
template<typename CELL_TYPE>
class SimpleCellPlotter
{
public:
    friend class BoostSerialization;
    friend class HPXSerialization;
    friend class PPMWriterTest;

    template<typename MEMBER, typename PALETTE>
    explicit SimpleCellPlotter(MEMBER CELL_TYPE:: *memberPointer, const PALETTE& palette) :
        cellToColorSelector(
            memberPointer,
            "unnamed parameter",
            boost::shared_ptr<FilterBase<CELL_TYPE> >(
                new SimpleCellPlotterHelpers::CellToColor<CELL_TYPE, MEMBER, PALETTE>(palette)))
    {}

    virtual ~SimpleCellPlotter()
    {}

    template<typename PAINTER>
    void operator()(
        const CELL_TYPE& cell,
        PAINTER& painter,
        const Coord<2>& cellDimensions) const
    {
        Color color;
        cellToColorSelector.copyMemberOut(
            &cell,
            MemoryLocation::HOST,
            reinterpret_cast<char*>(&color),
            MemoryLocation::HOST,
            1);

        painter.fillRect(
            0, 0,
            cellDimensions.x(), cellDimensions.y(),
            color);
    }

private:
    Selector<CELL_TYPE> cellToColorSelector;
};

}

#endif
