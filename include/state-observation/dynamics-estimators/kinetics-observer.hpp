/**
 * \file      kinetics-observer.hpp
 * \author    Mehdi Benallegue
 * \date      2018
 * \brief     Unified Kinetics estimator
 *
 * \details
 *
 *
 */

#ifndef KINETICSOBSERVER_HPP
#define KINETICSOBSERVER_HPP

#include <map>
#include <set>

#include <boost/utility.hpp>

#include <state-observation/api.h>
#include <state-observation/dynamical-system/dynamical-system-functor-base.hpp>
#include <state-observation/noise/noise-base.hpp>
#include <state-observation/observer/extended-kalman-filter.hpp>
#include <state-observation/sensors-simulation/accelerometer-gyrometer.hpp>
#include <state-observation/tools/definitions.hpp>
#include <state-observation/tools/rigid-body-kinematics.hpp>
#include <state-observation/tools/state-vector-arithmetics.hpp>

namespace stateObservation
{

/**
 * \class  KineticsObserver
 * \brief
 *
 */

class STATE_OBSERVATION_DLLAPI KineticsObserver : protected DynamicalSystemFunctorBase, protected StateVectorArithmetics
{
public:
  typedef kine::Kinematics Kinematics;
  typedef kine::Orientation Orientation;

  /// The constructor.
  ///  \li maxContacts : maximum number of contacts,
  ///  \li maxNumberOfIMU : the maximumnumber of IMUs
  KineticsObserver(unsigned maxContacts = 4, unsigned maxNumberOfIMU = 1);

  /// virtual destructor
  virtual ~KineticsObserver();

  /// Gets the state size
  Index getStateSize() const;

  /// Gets the measurement size
  Index getMeasurementSize() const;

  /// ///////////////////////////////////////////////////////////
  /// Getting, setting the current time and running the estimation
  /// //////////////////////////////////////////////////////////

  /// gets the sampling time
  double getSamplingTime() const;

  /// sets the sampling time
  void setSamplingTime(double);

  /// this function triggers the estimation itself
  Vector update();

  /// ////////////////////////////////
  /// Getting and setting the state
  /// ////////////////////////////////

  /// Gets an estimation of the state in the form of a state vector $\hat{x_{k+1}}$
  Vector getStateVector() const;

  /// Gets the internal sample time of the state vector
  stateObservation::TimeIndex getStateVectorSampleTime() const;

  /// Get the Kinematics of the observed frams
  Kinematics getKinematics() const;

  /// Get the kinematics of a given frame
  Kinematics getKinematicsOf(const Kinematics & localKinematics) const;
  Kinematics getKinematicsOf(const Kinematics & localKinematics);
  Kinematics getKinematicsOf(Kinematics & localKinematics) const;
  Kinematics getKinematicsOf(Kinematics & localKinematics);

  /// get the contact force provided by the estimator
  /// which is different from a contact sensor measurement
  Vector6 getContactWrench(int contactNbr) const;
  Kinematics getContactPosition(int contactNbr) const;

  /// gets the external unmodeled forces
  Vector6 getUnmodeledWrench() const;

  /// This function allows to estimate the acceleration
  /// it returns a Kinemactis
  Kinematics estimateAccelerations();

  /// Sets a value for the kinematics part of the state
  /// if resetForces is set to true the forces are set to zero
  ///
  void setStateKinematics(const Kinematics &, bool resetContactWrenches = true, bool resetCovariance = true);

  /// Allows to initializa the value of the gyro bias of the IMU
  /// corresponding to the numberOfIMU
  /// reset Covariance allows to reinitialize the gyro bias
  void setGyroBias(const Vector3 &, unsigned numberOfIMU = 1, bool resetCovariance = true);

  /// if only force or torque is available, set the unavailable value to zero
  void setStateUnmodeledWrench(const Vector6 &, bool resetCovariance = true);

  /// Sets a value of the state x_k provided from another source
  /// can be used for initialization of the estimator
  void setStateVector(const Vector &, bool resetCovariance = true);

