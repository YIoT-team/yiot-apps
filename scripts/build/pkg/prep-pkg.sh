#!/bin/bash
set -o errtrace

SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"

set -e
######################################################################
print_usage() {
    echo "====================="
    echo "=== $@"
    echo "====================="
}

######################################################################
PKG_QUICK="0"
while [ -n "$1" ]
 do
   case "$1" in
     -b) BIN_DIR="$2"
         shift
         ;;
     -t) PKG_TARGET_TYPE="$2"
         shift
         ;;
     -p) PKG_TYPE="$2"
         shift
         ;;         
     -n) PKG_NAME="$2"
         shift
         ;;                  
     -v) PKG_VERSION="$2"
         shift
         ;;                  
     -q) PKG_QUICK="1"
         ;;                           
     *) print_usage
         ;;
   esac
   shift
done
echo "TARGET_TYPE = $PKG_TARGET_TYPE"
######################################################################
MAJOR_VER=$(echo "$PKG_VERSION"| cut -d'.' -f1)
MINOR_VER=$(echo "$PKG_VERSION"| cut -d'.' -f2)
SUB_VER=$(echo "$PKG_VERSION"| cut -d'.' -f3)
BUILD_VER=$(echo "$PKG_VERSION"| cut -d'.' -f4)

export MAJOR_VER MINOR_VER SUB_VER BUILD_VER

PKG_SRC_NAME="${PKG_NAME}-${MAJOR_VER}.${MINOR_VER}.${SUB_VER}"
BUILD_DIR="${SCRIPT_DIR}/build/${PKG_TARGET_TYPE}/${PKG_TYPE}/${PKG_NAME}"

############################################################################################
print_message() {
    echo "====================="
    echo "=== $@"
    echo "====================="
}

############################################################################################
echo_info() {
echo "--- Package version"
echo "PKG_TARGET_TYPE:      ${PKG_TARGET_TYPE}"
echo "PKG_TYPE:             ${PKG_TYPE}"
echo "PKG_NAME:             ${PKG_NAME}"
echo "PKG_VERSION:          ${PKG_VERSION}"
echo "BUILD_DIR:            ${BUILD_DIR}"
echo "PKG_SRC_NAME:         ${PKG_SRC_NAME}"
echo "--- "
}

############################################################################################
create_dirs() {
 echo "Remove old dirs and create new"
 rm -rf ${SCRIPT_DIR}/build || true
 mkdir -p ${BUILD_DIR}/sdeb
 mkdir -p ${BUILD_DIR}/${PKG_SRC_NAME}/dist
}

