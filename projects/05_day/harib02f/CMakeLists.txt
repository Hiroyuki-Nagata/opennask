#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(05_day_harib02f_OS    ${root_BINARY_DIR}/projects/05_day/harib02f/os.img)
set(05_day_harib02f_SYS	  ${root_BINARY_DIR}/projects/05_day/harib02f/os.sys)
set(05_day_harib02f_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02f/ipl.bin)
set(05_day_harib02f_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02f/ipl10.nas)
set(05_day_harib02f_HEADB ${root_BINARY_DIR}/projects/05_day/harib02f/asmhead.bin)
set(05_day_harib02f_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02f/asmhead.nas)
set(05_day_harib02f_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02f/boot.bin)
set(05_day_harib02f_CCO	  ${root_BINARY_DIR}/projects/05_day/harib02f/boot.o)
set(05_day_harib02f_CCS	  ${root_SOURCE_DIR}/projects/05_day/harib02f/*.c)
set(05_day_harib02f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02f_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02f/naskfunc.nas)
set(05_day_harib02f_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02f/naskfunc.o)
set(05_day_harib02f_FONTS ${root_SOURCE_DIR}/projects/05_day/harib02f/hankaku.txt)
set(05_day_harib02f_FONTB ${root_BINARY_DIR}/projects/05_day/harib02f/hankaku.bin)
set(05_day_harib02f_FONTO ${root_BINARY_DIR}/projects/05_day/harib02f/hankaku.o)
set(05_day_harib02f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(05_day_harib02f_LIBGC ${root_BINARY_DIR}/projects/05_day/harib02f/libgo.o)
set(05_day_harib02f_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02f/*.o)


add_custom_target(05_day_harib02f_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02f_OS}
  DEPENDS 05_day_harib02f_img
)
add_custom_target(05_day_harib02f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 05_day_harib02f_img
)
add_custom_target(05_day_harib02f_clean
  COMMAND rm -f ${05_day_harib02f_OS}
  COMMAND rm -f ${05_day_harib02f_SYS}
  COMMAND rm -f ${05_day_harib02f_IPLB}
  COMMAND rm -f ${05_day_harib02f_HEADB}
  COMMAND rm -f ${05_day_harib02f_BOOTB}
  COMMAND rm -f ${05_day_harib02f_WILDOBJ}
)
add_custom_target(05_day_harib02f_ipl
  COMMAND ${NASK} ${05_day_harib02f_IPLS} ${05_day_harib02f_IPLB}
)
add_custom_target(05_day_harib02f_sys
  COMMAND ${NASK} ${05_day_harib02f_HEADS} ${05_day_harib02f_HEADB}
  COMMAND ${FONT} ${05_day_harib02f_FONTS} ${05_day_harib02f_FONTB}
  COMMAND ${B2O}  ${05_day_harib02f_FONTB} ${05_day_harib02f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${05_day_harib02f_LIBGE} ${05_day_harib02f_LIBGC}
  COMMAND ${NASK} ${05_day_harib02f_FUNCS} ${05_day_harib02f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${05_day_harib02f_LDS} ${05_day_harib02f_CCS} ${05_day_harib02f_WILDOBJ} -o ${05_day_harib02f_BOOTB}
  COMMAND cat ${05_day_harib02f_HEADB} ${05_day_harib02f_BOOTB} > ${05_day_harib02f_SYS}
  DEPENDS 05_day_harib02f_ipl
)
add_custom_target(05_day_harib02f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02f_IPLB} -i ${05_day_harib02f_OS}
  COMMAND mcopy -i ${05_day_harib02f_OS} ${05_day_harib02f_SYS} ::
  DEPENDS 05_day_harib02f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/asmhead_wine.img)

add_custom_target(05_day_harib02f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(05_day_harib02f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(05_day_harib02f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(05_day_harib02f_asmhead_od 05_day_harib02f_asmhead)
add_dependencies(05_day_harib02f_asmhead_od 05_day_harib02f_asmhead_wine)

add_dependencies(images 05_day_harib02f_asmhead)
add_dependencies(wine 05_day_harib02f_asmhead_wine)
add_dependencies(od 05_day_harib02f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/ipl10_wine.img)

add_custom_target(05_day_harib02f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(05_day_harib02f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(05_day_harib02f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(05_day_harib02f_ipl10_od 05_day_harib02f_ipl10)
add_dependencies(05_day_harib02f_ipl10_od 05_day_harib02f_ipl10_wine)

add_dependencies(images 05_day_harib02f_ipl10)
add_dependencies(wine 05_day_harib02f_ipl10_wine)
add_dependencies(od 05_day_harib02f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/naskfunc_wine.img)

add_custom_target(05_day_harib02f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(05_day_harib02f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(05_day_harib02f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(05_day_harib02f_naskfunc_od 05_day_harib02f_naskfunc)
add_dependencies(05_day_harib02f_naskfunc_od 05_day_harib02f_naskfunc_wine)

add_dependencies(images 05_day_harib02f_naskfunc)
add_dependencies(wine 05_day_harib02f_naskfunc_wine)
add_dependencies(od 05_day_harib02f_naskfunc_od)

#----------------------------------------------------------
