file(GLOB_RECURSE ENGINE_SOURCE_FILES Private/*.cpp)
configure_file(Public/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Public/Engine/version.h)

add_library(engine SHARED ${ENGINE_SOURCE_FILES})
target_link_libraries(
    engine PRIVATE
    glfw
    volk::volk volk::volk_headers
    GPUOpen::VulkanMemoryAllocator
    imgui::imgui
    fastgltf::fastgltf
)
target_include_directories(
    engine PRIVATE 
    ${Stb_INCLUDE_DIR}
)
target_link_libraries(
    engine PUBLIC
    nlohmann_json::nlohmann_json
    glm::glm
)
