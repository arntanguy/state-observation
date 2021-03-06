/**
 * \file      probability-law-simulation.hpp
 * \author    Mehdi Benallegue
 * \date       2013
 * \brief
 *
 *
 *
 */

#ifndef SENSORSSIMULATIONPROBABILITYLAWSIMULATIONHPP
#define SENSORSSIMULATIONPROBABILITYLAWSIMULATIONHPP

#include <boost/random.hpp>

#include <state-observation/api.h>
#include <state-observation/tools/definitions.hpp>

namespace stateObservation
{
namespace tools
{
class STATE_OBSERVATION_DLLAPI ProbabilityLawSimulation
{
public:
  /// gets a scalar Gaussian random variable
  /// having a given bias and standard deviation(std)
  /// default is the cetered unit Gaussian
  double getGaussianScalar(double std = 1, double bias = 0);

  /// gets vector Gaussian random variable
  /// having a given bias and standard deviation(std)
  static Matrix getGaussianVector(const Matrix & std, const Matrix & bias, Index rows, Index cols = 1);

protected:
  static boost::lagged_fibonacci1279 gen_;
};

} // namespace tools
} // namespace stateObservation

#endif // SENSORSSIMULATIONPROBABILITYLAWSIMULATIONHPP
