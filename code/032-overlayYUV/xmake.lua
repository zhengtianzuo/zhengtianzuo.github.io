if is_mode("release") then
    set_symbols("hidden")
    set_optimize("fastest")
    set_strip("all")

    -- define rule: c.build.optimization
rule("c.build.optimization")
    on_config(function (target)
        import("config")(target, "cc")
    end)

-- define rule: c++.build.optimization
rule("c++.build.optimization")
    on_config(function (target)
        import("config")(target, "cxx")
    end)

    -- xmake f -m release
else
    set_symbols("debug")
    set_optimize("none")

    -- xmake f -m debug
end

target("test")
    set_kind("binary")
    add_files("test.cpp")
    add_linkdirs("lib")
--    add_links("")
    add_includedirs("include")
