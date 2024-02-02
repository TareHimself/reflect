
function(Fetch REPOSITORY BRANCH DESTINATION)
  if(NOT EXISTS ${DESTINATION})
    execute_process(
      COMMAND git clone --depth 1 --branch ${BRANCH} ${REPOSITORY} ${DESTINATION}
    )
  endif()
endfunction()

function(GetBuildExt B_EXT)
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(${B_EXT} "_debug" PARENT_SCOPE)
  else()
    set(${B_EXT} "" PARENT_SCOPE)
  endif()
endfunction()

function(BuildOnly CLONED_DIR BUILD_DEST LOCAL_BUILD_TYPE BUILD_FN)
  set(BUILD_DIR ${CLONED_DIR}/build/${LOCAL_BUILD_TYPE})
  if("${BUILD_FN}" STREQUAL "")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=${LOCAL_BUILD_TYPE} -S ${CLONED_DIR} -B ${BUILD_DIR}
    )
  else()
    cmake_language(CALL ${BUILD_FN} ${LOCAL_BUILD_TYPE} ${CLONED_DIR} ${BUILD_DIR})
  endif()

  execute_process(
    COMMAND ${CMAKE_COMMAND} --build ${BUILD_DIR} --config ${LOCAL_BUILD_TYPE}
  )

  execute_process(
    COMMAND ${CMAKE_COMMAND} --install ${BUILD_DIR} --prefix ${BUILD_DEST} --config ${LOCAL_BUILD_TYPE}
  )
endfunction()

function(FetchAndBuild REPOSITORY BRANCH BUILD_DEST TEMP_DEST PRE_BUILD_FN BUILD_FN)
  if(NOT EXISTS ${BUILD_DEST})
    set(CLONED_DIR ${TEMP_DEST})
    
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      set(LOCAL_BUILD_TYPE "Debug")
    else()
      set(LOCAL_BUILD_TYPE "Release")
    endif()

    Fetch(${REPOSITORY} ${BRANCH} ${CLONED_DIR})

    if(NOT "${PRE_BUILD_FN}" STREQUAL "")
      cmake_language(CALL ${PRE_BUILD_FN} ${CLONED_DIR})
    endif()

    BuildOnly(${CLONED_DIR} ${BUILD_DEST} "Release" "${BUILD_FN}")
    BuildOnly(${CLONED_DIR} ${BUILD_DEST} "Debug" "${BUILD_FN}")
  endif()
endfunction()

# Fetches and builds a dependency
function(BuildThirdPartyDep FOLDER_NAME REPOSITORY VERSION RESULT PRE_BUILD_FN BUILD_ARGS)
  set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty)
  set(RESULT_DIR ${THIRD_PARTY_DIR}/${FOLDER_NAME}_${VERSION})
  set(CLONE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${FOLDER_NAME})

  FetchAndBuild(${REPOSITORY} ${VERSION} ${RESULT_DIR} ${CLONE_DIR} "${PRE_BUILD_FN}" "${BUILD_ARGS}")
  set(${RESULT} ${RESULT_DIR} PARENT_SCOPE)
endfunction()


