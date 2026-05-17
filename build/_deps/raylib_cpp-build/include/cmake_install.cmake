# Install script for directory: /Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/AudioDevice.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/AudioStream.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/AutomationEventList.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/BoundingBox.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Camera2D.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Camera3D.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Color.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/FileData.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/FileText.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Font.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Functions.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Gamepad.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Image.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Keyboard.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Material.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Matrix.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Mesh.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/MeshUnmanaged.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Model.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/ModelAnimation.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Mouse.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Music.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Ray.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/RayCollision.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/RaylibException.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/raylib-cpp-utils.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/raylib-cpp.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/raylib.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/raymath.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Rectangle.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/RenderTexture.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/ShaderUnmanaged.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Shader.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Sound.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Text.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Texture.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/TextureUnmanaged.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Touch.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Vector2.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Vector3.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Vector4.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/VrStereoConfig.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Wave.hpp"
    "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-src/include/Window.hpp"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/bocchichan/Repos/FiboTowerDefense/build/_deps/raylib_cpp-build/include/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
