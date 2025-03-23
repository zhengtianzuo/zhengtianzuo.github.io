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
if is_plat("windows") then
    add_links("crypto","Crypt32","Advapi32","Ws2_32","User32")
end
if is_plat("linux") then
	add_links("crypto","pthread","dl")
end