  /// Getters for the indexes of the state Vector
  inline unsigned kineIndex() const;
  inline unsigned posIndex() const;
  inline unsigned oriIndex() const;
  inline unsigned linVelIndex() const;
  inline unsigned angVelIndex() const;
  inline unsigned gyroBiasIndex(unsigned IMUNumber) const;
  inline unsigned unmodeledWrenchIndex() const;
  inline unsigned unmodeledForceIndex() const;
  inline unsigned unmodeledTorqueIndex() const;
  inline unsigned contactsIndex() const;
  inline unsigned contactIndex(unsigned contactNbr) const;
  inline unsigned contactKineIndex(unsigned contactNbr) const;
  inline unsigned contactPosIndex(unsigned contactNbr) const;
  inline unsigned contactOriIndex(unsigned contactNbr) const;
  inline unsigned contactForceIndex(unsigned contactNbr) const;
  inline unsigned contactTorqueIndex(unsigned contactNbr) const;
  inline unsigned contactWrenchIndex(unsigned contactNbr) const;

  /// Getters for the indexes of the state Vector
  inline unsigned kineIndexTangent() const;
  inline unsigned posIndexTangent() const;
  inline unsigned oriIndexTangent() const;
  inline unsigned linVelIndexTangent() const;
  inline unsigned angVelIndexTangent() const;
  inline unsigned gyroBiasIndexTangent(unsigned IMUNumber) const;
  inline unsigned unmodeledWrenchIndexTangent() const;
  inline unsigned unmodeledForceIndexTangent() const;
  inline unsigned unmodeledTorqueIndexTangent() const;
  inline unsigned contactsIndexTangent() const;
  inline unsigned contactIndexTangent(unsigned contactNbr) const;
  inline unsigned contactKineIndexTangent(unsigned contactNbr) const;
  inline unsigned contactPosIndexTangent(unsigned contactNbr) const;
  inline unsigned contactOriIndexTangent(unsigned contactNbr) const;
  inline unsigned contactForceIndexTangent(unsigned contactNbr) const;
  inline unsigned contactTorqueIndexTangent(unsigned contactNbr) const;
  inline unsigned contactWrenchIndexTangent(unsigned contactNbr) const;

  /// /////////////////////////////////////////////////////
  /// Setting and getting the state of the estimation
  /// ////////////////////////////////////////////////////
  void setWithUnmodeledWrench(bool b = true);

  /// Sets if the update() function estimates also the accelerations
  void setWithAccelerationEstimation(bool b = true);

  void setWithGyroBias(bool b = true);

  /// ///////////////////////////////////////////////
  /// Getting and setting input data and measurements
  /// /////////////////////////////////////////////

  /// sets the measurement of the IMU (gyrometer, accelerometer, kinematics and number of the IMU)
  /// accelero and gyrometer are the measurement
  /// localKine gets the kinematics of the IMU expressed in the observed frame
  /// the best is to provide the position, the orientation,
  /// the angular and linear velocities and the linear acceleration
  /// Nevertheless if velocities or accelerations are not available they will be
  /// automatically computed through finite differences
  /// the acceleroCov and the gyroCov are the covariance matrices of the sensors
  int setIMU(const Vector3 & accelero, const Vector3 & gyrometer, const Kinematics & localKine, int num = -1);
  int setIMU(const Vector3 & accelero,
             const Vector3 & gyrometer,
             const Matrix3 & acceleroCov,
             const Matrix3 gyroCov,
             const Kinematics & localKine,
             int num = -1);

  void setIMUDefaultCovarianceMatrix(const Matrix3 & acceleroCov, const Matrix3 & gyroCov);

  ////////// Contact stters, these are MANDATORY for every contact at every iteration ///
  /// if the contact is equipped with wrench sensor call setContactWrenchSensor
  /// otherwise calls etContactWithoutSensor

  /// sets the measurements of a force/torque sensor of the contact numbered contactNumber
  /// wrenchMeasurement is the measurment vector composed with 3D forces and 3D torques
  /// localKine sets the kinematics of the contact expressed in the observed frame
  /// the best is to provide the position , the orientation,
  /// the angular and the linear velocities.

