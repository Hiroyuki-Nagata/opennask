#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12a_OS    ${root_BINARY_DIR}/projects/15_day/harib12a/os.img)
set(15_day_harib12a_SYS	  ${root_BINARY_DIR}/projects/15_day/harib12a/os.sys)
set(15_day_harib12a_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12a/ipl.bin)
set(15_day_harib12a_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12a/ipl10.nas)
set(15_day_harib12a_HEADB ${root_BINARY_DIR}/projects/15_day/harib12a/asmhead.bin)
set(15_day_harib12a_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12a/asmhead.nas)
set(15_day_harib12a_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12a/boot.bin)
set(15_day_harib12a_CCO	  ${root_BINARY_DIR}/projects/15_day/harib12a/boot.o)
set(15_day_harib12a_CCS	  ${root_SOURCE_DIR}/projects/15_day/harib12a/*.c)
set(15_day_harib12a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12a_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12a/naskfunc.nas)
set(15_day_harib12a_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12a/naskfunc.o)
set(15_day_harib12a_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12a/hankaku.txt)
set(15_day_harib12a_FONTB ${root_BINARY_DIR}/projects/15_day/harib12a/hankaku.bin)
set(15_day_harib12a_FONTO ${root_BINARY_DIR}/projects/15_day/harib12a/hankaku.o)
set(15_day_harib12a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12a_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12a/libgo.o)
set(15_day_harib12a_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12a/*.o)


add_custom_target(15_day_harib12a_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12a_OS}
  DEPENDS 15_day_harib12a_img
)
add_custom_target(15_day_harib12a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 15_day_harib12a_img
)
add_custom_target(15_day_harib12a_clean
  COMMAND rm -f ${15_day_harib12a_OS}
  COMMAND rm -f ${15_day_harib12a_SYS}
  COMMAND rm -f ${15_day_harib12a_IPLB}
  COMMAND rm -f ${15_day_harib12a_HEADB}
  COMMAND rm -f ${15_day_harib12a_BOOTB}
  COMMAND rm -f ${15_day_harib12a_WILDOBJ}
)
add_custom_target(15_day_harib12a_ipl
  COMMAND ${NASK} ${15_day_harib12a_IPLS} ${15_day_harib12a_IPLB}
)
add_custom_target(15_day_harib12a_sys
  COMMAND ${NASK} ${15_day_harib12a_HEADS} ${15_day_harib12a_HEADB}
  COMMAND ${FONT} ${15_day_harib12a_FONTS} ${15_day_harib12a_FONTB}
  COMMAND ${B2O}  ${15_day_harib12a_FONTB} ${15_day_harib12a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12a_LIBGE} ${15_day_harib12a_LIBGC}
  COMMAND ${NASK} ${15_day_harib12a_FUNCS} ${15_day_harib12a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${15_day_harib12a_LDS} ${15_day_harib12a_CCS} ${15_day_harib12a_WILDOBJ} -o ${15_day_harib12a_BOOTB}
  COMMAND cat ${15_day_harib12a_HEADB} ${15_day_harib12a_BOOTB} > ${15_day_harib12a_SYS}
  DEPENDS 15_day_harib12a_ipl
)
add_custom_target(15_day_harib12a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12a_IPLB} -i ${15_day_harib12a_OS}
  COMMAND mcopy -i ${15_day_harib12a_OS} ${15_day_harib12a_SYS} ::
  DEPENDS 15_day_harib12a_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/asmhead_wine.img)

add_custom_target(15_day_harib12a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(15_day_harib12a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(15_day_harib12a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12a_asmhead_od 15_day_harib12a_asmhead)
add_dependencies(15_day_harib12a_asmhead_od 15_day_harib12a_asmhead_wine)

add_dependencies(images 15_day_harib12a_asmhead)
add_dependencies(wine 15_day_harib12a_asmhead_wine)
add_dependencies(od 15_day_harib12a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/ipl10_wine.img)

add_custom_target(15_day_harib12a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(15_day_harib12a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(15_day_harib12a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12a_ipl10_od 15_day_harib12a_ipl10)
add_dependencies(15_day_harib12a_ipl10_od 15_day_harib12a_ipl10_wine)

add_dependencies(images 15_day_harib12a_ipl10)
add_dependencies(wine 15_day_harib12a_ipl10_wine)
add_dependencies(od 15_day_harib12a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/naskfunc_wine.img)

add_custom_target(15_day_harib12a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(15_day_harib12a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(15_day_harib12a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12a_naskfunc_od 15_day_harib12a_naskfunc)
add_dependencies(15_day_harib12a_naskfunc_od 15_day_harib12a_naskfunc_wine)

add_dependencies(images 15_day_harib12a_naskfunc)
add_dependencies(wine 15_day_harib12a_naskfunc_wine)
add_dependencies(od 15_day_harib12a_naskfunc_od)

#----------------------------------------------------------
