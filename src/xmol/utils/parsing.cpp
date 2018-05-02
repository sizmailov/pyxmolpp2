#include "xmol/utils/parsing.h"

std::pair<bool, double>
xmol::utils::parsing::parse_fixed_precision_rt(const std::string& line, int pos,
                                               int width) noexcept {
  if (GSL_UNLIKELY(line.size() < pos + width)) {
    return {false, 0};
  }
  char* punct_pos = (char*)memchr(&line[pos], '.', width);
  int whole, precision;

  if (punct_pos == nullptr) {
    return {false, 0};
  } else {
    whole = punct_pos - &line.c_str()[pos];
    precision = width - 1 - whole;
    if (GSL_UNLIKELY(precision == 0)) {
      return {false, 0};
    }
  };

  while (line[pos] == ' ' && whole > 0) {
    --whole;
    pos++;
  }
  while (line[pos + whole + precision] == ' ' && precision > 0) {
    --precision;
  }
  width = whole + precision;
  if (precision > 0) {
    ++width;
  }

  switch (width) {
  case (2): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<2, 0, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }

  case (3): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<3, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<3, 1, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (4): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<4, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<4, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<4, 2, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (5): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<5, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<5, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<5, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<5, 3, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (6): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<6, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<6, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<6, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<6, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<6, 4, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (7): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<7, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<7, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<7, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<7, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<7, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<7, 5, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (8): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<8, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<8, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<8, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<8, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<8, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<8, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<8, 6, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (9): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<9, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<9, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<9, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<9, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<9, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<9, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<9, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<9, 7, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (10): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<10, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<10, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<10, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<10, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<10, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<10, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<10, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<10, 7, SpaceStrip::NONE>(line, pos);
    case (8):
      return parse_fixed_precision.parse<10, 8, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (11): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<11, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<11, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<11, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<11, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<11, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<11, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<11, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<11, 7, SpaceStrip::NONE>(line, pos);
    case (8):
      return parse_fixed_precision.parse<11, 8, SpaceStrip::NONE>(line, pos);
    case (9):
      return parse_fixed_precision.parse<11, 9, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (12): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<12, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<12, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<12, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<12, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<12, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<12, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<12, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<12, 7, SpaceStrip::NONE>(line, pos);
    case (8):
      return parse_fixed_precision.parse<12, 8, SpaceStrip::NONE>(line, pos);
    case (9):
      return parse_fixed_precision.parse<12, 9, SpaceStrip::NONE>(line, pos);
    case (10):
      return parse_fixed_precision.parse<12, 10, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (13): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<13, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<13, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<13, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<13, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<13, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<13, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<13, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<13, 7, SpaceStrip::NONE>(line, pos);
    case (8):
      return parse_fixed_precision.parse<13, 8, SpaceStrip::NONE>(line, pos);
    case (9):
      return parse_fixed_precision.parse<13, 9, SpaceStrip::NONE>(line, pos);
    case (10):
      return parse_fixed_precision.parse<13, 10, SpaceStrip::NONE>(line, pos);
    case (11):
      return parse_fixed_precision.parse<13, 11, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (14): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<14, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<14, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<14, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<14, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<14, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<14, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<14, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<14, 7, SpaceStrip::NONE>(line, pos);
    case (8):
      return parse_fixed_precision.parse<14, 8, SpaceStrip::NONE>(line, pos);
    case (9):
      return parse_fixed_precision.parse<14, 9, SpaceStrip::NONE>(line, pos);
    case (10):
      return parse_fixed_precision.parse<14, 10, SpaceStrip::NONE>(line, pos);
    case (11):
      return parse_fixed_precision.parse<14, 11, SpaceStrip::NONE>(line, pos);
    case (12):
      return parse_fixed_precision.parse<14, 12, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (15): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<15, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<15, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<15, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<15, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<15, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<15, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<15, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<15, 7, SpaceStrip::NONE>(line, pos);
    case (8):
      return parse_fixed_precision.parse<15, 8, SpaceStrip::NONE>(line, pos);
    case (9):
      return parse_fixed_precision.parse<15, 9, SpaceStrip::NONE>(line, pos);
    case (10):
      return parse_fixed_precision.parse<15, 10, SpaceStrip::NONE>(line, pos);
    case (11):
      return parse_fixed_precision.parse<15, 11, SpaceStrip::NONE>(line, pos);
    case (12):
      return parse_fixed_precision.parse<15, 12, SpaceStrip::NONE>(line, pos);
    case (13):
      return parse_fixed_precision.parse<15, 13, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (16): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<16, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<16, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<16, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<16, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<16, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<16, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<16, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<16, 7, SpaceStrip::NONE>(line, pos);
    case (8):
      return parse_fixed_precision.parse<16, 8, SpaceStrip::NONE>(line, pos);
    case (9):
      return parse_fixed_precision.parse<16, 9, SpaceStrip::NONE>(line, pos);
    case (10):
      return parse_fixed_precision.parse<16, 10, SpaceStrip::NONE>(line, pos);
    case (11):
      return parse_fixed_precision.parse<16, 11, SpaceStrip::NONE>(line, pos);
    case (12):
      return parse_fixed_precision.parse<16, 12, SpaceStrip::NONE>(line, pos);
    case (13):
      return parse_fixed_precision.parse<16, 13, SpaceStrip::NONE>(line, pos);
    case (14):
      return parse_fixed_precision.parse<16, 14, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }

  case (17): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<17, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<17, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<17, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<17, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<17, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<17, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<17, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<17, 7, SpaceStrip::NONE>(line, pos);
    case (8):
      return parse_fixed_precision.parse<17, 8, SpaceStrip::NONE>(line, pos);
    case (9):
      return parse_fixed_precision.parse<17, 9, SpaceStrip::NONE>(line, pos);
    case (10):
      return parse_fixed_precision.parse<17, 10, SpaceStrip::NONE>(line, pos);
    case (11):
      return parse_fixed_precision.parse<17, 11, SpaceStrip::NONE>(line, pos);
    case (12):
      return parse_fixed_precision.parse<17, 12, SpaceStrip::NONE>(line, pos);
    case (13):
      return parse_fixed_precision.parse<17, 13, SpaceStrip::NONE>(line, pos);
    case (14):
      return parse_fixed_precision.parse<17, 14, SpaceStrip::NONE>(line, pos);
    case (15):
      return parse_fixed_precision.parse<17, 15, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }
  case (18): {
    switch (precision) {
    case (0):
      return parse_fixed_precision.parse<18, 0, SpaceStrip::NONE>(line, pos);
    case (1):
      return parse_fixed_precision.parse<18, 1, SpaceStrip::NONE>(line, pos);
    case (2):
      return parse_fixed_precision.parse<18, 2, SpaceStrip::NONE>(line, pos);
    case (3):
      return parse_fixed_precision.parse<18, 3, SpaceStrip::NONE>(line, pos);
    case (4):
      return parse_fixed_precision.parse<18, 4, SpaceStrip::NONE>(line, pos);
    case (5):
      return parse_fixed_precision.parse<18, 5, SpaceStrip::NONE>(line, pos);
    case (6):
      return parse_fixed_precision.parse<18, 6, SpaceStrip::NONE>(line, pos);
    case (7):
      return parse_fixed_precision.parse<18, 7, SpaceStrip::NONE>(line, pos);
    case (8):
      return parse_fixed_precision.parse<18, 8, SpaceStrip::NONE>(line, pos);
    case (9):
      return parse_fixed_precision.parse<18, 9, SpaceStrip::NONE>(line, pos);
    case (10):
      return parse_fixed_precision.parse<18, 10, SpaceStrip::NONE>(line, pos);
    case (11):
      return parse_fixed_precision.parse<18, 11, SpaceStrip::NONE>(line, pos);
    case (12):
      return parse_fixed_precision.parse<18, 12, SpaceStrip::NONE>(line, pos);
    case (13):
      return parse_fixed_precision.parse<18, 13, SpaceStrip::NONE>(line, pos);
    case (14):
      return parse_fixed_precision.parse<18, 14, SpaceStrip::NONE>(line, pos);
    case (15):
      return parse_fixed_precision.parse<18, 15, SpaceStrip::NONE>(line, pos);
    case (16):
      return parse_fixed_precision.parse<18, 16, SpaceStrip::NONE>(line, pos);
    default:
      return {false, 0};
    }
  }

  default:
    return {false, 0};
  }
}
