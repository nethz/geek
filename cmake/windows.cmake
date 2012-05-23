

macro(_FIND_MSVC_REDIST VCVERS)
  message(STATUS "Looking for MSVC Redistributable Executable for MSVC
Version ${VCVERS}")
  set(SDKVERS "2.0")
  if(${VCVERS} EQUAL 8)
    set(SDKVERS "2.0")
  endif()
  if(${VCVERS} EQUAL 9)
    set(SDKVERS "3.5")
  endif()
  if(${VCVERS} EQUAL 10)
    set(SDKVERS "3.5")
  endif()
  if(MSVC${VCVERS}0)
    find_program(MSVC_REDIST NAMES
	vcredist_${CMAKE_MSVC_ARCH}/vcredist_${CMAKE_MSVC_ARCH}.exe
      PATHS
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VCExpress\\${VCVERS}.0;InstallDir]/../../SDK/v${SDKVERS}/BootStrapper/Packages/"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\${VCVERS}.0;InstallDir]/../../SDK/v${SDKVERS}/BootStrapper/Packages/"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\${VCVERS}.0;InstallDir]/../../SDK/v${SDKVERS}/BootStrapper/Packages/"
      )
    GET_FILENAME_COMPONENT(vcredist_name "${MSVC_REDIST}" NAME)
    install(PROGRAMS ${MSVC_REDIST} COMPONENT SupportFiles DESTINATION bin)
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\${vcredist_name}\\\"'")
    message(STATUS "MSVC_REDIST: ${MSVC_REDIST}")
  endif(MSVC${VCVERS}0)
endmacro()

macro(_find_d3dx9)
	 	FIND_PROGRAM(
		D3DX9
		NAMES
		D3DX9_43.dll
		PATHS
		)
	 install(PROGRAMS ${D3DX9} COMPONENT SupportFiles DESTINATION bin/release)
endmacro(_find_d3dx9)

if(enable_package)
	
	set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION bin/release)
	include (InstallRequiredSystemLibraries)
	if(MSVC80)
	_FIND_MSVC_REDIST(8)
	endif()

	if(MSVC90)
	_FIND_MSVC_REDIST(9)
	endif()


	if(MSVC100)
	_FIND_MSVC_REDIST(10)
	endif()

	_find_d3dx9()
	message($ENVIRONMENT)

	SET(CPACK_PACKAGE_VERSION_MAJOR "0") 
	SET(CPACK_PACKAGE_VERSION_MINOR "99") 
	SET(CPACK_PACKAGE_VERSION_PATCH "a") 

	include(CPack)
endif(enable_package)