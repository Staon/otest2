#!/bin/bash

src_dir="$1"
tg_dir="$2"

# -- replace all binaries by empty file
mkdir -p "${tg_dir}/bin"
fake_bin="${tg_dir}/bin/fake"
echo "#!/bin/bash" > "${fake_bin}"
echo "exit 1" >> "${fake_bin}"
chmod +x "${fake_bin}"
ls -1 "${src_dir}/bin/" | while read bin
do
  ln -sf "fake" "${tg_dir}/bin/${bin}"
done

# -- copy the include files
cp -R "${src_dir}/include" "${tg_dir}"

# -- prepare libraries
mkdir -p "${tg_dir}/lib"
cp -R "${src_dir}/lib/clang" "${src_dir}/lib/cmake" "${src_dir}/lib/python2.7" "${tg_dir}/lib"

# -- create empty libraries for non-important libraries
ls -1 "${src_dir}"/lib/*.a | while read lib
do
  raw_lib=$(basename "${lib}")
  ar rc "${tg_dir}/lib/${raw_lib}" empty.o
done
ls -1 "${src_dir}"/lib/*.so "${src_dir}"/lib/*.so.* | while read lib
do
  raw_lib=$(basename "${lib}")
  touch "${tg_dir}/lib/${raw_lib}"
done

# -- copy important libraries
important_libs="
    libclangTooling.a
    libclangASTMatchers.a
    libclangFrontend.a
    libclangDriver.a
    libclangParse.a
    libLLVMMCParser.a
    libclangSerialization.a
    libclangSema.a
    libclangEdit.a
    libclangAnalysis.a
    libLLVMBitReader.a
    libLLVMProfileData.a
    libclangAST.a
    libclangLex.a
    libclangBasic.a
    libLLVMCore.a
    libLLVMMC.a
    libLLVMBinaryFormat.a
    libLLVMDebugInfoCodeView.a
    libLLVMDebugInfoMSF.a
    libLLVMOption.a
    libLLVMSupport.a"
for lib in ${important_libs}
do
  cp "${src_dir}/lib/${lib}" "${tg_dir}/lib"
done

