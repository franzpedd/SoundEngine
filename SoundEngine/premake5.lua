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
        "Source/**.cpp"
    }

    includedirs
    {
        "%{includelist.SoundEngine}/Source",
        "%{includelist.OpenAL}",
        "%{includelist.SoundFile}"
    }

    links
    {
        "%{librarylist.OpenAL}",
        "%{librarylist.SoundFile}"
    }

    filter {"configurations:Debug"}
        links
        {
            "%{librarylist.OpenAL_D}",
            "%{librarylist.SoundFile_D}"
        }

    filter {"configurations:Release"}
        links
        {
            "%{librarylist.OpenAL_R}",
            "%{librarylist.SoundFile_R}"
        }