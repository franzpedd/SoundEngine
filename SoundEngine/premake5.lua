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
        "%{includelist.OpenAL}"
    }

    filter { "configurations:Debug" }

        -- under windows 
        if os.host() == "windows" then
            links
            {
                "%{librarylist.OpenAL}/Debug/Debug/OpenAL32.lib"
            }

            postbuildcommands  
            {
                "{COPYFILE} %{librarylist.OpenAL}/Debug/Debug/OpenAL32.dll " .. dirpath;
            }
        end

    filter {"configurations:Release" }
        
        -- under windows 
        if os.host() == "windows" then
            links
            {
                "%{librarylist.OpenAL}/Release/Release/OpenAL32.lib"
            }

            postbuildcommands  
            {
                "{COPYFILE} %{librarylist.OpenAL}/Release/Release/OpenAL32.dll " .. dirpath;
            }
        end
