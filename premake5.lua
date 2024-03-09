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

-- libraries directory
librarylist = {}
librarylist["OpenAL"] = "%{wks.location}/Thirdparty/openal/build";

-- project
include "SoundEngine"