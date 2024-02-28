#pragma once 

#include "ros/ros.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "cs_plain_guarded.h"

#include "cross_pkg_messages/ManualDriveCMD.h"
#include "cross_pkg_messages/RoverComputerDriveCMD.h"
#include "sensor_msgs/Joy.h"
#include "sensor_msgs/image_encodings.h"
#include "sensor_msgs/CompressedImage.h"
#include "theora_image_transport/Packet.h"
#include <cv_bridge/cv_bridge.h>

extern bool close_ui;