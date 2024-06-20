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

target("main")
    set_kind("binary")
    add_files("src/main.cpp")
    add_includedirs("include")
    add_deps("gl_homework")
    add_packages("freeglut", "glew", "fmt", "opencv")

after_build(function (target)
    os.cp("shader/", target:targetdir())
    os.cp("texture/", target:targetdir())
end)




