INCLUDES = -I$(top_srcdir)/nova/include

lib_LTLIBRARIES = libnova.la

#libnova_la_CFLAGS = -O3 -g
#libnova_la_LIBADD = -lGL -lSDL -lGLEW -lz -lfreetype -lxml2 -lpthread -lIL -lILU -lILUT

#AM_CXXFLAGS = $(libnova_la_CFLAGS)
#LIBS = $(libnova_la_LIBS)

EXTRA_DIST = nova_wgl_support.cpp nova_win32_context.cpp \
	nova_win32_workspace.cpp
######################################################
#		N O V A   E N G I N E   C O R E  F I L E S
######################################################

libnova_la_SOURCES = nova_memory.cpp \
                     nova_mesh_box.cpp \
                     nova_mesh_loader.cpp \
nova_3ds_loader.cpp             nova_movable_object.cpp \
nova_allocator.cpp              nova_object_constructor.cpp \
nova_base.cpp                   nova_object.cpp \
nova_bounding_box.cpp \
nova_camera.cpp                 nova_parser.cpp \
nova_color.cpp                  nova_precompiled_headers.cpp \
nova_configloader.cpp           nova_quaternion.cpp \
nova_console.cpp                nova_renderable_object.cpp \
nova_cpu_timer.cpp              nova_render_system.cpp \
nova_devil_codec.cpp            nova_render_target.cpp \
nova_display_list.cpp           nova_render_window.cpp \
nova_engine.cpp                 nova_resource_manager.cpp \
nova_error.cpp                  nova_scene_manager.cpp \
nova_event_listener.cpp         nova_sdl_support.cpp \
nova_events.cpp                 nova_sdl_workspace.cpp \
nova_fonts.cpp                  nova_smart_ptr.cpp \
nova_formats.cpp                nova_sockets.cpp \
nova_frustum.cpp                nova_stable_precompiled_headers.cpp \
nova_glsupport.cpp              nova_static_object.cpp \
nova_gui_base.cpp               nova_streams.cpp \
nova_gui_renderer.cpp           nova_string_utils.cpp \
nova_gzstreams.cpp              nova_texture_manager.cpp \
nova_hardware_buffer.cpp        nova_textures.cpp \
nova_hardware_pixel_buffer.cpp  nova_threads.cpp \
nova_image_codecs.cpp           nova_tree.cpp \
nova_images.cpp                 nova_unicode.cpp \
nova_lenear_scene_manager.cpp   nova_vector2d.cpp \
nova_log.cpp                    nova_vector3d.cpp \
nova_material.cpp               nova_vector4d.cpp \
nova_math.cpp                   nova_viewport.cpp \
nova_matrix2d.cpp \
nova_matrix3d.cpp \
nova_matrix4d.cpp

