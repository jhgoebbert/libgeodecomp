#ifndef LIBGEODECOMP_STORAGE_GRIDBASE_H
#define LIBGEODECOMP_STORAGE_GRIDBASE_H

#include <libgeodecomp/geometry/coord.h>
#include <libgeodecomp/geometry/coordbox.h>
#include <libgeodecomp/geometry/region.h>
#include <libgeodecomp/geometry/streak.h>
#include <libgeodecomp/storage/memorylocation.h>
#include <libgeodecomp/storage/selector.h>

namespace LibGeoDecomp {

/**
 * Forward declaration
 */
template<typename CELL, int DIM>
class ProxyGrid;

/**
 * This is an abstract base class for all grid classes. It's generic
 * because all methods are virtual, but not very efficient -- for the
 * same reason.
 */
template<typename CELL, int DIMENSIONS>
class GridBase
{
public:
    friend class ProxyGrid<CELL, DIMENSIONS>;

    typedef CELL CellType;
    const static int DIM = DIMENSIONS;

    virtual ~GridBase()
    {}

    virtual void set(const Coord<DIM>&, const CELL&) = 0;
    virtual void set(const Streak<DIM>&, const CELL*) = 0;
    virtual CELL get(const Coord<DIM>&) const = 0;
    virtual void get(const Streak<DIM>&, CELL *) const = 0;
    virtual void setEdge(const CELL&) = 0;
    virtual const CELL& getEdge() const = 0;
    virtual CoordBox<DIM> boundingBox() const = 0;

    Coord<DIM> dimensions() const
    {
        return boundingBox().dimensions;
    }


    bool operator==(const GridBase<CELL, DIMENSIONS>& other) const
    {
        if (getEdge() != other.getEdge()) {
            return false;
        }

        CoordBox<DIM> box = boundingBox();
        if (box != other.boundingBox()) {
            return false;
        }

        for (typename CoordBox<DIM>::Iterator i = box.begin(); i != box.end(); ++i) {
            if (get(*i) != other.get(*i)) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const GridBase<CELL, DIMENSIONS>& other) const
    {
        return !(*this == other);
    }

    /**
     * Allows the user to extract a single member variable of all
     * cells within the given region. Assumes that target points to an area with sufficient space.
     */
    template<typename MEMBER>
    void saveMember(
        MEMBER *target,
        MemoryLocation::Location targetLocation,
        const Selector<CELL>& selector,
        const Region<DIM>& region) const
    {
        if (!selector.template checkTypeID<MEMBER>()) {
            throw std::invalid_argument("cannot save member as selector was created for different type");
        }

        saveMemberImplementation(reinterpret_cast<char*>(target), targetLocation, selector, region);
    }

    /**
     * Same as saveMember(), but sans the type checking. Useful in
     * Writers and other components that might not know about the
     * variable's type.
     */
    void saveMemberUnchecked(
        char *target,
        MemoryLocation::Location targetLocation,
        const Selector<CELL>& selector,
        const Region<DIM>& region) const
    {
        saveMemberImplementation(target, targetLocation, selector, region);
    }

    /**
     * Used for bulk-setting of single member variables. Assumes that
     * source contains as many instances of the member as region
     * contains coordinates.
     */
    template<typename MEMBER>
    void loadMember(
        const MEMBER *source,
        MemoryLocation::Location sourceLocation,
        const Selector<CELL>& selector,
        const Region<DIM>& region)
    {
        if (!selector.template checkTypeID<MEMBER>()) {
            throw std::invalid_argument("cannot load member as selector was created for different type");
        }

        loadMemberImplementation(reinterpret_cast<const char*>(source), sourceLocation, selector, region);
    }

protected:
    virtual void saveMemberImplementation(
        char *target,
        MemoryLocation::Location targetLocation,
        const Selector<CELL>& selector,
        const Region<DIM>& region) const = 0;

    virtual void loadMemberImplementation(
        const char *source,
        MemoryLocation::Location sourceLocation,
        const Selector<CELL>& selector,
        const Region<DIM>& region) = 0;

};

}

#endif
