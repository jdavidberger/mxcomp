#include <mxcomp/utf.h>
#include <iostream>
#include <QUnit.hpp>
#include <sstream>

template <class T>
std::string asHex(const T& str) {
  std::stringstream ss;
  ss << std::endl;
  for(auto it : str)
    ss << std::hex << (uint32_t)it << " ";
  ss << std::endl;
  return ss.str();
}

using namespace std;
using namespace mxcomp::utf;

#define UNICODE(hex) \U ## hex

#define CONVERSION_TEST(uni) {						\
      auto utf32 = utf32_t(U###uni);					\
      auto utf16 = utf16_t(u###uni);					\
      auto wide  = std::wstring(L###uni);				\
      /* system wide -> utf8 */						\
      QUNIT_IS_EQUAL(#uni, convert(L###uni));				\
      /* utf32 -> utf8 */						\
      QUNIT_IS_EQUAL(#uni, convert(utf32));				\
      /* utf16 -> utf8 */						\
      QUNIT_IS_EQUAL(#uni, convert(utf16));				\
									\
      /* utf8 -> utf32 */						\
      QUNIT_IS_EQUAL(asHex(utf32), asHex(utf32::convert(#uni)));	\
      /* utf16 -> utf32 */						\
      QUNIT_IS_EQUAL(asHex(utf32), asHex(utf32::convert(utf16)));	\
      /* wide -> utf32 */						\
      QUNIT_IS_EQUAL(asHex(utf32), asHex(utf32::convert(wide)));	\
									\
      /* utf8 -> utf16 */						\
      QUNIT_IS_EQUAL(asHex(utf16), asHex(utf16::convert(#uni)));	\
      /* utf32 -> utf16 */						\
      QUNIT_IS_EQUAL(asHex(utf16), asHex(utf16::convert(utf32)));	\
      /* wide -> utf16 */						\
      QUNIT_IS_EQUAL(asHex(utf16), asHex(utf16::convert(wide)));	\
									\
      /* utf8 -> wide */						\
      QUNIT_IS_EQUAL(asHex(wide), asHex(utfW::convert(#uni)));		\
      /* utf16 -> wide */						\
      QUNIT_IS_EQUAL(asHex(wide), asHex(utfW::convert(utf16)));		\
      /* utf32 -> wide */						\
      QUNIT_IS_EQUAL(asHex(wide), asHex(utfW::convert(utf32)));		\
  }							

#define _CHAR_TEST(hex, uni)				\
  CONVERSION_TEST(\U ## hex)				\
  QUNIT_IS_EQUAL(0x ## hex, utf32::convert(#uni)[0])

#define CHAR_TEST(hex) \
  _CHAR_TEST(hex, \U ## hex)


int main() {
  QUnit::UnitTest qunit(std::cout, QUnit::normal);
  mxcomp::log::SetLogLevel(mxcomp::log::Debug);
  mxcomp::log::SetLogStream(std::cout);
  
  CHAR_TEST(00000055);
  CHAR_TEST(00002D63);
  CHAR_TEST(00002D4D);
  CHAR_TEST(0001D11E);
  CHAR_TEST(00000001);
  CHAR_TEST(0010FFFF);
  CHAR_TEST(0000FEFF);
  CHAR_TEST(0000FEFF);
  CHAR_TEST(00010000);
  CONVERSION_TEST("Testing");
  CONVERSION_TEST("  ¥ · £ · € · $ · ¢ · ₡ · ₢ · ₣ · ₤ · ₥ · ₦ · ₧ · ₨ · ₩ · ₪ · ₫ · ₭ · ₮ · ₯ · ₹");
  CONVERSION_TEST("");
  CONVERSION_TEST("ಠ_ಠ");

  CONVERSION_TEST("ᚠᛇᚻ᛫ᛒᛦᚦ᛫ᚠᚱᚩᚠᚢᚱ᛫ᚠᛁᚱᚪ᛫ᚷᛖᚻᚹᛦᛚᚳᚢᛗ"
		  "ᛋᚳᛖᚪᛚ᛫ᚦᛖᚪᚻ᛫ᛗᚪᚾᚾᚪ᛫ᚷᛖᚻᚹᛦᛚᚳ᛫ᛗᛁᚳᛚᚢᚾ᛫ᚻᛦᛏ᛫ᛞᚫᛚᚪᚾ"
		  "ᚷᛁᚠ᛫ᚻᛖ᛫ᚹᛁᛚᛖ᛫ᚠᚩᚱ᛫ᛞᚱᛁᚻᛏᚾᛖ᛫ᛞᚩᛗᛖᛋ᛫ᚻᛚᛇᛏᚪᚾ᛬");

  CONVERSION_TEST("私はガラスを食べられます。それは私を傷つけません。");
  CONVERSION_TEST("ಬಾ ಇಲ್ಲಿ ಸಂಭವಿಸು ಇಂದೆನ್ನ ಹೃದಯದಲಿ "
		  "ನಿತ್ಯವೂ ಅವತರಿಪ ಸತ್ಯಾವತಾರ"
		  "ಮಣ್ಣಾಗಿ ಮರವಾಗಿ ಮಿಗವಾಗಿ ಕಗವಾಗೀ... "
		  "ಮಣ್ಣಾಗಿ ಮರವಾಗಿ ಮಿಗವಾಗಿ ಕಗವಾಗಿ "
		  "ಭವ ಭವದಿ ಭತಿಸಿಹೇ ಭವತಿ ದೂರ "
		  "ನಿತ್ಯವೂ ಅವತರಿಪ ಸತ್ಯಾವತಾರ || ಬಾ ಇಲ್ಲಿ ||");
  return 0;
}
