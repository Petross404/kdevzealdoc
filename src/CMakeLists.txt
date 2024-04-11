set(kdevzealdoc_SRCS
    kdevzealdoc.cpp
)

ecm_qt_declare_logging_category(kdevzealdoc_SRCS
    HEADER debug.h
    IDENTIFIER PLUGIN_KDEVZEALDOC
    CATEGORY_NAME "kdevelop.plugins.kdevzealdoc"
    DESCRIPTION "KDevelop plugin: kdevzealdoc"
    EXPORT KDEVZEALDOC
)

kdevplatform_add_plugin(kdevzealdoc
    SOURCES ${kdevzealdoc_SRCS}
)

target_link_libraries(kdevzealdoc
    KDev::Interfaces
)