  void setContactWrenchSensor(const Vector6 & wrenchMeasurement, const Kinematics & localKine, unsigned contactNumber);
  void setContactWrenchSensor(const Vector6 & wrenchMeasurement,
                              const Matrix6 & wrenchCovMatrix,
                              const Kinematics & localKine,
                              unsigned contactNumber);

  void setContactWrenchSensorDefaultCovarianceMatrix(const Matrix6 & wrenchSensorCovMat);

  /// sets the the kinematics of the contact expressed in the observed frame
  /// the best is to provide the position, the orientation, the angular and the linear velocities.
  /// otherwise they will be automatically computed
  void setContactWithNoSensor(const Kinematics & localKine, unsigned contactNumber);

  /// Set a measurement of the pose. The input is the Measured kinematics
  /// namely position and orientation.
  void setAbsolutePoseSensor(const Kinematics &);
  void setAbsolutePoseSensor(const Kinematics &, const Matrix6 & CovarianceMatrix);

  /// TODO
  // void setVelocityGuess(const Kinematics)

  void setAbsolutePoseSensorDefaultCovarianceMatrix(const Matrix6 &);

  ///////////////////////////////////////////////
  /// Setting inputs to the dynamical system
  //////////////////////////////////////////////////
  /// Add known external forces and moments which are not due to contact
  /// they must be expressed in the same frame as the kinematic root
  void setAdditionalWrench(const Vector3 & force, const Vector3 & torque);

  /// sets the mass of the robot
  void setMass(double);

  /// Sets the 3x3 inertia matrix expressed in the local frame and  optionally
  ///  its time derivative (computed with finite differences otherwise)
  /// the Vector6 version is a vector containing the diagonal and the three non
  /// diagonal values concatenated
  /// it is highly recommended to update these values at every iteration
  void setInertiaMatrix(const Matrix3 & I, const Matrix3 & I_dot);
  void setInertiaMatrix(const Matrix3 & I);
  void setInertiaMatrix(const Vector6 & I, const Vector6 & I_dot);
  void setInertiaMatrix(const Vector6 & I);

  /// Sets the center of mass position expressed in the local frame
  /// and optionally its first and second order time derivarives
  /// computed through finite differences otherwise.
  /// it is highly recommended to update these values at every iteration
  void setCenterOfMass(const Vector3 & com, const Vector3 & com_dot, const Vector3 & com_dot_dot);
  void setCenterOfMass(const Vector3 & com, const Vector3 & com_dot);
  void setCenterOfMass(const Vector3 & com);

  /// Sets the angular momentum expressed in the local frame
  /// and optionally its time derivarive
  /// computed through finite differences otherwise.
  /// it is highly recommended to update these values at every iteration
  void setAngularMomentum(const Vector3 & sigma, const Vector3 & sigma_dot);
  void setAngularMomentum(const Vector3 & sigma);

  ///////////////////////////////
  /// Contact management
  ///////////////////////////////
  /// Set a new contact
  /// -pose is the initial guess on the position of the contact. Only position
  /// and orientation are enough
  /// -initialCovarianceMatrix is the covariance matrix expressing the
  ///  uncertainty of the initial guess (if no initial guess is available
  ///  give a rough position with a high initial covariance matrix)
  /// -processCovarianceMatrix is the covariance matrix expressing the
  ///  rate at which the contact slides (set to zero for no sliding)
  /// -linear, angular stiffness and damping set the flexibility model
  ///  of the contact
  /// set contactNumber to -1 in order to set the number automatically
  /// returns the number of this contact
  int addContact(const Kinematics & pose,
                 const Matrix12 & initialCovarianceMatrix,
                 const Matrix12 & processCovarianceMatrix,
                 const Matrix3 & linearStiffness,
                 const Matrix3 & linearDamping,
                 const Matrix3 & angularStiffness,
                 const Matrix3 & angularDamping,
                 int contactNumber = -1);
  /// version with default stiffness and damping
  /// use when the contact parameters are known
  int addContact(const Kinematics & pose,
                 const Matrix12 & initialCovarianceMatrix,
                 const Matrix12 & processCovarianceMatrix,
                 int contactNumber = -1);
  /// version when the contact position is perfectly known
  int addContact(const Kinematics & pose,
                 const Matrix3 & linearStiffness,
                 const Matrix3 & linearDamping,
                 const Matrix3 & angularStiffness,
                 const Matrix3 & angularDamping,
                 int contactNumber = -1);
  /// version when the position is perfectly known but not the stiffness and damping
  int addContact(const Kinematics & pose, int contactNumber = -1);

