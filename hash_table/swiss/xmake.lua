add_rules("mode.debug", "mode.release")
add_requires("abseil master")
-- add_requires("glib")


-- target("swiss")
--     set_kind("binary")
--     add_files("src/*.cpp")
--     set_languages("c99", "c++20")
--     set_optimize("fastest")
--     -- set_warnings("all", "error")
--     add_packages("abseil")


target("bench")
    set_kind("binary")
    add_files("src/bench.cpp") -- , "src/swisstable.cpp")
    set_languages("c99", "c++20")
    set_optimize("fastest")
    -- set_warnings("all", "error")
    add_packages("abseil")
    -- add_packages("glib")