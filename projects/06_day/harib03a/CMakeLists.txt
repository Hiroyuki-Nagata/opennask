#----------------------------------------------------------
message(STATUS "Entering directory projects/06_day/harib03a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(06_day_harib03a_OS    ${root_BINARY_DIR}/projects/06_day/harib03a/os.img)
set(06_day_harib03a_SYS	  ${root_BINARY_DIR}/projects/06_day/harib03a/os.sys)
set(06_day_harib03a_IPLB  ${root_BINARY_DIR}/projects/06_day/harib03a/ipl.bin)
set(06_day_harib03a_IPLS  ${root_SOURCE_DIR}/projects/06_day/harib03a/ipl10.nas)
set(06_day_harib03a_HEADB ${root_BINARY_DIR}/projects/06_day/harib03a/asmhead.bin)
set(06_day_harib03a_HEADS ${root_SOURCE_DIR}/projects/06_day/harib03a/asmhead.nas)
set(06_day_harib03a_BOOTB ${root_BINARY_DIR}/projects/06_day/harib03a/boot.bin)
set(06_day_harib03a_CCO	  ${root_BINARY_DIR}/projects/06_day/harib03a/boot.o)
set(06_day_harib03a_CCS	  ${root_SOURCE_DIR}/projects/06_day/harib03a/*.c)
set(06_day_harib03a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(06_day_harib03a_FUNCS ${root_SOURCE_DIR}/projects/06_day/harib03a/naskfunc.nas)
set(06_day_harib03a_FUNCO ${root_BINARY_DIR}/projects/06_day/harib03a/naskfunc.o)
set(06_day_harib03a_FONTS ${root_SOURCE_DIR}/projects/06_day/harib03a/hankaku.txt)
set(06_day_harib03a_FONTB ${root_BINARY_DIR}/projects/06_day/harib03a/hankaku.bin)
set(06_day_harib03a_FONTO ${root_BINARY_DIR}/projects/06_day/harib03a/hankaku.o)
set(06_day_harib03a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(06_day_harib03a_LIBGC ${root_BINARY_DIR}/projects/06_day/harib03a/libgo.o)
set(06_day_harib03a_WILDOBJ ${root_BINARY_DIR}/projects/06_day/harib03a/*.o)


add_custom_target(06_day_harib03a_run
  COMMAND ${QEMU} ${QEMUOPT} ${06_day_harib03a_OS}
  DEPENDS 06_day_harib03a_img
)
add_custom_target(06_day_harib03a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${06_day_harib03a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 06_day_harib03a_img
)
add_custom_target(06_day_harib03a_clean
  COMMAND rm -f ${06_day_harib03a_OS}
  COMMAND rm -f ${06_day_harib03a_SYS}
  COMMAND rm -f ${06_day_harib03a_IPLB}
  COMMAND rm -f ${06_day_harib03a_HEADB}
  COMMAND rm -f ${06_day_harib03a_BOOTB}
  COMMAND rm -f ${06_day_harib03a_WILDOBJ}
)
add_custom_target(06_day_harib03a_ipl
  COMMAND ${NASK} ${06_day_harib03a_IPLS} ${06_day_harib03a_IPLB}
)
add_custom_target(06_day_harib03a_sys
  COMMAND ${NASK} ${06_day_harib03a_HEADS} ${06_day_harib03a_HEADB}
  COMMAND ${FONT} ${06_day_harib03a_FONTS} ${06_day_harib03a_FONTB}
  COMMAND ${B2O}  ${06_day_harib03a_FONTB} ${06_day_harib03a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${06_day_harib03a_LIBGE} ${06_day_harib03a_LIBGC}
  COMMAND ${NASK} ${06_day_harib03a_FUNCS} ${06_day_harib03a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${06_day_harib03a_LDS} ${06_day_harib03a_CCS} ${06_day_harib03a_WILDOBJ} -o ${06_day_harib03a_BOOTB}
  COMMAND cat ${06_day_harib03a_HEADB} ${06_day_harib03a_BOOTB} > ${06_day_harib03a_SYS}
  DEPENDS 06_day_harib03a_ipl
)
add_custom_target(06_day_harib03a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${06_day_harib03a_IPLB} -i ${06_day_harib03a_OS}
  COMMAND mcopy -i ${06_day_harib03a_OS} ${06_day_harib03a_SYS} ::
  DEPENDS 06_day_harib03a_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/asmhead_wine.img)

add_custom_target(06_day_harib03a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(06_day_harib03a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(06_day_harib03a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(06_day_harib03a_asmhead_od 06_day_harib03a_asmhead)
add_dependencies(06_day_harib03a_asmhead_od 06_day_harib03a_asmhead_wine)

add_dependencies(images 06_day_harib03a_asmhead)
add_dependencies(wine 06_day_harib03a_asmhead_wine)
add_dependencies(od 06_day_harib03a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/ipl10_wine.img)

add_custom_target(06_day_harib03a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(06_day_harib03a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(06_day_harib03a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(06_day_harib03a_ipl10_od 06_day_harib03a_ipl10)
add_dependencies(06_day_harib03a_ipl10_od 06_day_harib03a_ipl10_wine)

add_dependencies(images 06_day_harib03a_ipl10)
add_dependencies(wine 06_day_harib03a_ipl10_wine)
add_dependencies(od 06_day_harib03a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/naskfunc_wine.img)

add_custom_target(06_day_harib03a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(06_day_harib03a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(06_day_harib03a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(06_day_harib03a_naskfunc_od 06_day_harib03a_naskfunc)
add_dependencies(06_day_harib03a_naskfunc_od 06_day_harib03a_naskfunc_wine)

add_dependencies(images 06_day_harib03a_naskfunc)
add_dependencies(wine 06_day_harib03a_naskfunc_wine)
add_dependencies(od 06_day_harib03a_naskfunc_od)

#----------------------------------------------------------