############################################################################################
prep_sources() {
   pushd ${BUILD_DIR}
      cp -rf ${SCRIPT_DIR}/${PKG_TARGET_TYPE}/${PKG_TYPE}/${PKG_NAME}/* ${BUILD_DIR}/sdeb
      cp -rf ${BIN_DIR}/* ${BUILD_DIR}/${PKG_SRC_NAME}/dist
      tar czf ${PKG_NAME}_${MAJOR_VER}.${MINOR_VER}.${SUB_VER}.orig.tar.gz ${PKG_SRC_NAME}
      rm -rf ${PKG_SRC_NAME}
   popd
}

############################################################################################
create_sdeb() {
 pushd ${BUILD_DIR}
  echo "------------- Create DEB from template ------------------------------"         
  pushd sdeb
     tar xJf ${PKG_NAME}.debian.tar.xz
     j2 -f env -o ${BUILD_DIR}/${PKG_NAME}_${MAJOR_VER}.${MINOR_VER}.${SUB_VER}-${BUILD_VER}.dsc ${PKG_NAME}.dsc.tmpl
     j2 -f env -o debian/changelog debian/changelog
     tar cJf ${BUILD_DIR}/${PKG_NAME}_${MAJOR_VER}.${MINOR_VER}.${SUB_VER}-${BUILD_VER}.debian.tar.xz debian
     rm -rf debian
  popd
  rm -rf sdeb
  echo "------------- Create checksum for DEB  ------------------------------"            
  file_dsc=${PKG_NAME}_${MAJOR_VER}.${MINOR_VER}.${SUB_VER}-${BUILD_VER}.dsc
  file_deb_name=${PKG_NAME}_${MAJOR_VER}.${MINOR_VER}.${SUB_VER}-${BUILD_VER}.debian.tar.xz
  file_src_name=${PKG_NAME}_${MAJOR_VER}.${MINOR_VER}.${SUB_VER}.orig.tar.gz

  file_deb_size=$(stat --printf="%s" "$file_deb_name")
  file_src_size=$(stat --printf="%s" "$file_src_name")

  file_sha1_src_hash=$(sha1sum "$file_src_name" | cut -f1 -d ' ')
  file_sha256_src_hash=$(sha256sum "$file_src_name" | cut -f1 -d ' ')
  file_md5_src_hash=$(md5sum "$file_src_name" | cut -f1 -d ' ')

  file_sha1_deb_hash=$(sha1sum "$file_deb_name" | cut -f1 -d ' ')
  file_sha256_deb_hash=$(sha256sum "$file_deb_name" | cut -f1 -d ' ')
  file_md5_deb_hash=$(md5sum "$file_deb_name" | cut -f1 -d ' ')

  echo "Checksums-Sha1:" >> $file_dsc
  echo " $file_sha1_src_hash $file_src_size $file_src_name"  >> $file_dsc
  echo " $file_sha1_deb_hash $file_deb_size $file_deb_name"  >> $file_dsc

  echo "Checksums-Sha256:" >> $file_dsc
  echo " $file_sha256_src_hash $file_src_size $file_src_name"  >> $file_dsc
  echo " $file_sha256_deb_hash $file_deb_size $file_deb_name"  >> $file_dsc

  echo "Files:" >> $file_dsc
  echo " $file_md5_src_hash $file_src_size $file_src_name"  >> $file_dsc
  echo " $file_md5_deb_hash $file_deb_size $file_deb_name"  >> $file_dsc
 popd
}

############################################################################################
build_deb() {
 pushd ${BUILD_PATH}
   export OS=raspbian
   export ARCH=armhf
   print_message "Initialization pbuilder root"   
   sudo pbuilder create --mirror http://mirror.truenetwork.ru/raspbian/raspbian/ --debug --distribution buster --debootstrapopts "--keyring=/usr/share/keyrings/raspbian-archive-keyring.gpg"

   print_message "Update pbuilder root"   
   sudo pbuilder --update --debug --mirror http://mirror.truenetwork.ru/raspbian/raspbian/ 
   
   print_message "Building DEB package"      
   sudo pbuilder --debug --mirror http://mirror.truenetwork.ru/raspbian/raspbian/ --build $(ls *.dsc)
   
   print_message "Copy results fro mcontainer"
   mkdir -p ${BUILD_PATH}/result
   cp -f /var/cache/pbuilder/result/*.deb ${BUILD_PATH}/result
 popd
}

############################################################################################

############################################################################################
build_quick_deb() {
    pushd ${BUILD_DIR}
	mkdir -p build
	tar xJf *.tar.xz --directory build/
	tar xzf *.tar.gz --strip 1 --directory build/
	pushd ${BUILD_DIR}/build
	    dpkg-buildpackage -rfakeroot -b -uc -us
	popd
    popd

}
############################################################################################

create_dirs
echo_info
prep_sources

if [ "${PKG_TYPE}" == "deb" ]; then
    create_sdeb
    if [ "${PKG_QUICK}" == "1" ]; then
        build_quick_deb
    else
        build_deb
    fi

elif [ "${PKG_TYPE}" == "rpm" ]; then
    echo ""

else
    echo "ERROR: Package type unknown"
    exit 127
fi

echo "------ END PREPARING SRPMS"
exit 0
