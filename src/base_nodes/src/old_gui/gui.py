#!/usr/bin/env python

from __future__ import print_function

import rospy
from sensor_msgs.msg import Joy

import os
import sys

#TODO: check if imgui is installed correclty
# import subprocess

# # implement pip as a subprocess:
# subprocess.check_call([sys.executable, '-m', 'pip', 'install', 
# 'imgui[glfw]'])


# For Linux/Wayland users.
if os.getenv("XDG_SESSION_TYPE") == "wayland":
    os.environ["XDG_SESSION_TYPE"] = "x11"


import glfw
import OpenGL.GL as gl
import imgui
from imgui.integrations.glfw import GlfwRenderer

import GuiPanel

path_to_font = None  # "path/to/font.ttf"

panels = [
    GuiPanel.GUIPanel("Status Panel"),
    GuiPanel.GUIPanel("Info Panel"),
]


def frame_commands():
    gl.glClearColor(0.1, 0.1, 0.1, 1)
    gl.glClear(gl.GL_COLOR_BUFFER_BIT)

    io = imgui.get_io()

    if io.key_ctrl and io.keys_down[glfw.KEY_Q]:
        sys.exit(0)

    if imgui.begin_main_menu_bar():
        if imgui.begin_menu("File", True):
            clicked_quit, selected_quit = imgui.menu_item("Quit", "Ctrl+Q", False, True)

            if clicked_quit:
                sys.exit(0)

            imgui.end_menu()
        imgui.end_main_menu_bar()

    for i in panels:
        i.drawInView()

    imgui.begin("Test")
    # imgui.text("Hello")
    for val in joyAxes:
        imgui.text("Axes: " + str(val))
    imgui.end()

def setupWindows(window):
    pass

def setupDefaultDocking(window): 
    # viewport = imgui.GetMainViewport()
    dockspace_id = imgui.GetID("MyDockSpace")

    dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode


    imgui.DockBuilderRemoveNode(dockspace_id); # clear any previous layout
    imgui.DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
    imgui.DockBuilderSetNodeSize(dockspace_id, window.Size);



    dock_id_top = imgui.DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.2, nullptr, dockspace_id);
    dock_id_down = imgui.DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25, nullptr, dockspace_id);
    dock_id_left = imgui.DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2, nullptr, dockspace_id);
    dock_id_right = imgui.DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.15, nullptr, dockspace_id);

    imgui.DockBuilderDockWindow("Window 1", dock_id_top);
    imgui.DockBuilderDockWindow("Window 2", dock_id_right);
    imgui.DockBuilderDockWindow("Window 3", dock_id_left);
    imgui.DockBuilderDockWindow("Window 4", dock_id_down);
    imgui.DockBuilderDockWindow("Window 0", dock_id_top);


    imgui.DockBuilderFinish(dockspace_id);


def render_frame(impl, window, font):
    glfw.poll_events()
    impl.process_inputs()
    imgui.new_frame()

    gl.glClearColor(0.1, 0.1, 0.1, 1)
    gl.glClear(gl.GL_COLOR_BUFFER_BIT)

    if font is not None:
        imgui.push_font(font)
    frame_commands()
    if font is not None:
        imgui.pop_font()

    imgui.render()
    impl.render(imgui.get_draw_data())
    glfw.swap_buffers(window)


def impl_glfw_init():
    width, height = 1600, 900
    window_name = "minimal ImGui/GLFW3 example"

    if not glfw.init():
        print("Could not initialize OpenGL context")
        sys.exit(1)

    glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
    glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
    glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, gl.GL_TRUE)

    window = glfw.create_window(int(width), int(height), window_name, None, None)
    glfw.make_context_current(window)

    if not window:
        glfw.terminate()
        print("Could not initialize Window")
        sys.exit(1)

    return window

joyAxes = []

def joyGot(values):
    global joyAxes
    joyAxes = values.axes;

def main():
    rospy.init_node('Station GUI')
    rospy.Subscriber("joy0", Joy, joyGot)

    imgui.create_context()
    window = impl_glfw_init()

    impl = GlfwRenderer(window)

    io = imgui.get_io()
    jb = io.fonts.add_font_from_file_ttf(path_to_font, 30) if path_to_font is not None else None
    impl.refresh_font_texture()

    # setupDefaultDocking(window);
    setupWindows(window)

    while not glfw.window_should_close(window):
        render_frame(impl, window, jb)

    impl.shutdown()
    glfw.terminate()


if __name__ == "__main__":
    main()
