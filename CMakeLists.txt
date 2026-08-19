cmake_minimum_required(VERSION 3.16)

# ------------------------------------------------------------------------------
# Project Configuration
# ------------------------------------------------------------------------------
project(NexusBankCore
    VERSION 1.0.0
    DESCRIPTION "Enterprise Console Banking & ATM System"
    LANGUAGES CXX
)

# Set C++ Standard globally (C++17 is required for std::filesystem)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------------------
# Output Directories (Standardize Output Paths)
# ------------------------------------------------------------------------------
# توجيه الملفات التنفيذية الناتجة إلى مجلد bin لضمان بيئة عمل نظيفة
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ------------------------------------------------------------------------------
# Source Files Discovery
# ------------------------------------------------------------------------------
# تجميع كافة ملفات التنفيذ .cpp داخل مجلد src بشكل ديناميكي
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# إنشاء الهدف التنفيذي للمشروع
add_executable(BankSystem ${SOURCES})

# ------------------------------------------------------------------------------
# Target Include Directories (Modern Target-Based Architecture)
# ------------------------------------------------------------------------------
# تضمين جميع المجلدات الفرعية لضمان عمل كافة أشكال الـ #include 
target_include_directories(BankSystem PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Lib
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI/Auth
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI/Clients
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI/FX
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI/Main
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UI/Transactions
)

# ------------------------------------------------------------------------------
# Cross-Platform Compiler Flags & Standards
# ------------------------------------------------------------------------------
if(MSVC)
    target_compile_options(BankSystem PRIVATE 
        /W4          # أقصى درجات التحقق من تحذيرات الكود
        /utf-8       # ضبط ترميز النصوص لدعم العربية والرموز
        /EHsc        # تفعيل معالجة الاستثناءات القياسية
    )
else()
    target_compile_options(BankSystem PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic
    )
endif()