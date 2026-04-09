# Custom Patch Testing – Zurück zum Upstream

## Aktueller Zustand
Die SherpaOnnxTts-App ist so konfiguriert, dass sie **gepatchte** native
Libs von `McMarius11/sherpa-onnx` (Release `custom_patch`) verwendet.
Betroffen ist nur `arm64-v8a`.

## Zurück zum offiziellen k2-fsa Upstream

### 1. `gradle.properties` ändern (eine Zeile)

```diff
- sherpa_onnx_native_source=custom
+ sherpa_onnx_native_source=official
```

Datei: `android/SherpaOnnxTts/gradle.properties`

### 2. Heruntergeladene Custom-Libs löschen

```bash
rm -f android/SherpaOnnxTts/app/src/main/jniLibs/arm64-v8a/*.so
```

### 3. Offizielle Libs bereitstellen

Die offiziellen Libs manuell herunterladen und extrahieren:

```bash
curl -SL -o /tmp/sherpa-android.tar.bz2 \
  https://github.com/k2-fsa/sherpa-onnx/releases/download/v1.12.36/sherpa-onnx-v1.12.36-android.tar.bz2
tar xf /tmp/sherpa-android.tar.bz2 -C android/SherpaOnnxTts/app/src/main/
```

### 4. Build

```bash
cd android/SherpaOnnxTts && ./gradlew assembleDebug
```

## Geänderte Dateien (gegenüber master)

| Datei | Änderung |
|-------|----------|
| `android/SherpaOnnxTts/gradle.properties` | Toggle-Property + URLs hinzugefügt |
| `android/SherpaOnnxTts/app/build.gradle` | Gradle-Task `downloadNativeLibs` + abiFilter |
| `android/SherpaOnnxTts/app/src/main/jniLibs/arm64-v8a/.gitignore` | `*.so` ignorieren |
| `sherpa-onnx/jni/common.h` | `GetJavaVM()`, `GetJNIEnvForCurrentThread()` Deklaration |
| `sherpa-onnx/jni/jni.cc` | `JNI_OnLoad`, `GetJavaVM()`, `GetJNIEnvForCurrentThread()` |
| `sherpa-onnx/jni/offline-tts.cc` | Thread-sichere Callbacks, Signatur-Fallback |
| `sherpa-onnx/jni/offline-speaker-diarization.cc` | Gleicher Thread-Safety-Fix |
| `sherpa-onnx/jni/sherpa-onnx-symbols.lds` | `JNI_OnLoad` exportiert |
| `fix-jni-callback-thread-safety.patch` | Patch-Datei mit allen JNI-Fixes |
