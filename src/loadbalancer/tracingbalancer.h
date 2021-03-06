#ifndef LIBGEODECOMP_LOADBALANCER_TRACINGBALANCER_H
#define LIBGEODECOMP_LOADBALANCER_TRACINGBALANCER_H

#include <libgeodecomp/config.h>
#include <libgeodecomp/loadbalancer/loadbalancer.h>
#include <boost/shared_ptr.hpp>
#include <iostream>

#ifdef LIBGEODECOMP_WITH_HPX

#include <libgeodecomp/communication/hpxserializationwrapper.h>

namespace LibGeoDecomp {
class TracingBalancer;
}

namespace hpx {
namespace serialization {
template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::TracingBalancer& object, const unsigned version);
}
}
#endif

namespace LibGeoDecomp {

/**
 * This class relies on another LoadBalancer to do the job, but is
 * able to pass debug output.
 */
class TracingBalancer : public LoadBalancer
{
public:
#ifdef LIBGEODECOMP_WITH_HPX
    template<typename ARCHIVE>
    friend void hpx::serialization::serialize(
        ARCHIVE& archive, TracingBalancer& object, const unsigned version);
#endif

    explicit TracingBalancer(
        LoadBalancer *balancer = 0,
        std::ostream& stream = std::cout) :
        balancer(balancer),
        stream(stream)
    {
    }

    virtual WeightVec balance(const WeightVec& weights, const LoadVec& relativeLoads)
    {
        stream << "TracingBalancer::balance()\n";
        stream << "  weights: " << weights << "\n";
        stream << "  relativeLoads: " << relativeLoads << "\n";

        return balancer->balance(weights, relativeLoads);
    }

private:
    boost::shared_ptr<LoadBalancer> balancer;
    std::ostream& stream;
};

}

#ifdef LIBGEODECOMP_WITH_HPX

HPX_TRAITS_NONINTRUSIVE_POLYMORPHIC(LibGeoDecomp::TracingBalancer);

namespace hpx {
namespace serialization {

template<class ARCHIVE>
void serialize(ARCHIVE& archive, LibGeoDecomp::TracingBalancer& object, const unsigned version)
{
    archive & hpx::serialization::base_object<LibGeoDecomp::LoadBalancer >(object);
    archive & object.balancer;
}

}
}

#endif

#endif
