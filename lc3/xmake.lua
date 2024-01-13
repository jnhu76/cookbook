set_project("lc3")
add_rules("mode.debug", "mode.release")

-- enable debug symbols
if is_mode("debug") then
    -- 添加DEBUG编译宏
    add_defines("DEBUG")
    -- 启用调试符号
    set_symbols("debug")
    -- 禁用优化
    set_optimize("none")

    set_warnings("all", "error")
end

if is_mode("release") then
    set_optimize("fastest")

    -- 隐藏符号
    set_symbols("hidden")
    
    -- strip所有符号
    set_strip("all")

    -- 忽略帧指针
    add_cxflags("-fomit-frame-pointer")
    add_mxflags("-fomit-frame-pointer")

    -- 添加扩展指令集
    -- add_vectorexts("sse2", "sse3", "ssse3", "mmx")

end

target("lc3")
    set_kind("binary")
    set_languages("c11", "cxx20")
    add_includedirs("src/lc3")
    add_files("src/*.cpp")
