#include <libgeodecomp.h>
#include <libgeodecomp/misc/patternoptimizer.h>
#include <libgeodecomp/misc/simulationfactory.h>
#include <libgeodecomp/parallelization/cacheblockingsimulator.h>
#include <libgeodecomp/parallelization/cudasimulator.h>
#include <libgeodecomp/misc/autotuningsimulator.h>

using namespace LibGeoDecomp;

class Cell
{
public:
    class API :
        public APITraits::HasFixedCoordsOnlyUpdate,
        public APITraits::HasUpdateLineX,
        public APITraits::HasStencil<Stencils::VonNeumann<3, 1> >,
        public APITraits::HasCubeTopology<3>,
        public APITraits::HasPredefinedMPIDataType<double>
    {};

    inline explicit Cell(double v = 0) :
        temp(v)
    {}

    template<typename NEIGHBORHOOD>
    __device__
    __host__
    void update(const NEIGHBORHOOD& hood, const unsigned& nanoStep)
    {
        temp = (hood[FixedCoord< 0,  0, -1>()].temp +
                hood[FixedCoord< 0, -1,  0>()].temp +
                hood[FixedCoord<-1,  0,  0>()].temp +
                hood[FixedCoord< 1,  0,  0>()].temp +
                hood[FixedCoord< 0,  1,  0>()].temp +
                hood[FixedCoord< 0,  0,  1>()].temp) * (1.0 / 6.0);
    }

    // fixme: use locally defined var for x-offset?
    template<typename NEIGHBORHOOD>
    static void updateLineX(Cell *target, long *x, long endX, const NEIGHBORHOOD& hood, int /* nanoStep */)
    {
        for (; *x < endX; ++x) {
            target[*x].temp = (hood[FixedCoord< 0,  0, -1>()].temp +
                               hood[FixedCoord< 0, -1,  0>()].temp +
                               hood[FixedCoord<-1,  0,  0>()].temp +
                               hood[FixedCoord< 1,  0,  0>()].temp +
                               hood[FixedCoord< 0,  1,  0>()].temp +
                               hood[FixedCoord< 0,  0,  1>()].temp) * (1.0 / 6.0);
        }
    }

    double temp;
};

class CellInitializer : public SimpleInitializer<Cell>
{
public:
    using SimpleInitializer<Cell>::gridDimensions;

    CellInitializer(
        int num,
        int maxSteps) :
        SimpleInitializer<Cell>(
            Coord<3>::diagonal(256) * num,
            maxSteps)
    {}

    virtual void grid(GridBase<Cell, 3> *ret)
    {
        CoordBox<3> box = ret->boundingBox();
        Coord<3> offset =
            Coord<3>::diagonal(gridDimensions().x() * 5 / 128);
        int size = gridDimensions().x() * 50 / 128;


        for (int z = 0; z < size; ++z) {
            for (int y = 0; y < size; ++y) {
                for (int x = 0; x < size; ++x) {
                    Coord<3> c = offset + Coord<3>(x, y, z);
                    if (box.inBounds(c)) {
                        ret->set(c, Cell(0.99999999999));
                    }
                }
            }
        }
    }
};
int normalizeSteps(double goal)
{
    std::cout << "in normalizeSteps" << std::endl;
    int steps = 5;
    int oldSteps = 5;
    CellInitializer init(1,1);
    SerialSimulationFactory<Cell> fab(init);
    double limit = fab(fab.parameters());
    double fitness = DBL_MAX;
    do{
        CellInitializer init(1,steps);
        SerialSimulationFactory<Cell> fab(init);
        fitness = fab(fab.parameters());
        oldSteps = steps;
        steps = ((double) steps / fitness)* (double)goal;
        if (steps < 1)
            steps =1;
        std::cout << "fitness: " << fitness << " goal " << goal << std::endl;
    }while((!(fitness > goal + limit && fitness < goal - limit )) && (!(oldSteps <= 1 && fitness > goal)));
    
    
    return oldSteps;
}

void runSimulation()
{
    std::cout << "runSimulation" << std::endl;
    int simSteps = 500;
    int optSteps = normalizeSteps(-0.5);
    std::cout << "optSteps: " << optSteps << std::endl;
    AutoTuningSimulator<Cell,PatternOptimizer> simFactory(CellInitializer(1,optSteps));
    simFactory.setSimulationSteps(20);
    simFactory.runTest();

    std::vector<std::string> simulations = simFactory.getSimulationNames();
    for (std::vector<std::string>::iterator iter = simulations.begin(); iter != simulations.end(); iter++){
        std::cout << "Factory Name: " << *iter << " Fitness: " << simFactory.getFitness(*iter) << std::endl
        << simFactory.getSimulationParameters(*iter)<< std::endl;
    }
    
    std::cout << "-----------------" << std::endl;

    // looking for the best, of the best, of the best! Sir [MIB I] ;)
    // TODO to get the best simulator for a long run, the build up costs need to be deducted.
    optSteps *= 5;
    double bestFitness = DBL_MAX* -1.0;
    std::string bestSimulator;
    for (std::vector<std::string>::iterator iter = simulations.begin(); iter != simulations.end(); iter++){
        if (simFactory.getFitness(*iter) >= bestFitness){
            bestSimulator = simFactory.getSimulatorType(*iter);
            bestFitness = simFactory.getFitness(*iter);
        }
    }
    std::cout << "Best Simulator: " << bestSimulator << " wiht fitness: " << bestFitness << std::endl;
    // let Will Smith running :)




    // HiParSimulator::HiParSimulator<Cell, RecursiveBisectionPartition<3> > sim(
    //     init,
    //     MPILayer().rank() ? 0 : new TracingBalancer(new NoOpBalancer()),
    //     1000,
    //     1);


    // SerialSimulator<Cell> sim(
    //     init);

    // CudaSimulator<Cell> sim(init, Coord<3>(128, 4, 1));

    // CacheBlockingSimulator<Cell> sim(
    //     init,
    //     2,
    //     Coord<2>(10, 10));

    // if (MPILayer().rank() == 0) {
    //     sim.addWriter(
    //         new TracingWriter<Cell>(outputFrequency, init->maxSteps()));
    // }

    // sim.run();
}


int main(int argc, char **argv)
{
    //MPI_Init(&argc, &argv);
    runSimulation();
    //MPI_Finalize();
}
