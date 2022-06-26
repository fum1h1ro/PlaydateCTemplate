require 'rake/clean'

PDXINFO_FILE = 'Source/pdxinfo'

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

def load_pdxinfo()
  pdxinfo = {}
  File.read(PDXINFO_FILE).each_line do |x|
    pair = x.chomp.split(/=/, 2)
    pdxinfo[pair[0]] = pair[1]
  end
  pdxinfo
end

PDXINFO = load_pdxinfo()
SDK_ROOT = get_sdk_root()
PDC = "#{SDK_ROOT}/bin/pdc"
PLAYDATE_SIMULATOR = "#{SDK_ROOT}/bin/Playdate Simulator.app"
BUILD_DIR = 'build_dir'
PDX_FILES = FileList["*.pdx"]
LUA_FILES = FileList["Source/**/*.lua"]
GAME_NAME = PDXINFO['name'].gsub(/\s/, '_')
BUILD_TARGETS = ['Simulator', 'Device']
BUILD_TYPES = ['Debug', 'Release']

directory BUILD_DIR

CLEAN.include(BUILD_DIR)
CLEAN.include('Source/pdex.*')
CLOBBER.include(PDX_FILES)

def make_game_name(type)
  "#{GAME_NAME}_#{type.downcase}"
end

def all_targets_and_types(&block)
  BUILD_TARGETS.each do |target|
    BUILD_TYPES.each do |type|
      block.call(target.downcase, type.downcase)
    end
  end
end

def define_cmake_make_task(target, type, option)
  build_dir = "#{BUILD_DIR}/#{target}/#{type.downcase}"
  directory build_dir
  desc "Generate Makefile (#{target}, #{type.downcase})"
  task type.downcase => build_dir do |t|
    cd t.source do
      unless File.exists?('Makefile')
        sh "cmake ../../.. -DGAME_NAME=#{make_game_name(type)} -DCMAKE_BUILD_TYPE=#{type} #{option}"
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
      sh "cmake ../../.. -DGAME_NAME=#{make_game_name('xcode')} #{option} -G Xcode"
      sh "open ."
    end
  end
end

def define_build_task(target, type)
  build_dir = "#{BUILD_DIR}/#{target}/#{type.downcase}"
  desc "Build (#{target}, #{type.downcase})"
  task type.downcase => "generate:#{target}:#{type.downcase}" do |t|
    cd build_dir do
      FileList['*.dylib', '*.elf'].each do |binfile|
        rm_f binfile
      end
      sh "rake #{make_game_name(type)}.pdx"
      sh "make"
    end
  end
end

BUILD_TYPES.each do |type|
  file "#{make_game_name(type)}.pdx" => LUA_FILES do |f|
    sh "#{PDC} -sdkpath #{SDK_ROOT} Source #{f.name}"
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
  all_targets_and_types do |target, type|
    dir = "#{BUILD_DIR}/#{target}/#{type}"
    if Dir.exists?(dir)
      sh "rake build:#{target}:#{type}"
    end
  end
end

namespace :run do
  desc "Run on Simulator(Debug)"
  task :debug do
    sh "open \"#{PLAYDATE_SIMULATOR}\" #{make_game_name('Debug')}.pdx"
  end
  desc "Run on Simulator"
  task :release do
    sh "open \"#{PLAYDATE_SIMULATOR}\" #{make_game_name('Release')}.pdx"
  end
end

desc "Test"
task :test => "build:simulator:debug" do
  cd "#{BUILD_DIR}/simulator/debug" do
    begin
      sh "make test"
    rescue
      print("================================================================================\n")
      File.read("Testing/Temporary/LastTest.log").each_line do |l|
        print(l)
      end
      print("================================================================================\n")
      raise
    end
  end
end
