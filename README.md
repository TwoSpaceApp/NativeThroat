# NativeThroat - Низкозадержечная аудиобиблиотека

Простая C++ библиотека для генерации или обработки аудио с низкой задержкой на Android. Использует библиотеку Google Oboe. Предназначена для интеграции в проекты на Dart/Flutter через FFI.

## Требования для сборки

*   **Android NDK**: Необходим для компиляции C++ кода под Android. Устанавливается через Android Studio (`SDK Manager` -> `SDK Tools` -> `NDK (Side by side)`).
*   **CMake**: Также устанавливается через Android Studio (`SDK Manager` -> `SDK Tools` -> `CMake`).
*   **Git**: Для скачивания зависимости Oboe.

## Как собрать библиотеку (`.so` файл)

1.  **Клонируйте репозиторий и зависимость:**
    ```sh
    git clone <адрес-вашего-репозитория>
    cd NativeThroat
    git submodule update --init --recursive
    ```

2.  **Создайте директорию для сборки:**
    ```sh
    mkdir build
    cd build
    ```

3.  **Настройте проект с помощью CMake:**
    Запустите команду ниже, **заменив пути к NDK и CMake на ваши**. Пути можно найти в Android Studio в SDK Manager.

    ```sh
    # Важно: команда выполняется из папки 'build'
    # Это одна длинная команда, скопируйте ее целиком
    
    cmake -G "Ninja" -DCMAKE_MAKE_PROGRAM="C:\path\to\your\android\sdk\cmake\<version>\bin\ninja.exe" -DCMAKE_TOOLCHAIN_FILE="C:\path\to\your\android\sdk\ndk\<version>\build\cmake\android.toolchain.cmake" -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-21 ..
    ```
    *Пример для Windows:*
    ```sh
    cmake -G "Ninja" -DCMAKE_MAKE_PROGRAM="C:\Users\YourUser\AppData\Local\Android\Sdk\cmake\4.1.2\bin\ninja.exe" -DCMAKE_TOOLCHAIN_FILE="C:\Users\YourUser\AppData\Local\Android\Sdk\ndk\29.0.14206865\build\cmake\android.toolchain.cmake" -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-21 ..
    ```

4.  **Скомпилируйте код:**
    ```sh
    cmake --build .
    ```

    После успешной сборки в папке `build` появится файл `libnative_audio.so`.

## Как использовать в Flutter

1.  **Скопируйте библиотеку в проект:**
    *   Создайте структуру папок в вашем Flutter-проекте: `android/app/src/main/jniLibs/arm64-v8a`.
    *   Поместите файл `libnative_audio.so` в эту папку.

2.  **Напишите Dart-код для вызова функций:**
    Используйте `dart:ffi` для загрузки библиотеки и вызова ее функций.

    ```dart
    import 'dart:ffi';
    import 'dart:io' show Platform;

    // Загружаем библиотеку
    final DynamicLibrary nativeAudioLib = Platform.isAndroid
        ? DynamicLibrary.open('libnative_audio.so')
        : DynamicLibrary.process();

    // Описываем и находим C-функции
    final createAudioEngine = nativeAudioLib
        .lookup<NativeFunction<Pointer<Void> Function()>>('create_audio_engine')
        .asFunction<Pointer<Void> Function()>();

    final startAudio = nativeAudioLib
        .lookup<NativeFunction<Void Function(Pointer<Void>)>>('start_audio')
        .asFunction<void Function(Pointer<Void>)>();

    final stopAudio = nativeAudioLib
        .lookup<NativeFunction<Void Function(Pointer<Void>)>>('stop_audio')
        .asFunction<void Function(Pointer<Void>)>();

    final deleteAudioEngine = nativeAudioLib
        .lookup<NativeFunction<Void Function(Pointer<Void>)>>('delete_audio_engine')
        .asFunction<void Function(Pointer<Void>)>();

    // Пример использования
    void main() {
      final engine = createAudioEngine();
      startAudio(engine);
      // ... какая-то работа ...
      stopAudio(engine);
      deleteAudioEngine(engine);
    }
    ```
