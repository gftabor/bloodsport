#include <ignition/math/Pose3.hh>
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/gazebo.hh"
#include <ignition/math/Pose3.hh>

namespace gazebo
{
  struct SpinningTestParams
  {
    double spinner_rad_per_sec;
    double wait_time_before_hit_sec;
    double wait_time_after_hit_sec;
    double upwards_force_kn;

    // When to stop the simulation
    double max_force_kn;

    // How many times per step before incrementing the hit force
    int num_attempts_per_hit;

    // Counters
    int hit_counter;
    int unstable_counter;

    // How many kN to increment by every hit
    int increment_amount_kn;
    double increment_amount_rad_per_sec;
  };

  class SpinupPlugin : public WorldPlugin
  {
    public: SpinupPlugin();
    public: virtual ~SpinupPlugin();
    public: virtual void OnUpdate();

    void Load(physics::WorldPtr _parent, sdf::ElementPtr);

    private: void ResetWorld();

    private: event::ConnectionPtr updateConnection;

    // Pointers to various parts of the robot
    private: physics::WorldPtr world;
    private: physics::ModelPtr robot_model;
    private: physics::LinkPtr spinner_link;
    private: physics::LinkPtr chassis_link;
    private: physics::JointPtr weapon_joint;

    // Params for the test
    private: SpinningTestParams params;

    private: unsigned long update_counter;
    private: double wait_time_before_hit_sec;
    private: double wait_time_after_hit_sec;
  };


};
