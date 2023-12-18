project('gst-gzdec', 'c',
  version : '0.1.0',
  default_options : [ 'warning_level=1',
                      'buildtype=debugoptimized' ])

core_conf = configuration_data()
core_conf.set('PACKAGE', '"@0@"'.format(meson.project_name()))
core_conf.set('VERSION', '"@0@"'.format(meson.project_version()))

configure_file(output : 'config.h', configuration : core_conf)

configinc = include_directories('.')

common_args = ['-DHAVE_CONFIG_H']

gst_req = '>= 1.0.0'

gst_dep = dependency('gstreamer-1.0', version : gst_req,
  fallback : ['gstreamer', 'gst_dep'])
gst_base_dep = dependency('gstreamer-base-1.0', version : gst_req,
  fallback : ['gstreamer', 'gst_base_dep'])  

# Set the directory where plugins should be installed.
#
# If the prefix is the user home directory, adjust the plugin installation
# path so that GStreamer can find it. Requires meson >= 0.53.0
fs = import('fs')
if fs.is_samepath(get_option('prefix'), '~')
  plugins_install_dir = '@0@/.local/share/gstreamer-1.0/plugins'.format(get_option('prefix'))
else
  plugins_install_dir = '@0@/gstreamer-1.0'.format(get_option('libdir'))
endif

plugin_deps = [gst_dep, gst_base_dep]
tool_deps = [gst_dep]

subdir('plugins')
subdir('tools')
