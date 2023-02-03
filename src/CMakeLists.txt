set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(SOURCE_FILES
	Main.cpp
	serverUI.h
	serverUI.cpp
	serverRequests.h
	serverRequests.cpp
	imguipp.h
	imguipp.cpp
	icons.h
	../bindings/imgui_impl_glfw.cpp
	../bindings/imgui_impl_opengl3.cpp
	../bindings/imgui_impl_glfw.h
	../bindings/imgui_impl_opengl3.h
	../bindings/imgui_impl_opengl3_loader.h)


message("********** CONAN_LIBS: " "${CONAN_LIBS}")


add_executable(ServerUI ${SOURCE_FILES})
target_link_libraries(ServerUI ${CONAN_LIBS})
	