  void removeContact(int contactnbr);

  void clearContacts();

  Index getNumberOfContacts() const;

  std::vector<int> getListOfContacts() const;

  /// Sets the covariance matrix of the flexibility Guess
  void setStateCovariance(const Matrix & P);

  void setKinematicsStateCovariance(const Matrix &);
  void setKinematicsInitCovarianceDefault(const Matrix &);
  void setKinematicsProcessCovariance(const Matrix &);

  void setGyroBiasStateCovariance(const Matrix3 & covMat, unsigned imuNumber);
  void setGyroBiasInitCovarianceDefault(const Matrix3 & covMat);
  void setGyroBiasProcessCovariance(const Matrix3 & covMat, unsigned imuNumber);

  void setUnmodeledWrenchStateCovMat(const Matrix6 & currentCovMat);
  void setUnmodeledWrenchIniCovMatDefault(const Matrix6 & initCovMat);
  void setUnmodeledWrenchProcessCovMat(const Matrix6 & processCovMat);

  void setContactStateCovMat(int contactNbr, const Matrix12 & contactCovMat);
  void setContactInitCovMatDefault(const Matrix12 & contactCovMat);
  void setContactProcessCovMat(int contactNbr, const Matrix12 & contactCovMat);

  /// Gets the covariance matrix of the flexibility
  Matrix getStateCovariance() const;

  /// Sets/gets the covariance matrices for the process noises
  /// \li Q process noise
  void setProcessNoiseCovariance(const Matrix & Q);

  /// gets the measurement vector
  Vector getMeasurementVector();

  /// Gets a const reference on the extended Kalman filter
  const ExtendedKalmanFilter & getEKF() const;

  /// Gets a reference on the extended Kalman filter
  /// modifying this object may lead to instabilities
  ExtendedKalmanFilter & getEKF();

  /// Resets the covariance matrices to their original values
  void resetStateCovarianceMat();
  void resetStateKinematicsCovMat();
  void resetStateGyroBiasCovMat(unsigned i);
  void resetStateUnmodeledWrenchCovMat();
  void resetStateContactsCovMat();
  void resetStateContactCovMat(unsigned contactNbr);

  void resetProcessCovarianceMat();
  void resetProcessKinematicsCovMat();
  void resetProcessGyroBiasCovMat(unsigned i);
  void resetProcessUnmodeledWrenchCovMat();
  void resetProcessContactsCovMat();
  void resetProcessContactCovMat(unsigned contactNbr);

  /// Reset the default values for the covariance matrix
  void resetSensorsDefaultCovMat();

  /// to reset all the sensor inputs and provided contact positions but keeps the contacts
  void resetInputs();

protected:
  struct Sensor
  {
    Sensor(int signalSize) : size(signalSize), time(0) {}
    virtual ~Sensor() {}
    int measIndex;
    int size;
    TimeIndex time;

    inline Vector extractFromVector(const Vector & v)
    {
      return v.segment(size, measIndex);
    }
  };

  struct IMU : public Sensor
  {
    virtual ~IMU() {}
    IMU() : Sensor(sizeIMUSignal) {}
    Kinematics kinematics;
    Vector6 acceleroGyro;
    Matrix3 covMatrixAccelero;
    Matrix3 covMatrixGyro;

    static int currentNumber;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };

  typedef std::vector<IMU, Eigen::aligned_allocator<IMU>> VectorIMU;
  typedef VectorIMU::iterator VectorIMUIterator;
  typedef VectorIMU::const_iterator VectorIMUConstIterator;

