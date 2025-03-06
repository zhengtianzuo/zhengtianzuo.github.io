if is_mode("release") then
    set_symbols("hidden")
    set_optimize("fastest")
    set_strip("all")
else
    set_symbols("debug")
    set_optimize("none")
end

target("test")
    set_kind("binary")
    add_files("test.cpp")
	add_includedirs("include")
    add_linkdirs("lib")
