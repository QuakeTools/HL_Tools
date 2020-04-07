target_sources(HLMV
	PRIVATE
		BindableResource.hpp
		DeviceBuffer.hpp
		Enums.hpp
		Exceptions.hpp
		FrameBuffer.hpp
		GraphicsAPIHelpers.hpp
		GraphicsDevice.hpp
		Pipeline.cpp
		Pipeline.hpp
		ResourceFactory.hpp
		ResourceLayout.hpp
		ResourceSet.hpp
		Shader.hpp
		Texture.hpp)

add_subdirectory(opengl)
