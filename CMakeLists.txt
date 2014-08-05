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
	