  struct Contact : public Sensor
  {
    Contact() : Sensor(sizeWrench), isSet(false), withRealSensor(false), stateIndex(-1), stateIndexTangent(-1) {}
    virtual ~Contact() {}

    Kinematics absPose;
    Vector6 wrench;
    CheckedMatrix6 sensorCovMatrix;

    Matrix3 linearStiffness;
    Matrix3 linearDamping;
    Matrix3 angularStiffness;
    Matrix3 angularDamping;

    bool isSet;
    bool withRealSensor;
    int stateIndex;
    int stateIndexTangent;

    Kinematics localKine; /// describes the kinematics of the contact point in the local frame
    static const Kinematics::Flags::Byte localKineFlags = /// flags for the components of the kinematics
        Kinematics::Flags::position | Kinematics::Flags::orientation | Kinematics::Flags::linVel
        | Kinematics::Flags::angVel;

    static int numberOfRealSensors;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };

  typedef std::vector<Contact, Eigen::aligned_allocator<Contact>> VectorContact;
  typedef VectorContact::iterator VectorContactIterator;
  typedef VectorContact::const_iterator VectorContactConstIterator;

  struct AbsolutePoseSensor : public Sensor
  {
    AbsolutePoseSensor() : Sensor(sizePose) {}

    Kinematics pose;
    CheckedMatrix6 covMatrix;
  };

protected:
  ///////////// DYNAMICAL SYSTEM IMPLEMENTATION
  virtual Vector stateDynamics(const Vector & x, const Vector & u, TimeIndex k);

  virtual Vector measureDynamics(const Vector & x, const Vector & u, TimeIndex k);

  void addUnmodeledAndContactWrench_(const Vector & stateVector, Vector3 & force, Vector3 & torque);

  void computeAccelerations_(Kinematics & stateKine,
                             const Vector3 & totalForceLocal,
                             const Vector3 & totalMomentLocal,
                             Vector3 & linAcc,
                             Vector3 & angAcc);

  /// the kinematics is not const to allow more optimized non const operators to work
  void computeContactForces_(VectorContactIterator i,
                             Kinematics & stateKine,
                             Kinematics & contactPose,
                             Vector3 & Force,
                             Vector3 torque);

  /// Sets a noise which disturbs the state dynamics
  virtual void setProcessNoise(NoiseBase *);

  /// Removes the process noise
  virtual void resetProcessNoise();
  /// Gets the process noise
  virtual NoiseBase * getProcessNoise() const;

  /// Sets a noise which disturbs the measurements
  virtual void setMeasurementNoise(NoiseBase *);
  /// Removes the measurement noise
  virtual void resetMeasurementNoise();
  /// Gets a pointer on the measurement noise
  virtual NoiseBase * getMeasurementNoise() const;

  /// Gets the input size
  virtual Index getInputSize() const;

public:
  virtual void stateSum(const Vector & stateVector, const Vector & tangentVector, Vector & sum);
  inline Vector stateSum(const Vector & stateVector, const Vector & tangentVector);

  virtual void stateDifference(const Vector & stateVector1, const Vector & stateVector2, Vector & difference);
  inline Vector stateDifference(const Vector & stateVector1, const Vector & stateVector2);

  virtual void measurementDifference(const Vector & measureVector1, const Vector & measureVector2, Vector & difference);
  ///////////////////////////////////////

  ////////////////////////////
  virtual void setFiniteDifferenceStep(const Vector & dx);
  virtual void useFiniteDifferencesJacobians(bool b = true);

protected:
  Vector stateNaNCorrection_();

  /// updates stateKine_ from the stateVector
  void updateKine_();

protected:
  unsigned maxContacts_;
  unsigned maxImuNumber_;

  AbsolutePoseSensor absPoseSensor_;
  VectorContact contacts_;
  VectorIMU imuSensors_;

  Index stateSize_;
  Index stateTangentSize_;
  Index measurementSize_;
  Index measurementTangentSize_;

  Kinematics stateKinematics_;

  Vector stateVector_;
  Vector stateVectorDx_;
  Vector oldStateVector_;

