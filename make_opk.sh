#!/bin/sh

OPK_NAME=vorton.opk
echo ${OPK_NAME}

# create default.gcw0.desktop
cat > default.gcw0.desktop <<EOF
[Desktop Entry]
Name=Vorton
Comment=Isometric action
Exec=vorton
Terminal=false
Type=Application
StartupNotify=true
Icon=icon
Categories=games;
EOF

# create opk
FLIST="data"
FLIST="${FLIST} default.gcw0.desktop"
FLIST="${FLIST} src/gp32/icon.png"
FLIST="${FLIST} vorton"

rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports

cat default.gcw0.desktop
rm -f default.gcw0.desktop
