#----------------------------------------------------------
message(STATUS "Entering directory projects/09_day/harib06a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(09_day_harib06a_OS    ${root_BINARY_DIR}/projects/09_day/harib06a/os.img)
set(09_day_harib06a_SYS	  ${root_BINARY_DIR}/projects/09_day/harib06a/os.sys)
set(09_day_harib06a_IPLB  ${root_BINARY_DIR}/projects/09_day/harib06a/ipl.bin)
set(09_day_harib06a_IPLS  ${root_SOURCE_DIR}/projects/09_day/harib06a/ipl10.nas)
set(09_day_harib06a_HEADB ${root_BINARY_DIR}/projects/09_day/harib06a/asmhead.bin)
set(09_day_harib06a_HEADS ${root_SOURCE_DIR}/projects/09_day/harib06a/asmhead.nas)
set(09_day_harib06a_BOOTB ${root_BINARY_DIR}/projects/09_day/harib06a/boot.bin)
set(09_day_harib06a_CCO	  ${root_BINARY_DIR}/projects/09_day/harib06a/boot.o)
set(09_day_harib06a_CCS	  ${root_SOURCE_DIR}/projects/09_day/harib06a/*.c)
set(09_day_harib06a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(09_day_harib06a_FUNCS ${root_SOURCE_DIR}/projects/09_day/harib06a/naskfunc.nas)
set(09_day_harib06a_FUNCO ${root_BINARY_DIR}/projects/09_day/harib06a/naskfunc.o)
set(09_day_harib06a_FONTS ${root_SOURCE_DIR}/projects/09_day/harib06a/hankaku.txt)
set(09_day_harib06a_FONTB ${root_BINARY_DIR}/projects/09_day/harib06a/hankaku.bin)
set(09_day_harib06a_FONTO ${root_BINARY_DIR}/projects/09_day/harib06a/hankaku.o)
set(09_day_harib06a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(09_day_harib06a_LIBGC ${root_BINARY_DIR}/projects/09_day/harib06a/libgo.o)
set(09_day_harib06a_WILDOBJ ${root_BINARY_DIR}/projects/09_day/harib06a/*.o)


add_custom_target(09_day_harib06a_run
  COMMAND ${QEMU} ${QEMUOPT} ${09_day_harib06a_OS}
  DEPENDS 09_day_harib06a_img
)
add_custom_target(09_day_harib06a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${09_day_harib06a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 09_day_harib06a_img
)
add_custom_target(09_day_harib06a_clean
  COMMAND rm -f ${09_day_harib06a_OS}
  COMMAND rm -f ${09_day_harib06a_SYS}
  COMMAND rm -f ${09_day_harib06a_IPLB}
  COMMAND rm -f ${09_day_harib06a_HEADB}
  COMMAND rm -f ${09_day_harib06a_BOOTB}
  COMMAND rm -f ${09_day_harib06a_WILDOBJ}
)
add_custom_target(09_day_harib06a_ipl
  COMMAND ${NASK} ${09_day_harib06a_IPLS} ${09_day_harib06a_IPLB}
)
add_custom_target(09_day_harib06a_sys
  COMMAND ${NASK} ${09_day_harib06a_HEADS} ${09_day_harib06a_HEADB}
  COMMAND ${FONT} ${09_day_harib06a_FONTS} ${09_day_harib06a_FONTB}
  COMMAND ${B2O}  ${09_day_harib06a_FONTB} ${09_day_harib06a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${09_day_harib06a_LIBGE} ${09_day_harib06a_LIBGC}
  COMMAND ${NASK} ${09_day_harib06a_FUNCS} ${09_day_harib06a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${09_day_harib06a_LDS} ${09_day_harib06a_CCS} ${09_day_harib06a_WILDOBJ} -o ${09_day_harib06a_BOOTB}
  COMMAND cat ${09_day_harib06a_HEADB} ${09_day_harib06a_BOOTB} > ${09_day_harib06a_SYS}
  DEPENDS 09_day_harib06a_ipl
)
add_custom_target(09_day_harib06a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${09_day_harib06a_IPLB} -i ${09_day_harib06a_OS}
  COMMAND mcopy -i ${09_day_harib06a_OS} ${09_day_harib06a_SYS} ::
  DEPENDS 09_day_harib06a_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/asmhead_wine.img)

add_custom_target(09_day_harib06a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(09_day_harib06a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(09_day_harib06a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(09_day_harib06a_asmhead_od 09_day_harib06a_asmhead)
add_dependencies(09_day_harib06a_asmhead_od 09_day_harib06a_asmhead_wine)

add_dependencies(images 09_day_harib06a_asmhead)
add_dependencies(wine 09_day_harib06a_asmhead_wine)
add_dependencies(od 09_day_harib06a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/ipl10_wine.img)

add_custom_target(09_day_harib06a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(09_day_harib06a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(09_day_harib06a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(09_day_harib06a_ipl10_od 09_day_harib06a_ipl10)
add_dependencies(09_day_harib06a_ipl10_od 09_day_harib06a_ipl10_wine)

add_dependencies(images 09_day_harib06a_ipl10)
add_dependencies(wine 09_day_harib06a_ipl10_wine)
add_dependencies(od 09_day_harib06a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/naskfunc_wine.img)

add_custom_target(09_day_harib06a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(09_day_harib06a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(09_day_harib06a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(09_day_harib06a_naskfunc_od 09_day_harib06a_naskfunc)
add_dependencies(09_day_harib06a_naskfunc_od 09_day_harib06a_naskfunc_wine)

add_dependencies(images 09_day_harib06a_naskfunc)
add_dependencies(wine 09_day_harib06a_naskfunc_wine)
add_dependencies(od 09_day_harib06a_naskfunc_od)

#----------------------------------------------------------
