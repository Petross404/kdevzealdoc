cmake_minimum_required(VERSION 3.20)

project(kdevzealdoc VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})


include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddTests)
include(ECMQtDeclareLoggingCategory)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(FeatureSummary)

set(QT_MIN_VERSION "5.5.0")
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Widgets)
set(KF5_DEP_VERSION "5.15.0")
find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS
    I18n
    ItemModels # needed because missing in KDevPlatformConfig.cmake, remove once dep on kdevplatform >=5.2.2
    ItemViews
)
find_package(KDevPlatform 5.3 CONFIG)
set_package_properties(KDevPlatform PROPERTIES
    TYPE REQUIRED
)

add_definitions(-DTRANSLATION_DOMAIN=\"kdevzealdoc\")
include_directories(
    src/
    src/zeal/
)

set(kdevzealdoc_UI_SRCS
    src/zealdocconfigpage.ui
)

set(kdevzealdoc_SRCS
    src/kdevzealdoc.cpp
    src/zealdocprovider.cpp
    src/zealdocumentation.cpp
    src/zealdocconfigpage.cpp
    src/util.cpp

    src/zeal/registry/docset.cpp
    src/zeal/registry/cancellationtoken.cpp
    src/zeal/util/plist.cpp
    src/zeal/util/sqlitedatabase.cpp
)

ki18n_wrap_ui(kdevzealdoc_SRCS
    src/zealdocconfigpage.ui
)

kdevplatform_add_plugin(kdevzealdoc
    JSON src/kdevzealdoc.json
    SOURCES ${kdevzealdoc_SRCS} ${kdevzealdoc_UI_SRCS}
)

target_link_libraries(kdevzealdoc
    Qt5::Core
    Qt5::Widgets
    KDev::Language
    KDev::Documentation

    sqlite3 # FIXME
)

install(DIRECTORY pics/16x16 DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor)
install(DIRECTORY pics/32x32 DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor)