  Vector3 additionalForce_;
  Vector3 additionalTorque_;

  Vector measurementVector_;
  Matrix measurementCovMatrix_;

  stateObservation::ExtendedKalmanFilter ekf_;
  bool finiteDifferencesJacobians_;
  bool withGyroBias_;
  bool withUnmodeledWrench_;
  bool withAccelerationEstimation_;

  IndexedVector3 com_, comd_, comdd_;
  IndexedVector3 sigma_, sigmad_;
  IndexedMatrix3 I_, Id_;

  TimeIndex k_est_;
  TimeIndex k_data_;

  double mass_;

  double dt_;

  NoiseBase * processNoise_;
  NoiseBase * measurementNoise_;

  /// function to call before adding any measurement
  /// detects if there is a new estimation beginning and then
  /// calls the reset of the iteration
  void startNewIteration_();

  virtual Matrix computeAMatrix_();
  virtual Matrix computeCMatrix_();

  /// Getters for the indexes of the state Vector using private types
  inline unsigned contactIndex(VectorContactConstIterator i) const;
  inline unsigned contactKineIndex(VectorContactConstIterator i) const;
  inline unsigned contactPosIndex(VectorContactConstIterator i) const;
  inline unsigned contactOriIndex(VectorContactConstIterator i) const;
  inline unsigned contactForceIndex(VectorContactConstIterator i) const;
  inline unsigned contactTorqueIndex(VectorContactConstIterator i) const;
  inline unsigned contactWrenchIndex(VectorContactConstIterator i) const;

  /// Getters for the indexes of the state Vector using private types
  inline unsigned contactIndexTangent(VectorContactConstIterator i) const;
  inline unsigned contactKineIndexTangent(VectorContactConstIterator i) const;
  inline unsigned contactPosIndexTangent(VectorContactConstIterator i) const;
  inline unsigned contactOriIndexTangent(VectorContactConstIterator i) const;
  inline unsigned contactForceIndexTangent(VectorContactConstIterator i) const;
  inline unsigned contactTorqueIndexTangent(VectorContactConstIterator i) const;
  inline unsigned contactWrenchIndexTangent(VectorContactConstIterator i) const;

private:
public:
  ///////////SIZE OF VECTORS
  static const unsigned sizeAcceleroSignal = 3;
  static const unsigned sizeGyroSignal = 3;
  static const unsigned sizeIMUSignal = sizeAcceleroSignal + sizeGyroSignal;

  static const unsigned sizePos = 3;
  static const unsigned sizeOri = 4;
  static const unsigned sizeOriTangent = 3;
  static const unsigned sizeLinVel = sizePos;
  static const unsigned sizeAngVel = sizeOriTangent;
  static const unsigned sizeGyroBias = sizeGyroSignal;

  static const unsigned sizeForce = 3;
  static const unsigned sizeTorque = 3;

  static const unsigned sizeWrench = sizeForce + sizeTorque;

  static const unsigned sizeStateKine = sizePos + sizeOri + sizeLinVel + sizeAngVel;
  static const unsigned sizeStateBase = sizeStateKine + sizeForce + sizeTorque;
  static const unsigned sizeStateKineTangent = sizePos + sizeOriTangent + sizeLinVel + sizeAngVel;
  static const unsigned sizeStateTangentBase = sizeStateKineTangent + sizeForce + sizeTorque;

  static const unsigned sizePose = sizePos + sizeOri;
  static const unsigned sizePoseTangent = sizePos + sizeOriTangent;

  static const unsigned sizeContactKine = sizePose;
  static const unsigned sizeContactKineTangent = sizePoseTangent;

  static const unsigned sizeContact = sizeContactKine + sizeWrench;
  static const unsigned sizeContactTangent = sizeContactKineTangent + sizeWrench;

  static const Kinematics::Flags::Byte flagsStateKine = Kinematics::Flags::position | Kinematics::Flags::orientation
                                                        | Kinematics::Flags::linVel | Kinematics::Flags::angVel;

  static const Kinematics::Flags::Byte flagsContactKine = Kinematics::Flags::position | Kinematics::Flags::orientation;

