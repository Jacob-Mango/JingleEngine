# Install script for directory: H:/RMIT/RR/Sample/libs/assimp-5.0.1/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/Assimp")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/Debug/assimp-vc141-mtd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/Release/assimp-vc141-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/MinSizeRel/assimp-vc141-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/RelWithDebInfo/assimp-vc141-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/Debug/assimp-vc141-mtd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/Release/assimp-vc141-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/MinSizeRel/assimp-vc141-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/RelWithDebInfo/assimp-vc141-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/anim.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/aabb.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/ai_assert.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/camera.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/color4.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/color4.inl"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/../include/assimp/config.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/defs.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Defines.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/cfileio.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/light.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/material.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/material.inl"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/matrix3x3.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/matrix3x3.inl"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/matrix4x4.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/matrix4x4.inl"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/mesh.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/pbrmaterial.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/postprocess.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/quaternion.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/quaternion.inl"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/scene.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/metadata.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/texture.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/types.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/vector2.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/vector2.inl"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/vector3.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/vector3.inl"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/version.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/cimport.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/importerdesc.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Importer.hpp"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/DefaultLogger.hpp"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/ProgressHandler.hpp"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/IOStream.hpp"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/IOSystem.hpp"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Logger.hpp"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/LogStream.hpp"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/NullLogger.hpp"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/cexport.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Exporter.hpp"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/DefaultIOStream.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/DefaultIOSystem.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/ZipArchiveIOSystem.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/SceneCombiner.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/fast_atof.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/qnan.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/BaseImporter.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Hash.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/MemoryIOWrapper.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/ParsingUtils.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/StreamReader.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/StreamWriter.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/StringComparison.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/StringUtils.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/SGSpatialSort.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/GenericProperty.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/SpatialSort.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/SkeletonMeshBuilder.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/SmoothingGroups.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/SmoothingGroups.inl"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/StandardShapes.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/RemoveComments.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Subdivision.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Vertex.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/LineSplitter.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/TinyFormatter.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Profiler.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/LogAux.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Bitmap.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/XMLTools.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/IOStreamBuffer.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/CreateAnimMesh.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/irrXMLWrapper.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/BlobIOSystem.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/MathFunctions.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Macros.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Exceptional.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Compiler/pushpack1.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Compiler/poppack1.h"
    "H:/RMIT/RR/Sample/libs/assimp-5.0.1/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/Debug/assimp-vc141-mtd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "H:/RMIT/RR/Sample/libs/assimp-5.0.1/BINARIES/x64/code/RelWithDebInfo/assimp-vc141-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

