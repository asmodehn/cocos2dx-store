#Mandatory commands to use wkcmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE ( "CMake/WkCMake.cmake" )
WkCMakeDir( "CMake" )

#Project Declaration
WkProject( Cocos2dxStore CXX )

WkIncludeDir ( "Soomla" )

#extra includes because Cocos2dxStore doesnt include headers with path...
set( ${PROJECT_NAME}_INCLUDE_DIRS "Soomla/data"
	"Soomla/domain"
	"Soomla/domain/virtualCurrencies"
	"Soomla/domain/virtualGoods"
	"Soomla/jsb"
	"Soomla/PurchaseTypes"
	"Soomla/rewards"
	)
	


WkAddCFlags("-DCOCOS2D_DEBUG=1")
WkAddCXXFlags("-DCOCOS2D_DEBUG=1")

WkAddCFlags("-DSOOMLA_DEBUG=1")
WkAddCXXFlags("-DSOOMLA_DEBUG=1")

if(WIN32)
  WkAddCXXFlags("-D_USRDLL -DCOCOS2DXWIN32_EXPORTS -D_WINDOWS -DWIN32")

  if(MSVC)
    WkAddCXXFlags( "-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS" )
    WkAddCXXFlags( "-wd4251 -wd4334 -wd4820 -wd4710 -wd4514 -wd4056 -wd4996 -wd4099 -wd4819" )
    WkAddCXXFlags( "-MP -Qpar" )
    WkAddCXXFlags( "-Zi" )
  else(MSVC)#MINGW
  
  endif(MSVC)
elseif(APPLE)


else()#Linux

  WkAddCFlags("-DLINUX")
  WkAddCXXFlags("-DLINUX")
endif()


if(NOT MSVC)# all gcc
WkAddCFlags( "-std=c99")
WkAddCXXFlags("-std=c++11")
endif()

if(MINGW)
  WkAddCXXFlags("-DGLEW_STATIC")
endif()



WkDepends("../soomla-cocos2dx-core")

set( ${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS} ${${PROJECT_NAME}_SoomlaCocos2dxCore_INCLUDE_DIRS} )

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

WkLibraryBuild( Cocos2dxStore STATIC "Soomla")

#Temporary work around wkcmake bug with dependencies at multiple levels
set(Cocos2dxStore_SoomlaCocos2dxCore_LIBRARIES "SoomlaCocos2dxCore" ${SoomlaCocos2dxCore_jansson_LIBRARIES} CACHE PATH "SoomlaCocos2dxCore Libraries")
target_link_libraries(Cocos2dxStore SoomlaCocos2dxCore)
	
set( ${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS} 
	CACHE PATH "Headers Directories"
)
	
