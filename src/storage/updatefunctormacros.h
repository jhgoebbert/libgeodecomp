#ifndef LIBGEODECOMP_STORAGE_UPDATEFUNCTORMACROS_H
#define LIBGEODECOMP_STORAGE_UPDATEFUNCTORMACROS_H

#ifdef LIBGEODECOMP_WITH_THREADS
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_1                         \
    if (concurrencySpec.enableOpenMP() &&                               \
        !modelThreadingSpec.hasOpenMP()) {                              \
        if (concurrencySpec.preferStaticScheduling()) {                 \
            _Pragma("omp parallel for schedule(static)")                \
            for (std::size_t c = 0; c < region.numPlanes(); ++c) {      \
                typename Region<DIM>::StreakIterator e =                \
                    region.planeStreakIterator(c + 1);                  \
                typedef typename Region<DIM>::StreakIterator Iter;      \
                for (Iter i = region.planeStreakIterator(c + 0);        \
                     i != e;                                            \
                     ++i) {                                             \
                    LGD_UPDATE_FUNCTOR_BODY;                            \
                }                                                       \
            }                                                           \
    /**/
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_2                         \
        } else {                                                        \
            _Pragma("omp parallel for schedule(dynamic)")               \
            for (std::size_t c = 0; c < region.numPlanes(); ++c) {      \
                typename Region<DIM>::StreakIterator e =                \
                    region.planeStreakIterator(c + 1);                  \
                typedef typename Region<DIM>::StreakIterator Iter;      \
                for (Iter i = region.planeStreakIterator(c + 0);        \
                     i != e;                                            \
                     ++i) {                                             \
                    LGD_UPDATE_FUNCTOR_BODY;                            \
                }                                                       \
            }                                                           \
        }                                                               \
        return;                                                         \
    }                                                                   \
    /**/
#else
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_1
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_2
#endif

#ifdef LIBGEODECOMP_WITH_HPX
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_3                         \
    if (concurrencySpec.enableHPX() && !modelThreadingSpec.hasHPX()) {  \
        hpx::parallel::for_each(                                        \
            hpx::parallel::par,                                         \
            boost::make_counting_iterator(std::size_t(0)),              \
            boost::make_counting_iterator(region.numPlanes()),          \
            [&](std::size_t c) {                                        \
                typename Region<DIM>::StreakIterator e =                \
                    region.planeStreakIterator(c + 1);                  \
                typedef typename Region<DIM>::StreakIterator Iter;      \
                for (Iter i = region.planeStreakIterator(c + 0);        \
                     i != e;                                            \
                     ++i) {                                             \
                    LGD_UPDATE_FUNCTOR_BODY;                            \
                }                                                       \
            });                                                         \
                                                                        \
        return;                                                         \
    }                                                                   \
    /**/
#else
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_3
    /**/
#endif

#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_4                         \
    for (typename Region<DIM>::StreakIterator i = region.beginStreak(); \
         i != region.endStreak();                                       \
         ++i) {                                                         \
        LGD_UPDATE_FUNCTOR_BODY;                                        \
    }                                                                   \
    /**/


#endif
