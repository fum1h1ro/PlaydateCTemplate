require 'rake/clean'

GAME_NAME = 'my_first_game'


def get_sdk_root()
  env = ENV['PLAYDATE_SDK_PATH']
  return env unless env.nil?
  File.read(File.expand_path("~/.Playdate/config")).each_line do |x|
    ssv = x.split(/\s+/)
    if ssv[0] == 'SDKRoot'
      return ssv[1]
    end
  end
  raise RuntimeError.new('cannot found SDK')
end

SDK_ROOT = get_sdk_root()
PLAYDATE_SIMULATOR = "#{SDK_ROOT}/bin/Playdate Simulator.app"
BUILD_DIR = 'build_dir'
PDX_FILE = FileList["*.pdx"]

directory BUILD_DIR

CLEAN.include(BUILD_DIR)
CLEAN.include('Source/pdex.*')
CLOBBER.include(PDX_FILE)


def define_cmake_make_task(target, type, option)
  build_dir = "#{BUILD_DIR}/#{target}/#{type.downcase}"
  directory build_dir
  desc "Generate Makefile (#{target}, #{type.downcase})"
  task type.downcase => build_dir do |t|
    cd t.source do
      unless File.exists?('Makefile')
        sh "cmake ../../.. -DGAME_NAME=#{GAME_NAME} -DCMAKE_BUILD_TYPE=#{type} #{option}"
      end
    end
  end
end

def define_cmake_xcode_task(target, option)
  build_dir = "#{BUILD_DIR}/#{target}/xcode"
  directory build_dir
  desc "Generate Xcode project (#{target})"
  task target.downcase => build_dir do |t|
    cd t.source do
      sh "cmake ../../.. -DGAME_NAME=#{GAME_NAME} #{option} -G Xcode"
      sh "open ."
    end
  end
end

def define_build_task(target, type)
  build_dir = "#{BUILD_DIR}/#{target}/#{type.downcase}"
  desc "Build (#{target}, #{type.downcase})"
  task type.downcase => "generate:#{target}:#{type.downcase}" do |t|
    cd build_dir do
      sh "make"
    end
  end
end

namespace :generate do
  namespace :simulator do
    define_cmake_make_task('simulator', 'Debug', "")
    define_cmake_make_task('simulator', 'Release', "")
  end
  namespace :device do
    define_cmake_make_task('device', 'Debug', "-DCMAKE_TOOLCHAIN_FILE=#{SDK_ROOT}/C_API/buildsupport/arm.cmake")
    define_cmake_make_task('device', 'Release', "-DCMAKE_TOOLCHAIN_FILE=#{SDK_ROOT}/C_API/buildsupport/arm.cmake")
  end

  define_cmake_xcode_task('xcode', "")
  #define_cmake_xcode_task('device', "-DCMAKE_TOOLCHAIN_FILE=#{SDK_ROOT}/C_API/buildsupport/arm.cmake")
end

desc "Build"
namespace :build do
  namespace :simulator do
    define_build_task('simulator', 'Debug')
    define_build_task('simulator', 'Release')
  end
  namespace :device do
    define_build_task('device', 'Debug')
    define_build_task('device', 'Release')
  end
end

desc "Build all"
task :build do
  if Dir.exists?(BUILD_DIR)
    ['simulator', 'device'].each do |target|
      ['debug', 'release'].each do |type|
        dir = "#{BUILD_DIR}/#{target}/#{type}"
        if Dir.exists?(dir)
          cd dir do
            sh "make"
          end
        end
      end
    end
  end
end

desc "Run on Simulator"
task :run do
  sh "open \"#{PLAYDATE_SIMULATOR}\" #{PDX_FILE}"
end

desc "Test"
task :test => "build:simulator:debug" do
  cd "#{BUILD_DIR}/simulator/debug" do
    sh "make test"
  end
end
