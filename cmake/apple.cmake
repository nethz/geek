option(build_ios "build for ios" OFF)

if(build_ios)
	find_package(iPhoneSDK)
    macro_log_feature(iPhoneSDK_FOUND "iPhone SDK" "iPhone SDK" "http://developer.apple.com/iphone" FALSE "" "")
	
	include(CMakeForceCompiler)
	CMAKE_FORCE_C_COMPILER(g++-4.2 GNU)
	CMAKE_FORCE_CXX_COMPILER(g++-4.2 GNU)
	
	set(CMAKE_OSX_SYSROOT iphoneos)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "")
	set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit")
	set(XCODE_ATTRIBUTE_SDKROOT iphoneos)
	set(OGRE_BUILD_RENDERSYSTEM_GLES TRUE CACHE BOOL "Forcing OpenGL ES RenderSystem for iPhone" FORCE)
	set(OGRE_STATIC TRUE CACHE BOOL "Forcing static build for iPhone" FORCE)
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.\${PRODUCT_NAME:rfc1034identifier}")
	set(OGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE TRUE CACHE BOOL "Forcing viewport orientation support for iPhone" FORCE)

	#set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
	# armv7 only
	set(CMAKE_OSX_ARCHITECTURES $(ARCHS_UNIVERSAL_IPHONE_OS))

	remove_definitions(-msse)
else()
	find_package(Carbon)
    macro_log_feature(Carbon_FOUND "Carbon" "Carbon" "http://developer.apple.com/mac" TRUE "" "")

    find_package(Cocoa)
    macro_log_feature(Cocoa_FOUND "Cocoa" "Cocoa" "http://developer.apple.com/mac" TRUE "" "")

    find_package(IOKit)
    macro_log_feature(IOKit_FOUND "IOKit" "IOKit HID framework needed by the samples" "http://developer.apple.com/mac" FALSE "" "")
endif(build_ios)