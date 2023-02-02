#!/usr/bin/env python

from __future__ import print_function

import rospy

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



active = {
    "window": True,
    "child": False,
    "tooltip": False,
    "menu bar": False,
    "popup": False,
    "popup modal": False,
    "popup context item": False,
    "popup context window": False,
    "drag drop": False,
    "group": False,
    "tab bar": False,
    "list box": False,
    "popup context void": False,
    "table": False,
}

path_to_font = None  # "path/to/font.ttf"

opened_state = True


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

    

        imgui.begin("Example: table")
        if imgui.begin_table("data", 2):
            imgui.table_next_column()
            imgui.table_header("A")
            imgui.table_next_column()
            imgui.table_header("B")

            imgui.table_next_row()
            imgui.table_next_column()
            imgui.text("123")

            imgui.table_next_column()
            imgui.text("456")

            imgui.table_next_row()
            imgui.table_next_column()
            imgui.text("789")

            imgui.table_next_column()
            imgui.text("111")

            imgui.table_next_row()
            imgui.table_next_column()
            imgui.text("222")

            imgui.table_next_column()
            imgui.text("333")
            imgui.end_table()
        imgui.end()


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


def main():
    imgui.create_context()
    window = impl_glfw_init()

    impl = GlfwRenderer(window)

    io = imgui.get_io()
    jb = io.fonts.add_font_from_file_ttf(path_to_font, 30) if path_to_font is not None else None
    impl.refresh_font_texture()

    while not glfw.window_should_close(window):
        render_frame(impl, window, jb)

    impl.shutdown()
    glfw.terminate()


if __name__ == "__main__":
    main()
