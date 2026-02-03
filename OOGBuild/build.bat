@echo off
setlocal

SET NDK=C:\Users\harep\AppData\Local\Android\Sdk\ndk\29.0.14206865

SET ANDROID_ABI=arm64-v8a
SET MIN_SDK_VERSION=21
SET CMAKE_BIN_DIR=C:\Users\harep\AppData\Local\Android\Sdk\cmake\4.1.2\bin
SET PATH=%CMAKE_BIN_DIR%;%PATH%

SET CMAKE_TOOLCHAIN_FILE=%NDK%\build\cmake\android.toolchain.cmake
SET BUILD_DIR=build\%ANDROID_ABI%
SET INSTALL_DIR=%CD%\install\%ANDROID_ABI%

echo "--- Building for %ANDROID_ABI% ---"
echo "--- Using Ninja from: %CMAKE_BIN_DIR% ---"

cmake -S ogg -B %BUILD_DIR%\ogg ^
    -G "Ninja" ^
    -DCMAKE_TOOLCHAIN_FILE="%CMAKE_TOOLCHAIN_FILE%" ^
    -DANDROID_ABI=%ANDROID_ABI% ^
    -DANDROID_NATIVE_API_LEVEL=%MIN_SDK_VERSION% ^
    -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
    -DBUILD_SHARED_LIBS=OFF
cmake --build %BUILD_DIR%\ogg
cmake --install %BUILD_DIR%\ogg

cmake -S opus -B %BUILD_DIR%\opus ^
    -G "Ninja" ^
    -DCMAKE_TOOLCHAIN_FILE="%CMAKE_TOOLCHAIN_FILE%" ^
    -DANDROID_ABI=%ANDROID_ABI% ^
    -DANDROID_NATIVE_API_LEVEL=%MIN_SDK_VERSION% ^
    -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
    -DBUILD_SHARED_LIBS=OFF
cmake --build %BUILD_DIR%\opus
cmake --install %BUILD_DIR%\opus

echo "--- Configuring opusfile with explicit dependency paths ---"
cmake -S opusfile -B %BUILD_DIR%\opusfile ^
    -G "Ninja" ^
    -DCMAKE_TOOLCHAIN_FILE="%CMAKE_TOOLCHAIN_FILE%" ^
    -DANDROID_ABI=%ANDROID_ABI% ^
    -DANDROID_NATIVE_API_LEVEL=%MIN_SDK_VERSION% ^
    -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
    -DCMAKE_PREFIX_PATH="%INSTALL_DIR%" ^
    -DOgg_INCLUDE_DIR="%INSTALL_DIR%/include" ^
    -DOgg_LIBRARY="%INSTALL_DIR%/lib/libogg.a" ^
    -DOpus_INCLUDE_DIR="%INSTALL_DIR%/include" ^
    -DOpus_LIBRARY="%INSTALL_DIR%/lib/libopus.a" ^
    -DBUILD_SHARED_LIBS=OFF
cmake --build %BUILD_DIR%\opusfile
cmake --install %BUILD_DIR%\opusfile

echo "--- Build finished for %ANDROID_ABI% ---"
echo "--- Libraries are in %INSTALL_DIR%\lib ---"
echo "--- Headers are in %INSTALL_DIR%\include ---"

endlocal