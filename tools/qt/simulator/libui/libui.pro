CONFIG -= qt

TEMPLATE = lib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
    CJSON_HIDE_SYMBOLS \
    "ENABLE_SHAPING=0" \
    "ENABLE_ICU=1" \
    "ENABLE_VECTOR_FONT=1" \
    "ENABLE_BITMAP_FONT=0" \
    "ENABLE_MULTI_FONT=0" \
    "ENABLE_STATIC_FONT=0" \
    "DEFAULT_ANIMATION=1"


DEFINES += QT_COMPILER

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../libs

SOURCES += \
    ../../../../../utils/frameworks/graphic_timer.cpp \
    ../../../../frameworks/animator/animator.cpp \
    ../../../../frameworks/animator/gif_canvas_image_animator.cpp \
    ../../../../frameworks/animator/animator_manager.cpp \
    ../../../../frameworks/animator/easing_equation.cpp \
    ../../../../frameworks/animator/interpolation.cpp \
    ../../../../frameworks/common/graphic_startup.cpp \
    ../../../../frameworks/common/image_decode_ability.cpp \
    ../../../../frameworks/common/image.cpp \
    ../../../../frameworks/common/input_device_manager.cpp \
    ../../../../frameworks/common/screen.cpp \
    ../../../../frameworks/common/task.cpp \
    ../../../../frameworks/common/text.cpp \
    ../../../../frameworks/common/typed_text.cpp \
    ../../../../frameworks/components/root_view.cpp \
    ../../../../frameworks/components/text_adapter.cpp \
    ../../../../frameworks/components/ui_abstract_clock.cpp \
    ../../../../frameworks/components/ui_abstract_progress.cpp \
    ../../../../frameworks/components/ui_abstract_scroll.cpp \
    ../../../../frameworks/components/ui_abstract_scroll_bar.cpp \
    ../../../../frameworks/components/ui_analog_clock.cpp \
    ../../../../frameworks/components/ui_arc_label.cpp \
    ../../../../frameworks/components/ui_arc_scroll_bar.cpp \
    ../../../../frameworks/components/ui_axis.cpp \
    ../../../../frameworks/components/ui_box_progress.cpp \
    ../../../../frameworks/components/ui_box_scroll_bar.cpp \
    ../../../../frameworks/components/ui_button.cpp \
    ../../../../frameworks/components/ui_canvas.cpp \
    ../../../../frameworks/components/ui_chart.cpp \
    ../../../../frameworks/components/ui_checkbox.cpp \
    ../../../../frameworks/components/ui_circle_progress.cpp \
    ../../../../frameworks/components/ui_dialog.cpp \
    ../../../../frameworks/components/ui_digital_clock.cpp \
    ../../../../frameworks/components/ui_image_animator.cpp \
    ../../../../frameworks/components/ui_image_view.cpp \
    ../../../../frameworks/components/ui_label.cpp \
    ../../../../frameworks/components/ui_label_button.cpp \
    ../../../../frameworks/components/ui_list.cpp \
    ../../../../frameworks/components/ui_picker.cpp \
    ../../../../frameworks/components/ui_qrcode.cpp \
    ../../../../frameworks/components/ui_radio_button.cpp \
    ../../../../frameworks/components/ui_repeat_button.cpp \
    ../../../../frameworks/components/ui_scroll_view.cpp \
    ../../../../frameworks/components/ui_slider.cpp \
    ../../../../frameworks/components/ui_swipe_view.cpp \
    ../../../../frameworks/components/ui_texture_mapper.cpp \
    ../../../../frameworks/components/ui_time_picker.cpp \
    ../../../../frameworks/components/ui_toggle_button.cpp \
    ../../../../frameworks/components/ui_view.cpp \
    ../../../../frameworks/components/ui_view_group.cpp \
    ../../../../frameworks/dock/focus_manager.cpp \
    ../../../../frameworks/core/render_manager.cpp \
    ../../../../frameworks/core/task_manager.cpp \
    ../../../../frameworks/default_resource/check_box_res.cpp \
    ../../../../frameworks/dfx/event_injector.cpp \
    ../../../../frameworks/dfx/key_event_injector.cpp \
    ../../../../frameworks/dfx/performance_task.cpp \
    ../../../../frameworks/dfx/point_event_injector.cpp \
    ../../../../frameworks/dfx/ui_dump_dom_tree.cpp \
    ../../../../frameworks/dfx/ui_view_bounds.cpp \
    ../../../../frameworks/dock/input_device.cpp \
    ../../../../frameworks/dock/key_input_device.cpp \
    ../../../../frameworks/dock/pointer_input_device.cpp \
    ../../../../frameworks/dock/rotate_input_device.cpp \
    ../../../../frameworks/dock/screen_device_proxy.cpp \
    ../../../../frameworks/dock/vibrator_manager.cpp \
    ../../../../frameworks/dock/virtual_input_device.cpp \
    ../../../../frameworks/engines/gfx/gfx_engine_manager.cpp \
    ../../../../frameworks/draw/clip_utils.cpp \
    ../../../../frameworks/draw/draw_arc.cpp \
    ../../../../frameworks/draw/draw_curve.cpp \
    ../../../../frameworks/draw/draw_image.cpp \
    ../../../../frameworks/draw/draw_label.cpp \
    ../../../../frameworks/draw/draw_line.cpp \
    ../../../../frameworks/draw/draw_rect.cpp \
    ../../../../frameworks/draw/draw_triangle.cpp \
    ../../../../frameworks/draw/draw_utils.cpp \
    ../../../../frameworks/engines/gfx/gfx_enginex_manager.cpp \
    ../../../../frameworks/events/event.cpp \
    ../../../../frameworks/font/base_font.cpp \
    ../../../../frameworks/font/glyphs_manager.cpp \
    ../../../../frameworks/font/ui_font.cpp \
    ../../../../frameworks/font/ui_font_adaptor.cpp \
    ../../../../frameworks/font/ui_font_allocator.cpp \
    ../../../../frameworks/font/ui_font_bitmap.cpp \
    ../../../../frameworks/font/ui_font_builder.cpp \
    ../../../../frameworks/font/ui_font_cache.cpp \
    ../../../../frameworks/font/ui_font_vector.cpp \
    ../../../../frameworks/font/ui_line_break.cpp \
    ../../../../frameworks/font/ui_multi_font_manager.cpp \
    ../../../../frameworks/font/ui_text_shaping.cpp \
    ../../../../../utils/frameworks/graphics/graphic_geometry/agg_arc.cpp \
    ../../../../../utils/frameworks/graphics/graphic_geometry/agg_arrowhead.cpp \
    ../../../../../utils/frameworks/graphics/graphic_geometry/agg_bezier_arc.cpp \
    ../../../../../utils/frameworks/graphics/graphic_geometry/agg_curves.cpp \
    ../../../../../utils/frameworks/graphics/graphic_geometry/agg_line_aa_basics.cpp \
    ../../../../../utils/frameworks/graphics/graphic_geometry/agg_rounded_rect.cpp \
    ../../../../../utils/frameworks/graphics/graphic_color/agg_color_rgba.cpp \
    ../../../../../utils/frameworks/graphics/graphic_common/agg_sqrt_tables.cpp \
    ../../../../../utils/frameworks/graphics/graphic_filter/agg_image_filters.cpp \
    ../../../../../utils/frameworks/graphics/graphic_transform/agg_trans_affine.cpp \
    ../../../../../utils/frameworks/graphics/graphic_transform/agg_trans_single_path.cpp \
    ../../../../../utils/frameworks/graphics/graphic_transform/agg_trans_warp_magnifier.cpp \
    ../../../../../utils/frameworks/graphics/graphic_vertex_generate/agg_vcgen_contour.cpp \
    ../../../../../utils/frameworks/graphics/graphic_vertex_generate/agg_vcgen_dash.cpp \
    ../../../../../utils/frameworks/graphics/graphic_vertex_generate/agg_vcgen_markers_term.cpp \
    ../../../../../utils/frameworks/graphics/graphic_vertex_generate/agg_vcgen_smooth_poly1.cpp \
    ../../../../../utils/frameworks/graphics/graphic_vertex_generate/agg_vcgen_stroke.cpp \
