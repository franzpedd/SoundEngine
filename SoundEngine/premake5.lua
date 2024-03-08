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
    
    

    filter { "configurations:Debug" }

        -- under windows 
        if os.host() == "windows" then
            links
            {
                "%{librarylist.OpenAL}/Debug/Debug/OpenAL32.lib",
                "%{librarylist.SoundFile}/Debug/Debug/sndfile.lib"
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
                "%{librarylist.OpenAL}/Release/Release/OpenAL32.lib",
                "%{librarylist.SoundFile}/Release/Release/sndfile.lib"
            }

            postbuildcommands  
            {
                "{COPYFILE} %{librarylist.OpenAL}/Release/Release/OpenAL32.dll " .. dirpath;
            }
        end
