Gazebo Bloodsport Sim

Build the plugins:
```
cd plugins
mkdir build
cd build
cmake ..
make
```

Path Setups:
- Ensure to add gazebo/models to your model path in bashrc.  `export GAZEBO_MODEL_PATH=/home/justin/code/bloodsport_ws/src/bloodsport/gazebo/models:$GAZEBO_MODEL_PATH`
- Ensure to add gazebo/plugins/build to your plugin path in bashrc.  `export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:/home/justin/code/bloodsport_ws/src/bloodsport/gazebo/plugins/build`

To run:
`gazebo stability_test.world --verbose`
