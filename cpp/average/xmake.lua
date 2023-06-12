add_rules("mode.debug", "mode.release")


target("average")
    set_plat(os.host())
    if is_plat("windows") then
        set_toolchains("msvc")
    end
    if is_plat("linux") then
        set_toolchains("clang")
    end
    set_kind("binary")
    add_files("src/*.cpp")
    set_languages("c17", "cxx20")
    set_warnings("all", "error")
    set_optimize("fastest") -- none, fast, faster, fastest, smallest, aggressive
    add_vectorexts("avx2") -- "sse", "sse2", "sse3", "ssse3", "avx", "avx2"
