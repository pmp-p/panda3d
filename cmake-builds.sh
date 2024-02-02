#!/bin/bash


export PYBUILD=${PYBUILD:-3.12}
export SDKROOT=${SDKROOT:-/opt/python-wasm-sdk}

export SRCDIR=$(realpath $(dirname $0))

echo SRCDIR=$SRCDIR

if ${CI:-false}
then
    pushd ${SRCDIR}
    wget -O- https://patch-diff.githubusercontent.com/raw/pmp-p/panda3d/pull/10.diff | patch -p1
    popd
fi


if echo $@|grep -q host
then

    echo "building  ${SRCDIR} for host $(arch)"

    CC=clang CXX=clang++ ${SDKROOT}/devices/x86_64/usr/bin/cmake -S ${SRCDIR} \
     -DCMAKE_BUILD_TYPE=Debug -DHAVE_AUDIO=YES -DHAVE_THREADS=NO \
     -DHAVE_EGL=NO -DHAVE_GL=YES -DHAVE_GLX=YES -DHAVE_X11=YES -DHAVE_GLES1=NO -DHAVE_GLES2=YES \
     -DHAVE_OPENSSL=NO \
 -DHAVE_NET=NO -DWANT_NATIVE_NET=NO -DDO_PSTATS=NO \
     -DHAVE_ZLIB=YES -DHAVE_PNG=YES  \
     -DHAVE_TIFF=NO -DHAVE_JPEG=YES \
     -DHAVE_OPUS=NO \
     -DHAVE_HARFBUZZ=YES -DHAVE_FREETYPE=YES \
 -DHAVE_IOS_TYPEDEFS=1 -DHAVE_TINYDISPLAY=1 -DBUILD_SHARED_LIBS=YES \
    \
     -DCMAKE_INSTALL_PREFIX=${SDKROOT}/devices/$(arch)/usr \
     -DHAVE_PYTHON=1 -DPython_ROOT_DIR=${SDKROOT}/devices/$(arch)/usr \
     -DWANT_PYTHON_VERSION=${PYBUILD}


# https://cmake.org/cmake/help/v3.13/module/FindPython.html
#     -DPython_VERSION=3.12 -DPython_VERSION_MAJOR=3 -DPython_VERSION_MINOR=12 -DPython_VERSION_PATCH=1 \
#        -DPython3_EXECUTABLE:FILEPATH=${SDKROOT}/devices/$(arch)/usr/bin/python${PYBUILD} \
#        -DPython3_INCLUDE_DIR=${SDKROOT}/devices/$(arch)/usr/include/python${PYBUILD} \
#        -DPython_LIBRARIES=${SDKROOT}/devices/$(arch)/usr/lib \
#        -DPython3_FOUND=TRUE \
#        -DPython3_Development_FOUND=TRUE \
#        -DPython3_Development.Module_FOUND=TRUE \
#        -DPython3_Development.Embed_FOUND=TRUE \
#    \
# -DHAVE_SDL2=1


    exit 0
fi



if echo $EMSDK|grep -q emsdk
then
    echo building with emsdk

    EMCC_CFLAGS="-sNODERAWFS" emcmake ${SDKROOT}/devices/x86_64/usr/bin/cmake ${SRCDIR} \
     -DCMAKE_BUILD_TYPE=Release \
     -DHAVE_THREADS=NO \
     -DHAVE_EGL=NO -DHAVE_GL=NO -DHAVE_GLX=NO -DHAVE_X11=NO -DHAVE_GLES1=NO -DHAVE_GLES2=NO \
     -DHAVE_OPENSSL=NO \
 -DHAVE_NET=NO -DWANT_NATIVE_NET=NO -DDO_PSTATS=NO \
     -DHAVE_ZLIB=1 -DHAVE_PNG=NO  \
     -DHAVE_TIFF=NO -DHAVE_JPEG=NO \
     -DHAVE_AUDIO=NO -DHAVE_OPUS=NO \
     -DHAVE_HARFBUZZ=1 -DHAVE_FREETYPE=1 \
 -DHAVE_IOS_TYPEDEFS=1 -DHAVE_TINYDISPLAY=1 \
    \
     -DHAVE_PYTHON=NO \
    \
     -DCMAKE_INSTALL_PREFIX=${PREFIX}

#    -DTHIRDPARTY_DIRECTORY=${PREFIX} \

else
    pushd ${SDKROOT:-/opt/python-wasm-sdk}

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

    echo "

==============================================================
    WASI_SDK_PREFIX=$WASI_SDK_PREFIX
    WASI_SYSROOT=$WASI_SYSROOT
    CMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE

==============================================================


"

    ${SDKROOT}/devices/x86_64/usr/bin/cmake ${SRCDIR} \
     -DCMAKE_BUILD_TYPE=Release \
     -DHAVE_THREADS=NO \
     -DHAVE_EGL=NO -DHAVE_GL=NO -DHAVE_GLX=NO -DHAVE_X11=NO -DHAVE_GLES1=NO -DHAVE_GLES2=NO \
     -DHAVE_OPENSSL=NO \
 -DHAVE_AUDIO=1 -DHAVE_PYTHON=0\
 -DHAVE_OPUS=NO \
     -DHAVE_HARFBUZZ=NO -DHAVE_FREETYPE=NO \
     -DPHAVE_IOSTREAM=1 -DHAVE_TINYDISPLAY=1 -DHAVE_TIFF=NO  \
    \
    -DCMAKE_SYSTEM_NAME=WASI \
    -DWASISDK=${SDKROOT}/wasisdk \
    -DWASI_SDK_PREFIX=${WASI_SDK_PREFIX} \
    \
     -DHAVE_NET=NO -DWANT_NATIVE_NET=NO -DDO_PSTATS=NO \
     -DHAVE_PYTHON=NO -DZLIB_ROOT=${WASI_SYSROOT} \
    \
     -DHOST_PATH_PZIP=/opt/python-wasm-sdk/build/panda3d-host/bin/pzip \
     -DCMAKE_INSTALL_PREFIX=${PREFIX} \


#     -DHAVE_PYTHON=1 -DPython_ROOT_DIR=${SDKROOT}/devices/wasisdk/usr \
#     -DWANT_PYTHON_VERSION=${PYBUILD}


#      -DHAVE_ZLIB=0 -DHAVE_PNG=NO -DHAVE_JPEG=NO \
# -DCMAKE_SYSTEM_NAME=WASI
#     -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \

# not enough
#  -DCMAKE_TOOLCHAIN_FILE=/opt/python-wasm-sdk/wasisdk/upstream/share/cmake/wasi-sdk.cmake


#     -DTHIRDPARTY_DIRECTORY=${PREFIX} \


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


if $CI
then
    ${SDKROOT}/wasisdk/bin/wasi-run ./bin/pview.wasi -c -L -s $(pwd)/out.bmp
    if md5sum out.bmp | grep -q "a0345fc5e8b77b9715df69e271e22c64"
    then
        exit 0
    else
        echo "error screen output checksum does not match"
        exit 5
    fi
else
    echo not running test
    exit 0
fi

