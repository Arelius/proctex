Build {
   Units = function()
      local glsl = Program {
         Name = "glsltex",
         Sources = { "glsl/main.cpp", "glsl/shader.cpp" },
         Libs = { "../external/SDL2-2.0.4/lib/x64/SDL2.lib", "../external/SDL2-2.0.4/lib/x64/SDL2main.lib", "opengl32.lib" },
      }
      Default(glsl)
   end,

   Env = {
      CCOPTS = "/0d",
      CPPPATH = "../external/SDL2-2.0.4/include",
      PROGOPTS = "/SUBSYSTEM:WINDOWS /DEBUG",
      GENERATE_PDB = "1",
   },

   Configs = {
      Config {
         Name = "win64-msvc",
         DefaultOnHost = "windows",
         Tools = { {"msvc"; TargetArch = "x64"} },
      },
   },
}