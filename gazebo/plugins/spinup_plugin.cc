#include "spinup_plugin.hh"

using namespace gazebo;
GZ_REGISTER_WORLD_PLUGIN(SpinupPlugin)

SpinupPlugin::SpinupPlugin() : WorldPlugin()
{
}

SpinupPlugin::~SpinupPlugin()
{
}

void SpinupPlugin::Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/)
{
  std::cout << "Spinup plugin loaded" << std::endl;

  // Listen to the update event. This event is broadcast every
  // simulation iteration.
  this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      std::bind(&SpinupPlugin::OnUpdate, this));

  this->world = _parent;

  this->robot_model = this->world->ModelByName("robot");

  this->weapon_joint = this->robot_model->GetJoint("weapon_shaft_joint");

  this->chassis_link = this->robot_model->GetChildLink("drive_base::drive_base_link");
  this->spinner_link = this->robot_model->GetChildLink("spinner::spinner_link");

  // TODO: this could probably be a param thing in the sdf
  this->params.num_attempts_per_hit = 12;
  this->params.increment_amount_kn = 1000;
  this->params.wait_time_before_hit_sec = 0.5;
  this->params.wait_time_after_hit_sec = 2.0;

  // Kickoff first hit
  this->ResetWorld(135.0, 15000.0);

  this->params.hit_counter = 0;
  this->params.unstable_counter = 0;

  this->update_counter = 0;
}

void SpinupPlugin::OnUpdate()
{
  // This lets the world settle at the beginning, don't add a force until
  // the timeout specificed by params.wait_time_before_hit_sec is reached.
  if (this->update_counter == this->params.wait_time_before_hit_sec*1000)
  {
    std::cout << "Hitting at " << this->params.upwards_force_kn << " kN" << std::endl;
    this->spinner_link->AddLinkForce(
      ignition::math::Vector3d(0, this->params.upwards_force_kn, 0),
      ignition::math::Vector3d(0, 0, 0.5));
    this->params.hit_counter++;
  }

  // Gets robot chassis orientation in one axis
  // If the angle is above a certain angle, we just say it's unstable... whatever
  double x = this->chassis_link->WorldPose().Rot().Euler()[0];
  bool isUnstable = (fabs(x - 1.5707) > 0.5);

  // TODO: These "1000"s should be taken from the world, as this is assuming there is
  //       1000 updates per second

  // After a certain period of time, specified by before_time + after_time, time out and restart the test
  bool isTimedout = (this->update_counter >= this->params.wait_time_before_hit_sec*1000.0 + this->params.wait_time_after_hit_sec*1000.0);

  // If this hit timed out, or the robot went unstable, reset and go do the next test
  if (isTimedout || isUnstable)
  {
    if (isUnstable)
    {
      this->params.unstable_counter++;
      std::cout << "Robot went unstable!" << std::endl;
    }

    // Check to see if we're moving onto the next set of hits
    if (this->params.hit_counter >= this->params.num_attempts_per_hit)
    {
      // Save data.
      // For now just print to console
      std::cout << "Hits with force " << this->params.upwards_force_kn;
      std::cout << " went unstable " << this->params.unstable_counter << " out of ";
      std::cout << this->params.num_attempts_per_hit << " times" << std::endl;

      this->params.hit_counter = 0;
      this->params.unstable_counter = 0;

      this->params.upwards_force_kn += this->params.increment_amount_kn;
    }
    else
    {
      std::cout << "On attempt " << this->params.hit_counter << std::endl;
    }

    // Reset the test
    this->ResetWorld(135.0, this->params.upwards_force_kn);
  }

  this->update_counter++;
}

void SpinupPlugin::ResetWorld(double spinner_rad_per_sec, double upwards_force_kn)
{
  std::cout << "Resetting world" << std::endl;
  this->params.spinner_rad_per_sec = spinner_rad_per_sec;

  this->params.upwards_force_kn = upwards_force_kn;

  this->world->Reset();
  this->weapon_joint->SetVelocity(0, this->params.spinner_rad_per_sec);
  this->update_counter = 0;
}
