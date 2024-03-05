project "SoundEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir(dirpath)
    objdir(objpath)

    files
    {
        "Source/**.c",
        "Source/**.h",
        "Source/**.hpp",
        "Source/**.cpp",

        "Thirdparty/**.c",
        "Thirdparty/**.h",
        "Thirdparty/**.hpp",
        "Thirdparty/**.cpp"
    }

    includedirs
    {
        "%{includelist.SoundEngine}/Source",
        "%{includelist.SoundEngine}/Thirdparty",

        "%{includelist.SteamAudio}"
    }

    filter {"system:windows"}

        links
        {
            "%{librarylist.SteamAudio}/windows-x64/phonon.lib"
        }


        postbuildcommands
        {
            "{COPY} %{librarylist.SteamAudio}\\windows-x64\\phonon.dll " .. dirpath
        }