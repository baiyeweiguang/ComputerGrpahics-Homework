add_rules("mode.debug", "mode.release")

set_languages("cxx17")

add_requires("glew")
add_requires("freeglut")
add_requires("glm")
add_requires("fmt")
add_requires("opencv")

add_cxxflags("-Wno-delete-incomplete")
-- set_targetdir("build")

target("gl_homework")
    set_kind("shared")
    add_files("src/impl/*.cpp")
    add_includedirs("include")
    add_packages("glew", "freeglut", "glm", "fmt", "opencv")
    set_installdir("install")
    after_install(function (target)
        -- copy include
        os.cp("include/", target:installdir())
    end)


target("example")
    set_kind("binary")
    add_files("example/example.cpp")
    add_deps("gl_homework")
    add_includedirs("include")
    add_packages("freeglut", "glew", "fmt")
    after_build(function (target)
        os.cp("shader/", target:targetdir())
        os.cp("texture/", target:targetdir())
    end)




