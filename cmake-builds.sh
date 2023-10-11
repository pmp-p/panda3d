#!/bin/bash


# pview: /data/git/panda3d/dtool/src/dtoolbase/mutexPosixImpl.I:99: void ReMutexPosixImpl::lock(): Assertion `result == 0' failed.
# Aborted


export PYBUILD=3.11
export SDKROOT=/opt/python-wasm-sdk

export SRCDIR=$(dirname $0)
echo SRCDIR=$SRCDIR

if echo $@|grep -q host
then

    echo "building  ${SRCDIR} for host $(arch)"

    #-DHAVE_EGL=NO -DHAVE_X11=YES -DHAVE_TINYDISPLAY=1 -DHAVE_THREADS=NO -DHAVE_AUDIO=OFF

# Debug no compat with -DWANT_NATIVE_NET=NO

    ${SDKROOT}/devices/x86_64/usr/bin/cmake -S ${SRCDIR} \
     -DCMAKE_BUILD_TYPE=Debug -DHAVE_AUDIO=NO -DHAVE_THREADS=NO \
     -DHAVE_EGL=NO -DHAVE_GL=NO -DHAVE_GLX=NO -DHAVE_X11=YES -DHAVE_GLES1=NO -DHAVE_GLES2=NO \
     -DHAVE_OPENSSL=NO \
     -DHAVE_ZLIB=YES -DHAVE_PNG=NO  \
     -DHAVE_TIFF=NO -DHAVE_JPEG=NO \
     -DHAVE_OPUS=NO \
     -DHAVE_HARFBUZZ=YES -DHAVE_FREETYPE=YES \
 -DHAVE_IOS_TYPEDEFS=1 -DHAVE_TINYDISPLAY=1  \
    \
     -DHAVE_PYTHON=NO -DBUILD_SHARED_LIBS=NO \
    \
     -DCMAKE_INSTALL_PREFIX=${SDKROOT}/devices/$(arch)/usr

# -DHAVE_SDL2=1


    exit 0
fi



if echo $EMSDK|grep emsdk
then
    echo building emsdk

    EMCC_CFLAGS="-sNODERAWFS" emcmake ${SDKROOT}/devices/x86_64/usr/bin/cmake ${SRCDIR} \
     -DCMAKE_BUILD_TYPE=Release \
     -DHAVE_THREADS=NO \
     -DHAVE_EGL=NO -DHAVE_GL=NO -DHAVE_GLX=NO -DHAVE_X11=NO -DHAVE_GLES1=NO -DHAVE_GLES2=NO \
     -DHAVE_OPENSSL=NO \
     -DHAVE_ZLIB=1 -DHAVE_PNG=NO  \
     -DHAVE_TIFF=NO -DHAVE_JPEG=NO \
     -DHAVE_AUDIO=NO -DHAVE_OPUS=NO \
     -DHAVE_HARFBUZZ=1 -DHAVE_FREETYPE=1 \
     -DTHIRDPARTY_DIRECTORY=${PREFIX} \
 -DHAVE_IOS_TYPEDEFS=1 -DHAVE_TINYDISPLAY=1 \
    \
     -DHAVE_PYTHON=NO \
    \
     -DCMAKE_INSTALL_PREFIX=${PREFIX}

#      -DWANT_NATIVE_NET=NO


else
    pushd ${SDKROOT}

    . scripts/wasisdk-fetch.sh

    popd


    mkdir -p wasmbin
    cat > wasmbin/interrogate << END
    #!/bin/bash
    # cd /
    ${SDKROOT}/wasisdk/bin/wasi-run $(pwd)/bin/interrogate \$@
END

    cat > wasmbin/interrogate_module << END
    #!/bin/bash
    # cd /
    ${SDKROOT}/wasisdk/bin/wasi-run $(pwd)/bin/interrogate_module \$@
END

    cat > wasmbin/pzip << END
    #!/bin/bash
    # cd /
    ${SDKROOT}/wasisdk/bin/wasi-run $(pwd)/bin/pzip \$@
END


    chmod +x $(pwd)/wasmbin/*

    # HAVE_EGG

    echo "WASI_SDK_PREFIX=$WASI_SDK_PREFIX"

    ${SDKROOT}/devices/x86_64/usr/bin/cmake ${SRCDIR} \
     -DCMAKE_BUILD_TYPE=Release \
     -DHAVE_THREADS=NO \
     -DHAVE_EGL=NO -DHAVE_GL=NO -DHAVE_GLX=NO -DHAVE_X11=NO -DHAVE_GLES1=NO -DHAVE_GLES2=NO \
     -DHAVE_OPENSSL=NO \
     -DHAVE_ZLIB=1 -DHAVE_PNG=NO  \
     -DHAVE_TIFF=NO -DHAVE_JPEG=NO \
     -DHAVE_AUDIO=NO -DHAVE_OPUS=NO \
     -DHAVE_HARFBUZZ=NO -DHAVE_FREETYPE=NO \
     -DTHIRDPARTY_DIRECTORY=${PREFIX} \
     -DPHAVE_IOSTREAM=1 -DWANT_NATIVE_NET=NO -DHAVE_TINYDISPLAY=1 \
    \
     -DHAVE_PYTHON=NO \
    \
     -DCMAKE_SYSTEM_NAME=WASI -DWASISDK=${SDKROOT}/wasisdk \
     -DWASI_SDK_PREFIX=${WASI_SDK_PREFIX} \
     -DCMAKE_TOOLCHAIN_FILE=${WASI_SDK_PREFIX}/../share/cmake/wasi-sdk.cmake \
     -DCMAKE_INSTALL_PREFIX=${PREFIX} \
    -DHOST_PATH_PZIP=/opt/python-wasm-sdk/build/panda3d-host/bin/pzip



    #\
    # -DHAVE_PYTHON=1 \
    # -DHOST_PATH_INTERROGATE=$(pwd)/wasmbin/interrogate \
    # -DHOST_PATH_INTERROGATE_MODULE=$(pwd)/wasmbin \
    #    -DPython3_EXECUTABLE:FILEPATH=${SDKROOT}/python3-wasi \
    #    -DPython3_INCLUDE_DIR=${SDKROOT}/devices/wasisdk/usr/include/python${PYBUILD} \
    #    -DPython3_LIBRARY=${SDKROOT}/devices/wasisdk/usr/lib \
    #    -DPython3_FOUND=TRUE \
    #    -DPython3_Development_FOUND=TRUE \
    #    -DPython3_Development.Module_FOUND=TRUE \
    #    -DPython3_Development.Embed_FOUND=TRUE \
    #\

# -DHOST_PATH_PZIP=${HOST}/bin/pzip



fi

make


