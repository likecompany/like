include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG master
)

CPMAddPackage(
        NAME restinio
        GITHUB_REPOSITORY likecompany/restinio
        GIT_TAG master
)

CPMAddPackage(
        NAME glaze
        GITHUB_REPOSITORY stephenberry/glaze
        GIT_TAG main
)

CPMAddPackage(
        NAME engine
        GITHUB_REPOSITORY likecompany/engine
        GIT_TAG master
)