#    ../../../../../utils/frameworks/graphics/graphic_geometry/agg_embedded_raster_fonts.cpp \
#    ../../../../../utils/frameworks/graphics/graphic_geometry/agg_line_profile_aa.cpp \
#    ../../../../../utils/frameworks/graphics/graphic_vertex_generate/agg_vpgen_segmentator.cpp \
    ../../../../frameworks/imgdecode/cache_manager.cpp \
    ../../../../frameworks/imgdecode/file_img_decoder.cpp \
    ../../../../frameworks/imgdecode/image_load.cpp \
    ../../../../frameworks/layout/flex_layout.cpp \
    ../../../../frameworks/layout/grid_layout.cpp \
    ../../../../frameworks/layout/list_layout.cpp \
    ../../../../frameworks/themes/theme.cpp \
    ../../../../frameworks/themes/theme_manager.cpp \
    ../../../../../utils/frameworks/color.cpp \
    ../../../../../utils/frameworks/geometry2d.cpp \
    ../../../../../utils/frameworks/graphic_math.cpp \
    ../../../../../utils/frameworks/graphic_performance.cpp \
    ../../../../../utils/frameworks/hal_tick.cpp \
    ../../../../../utils/frameworks/mem_api.cpp \
    ../../../../../utils/frameworks/style.cpp \
    ../../../../../utils/frameworks/sys_info.cpp \
    ../../../../../utils/frameworks/transform.cpp \
    ../../../../../utils/frameworks/version.cpp \
    ../../../../../../../third_party/bounds_checking_function/src/memset_s.c \
    ../../../../../../../third_party/cJSON/cJSON.c \

