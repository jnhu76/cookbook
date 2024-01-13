add_rules("mode.debug", "mode.release")

target("smartpointer")
    set_kind("binary")
    add_files("src/*.cpp")
    set_languages("cxx20")
    set_optimize("fastest")
