if is_mode("release") then
    set_symbols("hidden")
    set_optimize("fastest")
    set_strip("all")
else
    set_symbols("debug")
    set_optimize("none")
end

target("cpplib")
    set_kind("shared")
    add_files("cpplib.cpp")
	add_defines("cpplib_exports")
	add_includedirs("../include")
    add_linkdirs("../lib")
	add_links("spdlog")
	