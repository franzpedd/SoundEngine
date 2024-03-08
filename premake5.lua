workspace "Solution"
configurations { "Debug", "Release" }
architecture "x86_64"
startproject "SoundEngine"

-- global build path and global vulkan directory
dirpath = "%{wks.location}/.build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
objpath = "%{wks.location}/.build/temp/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- includes directory
includelist = {}
includelist["SoundEngine"] = "%{wks.location}/SoundEngine"
includelist["OpenAL"] = "%{wks.location}/Thirdparty/openal/include"
includelist["SoundFile"] = "%{wks.location}/Thirdparty/libsndfile/include"

-- libraries directory
librarylist = {}
librarylist["OpenAL"] = "%{wks.location}/Thirdparty/openal/build";
librarylist["SoundFile"] = "%{wks.location}/Thirdparty/libsndfile/build";

-- project
include "SoundEngine"