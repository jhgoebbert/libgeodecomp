#ifndef LIBGEODECOMP_STORAGE_BOXCELL_H
#define LIBGEODECOMP_STORAGE_BOXCELL_H

#include <libgeodecomp/misc/apitraits.h>
#include <libgeodecomp/geometry/coord.h>
#include <libgeodecomp/geometry/coordbox.h>
#include <libgeodecomp/misc/stdcontaineroverloads.h>
#include <libgeodecomp/storage/neighborhooditerator.h>
#include <libgeodecomp/storage/fixedarray.h>

namespace LibGeoDecomp {

/**
 * This class is an adapter for implementing n-body codes and
 * molecular dynamics (MD) applications with LibGeoDecomp. A BoxCell
 * represents a fixed volume of the simulation space. It stores those
 * particles (of type Cargo) which reside in its area in the given
 * CONTAINER type (e.g. LibGeoDecomp::FixedArray or std::vector). Particles can
 * access neighboring particles in a given distance during update().
 */
template<typename CONTAINER>
class BoxCell
{
public:
    friend class BoxCellTest;

    typedef CONTAINER Container;
    typedef typename Container::value_type Cargo;
    typedef typename Container::value_type value_type;
    typedef typename Container::const_iterator const_iterator;
    typedef typename Container::iterator iterator;
    typedef typename APITraits::SelectTopology<Cargo>::Value Topology;

    class API :
        public APITraits::SelectAPI<Cargo>::Value,
        public APITraits::HasStencil<Stencils::Moore<Topology::DIM, 1> >
    {};

    const static int DIM = Topology::DIM;

    template<typename NEIGHBORHOOD, typename COLLECTION_INTERFACE>
    class NeighborhoodAdapter
    {
    public:
        typedef typename NeighborhoodIterator<NEIGHBORHOOD, DIM, COLLECTION_INTERFACE>::Adapter Value;
    };

    inline explicit BoxCell(
        const FloatCoord<DIM>& origin = Coord<DIM>(),
        const FloatCoord<DIM>& dimension = Coord<DIM>()) :
        origin(origin),
        dimension(dimension)
    {}

    inline const_iterator begin() const
    {
        return particles.begin();
    }

    inline iterator begin()
    {
        return particles.begin();
    }

    inline const_iterator end() const
    {
        return particles.end();
    }

    inline iterator end()
    {
        return particles.end();
    }

    inline void insert(const Cargo& particle)
    {
        particles << particle;
    }

    inline std::size_t size() const
    {
        return particles.size();
    }

    inline
    const Cargo& operator[](const std::size_t i) const
    {
        return particles[i];
    }

    inline
    Cargo& operator[](const std::size_t i)
    {
        return particles[i];
    }

    inline
    BoxCell& operator<<(const Cargo& cargo)
    {
        particles << cargo;
        return *this;
    }

    template<class HOOD>
    inline void update(const HOOD& hood, const int nanoStep)
    {
        *this = hood[Coord<DIM>()];

        typedef CollectionInterface::PassThrough<typename HOOD::Cell> PassThroughType;
        typedef typename NeighborhoodAdapter<HOOD, PassThroughType>::Value NeighborhoodAdapterType;
        NeighborhoodAdapterType adapter(&hood);

        updateCargo(adapter, adapter, nanoStep);
    }

    template<class NEIGHBORHOOD_ADAPTER_SELF, class NEIGHBORHOOD_ADAPTER_ALL>
    inline void updateCargo(
        const NEIGHBORHOOD_ADAPTER_SELF& ownNeighbors,
        const NEIGHBORHOOD_ADAPTER_ALL& allNeighbors,
        const int nanoStep)
    {
        if (nanoStep == 0) {
            particles.clear();
            addContainedParticles(ownNeighbors.begin(), ownNeighbors.end());
        }

        for (typename Container::iterator i = particles.begin(); i != particles.end(); ++i) {
            i->update(allNeighbors, nanoStep);
        }
    }

private:
    FloatCoord<DIM> origin;
    FloatCoord<DIM> dimension;
    Container particles;

    template<typename ITERATOR>
    void addContainedParticles(const ITERATOR& begin, const ITERATOR& end)
    {
        FloatCoord<DIM> oppositeCorner = origin + dimension;

        for (ITERATOR i = begin; i != end; ++i) {
            if (APITraits::SelectPositionChecker<Cargo>::value(*i, origin, oppositeCorner)) {
                particles << *i;
            }
        }
    }

};

}

#endif
