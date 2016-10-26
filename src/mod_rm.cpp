#include "mod_rm.hpp"
#include "tinyexpr.h"

namespace nask_utility {
     namespace ModRM {
	  const std::string get_MMMSSS_from_reg(const std::string& reg) {
	       if (REGISTERS_MMM_MAP.count(reg) != 0) {
		    return REGISTERS_MMM_MAP.at(reg);
	       } else if (SEGMENT_REGISTERS_SSS_MAP.count(reg) != 0) {
		    return SEGMENT_REGISTERS_SSS_MAP.at(reg);
	       } else {
		    return "000";
	       }
	  }

	  const std::string get_rm_from_reg(const std::string& src_reg) {
	       std::smatch match;
	       if (regex_match(src_reg, match, rm000)) {
		    return "000";
	       } else if (regex_match(src_reg, match, rm001)) {
		    return "001";
	       } else if (regex_match(src_reg, match, rm010)) {
		    return "010";
	       } else if (regex_match(src_reg, match, rm011)) {
		    return "011";
	       }

	       return "110";
	  };

	  const uint8_t get_opecode_from_reg(uint8_t base, const std::string& reg_name) {
	       std::tuple<std::string, std::string> tuple = REGISTERS_RRR_MAP.at(reg_name);
	       std::string register_number = std::get<0>(tuple);
	       const std::bitset<8> bitset("00000" + register_number);
	       log()->info("+rb, +rw, +rd, +ro = 00000{}", register_number);
	       const uint8_t ret = base + bitset.to_ulong();
	       log()->info("0x{:02x} + 0x{:02x} = 0x{:02x}", base, bitset.to_ulong(), ret);
	       return ret;
	  }

	  // @param m       : [mod] mods::REG_REG, REG_DISP8, REG_DISP16, REG
	  // @param dst_reg : [reg] Register with std::string
	  // @param reg     : [r/m] '/x' with Enum
	  uint8_t generate_modrm(enum mods m, const std::string& dst_reg, enum reg_field reg) {
	       //
	       // Generate ModR/M byte with arguments
	       // [mod] 2bit
	       // [reg] 3bit
	       // [r/m] 3bit
	       //
	       std::string modrm = ModRM::MOD_TO_STR.at(m);
	       if (reg == SLASH_R) {
		    modrm += get_rm_from_reg(dst_reg);
	       } else {
		    switch (reg) {
		    case SLASH_0:
			 modrm += "000";
			 break;
		    case SLASH_1:
			 modrm += "001";
			 break;
		    case SLASH_2:
			 modrm += "010";
			 break;
		    case SLASH_3:
			 modrm += "011";
			 break;
		    case SLASH_4:
			 modrm += "100";
			 break;
		    case SLASH_5:
			 modrm += "101";
			 break;
		    case SLASH_6:
			 modrm += "110";
			 break;
		    case SLASH_7:
			 modrm += "111";
			 break;
		    }
	       }
	       modrm += get_rm_from_reg(dst_reg);
	       std::bitset<8> bs(modrm);
	       return bs.to_ulong();
	  };

	  bool is_accumulator(const std::string& reg) {
	       std::smatch match;
	       return regex_match(reg, match, rm000);
	  }
     };
}