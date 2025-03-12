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
	set_languages("c++17")
    add_files("test.cpp")
	add_includedirs("include")
    add_linkdirs("lib")
if is_plat("windows") then
	add_links("benchmark", "benchmark_main", "Shlwapi")
end
if is_plat("linux") then
	add_links("benchmark", "benchmark_main", "tbb", "pthread")
end
    add_defines("BENCHMARK_STATIC_DEFINE")
	