  static const Kinematics::Flags::Byte flagsPoseKine = Kinematics::Flags::position | Kinematics::Flags::orientation;

  static const Kinematics::Flags::Byte flagsIMUKine = Kinematics::Flags::position | Kinematics::Flags::orientation
                                                      | Kinematics::Flags::linVel | Kinematics::Flags::angVel
                                                      | Kinematics::Flags::linAcc;

  ////////////DEFAULT VALUES //////
  static const double defaultMass;

  static const double statePoseInitVarianceDefault;
  static const double stateOriInitVarianceDefault;
  static const double stateLinVelInitVarianceDefault;
  static const double stateAngVelInitVarianceDefault;
  static const double gyroBiasInitVarianceDefault;
  static const double unmodeledWrenchInitVarianceDefault;
  static const double contactForceInitVarianceDefault;
  static const double contactTorqueInitVarianceDefault;

  static const double statePoseProcessVarianceDefault;
  static const double stateOriProcessVarianceDefault;
  static const double stateLinVelProcessVarianceDefault;
  static const double stateAngVelProcessVarianceDefault;
  static const double gyroBiasProcessVarianceDefault;
  static const double unmodeledWrenchProcessVarianceDefault;
  static const double contactPositionProcessVarianceDefault;
  static const double contactOrientationProcessVarianceDefault;
  static const double contactForceProcessVarianceDefault;
  static const double contactTorqueProcessVarianceDefault;

  static const double acceleroVarianceDefault;
  static const double gyroVarianceDefault;
  static const double forceSensorVarianceDefault;
  static const double torqueSensorVarianceDefault;
  static const double positionSensorVarianceDefault;
  static const double orientationSensorVarianceDefault;

  static const double linearStiffnessDefault;
  static const double angularStiffnessDefault;
  static const double linearDampingDefault;
  static const double angularDampingDefault;

  ////////////
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

protected:
  /// Default Stiffness and damping
  Matrix3 linearStiffnessMatDefault_;
  Matrix3 angularStiffnessMatDefault_;
  Matrix3 linearDampingMatDefault_;
  Matrix3 angularDampingMatDefault_;

  ////////////Sensor Covariance mnatrices
  Matrix3 acceleroCovMatDefault_;
  Matrix3 gyroCovMatDefault_;
  Matrix6 contactWrenchSensorCovMatDefault_;
  Matrix6 absPoseSensorCovMatDefault_;

  Matrix3 statePosInitCovMat_;
  Matrix3 stateOriInitCovMat_;
  Matrix3 stateLinVelInitCovMat_;
  Matrix3 stateAngVelInitCovMat_;
  Matrix3 gyroBiasInitCovMat_;
  Matrix6 unmodeledWrenchInitCovMat_;
  Matrix12 contactInitCovMat_;

  Matrix3 statePosProcessCovMat_;
  Matrix3 stateOriProcessCovMat_;
  Matrix3 stateLinVelProcessCovMat_;
  Matrix3 stateAngVelProcessCovMat_;
  Matrix3 gyroBiasProcessCovMat_;
  Matrix6 unmodeledWrenchProcessCovMat_;
  Matrix3 contactPositionProcessCovMat_;
  Matrix3 contactOrientationProcessCovMat_;
  Matrix3 contactForceProcessCovMat_;
  Matrix3 contactTorqueProcessCovMat_;
  Matrix12 contactProcessCovMat_;

  Matrix12 stateKinematicsInitCovMat_;
  Matrix12 stateKinematicsProcessCovMat_;

  /// default derivation steps
  static const double defaultdx;

  /// a structure to optimize computations
  struct Opt
  {
    Opt() : kine(kine1), ori(kine.orientation), ori1(kine1.orientation), ori2(kine2.orientation) {}

    Kinematics kine1, kine2;
    Kinematics & kine;
    Orientation & ori;
    Orientation & ori1;
    Orientation & ori2;
  } opt_;
};

#include <state-observation/dynamics-estimators/kinetics-observer.hxx>

} // namespace stateObservation

#endif /// KINETICSOBSERVER_HPP
