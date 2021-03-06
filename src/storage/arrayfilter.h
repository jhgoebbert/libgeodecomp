#ifndef LIBGEODECOMP_STORAGE_ARRAYFILTER_H
#define LIBGEODECOMP_STORAGE_ARRAYFILTER_H

#include <libgeodecomp/storage/filter.h>

namespace LibGeoDecomp {

/**
 * This filter can be used to extract/manipulate member arrays (e.g.
 * double foo[3]) in user-supplied models (cell classes).
 */
template<typename CELL, typename MEMBER, typename EXTERNAL, int ARITY = 1>
class ArrayFilter : public FilterBase<CELL>
{
public:
    friend class PolymorphicSerialization;
    friend class BoostSerialization;
    friend class HPXSerialization;

    std::size_t sizeOf() const
    {
        return ARITY * sizeof(EXTERNAL);
    }

#ifdef LIBGEODECOMP_WITH_SILO
    int siloTypeID() const
    {
        return FilterHelpers::GetSiloTypeID<EXTERNAL>()();
    }
#endif

#ifdef LIBGEODECOMP_WITH_MPI
    virtual MPI_Datatype mpiDatatype() const
    {
        return FilterHelpers::GetMPIDatatype<EXTERNAL>()();
    }
#endif

    virtual std::string typeName() const
    {
        return FilterHelpers::GetTypeName<EXTERNAL>()();
    }

    virtual int arity() const
    {
        return ARITY;
    }

    /**
     * Copy a streak of variables to an AoS layout.
     */
    virtual void copyStreakInImpl(
        const EXTERNAL *source,
        MemoryLocation::Location sourceLocation,
        MEMBER *target,
        MemoryLocation::Location targetLocation,
        const std::size_t num,
        const std::size_t stride) = 0;

    /**
     * Extract a steak of members from an AoS layout.
     */
    virtual void copyStreakOutImpl(
        const MEMBER *source,
        MemoryLocation::Location sourceLocation,
        EXTERNAL *target,
        MemoryLocation::Location targetLocation,
        const std::size_t num,
        const std::size_t stride) = 0;

    /**
     * Copy a streak of variables to the members of a streak of cells.
     */
    virtual void copyMemberInImpl(
        const EXTERNAL *source,
        MemoryLocation::Location sourceLocation,
        CELL *target,
        MemoryLocation::Location targetLocation,
        std::size_t num,
        MEMBER (CELL:: *memberPointer)[ARITY]) = 0;

    /**
     * Extract a streak of members from a streak of cells.
     */
    virtual void copyMemberOutImpl(
        const CELL *source,
        MemoryLocation::Location sourceLocation,
        EXTERNAL *target,
        MemoryLocation::Location targetLocation,
        std::size_t num,
        MEMBER (CELL:: *memberPointer)[ARITY]) = 0;

    /**
     * Do not override this function! It is final.
     */
    void copyStreakIn(
        const char *source,
        MemoryLocation::Location sourceLocation,
        char *target,
        MemoryLocation::Location targetLocation,
        const std::size_t num,
        const std::size_t stride)
    {
        copyStreakInImpl(
            reinterpret_cast<const EXTERNAL*>(source),
            sourceLocation,
            reinterpret_cast<MEMBER*>(target),
            targetLocation,
            num,
            stride);
    }

    /**
     * Do not override this function! It is final.
     */
    void copyStreakOut(
        const char *source,
        MemoryLocation::Location sourceLocation,
        char *target,
        MemoryLocation::Location targetLocation,
        const std::size_t num,
        const std::size_t stride)
    {
        copyStreakOutImpl(
            reinterpret_cast<const MEMBER*>(source),
            sourceLocation,
            reinterpret_cast<EXTERNAL*>(target),
            targetLocation,
            num,
            stride);
    }

    /**
     * Do not override this function! It is final.
     */
    void copyMemberIn(
        const char *source,
        MemoryLocation::Location sourceLocation,
        CELL *target,
        MemoryLocation::Location targetLocation,
        std::size_t num,
        char CELL:: *memberPointer)
    {
        copyMemberInImpl(
            reinterpret_cast<const EXTERNAL*>(source),
            sourceLocation,
            target,
            targetLocation,
            num,
            reinterpret_cast<MEMBER (CELL:: *)[ARITY]>(memberPointer));
    }

    /**
     * Do not override this function! It is final.
     */
    void copyMemberOut(
        const CELL *source,
        MemoryLocation::Location sourceLocation,
        char *target,
        MemoryLocation::Location targetLocation,
        std::size_t num,
        char CELL:: *memberPointer)
    {
        copyMemberOutImpl(
            source,
            sourceLocation,
            reinterpret_cast<EXTERNAL*>(target),
            targetLocation,
            num,
            reinterpret_cast<MEMBER (CELL:: *)[ARITY]>(memberPointer));
    }

    bool checkExternalTypeID(const std::type_info& otherID) const
    {
        return typeid(EXTERNAL) == otherID;
    }

};

}

#endif
