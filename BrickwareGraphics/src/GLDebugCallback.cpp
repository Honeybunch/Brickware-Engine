#ifdef GL_SUPPORT

#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/GLDebugCallback.hpp"
#include "BrickwareUtils/Logger.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	void APIENTRY GLPrintErrorCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar *msg, const void *data)
#else
	void APIENTRY GLPrintErrorCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar *msg, void *data)
#endif
	{
		const char* _source;
		const char* _type;
		const char* _severity;

		switch (source) {
		case GL_DEBUG_SOURCE_API:
			_source = "API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			_source = "WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			_source = "SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			_source = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			_source = "APPLICATION";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			_source = "UNKNOWN";
			break;

		default:
			_source = "UNKNOWN";
			break;
		}

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			_type = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			_type = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			_type = "UDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			_type = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			_type = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			_type = "OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			_type = "MARKER";
			break;

		default:
			_type = "UNKNOWN";
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			_severity = "HIGH";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			_severity = "MEDIUM";
			break;

		case GL_DEBUG_SEVERITY_LOW:
			_severity = "LOW";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			_severity = "NOTIFICATION";
			break;

		default:
			_severity = "UNKNOWN";
			break;
		}

		if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
		{
			std::string error = std::to_string(id) + ": " + _type + " of " + _severity + " severity, raised from " + _source + ": " + msg + "\n";

			//Live log the error to the console if we're in debug mode
#ifdef BRICKWARE_DEBUG
			printf(error.c_str());
#endif
			//Log the error to a file
			Brickware::Utility::Logger::Log(error);
		}

	}

#ifdef __cplusplus
}
#endif

#endif
