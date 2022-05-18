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
PLAYDATE_SIMULATOR = "#{SDK_ROOT}/bin/Playdate Simulator.app"#/Contents/MacOS/Playdate Simulator"
BUILD_DIR = 'build_dir'
PDX_FILE = FileList["*.pdx"]

directory BUILD_DIR

CLEAN.include(BUILD_DIR)
CLEAN.include('source/pdex.*')
CLOBBER.include(PDX_FILE)


def define_cmake_make_task(target, type, option)
  desc "Generate Makefile (#{target}, #{type})"
  task type.downcase => BUILD_DIR do |t|
    cd t.source
    sh "cmake .. -DGAME_NAME=#{GAME_NAME} -DCMAKE_BUILD_TYPE=#{type} #{option}"
  end
end

def define_cmake_xcode_task(target, option)
  desc "Generate Xcode project (#{target})"
  task target.downcase => BUILD_DIR do |t|
    cd t.source
    sh "cmake .. -DGAME_NAME=#{GAME_NAME} #{option} -G Xcode"
    sh "open ."
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

  namespace :xcode do
    define_cmake_xcode_task('simulator', "")
    #define_cmake_xcode_task('device', "-DCMAKE_TOOLCHAIN_FILE=#{SDK_ROOT}/C_API/buildsupport/arm.cmake")
  end
end

desc "Build"
task :build do
  cd BUILD_DIR
  sh "make"
end

desc "Run on Simulator"
task :run do
  sh "open \"#{PLAYDATE_SIMULATOR}\" #{PDX_FILE}"
end

desc "Test"
task :test do
  cd BUILD_DIR
  sh "make test"
end
