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


WkLibraryBuild( Cocos2dxStore STATIC "Soomla")

	
set( ${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS} 
	CACHE PATH "Headers Directories"
)