HEADERS += \
    ../../../../../utils/interfaces/innerkits/graphic_timer.h \
    ../../../../frameworks/common/typed_text.h \
    ../../../../frameworks/core/render_manager.h \
    ../../../../frameworks/default_resource/check_box_res.h \
    ../../../../frameworks/dfx/key_event_injector.h \
    ../../../../frameworks/dfx/point_event_injector.h \
    ../../../../frameworks/dfx/ui_view_bounds.h \
    ../../../../frameworks/dock/input_device.h \
    ../../../../frameworks/dock/pointer_input_device.h \
    ../../../../frameworks/dock/virtual_input_device.h \
    ../../../../frameworks/draw/clip_utils.h \
    ../../../../frameworks/draw/draw_arc.h \
    ../../../../frameworks/draw/draw_curve.h \
    ../../../../frameworks/draw/draw_image.h \
    ../../../../frameworks/draw/draw_label.h \
    ../../../../frameworks/draw/draw_line.h \
    ../../../../frameworks/draw/draw_rect.h \
    ../../../../frameworks/draw/draw_triangle.h \
    ../../../../frameworks/draw/draw_utils.h \
    ../../../../frameworks/font/ui_font_adaptor.h \
    ../../../../frameworks/font/ui_multi_font_manager.h \
    ../../../../frameworks/render/agg_pixfmt_base.h \
    ../../../../frameworks/render/agg_pixfmt_rgba.h \
    ../../../../frameworks/render/agg_pixfmt_transposer.h \
    ../../../../frameworks/render/agg_renderer_base.h \
    ../../../../frameworks/render/agg_renderer_scanline.h \
    ../../../../frameworks/render/agg_rendering_buffer.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_color/agg_color_gray.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_color/agg_color_rgba.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_common/agg_basics.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_common/agg_clip_liang_barsky.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_common/agg_gamma_functions.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_common/agg_gamma_lut.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_common/agg_math.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_depict/agg_conv_adaptor_vcgen.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_depict/agg_conv_contour.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_depict/agg_conv_curve.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_depict/agg_conv_dash.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_depict/agg_conv_stroke.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_depict/agg_conv_transform.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_filter/agg_blur.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_filter/agg_image_filters.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_arc.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_array.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_arrowhead.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_bezier_arc.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_bounding_rect.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_curves.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_dda_line.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_ellipse.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_ellipse_bresenham.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_line_aa_basics.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_math_stroke.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_path_storage.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_rounded_rect.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_shorten_path.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_geometry/agg_vertex_sequence.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_rasterizer/agg_rasterizer_cells_aa.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_rasterizer/agg_rasterizer_scanline_aa.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_rasterizer/agg_rasterizer_scanline_aa_nogamma.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_rasterizer/agg_rasterizer_sl_clip.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_scanline/agg_scanline_p.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_scanline/agg_scanline_u.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_spancolor_fill/agg_gradient_lut.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_spancolor_fill/agg_span_allocator.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_spancolor_fill/agg_span_converter.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_spancolor_fill/agg_span_gradient.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_spancolor_fill/agg_span_image_filter.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_spancolor_fill/agg_span_image_filter_rgba.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_spancolor_fill/agg_span_interpolator_linear.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_spancolor_fill/agg_span_pattern_rgba.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_transform/agg_image_accessors.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_transform/agg_trans_affine.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_transform/agg_trans_single_path.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_transform/agg_trans_viewport.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_transform/agg_trans_warp_magnifier.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_transform/agg_ctrl.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_vertex_generate/agg_vcgen_contour.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_vertex_generate/agg_vcgen_dash.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_vertex_generate/agg_vcgen_markers_term.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_vertex_generate/agg_vcgen_smooth_poly1.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/graphic_vertex_generate/agg_vcgen_stroke.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/util/agg_color_conv.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/util/agg_color_conv_rgb16.h \
    ../../../../../utils/interfaces/kits/gfx_utils/graphics/util/agg_color_conv_rgb8.h \
    ../../../../frameworks/imgdecode/cache_manager.h \
    ../../../../frameworks/imgdecode/file_img_decoder.h \
    ../../../../frameworks/imgdecode/image_load.h \
    ../../../../interfaces/innerkits/common/graphic_startup.h \
    ../../../../interfaces/innerkits/common/image_decode_ability.h \
    ../../../../interfaces/innerkits/common/input_device_manager.h \
    ../../../../interfaces/innerkits/common/task_manager.h \
    ../../../../interfaces/innerkits/dock/focus_manager.h \
    ../../../../interfaces/innerkits/dock/rotate_input_device.h \
    ../../../../interfaces/innerkits/dock/vibrator_manager.h \
    ../../../../interfaces/innerkits/engines/gfx/gfx_enginex_manager.h \
    ../../../../interfaces/innerkits/font/ui_font_builder.h \
    ../../../../interfaces/innerkits/engines/gfx/gfx_engine_manager.h \
    ../../../../interfaces/kits/animator/animator.h \
    ../../../../interfaces/kits/animator/gif_canvas_image_animator.h \
    ../../../../interfaces/kits/animator/easing_equation.h \
    ../../../../interfaces/kits/animator/interpolation.h \
    ../../../../interfaces/kits/common/image.h \
    ../../../../interfaces/kits/common/screen.h \
    ../../../../interfaces/kits/common/task.h \
    ../../../../interfaces/kits/common/text.h \
    ../../../../interfaces/kits/components/abstract_adapter.h \
    ../../../../interfaces/kits/components/root_view.h \
    ../../../../interfaces/kits/components/text_adapter.h \
    ../../../../interfaces/kits/components/ui_abstract_clock.h \
    ../../../../interfaces/kits/components/ui_abstract_progress.h \
    ../../../../interfaces/kits/components/ui_abstract_scroll.h \
    ../../../../interfaces/kits/components/ui_analog_clock.h \
    ../../../../interfaces/kits/components/ui_arc_label.h \
    ../../../../interfaces/kits/components/ui_axis.h \
    ../../../../interfaces/kits/components/ui_box_progress.h \
    ../../../../interfaces/kits/components/ui_button.h \
    ../../../../interfaces/kits/components/ui_canvas.h \
    ../../../../interfaces/kits/components/ui_chart.h \
    ../../../../interfaces/kits/components/ui_checkbox.h \
    ../../../../interfaces/kits/components/ui_circle_progress.h \
    ../../../../interfaces/kits/components/ui_dialog.h \
    ../../../../interfaces/kits/components/ui_digital_clock.h \
    ../../../../interfaces/kits/components/ui_image_animator.h \
    ../../../../interfaces/kits/components/ui_image_view.h \
    ../../../../interfaces/kits/components/ui_label.h \
    ../../../../interfaces/kits/components/ui_label_button.h \
    ../../../../interfaces/kits/components/ui_list.h \
    ../../../../interfaces/kits/components/ui_picker.h \
    ../../../../interfaces/kits/components/ui_qrcode.h \
    ../../../../interfaces/kits/components/ui_radio_button.h \
    ../../../../interfaces/kits/components/ui_repeat_button.h \
    ../../../../interfaces/kits/components/ui_scroll_view.h \
    ../../../../interfaces/kits/components/ui_slider.h \
    ../../../../interfaces/kits/components/ui_swipe_view.h \
    ../../../../interfaces/kits/components/ui_texture_mapper.h \
    ../../../../interfaces/kits/components/ui_time_picker.h \
    ../../../../interfaces/kits/components/ui_toggle_button.h \
    ../../../../interfaces/kits/components/ui_view.h \
    ../../../../interfaces/kits/components/ui_view_group.h \
    ../../../../interfaces/kits/dfx/event_injector.h \
    ../../../../interfaces/kits/dfx/ui_dump_dom_tree.h \
    ../../../../interfaces/kits/events/aod_callback.h \
    ../../../../interfaces/kits/events/cancel_event.h \
    ../../../../interfaces/kits/events/click_event.h \
    ../../../../interfaces/kits/events/drag_event.h \
    ../../../../interfaces/kits/events/event.h \
    ../../../../interfaces/kits/events/long_press_event.h \
    ../../../../interfaces/kits/events/press_event.h \
    ../../../../interfaces/kits/events/release_event.h \
    ../../../../interfaces/kits/events/virtual_device_event.h \
    ../../../../interfaces/kits/font/ui_font.h \
    ../../../../interfaces/kits/font/ui_font_header.h \
    ../../../../interfaces/kits/layout/flex_layout.h \
    ../../../../interfaces/kits/layout/grid_layout.h \
    ../../../../interfaces/kits/layout/layout.h \
    ../../../../interfaces/kits/layout/list_layout.h \
    ../../../../interfaces/kits/themes/theme.h \
    ../../../../interfaces/kits/themes/theme_manager.h \
    ../../../../interfaces/kits/window/window.h \
    ../../../../../utils/interfaces/innerkits/graphic_config.h \
    ../../../../../utils/interfaces/innerkits/graphic_locker.h \
    ../../../../../utils/interfaces/innerkits/graphic_semaphore.h \
    ../../../../../utils/interfaces/innerkits/hal_tick.h \
    ../../../../../utils/interfaces/kits/color.h \
    ../../../../../utils/interfaces/kits/file.h \
    ../../../../../utils/interfaces/kits/geometry2d.h \
    ../../../../../utils/interfaces/kits/graphic_assert.h \
    ../../../../../utils/interfaces/kits/graphic_log.h \
    ../../../../../utils/interfaces/kits/graphic_math.h \
    ../../../../../utils/interfaces/kits/graphic_types.h \
    ../../../../../utils/interfaces/kits/heap_base.h \
    ../../../../../utils/interfaces/kits/image_info.h \
    ../../../../../utils/interfaces/kits/input_event_info.h \
    ../../../../../utils/interfaces/kits/list.h \
    ../../../../../utils/interfaces/kits/mem_api.h \
    ../../../../../utils/interfaces/kits/rect.h \
    ../../../../../utils/interfaces/kits/style.h \
    ../../../../../utils/interfaces/kits/sys_info.h \
    ../../../../../utils/interfaces/kits/transform.h \
    ../../../../../utils/interfaces/kits/version.h \
    ../../../../../wms/interfaces/innerkits/lite_wm_type.h \
    ../../../../../../../third_party/bounds_checking_function/include/securec.h \
    ../../../../../../../third_party/bounds_checking_function/include/securectype.h \
    ../../../../../../../third_party/bounds_checking_function/src/input.inl \
    ../../../../../../../third_party/bounds_checking_function/src/output.inl \
    ../../../../../../../third_party/bounds_checking_function/src/secinput.h \
    ../../../../../../../third_party/bounds_checking_function/src/securecutil.h \
    ../../../../../../../third_party/bounds_checking_function/src/secureprintoutput.h \
    ../../../../../../../third_party/cJSON/cJSON.h \
    ../../../../../../../third_party/cJSON/cJSON_Utils.h \
    ../../../../../../../tools/developer_tools_lite/graphic_tool/iar_project/config/gpu_2d/graphic_config.h




INCLUDEPATH += \
    ../../../../frameworks \
    ../../../../frameworks/graphics/include \
    ../../../../../utils/frameworks/windows \
    ../../../../../utils/interfaces/innerkits \
    ../../../../../utils/interfaces/kits \
    ../../../../../ui/interfaces/innerkits \
    ../../../../../ui/interfaces/kits \
    ../../../../../../../third_party/bounds_checking_function/include \
    ../../../../../../../third_party/harfbuzz/src \
    ../../../../../../../third_party/icu/icu4c/source/common \
    ../../../../../../../third_party/freetype/include \
    ../../../../../../../third_party/cJSON \
    ../../../../../../../third_party/libjpeg \
    ../../../../../../../third_party/libpng \
    ../../../../../../../third_party/qrcodegen/cpp \
    ../../../../../../../third_party/giflib

LIBS += $$OUT_PWD/../libs/libpng.dll
LIBS += $$OUT_PWD/../libs/libjpeg.dll
LIBS += $$OUT_PWD/../libs/qrcodegen.dll
LIBS += $$OUT_PWD/../libs/freetype.dll
LIBS += $$OUT_PWD/../libs/libharfbuzz.a
LIBS += $$OUT_PWD/../libs/icu.dll
LIBS += $$OUT_PWD/../libs/giflib.dll
